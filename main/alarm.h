#ifndef ALARM_H_
#define ALARM_H_

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_err.h"

esp_err_t alarm_create(int hour, int min, int sec, int utc_offset, TimerCallbackFunction_t callback);

void alarm_set(int hour, int min, int sec, int utc_offset);

time_t alarm_get_trigger_time();

void alarm_disable();

#endif // ALARM_H_