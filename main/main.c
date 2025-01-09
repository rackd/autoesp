#include "gpio.h"
#include "wifi.h"
#include "http.h"
#include "esp_log.h"

void app_main() {
	ESP_LOGI("MAIN", "APP START");
	gpio_init();
	wifi_init();
	http_server_init();
}
