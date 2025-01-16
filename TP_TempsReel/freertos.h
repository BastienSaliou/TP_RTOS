#ifndef FREERTOS_H
#define FREERTOS_H

#define RUN_BEFORE  __attribute__((constructor))
#define RUN_AFTER   __attribute__((destructor))

#include <pthread.h>

typedef unsigned char byte_t;
typedef struct xQueue_s {
    char *ary;
    unsigned int first;
    unsigned int last;
    unsigned int nr_elem;
    unsigned int max_elem;
    unsigned int size_elem;
    pthread_mutex_t m;
    pthread_cond_t c_not_empty;
    pthread_cond_t c_not_full;
} xQueue_t;
#define XQUEUE_INITIALIZER                                                     \
    {                                                                          \
        .ary = NULL, .first = 0, .last = 0, .nr_elem = 0, .max_elem = 0,       \
        .size_elem = 0, .m = PTHREAD_MUTEX_INITIALIZER,                        \
        .c_not_empty = PTHREAD_COND_INITIALIZER,                               \
        .c_not_full = PTHREAD_COND_INITIALIZER                                 \
    }

typedef xQueue_t *xQueueHandle;
typedef void *(*xTask_t)(void *);

#define XQUEUE_MAX_COUNT 5
static unsigned int xQueue_count = 0;
static xQueue_t xQueue_ary[XQUEUE_MAX_COUNT] = {XQUEUE_INITIALIZER};

void wait_ms(unsigned long ms);
void wait_rnd_ms(unsigned long min_ms, unsigned long max_ms);

xQueueHandle xQueueCreate(unsigned int max_elem, unsigned int size_elem);
void vQueueDelete(xQueueHandle handle);

int xQueueReceive(xQueueHandle h, void *dst_value, unsigned long waiting_ms);
int xQueueSendToBack(xQueueHandle h, void *src_value, unsigned long waiting_ms);
int xTaskCreate(xTask_t task_call, const char *task_name, size_t stack_size,
                void *task_parm, unsigned int priority, pthread_t *task_handle);

static void vTaskStartScheduler(void) {}

#endif
