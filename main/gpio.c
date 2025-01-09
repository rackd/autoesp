#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

#define BUTTON_PIN 		 0 
#define RELAY_PIN        12
#define LED_PIN			 13

static const char *TAG = "GPIO_SERVICE";

void gpio_init() {
	ESP_LOGI(TAG, "Initiating GPIO");

	ESP_LOGI(TAG, "gpio_config()");
	gpio_config_t io_conf;
	io_conf.pin_bit_mask = ((uint64_t)1 << RELAY_PIN);
	io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&io_conf);

	gpio_config_t io_conf2;
	io_conf2.pin_bit_mask = ((uint64_t)1 << LED_PIN);
	io_conf2.mode = GPIO_MODE_OUTPUT;
    io_conf2.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf2.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf2.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&io_conf2);

	ESP_LOGI(TAG, "Success");
}

void gpio_set_low() {
	ESP_LOGI(TAG, "gpio_set_level()");
	esp_err_t res = gpio_set_level(RELAY_PIN, 0);
	ESP_ERROR_CHECK(res);
}

void gpio_set_high() {
	ESP_LOGI(TAG, "gpio_set_level()");
	esp_err_t res = gpio_set_level(RELAY_PIN, 1);
	ESP_ERROR_CHECK(res);
}

int gpio_get_state() {
	ESP_LOGI(TAG, "gpio_get_level()");
	return gpio_get_level(RELAY_PIN);
}

void gpio_toggle() {
	ESP_LOGI(TAG, "gpio_set_level()");
	esp_err_t res = gpio_set_level(RELAY_PIN, !gpio_get_state());
	ESP_ERROR_CHECK(res);
}