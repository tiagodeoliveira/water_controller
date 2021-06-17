#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_system.h"
#include "esp_wifi.h"

#include "nvs_flash.h"

#include "iot_system_init.h"
#include "iot_wifi.h"

#include "utils.h"

#define PROVISON_PARTITION_NAME "provision"
#define PROVISON_NAMESPACE_NAME "keys"

#define CREDENTIALS_PARTITION_NAME "credentials"
#define CREDENTIALS_NAMESPACE_NAME "secrets"

static const char *TAG = "JIT_PROVISION";
WIFINetworkParams_t xNetworkParams;

static void init_system(void) {
  esp_err_t ret = nvs_flash_init();
  if ((ret == ESP_ERR_NVS_NO_FREE_PAGES) ||
      (ret == ESP_ERR_NVS_NEW_VERSION_FOUND)) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  if (SYSTEM_Init() == pdPASS) {
  }
}

static nvs_handle init_partition(char *name, char *namespace) {
  nvs_handle nvs_handle;
  esp_err_t ret = nvs_flash_init_partition(name);

  if ((ret == ESP_ERR_NVS_NO_FREE_PAGES) ||
      (ret == ESP_ERR_NVS_NEW_VERSION_FOUND)) {
    ESP_ERROR_CHECK(nvs_flash_erase_partition(name));
    ESP_ERROR_CHECK(nvs_flash_init_partition(name));
  }

  ESP_ERROR_CHECK(
      nvs_open_from_partition(name, namespace, NVS_READWRITE, &nvs_handle));
  ESP_LOGI(TAG, "Partition correctly loaded!");

  return nvs_handle;
}

void load_params() {
  nvs_handle creds_handle =
      init_partition(CREDENTIALS_PARTITION_NAME, CREDENTIALS_NAMESPACE_NAME);
  nvs_handle provision_handle =
      init_partition(PROVISON_PARTITION_NAME, PROVISON_NAMESPACE_NAME);

  char *ssid = NULL;
  char *ssid_pass = NULL;
  char *serial_number = NULL;

  ESP_LOGI(TAG, "Loading parameters from provision partition!");
  ssid = get_key_string_value(provision_handle, "ssid");
  ssid_pass = get_key_string_value(provision_handle, "ssid_pass");
  serial_number = get_key_string_value(provision_handle, "serial_number");

  if (ssid == NULL || ssid_pass == NULL || serial_number == NULL) {
    ESP_LOGI(TAG, "Loading parameters from credentials partition!");
    ssid = get_key_string_value(creds_handle, "ssid");
    ssid_pass = get_key_string_value(creds_handle, "ssid_pass");
    serial_number = get_key_string_value(creds_handle, "serial_number");
  }

  ESP_LOGI(TAG, "params loaded: %s, %s, %s", ssid, ssid_pass, serial_number);
  xNetworkParams.pcSSID = ssid;
  xNetworkParams.ucSSIDLength = sizeof(ssid);
  xNetworkParams.pcPassword = ssid_pass;
  xNetworkParams.ucPasswordLength = sizeof(ssid_pass);
  xNetworkParams.xSecurity = eWiFiSecurityWPA2;
}

void init_wifi() {
  tcpip_adapter_init();
  WIFIReturnCode_t xWifiStatus = WIFI_On();
  if (xWifiStatus == eWiFiSuccess) {
    ESP_LOGI(TAG, "Wifi is on");
    WIFINetworkParams_t *const pxNetworkParams = &xNetworkParams;
    ESP_LOGI(TAG, "Connecting to %s", pxNetworkParams->pcSSID);
    xWifiStatus = WIFI_ConnectAP(pxNetworkParams);
    if (xWifiStatus == eWiFiSuccess) {
      ESP_LOGI(TAG, "Wifi connected");
      return;
    }
  }

  ESP_LOGE(TAG, "Wifi failed %d", xWifiStatus);
}

void app_main() {
  init_system();
  load_params();
  init_wifi();
  ESP_LOGI(TAG, "driver started");
}
