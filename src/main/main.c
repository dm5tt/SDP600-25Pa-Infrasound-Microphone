#include <stdio.h>

#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"

#include "nvs_flash.h"

#include <driver/i2c.h>
#include <esp_log.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "sdp600.h"
#include "tcp_socket.h"
#include "wifi.h"

void app_main(void)
{
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    wifi_init_sta();

    sdp600_i2c_init();

    xTaskCreate(tcp_server_task, "tcp_server", 10000, NULL, 8, NULL);
}