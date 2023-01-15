#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include "sensor.h"

#define PORT 1234
#define TIMEOUT_SEC 30

char *TAG = "tcp";

void tcp_server_task(void *pvParameters)
{
    int sock = -1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create a socket that uses an internet IPv4 address,
    //  Set the socket to be stream based (TCP), and set the socket to be non-blocking
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the server address
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        ESP_LOGE(TAG, "Unable to bind socket: errno %d", errno);
        vTaskDelete(NULL);
    }

    // Set the socket to listen mode
    if (listen(sock, 5) < 0)
    {
        ESP_LOGE(TAG, "Listen failed: errno %d", errno);
        vTaskDelete(NULL);
    }

    ESP_LOGI(TAG, "Socket created, listening on port: %d", PORT);

    sensor_data temp;
    sensor_data measurements[SENSOR_MAX_CACHE] = {0};
    uint8_t index = 0;

    while (1)
    {
        int client_sock = accept(sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0)
        {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            continue;
        }
        ESP_LOGI(TAG, "Connection accepted");

        xEventGroupSetBits(sensor_events, SENSOR_SOCKET_CONNECTED);
        
        while (1)
        {
            if (index < SENSOR_MAX_CACHE && uxQueueMessagesWaiting(sensorStreamBuffer) > 0)
            {
                xQueueReceive(sensorStreamBuffer, &temp, 0);
                memcpy(&measurements[index++], &temp, sizeof(sensor_data));
            }

            if (index == SENSOR_MAX_CACHE)
            {
                if (write(client_sock, measurements, sizeof(measurements)) < 0)
                {
                    index = 0;
                    break;
                }
                index = 0;
            }
            vTaskDelay(1);
        }

        if (shutdown(client_sock, 0) < 0)
        {
            ESP_LOGE(TAG, "Error occurred during shutdown: errno %d", errno);
        }

        close(client_sock);
        xEventGroupClearBits(sensor_events, SENSOR_SOCKET_CONNECTED);
        xQueueReset(sensorStreamBuffer);
    }
}