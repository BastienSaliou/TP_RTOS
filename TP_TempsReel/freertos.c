#define _GNU_SOURCE // for pthread_set_name_np
#include <pthread.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "freertos.h"

#define THOUSAND (1000)
#define MILLION (1000 * THOUSAND)
#define BILLION (1000 * MILLION)

static void *emallocz(unsigned long size) {
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, "[KO] unable to alloc memory %ld bytes\n", size);
        exit(EXIT_FAILURE);
    }
    memset(mem, 0, size);
    return mem;
}

xQueueHandle xQueueCreate(unsigned int max_elem, unsigned int size_elem) {
    assert(xQueue_count < XQUEUE_MAX_COUNT);
    xQueueHandle handle = &xQueue_ary[xQueue_count++];
    handle->ary = emallocz(max_elem * size_elem);
    handle->max_elem = max_elem;
    handle->size_elem = size_elem;
    handle->first = handle->last = handle->nr_elem = 0;
    pthread_mutex_init(&handle->m, NULL);
    pthread_cond_init(&handle->c_not_empty, NULL);
    pthread_cond_init(&handle->c_not_full, NULL);
    return handle;
}

void vQueueDelete(xQueueHandle handle) {
    free(handle->ary);
    handle->ary = NULL;
    handle->first = 0;
    handle->last = 0;
    handle->nr_elem = 0;
    handle->max_elem = 0;
    handle->size_elem = 0;
    pthread_mutex_destroy(&handle->m);
    pthread_cond_destroy(&handle->c_not_empty);
    pthread_cond_destroy(&handle->c_not_full);
}

void wait_ms(unsigned long ms) {
    struct timespec ts;
    ts.tv_sec = ms / THOUSAND;
    ts.tv_nsec = (ms % THOUSAND) * MILLION;

    nanosleep(&ts, &ts);
}

void wait_rnd_ms(unsigned long min_ms, unsigned long max_ms) {
    unsigned long duration = rand() % (max_ms - min_ms);
    wait_ms(duration + min_ms);
}

static int xQueueIsFull_no_protection(xQueueHandle h) {
    return h->nr_elem == h->max_elem;
}

static int xQueueIsEmpty_no_protection(xQueueHandle h) {
    return h->nr_elem <= 0;
}

static void calc_when_from_now(struct timespec *when,
                               unsigned long waiting_ms) {
    // get "now" time and add waiting_ms
    timespec_get(when, TIME_UTC);
    time_t sec, ms;
    sec = ((time_t) waiting_ms) / THOUSAND;
    ms = ((time_t) waiting_ms) % THOUSAND;
    when->tv_sec += sec;
    when->tv_nsec += ms * MILLION;
    // check overflow
    when->tv_sec += when->tv_nsec / BILLION;
    when->tv_nsec = when->tv_nsec % BILLION;
    assert(when->tv_sec >= 0);
    assert(when->tv_nsec >= 0);
    assert(when->tv_nsec < BILLION);
}

int xQueueSendToBack(xQueueHandle h, void *src_value,
                     unsigned long waiting_ms) {
    struct timespec timeout;

    pthread_mutex_lock(&h->m);
    if (xQueueIsFull_no_protection(h)) {
        calc_when_from_now(&timeout, waiting_ms);
        int rc;
        while (xQueueIsFull_no_protection(h)) {
            rc = pthread_cond_timedwait(&h->c_not_full, &h->m, &timeout);
            switch(rc) {
            case ETIMEDOUT:
                fprintf(stderr, "[KO] unable to send in time (%d=%s)\n", rc,
                        strerror(rc));
                goto fail;
            case 0:
                break;
            default:
                fprintf(stderr, "[KO] unable to send (%d=%s)\n", rc,
                        strerror(rc));
                goto fail;
            }
        }
    }
    void *dst = &h->ary[(h->last % h->max_elem) * h->size_elem];
    h->nr_elem++;
    h->last++;
    memcpy(dst, src_value, h->size_elem);
    // wait_rnd_ms(waiting_ms / 5, waiting_ms);
    pthread_cond_broadcast(&h->c_not_empty);
    pthread_mutex_unlock(&h->m);
    return 1;
fail:
    pthread_mutex_unlock(&h->m);
    return 0;
}

int xQueueReceive(xQueueHandle h, void *dst_value, unsigned long waiting_ms) {
    struct timespec timeout;

    pthread_mutex_lock(&h->m);
    if (xQueueIsEmpty_no_protection(h)) {
        calc_when_from_now(&timeout, waiting_ms);
        int rc;
        while (xQueueIsEmpty_no_protection(h)) {
            switch (
                rc = pthread_cond_timedwait(&h->c_not_empty, &h->m, &timeout)) {
            case ETIMEDOUT:
                fprintf(stderr, "[KO] unable to receive in time (%d=%s)\n", rc,
                        strerror(rc));
                goto fail;
            case 0:
                break;
            default:
                fprintf(stderr, "[KO] unable to receive (%d=%s)\n", rc,
                        strerror(rc));
                goto fail;
            }
        }
    }
    void *src = &h->ary[(h->first % h->max_elem) * h->size_elem];
    h->nr_elem--;
    h->first++;
    memcpy(dst_value, src, h->size_elem);
    // to make time response irregular
    wait_rnd_ms(waiting_ms / 5, waiting_ms);
    pthread_cond_broadcast(&h->c_not_full);
    pthread_mutex_unlock(&h->m);
    return 1;
fail:
    pthread_mutex_unlock(&h->m);
    return 0;
}

int xTaskCreate(xTask_t task_call, const char *task_name, size_t stack_size,
                void *task_parm, unsigned int priority,
                pthread_t *task_handle) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);                     // FIXME
    pthread_attr_setstacksize(&attr, stack_size); // FIXME

    pthread_create(task_handle, &attr, task_call, task_parm);
    pthread_setname_np(*task_handle, task_name);
    pthread_attr_destroy(&attr);
    return 0;
}
