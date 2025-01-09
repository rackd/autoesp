idf_component_register(
    SRCS "alarm.c" "gpio.c" "http.c" "main.c" "wifi.c"
    INCLUDE_DIRS "."
    REQUIRES esp_event nvs_flash esp_http_server pthread
)

