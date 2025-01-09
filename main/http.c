#include "gpio.h"
#include "alarm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "esp_system.h"
#include <sys/time.h>
#include <string.h>

static const char *TAG = "HTTP_SERVICE";

esp_err_t alarm_set_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Got /aset (alarm set)");
    int hour, min, utc_offset;
    char header_value[10];

    if (httpd_req_get_hdr_value_str(req, "hour", header_value,
    sizeof(header_value)) != ESP_OK) {
        ESP_LOGE(TAG, "Hour header missing");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    hour = atoi(header_value);

    if (httpd_req_get_hdr_value_str(req, "min", header_value,
    sizeof(header_value)) != ESP_OK) {
        ESP_LOGE(TAG, "Minute header missing");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    min = atoi(header_value);

    if (httpd_req_get_hdr_value_str(req, "utc", header_value,
    sizeof(header_value)) != ESP_OK) {
        ESP_LOGE(TAG, "UTC-Offset header missing");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    utc_offset = atoi(header_value);
    
    alarm_set(hour, min, 0, utc_offset);

    char response[20];
    snprintf(response, sizeof(response), "%ld", alarm_get_trigger_time());
    ESP_LOGI(TAG, "Sending next alarm timestamp: %s", response);

    httpd_resp_set_type(req, "text/plain");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}

esp_err_t alarm_disable_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Got /adis (alarm disable)");
    alarm_disable();
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

esp_err_t alarm_next_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Got /anext (alarm next)");

    char response[20];
    snprintf(response, sizeof(response), "%ld", alarm_get_trigger_time());
    ESP_LOGI(TAG, "Sending next alarm timestamp: %s", response);

    httpd_resp_set_type(req, "text/plain");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}


esp_err_t time_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Got /time");
    time_t now;
    time(&now);

    char now_str[20];
    snprintf(now_str, sizeof(now_str), "%ld", now);
    ESP_LOGI(TAG, "Sending timestamp: %s", now_str);

    httpd_resp_set_type(req, "text/plain");
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, now_str, strlen(now_str));
    return ESP_OK;
}

esp_err_t reset_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Got /reset");
    fflush(stdout);
    esp_restart();
    return ESP_OK;
}

esp_err_t synct_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Got /synct");

    char header_value[20];
    if (httpd_req_get_hdr_value_str(req, "time_t", header_value,
    sizeof(header_value)) != ESP_OK) {
        ESP_LOGE(TAG, "Hour header missing");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    time_t now = (time_t)atol(header_value);
    struct timeval tv = { .tv_sec = now, .tv_usec = 0 };
    settimeofday(&tv, NULL);
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

esp_err_t toggle_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Got /toggle");
	gpio_toggle();
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

esp_err_t off_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Got /off");
	gpio_set_low();
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, NULL, 0);
	return ESP_OK;
}

esp_err_t on_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Got /on");
	gpio_set_high();
    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, NULL, 0);
	return ESP_OK;
}
httpd_uri_t alarm_set_ = {
    .uri       = "/aset",
    .method    = HTTP_POST,
    .handler   = alarm_set_handler,
    .user_ctx  = NULL
};

httpd_uri_t alarm_disable_ = {
    .uri       = "/adis",
    .method    = HTTP_POST,
    .handler   = alarm_disable_handler,
    .user_ctx  = NULL
};

httpd_uri_t alarm_next = {
    .uri       = "/anext",
    .method    = HTTP_GET,
    .handler   = alarm_next_handler,
    .user_ctx  = NULL
};

httpd_uri_t time_ = {
    .uri       = "/time",
    .method    = HTTP_GET,
    .handler   = time_handler,
    .user_ctx  = NULL
};

httpd_uri_t reset = {
    .uri       = "/reset",
    .method    = HTTP_POST,
    .handler   = reset_handler,
    .user_ctx  = NULL
};

httpd_uri_t synct = {
    .uri       = "/synct",
    .method    = HTTP_POST,
    .handler   = synct_handler,
    .user_ctx  = NULL
};

httpd_uri_t toggle = {
    .uri       = "/toggle",
    .method    = HTTP_POST,
    .handler   = toggle_handler,
    .user_ctx  = NULL
};

httpd_uri_t off = {
    .uri       = "/off",
    .method    = HTTP_POST,
    .handler   = off_handler,
    .user_ctx  = NULL
};

httpd_uri_t on = {
    .uri       = "/on",
    .method    = HTTP_POST,
    .handler   = on_handler,
    .user_ctx  = NULL
};

httpd_handle_t http_server_init() {
    esp_err_t res;
	ESP_LOGI(TAG, "Initiating HTTP server");

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    ESP_LOGI(TAG, "Port: %d", config.server_port);

    ESP_LOGI(TAG, "httpd_start()");
    res = httpd_start(&server, &config);
    ESP_ERROR_CHECK(res);

    ESP_LOGI(TAG, "httpd_register_uri_handler()");
    res = httpd_register_uri_handler(server, &toggle);
    ESP_ERROR_CHECK(res);
    res = httpd_register_uri_handler(server, &off);
    ESP_ERROR_CHECK(res);
    res = httpd_register_uri_handler(server, &on);
    ESP_ERROR_CHECK(res);
    res = httpd_register_uri_handler(server, &synct);
    ESP_ERROR_CHECK(res);
    res = httpd_register_uri_handler(server, &time_);
    ESP_ERROR_CHECK(res);
    res = httpd_register_uri_handler(server, &alarm_set_);
    ESP_ERROR_CHECK(res);
    res = httpd_register_uri_handler(server, &alarm_disable_);
    ESP_ERROR_CHECK(res);
    res = httpd_register_uri_handler(server, &alarm_next);
    ESP_ERROR_CHECK(res);
    //res = httpd_register_uri_handler(server, &reset);
   // ESP_ERROR_CHECK(res);
    ESP_LOGI(TAG, "Success");
    return NULL;
}



