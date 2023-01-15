#include <stdio.h>
#include "lwip/apps/sntp.h"


#include "sensor.h"
#include "sdp600.h"

QueueHandle_t sensorStreamBuffer;
EventGroupHandle_t sensor_events;

void sensor_init()
{
    sensorStreamBuffer = xQueueCreate(SENSOR_MAX_CACHE, sizeof(sensor_data));
    sensor_events =  xEventGroupCreate();
}

void sensor_task(void *pvParameters)
{
    sensor_data data_tx = {0};
    struct timeval now;

    sdp600_soft_reset();

    while (1)
    {

        /* Only start sampling when we have a valid TCP connection to the reception side */
        xEventGroupWaitBits(sensor_events, SENSOR_SOCKET_CONNECTED, pdFALSE, pdTRUE, portMAX_DELAY);

        sdp600_read(&data_tx.pres);

        gettimeofday(&now, NULL);
        data_tx.timestamp = (uint64_t)now.tv_sec * 1000000ULL + (uint64_t)now.tv_usec;

        xQueueSend(sensorStreamBuffer, &data_tx, (TickType_t)10);

        vTaskDelay(1);
    }
}