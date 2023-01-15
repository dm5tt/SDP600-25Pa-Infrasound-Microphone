#ifndef SENSOR_H
#define SENSOR_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>

typedef struct {
    int16_t pres;
    uint64_t timestamp;
} __attribute__((__packed__)) sensor_data;


void sensor_init();
void sensor_task(void *pvParameters);

extern QueueHandle_t sensorStreamBuffer;
#define SENSOR_MAX_CACHE 50


// Deklaration der Event Group
extern EventGroupHandle_t sensor_events;
#define SENSOR_SOCKET_CONNECTED (1 << 0)


#endif