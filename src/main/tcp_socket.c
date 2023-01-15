#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include "sdp600.h"

#define PORT 1234
#define TIMEOUT_SEC 30

char *TAG = "tcp";

volatile bool run = false;

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

    sdp600_soft_reset();
    vTaskDelay(100);

    while (1)
    {
        int client_sock = accept(sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0)
        {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            continue;
        }
        ESP_LOGI(TAG, "Connection accepted");

        int16_t measurements[50];
        int16_t data;
        uint8_t index = 0;
        
        while (1)
        {

            /* Collect 50 measurements before sending them over the socket */
            sdp600_read(&data);
            measurements[index++] = data;

            if (index == 50)
            {
                if (write(client_sock, &measurements, sizeof(measurements)) < 0)
                {
                    index = 0;
                    break;
                }
                index = 0;
            }
        }

        if (shutdown(client_sock, 0) < 0)
        {
            ESP_LOGE(TAG, "Error occurred during shutdown: errno %d", errno);
        }
        close(client_sock);
    }
}
