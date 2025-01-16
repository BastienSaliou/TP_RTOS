#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "mux_demux.h"
#include "package.h"

// Configuration
#define TRAME_SIZE 18
#define QUEUE_LENGTH 10
#define WAIT_TIME_MS 500
#define STACK_SIZE 150

#define HEADER_X 0x01
#define HEADER_Y 0x02
#define HEADER_Z 0x03

// Macros pour affichage sécurisé
#define SAFE_PRINTF(fmt, ...)        \
    do {                             \
        taskENTER_CRITICAL();        \
        printf(fmt, ##__VA_ARGS__);  \
        fflush(stdout);              \
        taskEXIT_CRITICAL();         \
    } while (0)


QueueHandle_t queue_file_1, queue_file_2, queue_file_3;

// Gestion des débordements
void handle_queue_overflow(const char *queue_name) {
    SAFE_PRINTF("[ERREUR] Débordement dans %s\n", queue_name);
}

// Tâche de génération des trames
void Task_GenerateFrames(void *pvParameters) {
    uint32_t timestamp = 0;
    uint8_t trame[TRAME_SIZE];

    while (1) {
        ChannelData x = generate_channel_data(HEADER_X);
        ChannelData y = generate_channel_data(HEADER_Y);
        ChannelData z = generate_channel_data(HEADER_Z);

        Status status = {0x04, 0x00};
        multiplex(trame + 4, x, y, z, status);

        memcpy(trame, &timestamp, sizeof(timestamp));

        if (xQueueSend(queue_file_1, trame, pdMS_TO_TICKS(WAIT_TIME_MS)) != pdPASS) {
            handle_queue_overflow("queue_file_1");
        }
        if (xQueueSend(queue_file_2, trame, pdMS_TO_TICKS(WAIT_TIME_MS)) != pdPASS) {
            handle_queue_overflow("queue_file_2");
        }
        if (xQueueSend(queue_file_3, trame, pdMS_TO_TICKS(WAIT_TIME_MS)) != pdPASS) {
            handle_queue_overflow("queue_file_3");
        }

        timestamp++;
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 seconde
    }
}

// Tâche de traitement des données
void Task_ProcessQueue(void *pvParameters) {
    QueueHandle_t queue = (QueueHandle_t)pvParameters;
    uint8_t trame[TRAME_SIZE];

    while (1) {
        if (xQueueReceive(queue, trame, pdMS_TO_TICKS(WAIT_TIME_MS)) == pdPASS) {
            uint32_t timestamp;
            memcpy(&timestamp, trame, sizeof(timestamp));

            ChannelData x = {0}, y = {0}, z = {0};
            Status status = {0};
            demultiplex(trame + 4, &x, &y, &z, &status);

            swap_channel_data(&x, &y, &z);

            SAFE_PRINTF("[TRAITEMENT] Timestamp: %u | X: %02X %02X %02X %02X | Y: %02X %02X %02X %02X | Z: %02X %02X %02X %02X\n",
                        timestamp,
                        x.header, x.data[0], x.data[1], x.data[2],
                        y.header, y.data[0], y.data[1], y.data[2],
                        z.header, z.data[0], z.data[1], z.data[2]);
        }
    }
}

// Tâche de calcul de la charge système
void Task_CalculCharge(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(5000);

    while (1) {
        UBaseType_t idle_count = uxTaskGetIdleRunTimeCounter();
        SAFE_PRINTF("[CHARGE] Charge système : %lu%%\n", 100 - idle_count);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}


int main(void) {
    
    queue_file_1 = xQueueCreate(QUEUE_LENGTH, TRAME_SIZE);
    queue_file_2 = xQueueCreate(QUEUE_LENGTH, TRAME_SIZE);
    queue_file_3 = xQueueCreate(QUEUE_LENGTH, TRAME_SIZE);

    if (!queue_file_1 || !queue_file_2 || !queue_file_3) {
        SAFE_PRINTF("[ERREUR] Échec de création des files.\n");
        return -1;
    }
    // Création des tâches
    xTaskCreate(Task_GenerateFrames, "GenerateFrames", STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(Task_ProcessQueue, "ProcessQueue1", STACK_SIZE, queue_file_1, 1, NULL);
    xTaskCreate(Task_ProcessQueue, "ProcessQueue2", STACK_SIZE, queue_file_2, 1, NULL);
    xTaskCreate(Task_ProcessQueue, "ProcessQueue3", STACK_SIZE, queue_file_3, 1, NULL);
    xTaskCreate(Task_CalculCharge, "CalculCharge", STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();

    return -1;
}
