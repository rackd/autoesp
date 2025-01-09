#include "alarm.h"
#include <time.h>
#include "esp_log.h"

static const char *TAG = "ALARM_SERVICE";
static TimerHandle_t alarm_timer = NULL;
static time_t next_alarm_epoch = -1;
static int g_hour = -1;
static int g_min = -1;
static int g_sec = -1;
static int g_utc_offset = -1;

void alarm_callback(TimerHandle_t xTimer) {
    ESP_LOGI("AlarmCallback", "Daily alarm triggered!");

    next_alarm_epoch += 24 * 60 * 60;

    struct tm next_time;
    localtime_r(&next_alarm_epoch, &next_time);

    esp_err_t res = alarm_create(g_hour, g_min, g_sec, g_utc_offset,
        alarm_callback);
    if (res != ESP_OK) {
        ESP_LOGE(TAG, "Failed to reschedule alarm.");
    }
}

esp_err_t alarm_create(int hour, int min, int sec, int utc_offset,
TimerCallbackFunction_t callback) {
    struct tm timeinfo;
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);

    timeinfo.tm_hour = (hour - utc_offset) % 24;
    if (timeinfo.tm_hour < 0) {
        timeinfo.tm_hour += 24;
    }
    timeinfo.tm_min = min;
    timeinfo.tm_sec = sec;

    time_t alarm_time = mktime(&timeinfo);
    if (alarm_time == -1) {
        ESP_LOGE(TAG, "Failed to convert time_t");
        return ESP_FAIL;
    }

    if (alarm_time <= now) {
        alarm_time += 24 * 60 * 60;
    }

    next_alarm_epoch = alarm_time;

    int64_t delay = (int64_t)(alarm_time - now) * 1000 / portTICK_PERIOD_MS;

    if (alarm_timer != NULL) {
        xTimerStop(alarm_timer, 0);
        xTimerDelete(alarm_timer, 0);
    }

    alarm_timer = xTimerCreate("AlarmTime", pdMS_TO_TICKS(delay),
        pdFALSE, NULL, callback);
    if (alarm_timer == NULL) {
        ESP_LOGE(TAG, "Failed to create timer.");
        return ESP_FAIL;
    }

    if (xTimerStart(alarm_timer, 0) != pdPASS) {
        ESP_LOGE(TAG, "Failed to start timer.");
        return ESP_FAIL;
    }

    return ESP_OK;
}

void alarm_disable() {
    if (alarm_timer != NULL) {
        xTimerStop(alarm_timer, 0);
        xTimerDelete(alarm_timer, 0);
        alarm_timer = NULL;
    }
    next_alarm_epoch = -1;
    ESP_LOGI(TAG, "Alarm disabled.");
}

time_t alarm_get_trigger_time() {
    if (alarm_timer != NULL) {
        return next_alarm_epoch;
    } else {
        return -1;
    }
}

void alarm_set(int hour, int min, int sec, int utc_offset) {
	ESP_LOGI(TAG, "Initiating alarm");
    ESP_LOGI(TAG, "Hour: %i, Min: %i, Sec: %i, Utc Offset: %i", hour, min, sec,
        utc_offset);

    esp_err_t res = alarm_create(hour, min, sec, utc_offset, alarm_callback);
    ESP_ERROR_CHECK(res);

    ESP_LOGI(TAG, "Success");
}