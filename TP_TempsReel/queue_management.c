#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Byte.h"
#include "package.h"
#include "mux_demux.h"

#define QUEUE_SIZE 10
#define SAMPLE_RATE 1 // 1 Hz
#define TRAME_SIZE 14

typedef struct {
    uint8_t trame[TRAME_SIZE];
    uint32_t timestamp;
} QueueElement;

typedef struct {
    QueueElement elements[QUEUE_SIZE];
    int front;
    int rear;
    int count;
} CircularQueue;

// Function prototypes
void initQueue(CircularQueue *queue);
int enqueue(CircularQueue *queue, QueueElement element);
int dequeue(CircularQueue *queue, QueueElement *element);
int isQueueFull(CircularQueue *queue);
int isQueueEmpty(CircularQueue *queue);
void delay(int seconds);

// Initialize the queue
void initQueue(CircularQueue *queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
}

// Add an element to the queue
int enqueue(CircularQueue *queue, QueueElement element) {
    if (isQueueFull(queue)) {
        printf("Queue overflow detected!\n");
        return -1; // Queue is full
    }
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    queue->elements[queue->rear] = element;
    queue->count++;
    return 0; // Success
}

// Remove an element from the queue
int dequeue(CircularQueue *queue, QueueElement *element) {
    if (isQueueEmpty(queue)) {
        printf("Queue underflow detected!\n");
        return -1; // Queue is empty
    }
    *element = queue->elements[queue->front];
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    queue->count--;
    return 0; // Success
}

// Check if the queue is full
int isQueueFull(CircularQueue *queue) {
    return queue->count == QUEUE_SIZE;
}

// Check if the queue is empty
int isQueueEmpty(CircularQueue *queue) {
    return queue->count == 0;
}

// Simulate a delay (in seconds)
void delay(int seconds) {
    clock_t start_time = clock();
    while (clock() < start_time + seconds * CLOCKS_PER_SEC);
}

int main() {
    CircularQueue queue;
    initQueue(&queue);

    uint32_t timestamp = 0; // Simulated timestamp

    while (1) {
        // Generate and multiplex data
        ChannelData x = { .header = 0x01 };
        ChannelData y = { .header = 0x02 };
        ChannelData z = { .header = 0x03 };
        Status status = generate_status();

        for (int i = 0; i < 3; i++) {
            x.data[i] = generate_random_data();
            y.data[i] = generate_random_data();
            z.data[i] = generate_random_data();
        }

        uint8_t trame[TRAME_SIZE];
        multiplex(trame, x, y, z, status);

        QueueElement element;
        memcpy(element.trame, trame, TRAME_SIZE);
        element.timestamp = timestamp;

        if (enqueue(&queue, element) == 0) {
            printf("Enqueued: Timestamp=%d, Trame=", element.timestamp);
            for (int i = 0; i < TRAME_SIZE; i++) {
                printf("%02X ", element.trame[i]);
            }
            printf("\n");
        }

        // Simulate a sampling rate of 1 Hz
        delay(SAMPLE_RATE);
        timestamp++;

        // Dequeue and demultiplex elements (example of processing loop)
        while (!isQueueEmpty(&queue)) {
            QueueElement dequeued_element;
            if (dequeue(&queue, &dequeued_element) == 0) {
                printf("Dequeued: Timestamp=%d, Trame=", dequeued_element.timestamp);
                for (int i = 0; i < TRAME_SIZE; i++) {
                    printf("%02X ", dequeued_element.trame[i]);
                }
                printf("\n");

                ChannelData x_demux = {0}, y_demux = {0}, z_demux = {0};
                Status status_demux = {0};

                demultiplex(dequeued_element.trame, &x_demux, &y_demux, &z_demux, &status_demux);

                printf("Demultiplexed: X=[%02X %02X %02X %02X], Y=[%02X %02X %02X %02X], Z=[%02X %02X %02X %02X], Status= %02X %02X\n",
                       x_demux.header, x_demux.data[0], x_demux.data[1], x_demux.data[2],
                       y_demux.header, y_demux.data[0], y_demux.data[1], y_demux.data[2],
                       z_demux.header, z_demux.data[0], z_demux.data[1], z_demux.data[2],
                       status_demux.header, status_demux.status);
            }
        }
    }

    return 0;
}
