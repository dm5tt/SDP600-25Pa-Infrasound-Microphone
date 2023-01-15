#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "nvs_flash.h"


#include "sensor.h"
#include "sntp.h"
#include "wifi.h"
#include "tcp_socket.h"
#include "sdp600.h"




void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();
    initialize_sntp();
    wait_for_sntp_sync();
    sensor_init();
    sdp600_i2c_init();

    xTaskCreate(tcp_server_task, "tcp_server", 10000, NULL, 8, NULL);
    xTaskCreate(sensor_task, "sensor_task", 10000, NULL, 9, NULL);
    xTaskCreate(update_time_task, "update_time_task", 10000, NULL, 5, NULL);
}