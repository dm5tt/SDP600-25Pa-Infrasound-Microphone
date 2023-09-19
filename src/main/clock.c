#include <stdio.h>
#include <time.h>
#include "esp_log.h"
#include "esp_sntp.h"

#define TEN_MINUTES_DELAY_TICKS (10 * 60 * (TickType_t)1000)


static const char *TAG = "SNTP";

void clock_init()
{
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();
}

void clock_sync()
{
    ESP_LOGI(TAG, "Waiting for SNTP synchronization...");
    time_t now = 0;
    struct tm timeinfo = {0};

    int retry = 0;
    const int retry_count = 10;
    while (timeinfo.tm_year < (2020 - 1900) && ++retry < retry_count)
    {
        ESP_LOGI(TAG, "Waiting for SNTP... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (timeinfo.tm_year < (2020 - 1900))
    {
        ESP_LOGW(TAG, "SNTP synchronization failed");
    }
    else
    {
        ESP_LOGI(TAG, "SNTP synchronized. Timestamp: %04d-%02d-%02d %02d:%02d:%02d",
                 timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                 timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    }
}
void clock_task(void *pvParameters)
{
    while (1)
    {
        // Warte auf erfolgreiche Synchronisierung mit dem NTP-Server
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

        // Warte zusätzliche 100 Millisekunden zur Sicherheit
        vTaskDelay(100 / portTICK_PERIOD_MS);

        // Hole die synchronisierte Zeit von der NTP
        struct timeval now;
        gettimeofday(&now, NULL);

        // Zeige die aktualisierte Uhrzeit an
        ESP_LOGI(TAG, "Refreshed SNTP Time: %s", ctime(&now.tv_sec));
        

        vTaskDelay(TEN_MINUTES_DELAY_TICKS);
    }
}