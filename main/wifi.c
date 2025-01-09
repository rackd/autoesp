#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_event_base.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#define WIFI_SSID    "ssid"
#define WIFI_PASS    "password"

static const char *TAG = "WIFI_SERVICE";

void wifi_init() {
	ESP_LOGI(TAG, "Initiating Wifi");
	ESP_LOGI(TAG, "SSID: %s", WIFI_SSID);

	esp_err_t res;
	ESP_LOGI(TAG, "nvs_flash_init()");
	res = nvs_flash_init();
	ESP_ERROR_CHECK(res);
	
	ESP_LOGI(TAG, "esp_netif_init()");
	res = esp_netif_init();
	ESP_ERROR_CHECK(res);
	
	ESP_LOGI(TAG, "esp_event_loop_create_default()");
	res = esp_event_loop_create_default();
	ESP_ERROR_CHECK(res);
	
	ESP_LOGI(TAG, "esp_wifi_init()");
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	res = esp_wifi_init(&cfg);
	ESP_ERROR_CHECK(res);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
			.ssid_len = strlen(WIFI_SSID),
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
			.ssid_hidden = 0,
			.max_connection = 4
        },
    };
	
	ESP_LOGI(TAG, "esp_wifi_set_mode()");
	res = esp_wifi_set_mode(WIFI_MODE_AP);
    ESP_ERROR_CHECK(res);
	
	ESP_LOGI(TAG, "esp_wifi_set_config()");
	res = esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
	ESP_ERROR_CHECK(res);
	
	ESP_LOGI(TAG, "esp_wifi_start()");
	res = esp_wifi_start();
	ESP_ERROR_CHECK(res);
	
	ESP_LOGI(TAG, "Success");
}
