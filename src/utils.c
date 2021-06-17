#include "FreeRTOS.h"

#include "esp_log.h"
#include "utils.h"

char *get_key_string_value(nvs_handle nvs_handler, const char *key) {
  size_t required_size = 0;

  esp_err_t err = nvs_get_str(nvs_handler, key, NULL, &required_size);

  if (err == ESP_OK && required_size > 1) {
    char *out_value = malloc(required_size);
    err = nvs_get_str(nvs_handler, key, out_value, &required_size);
    if (err == ESP_OK) {
      return out_value;
    } else {
      free(out_value);
    }
  } else {
    ESP_LOGD("UTILS", "Could not find parameter!");
  }

  return NULL;
}