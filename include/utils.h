#include "FreeRTOS.h"

#include "nvs_flash.h"

char *get_key_string_value(nvs_handle nvs_handler, const char *key);