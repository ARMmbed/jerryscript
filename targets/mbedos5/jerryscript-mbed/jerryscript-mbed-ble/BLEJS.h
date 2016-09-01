/* Copyright (c) 2016 ARM Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _JERRYSCRIPT_MBED_BLE_BLEJS_H
#define _JERRYSCRIPT_MBED_BLE_BLEJS_H
#include "jerry-core/jerry-api.h"

#include "jerryscript-mbed-event-loop/EventLoop.h"
#include "jerryscript-mbed-ble/ble-js.h"

#include "Callback.h"
#include "ble/BLE.h"

typedef struct {
    uint8_t *buffer;
    size_t buffer_length;
    GattCharacteristic *characteristic;
} char_data_t;

static uint16_t hex_str_to_u16(const char* buf, const size_t buf_size) {
    if (buf_size > 4) {
        return 0;
    }

    uint16_t ans = 0;
    for (size_t i = 0; i < buf_size; i++) {
        ans <<= 4;
        if (buf[i] >= 'A' && buf[i] <= 'F') {
            ans |= (buf[i] - 'A') + 10;
        } else if (buf[i] >= 'a' && buf[i] <= 'f') {
            ans |= (buf[i] - 'a') + 10;
        } else if (buf[i] >= '0' && buf[i] <= '9') {
            ans |= (buf[i] - '0');
        } else {
            return 0;
        }
    }

    return ans;
}

class BLEJS {
 public:
    BLEJS(BLE &ble) : ble(ble) {
        this_obj = jerry_create_null();
    }

    ~BLEJS() {
    }

    void init(jerry_value_t f) {
        if (!jerry_value_is_function(f)) {
            return;
        }

        init_cb_function = f;
        ble.init(this, &BLEJS::initComplete);
    }

    void startAdvertising(jerry_value_t device_name_js, jerry_value_t service_uuids) {
        size_t device_name_length = jerry_get_string_length(device_name_js);
        uint32_t uuids_length = jerry_get_array_length(service_uuids);

        // add an extra character to ensure there's a null character after the device name
        char* device_name = (char*)calloc(device_name_length + 1, sizeof(char));
        jerry_string_to_char_buffer(device_name_js, (jerry_char_t*)device_name, device_name_length);

        // build an array of 16-bit uuids
        uint16_t* uuids = (uint16_t*)calloc(uuids_length, sizeof(uint16_t));

        for (uint32_t i = 0; i < uuids_length; i++) {
            char buf[5] = {0};
            jerry_value_t uuid_str_obj = jerry_get_property_by_index(service_uuids, i);

            if (!jerry_value_is_string(uuid_str_obj)) {
                LOG_PRINT_ALWAYS("invalid uuid argument (%u). Ignoring.\r\n", i);
                jerry_release_value(uuid_str_obj);
                continue;
            }

            jerry_string_to_char_buffer(uuid_str_obj, (jerry_char_t*)buf, 4);
            jerry_release_value(uuid_str_obj);

            uuids[i] = hex_str_to_u16(buf, 4);
        }

        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuids, uuids_length * 2);
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)device_name, device_name_length);
        ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
        ble.gap().setDeviceName((const uint8_t*)device_name);
        ble.gap().setAdvertisingInterval(1000); /* 1000ms. */
        ble.gap().startAdvertising();

        free(uuids);
        free(device_name);
    }

    void startAdvertising() {
        ble.gap().startAdvertising();
    }

    void stopAdvertising() {
        ble.stopAdvertising();
    }

    void addService(GattService *srvc) {
        ble.addService(*srvc);
    }

    void onConnection(jerry_value_t f) {
        connect_cb_function = f;
        ble.gap().onConnection(this, &BLEJS::connectionCallback);
    }

    void onDisconnection(jerry_value_t f) {
        disconnect_cb_function = f;
        ble.gap().onDisconnection(this, &BLEJS::disconnectionCallback);
    }

    bool isConnected() {
      return ble.gap().getState().connected;
    }

 private:
    void initComplete(BLE::InitializationCompleteCallbackContext *context) {
        if (context->error != BLE_ERROR_NONE) {
            LOG_PRINT_ALWAYS("Error while initialising BLE context\r\n");
            return;
        }

        if (jerry_value_is_function(init_cb_function)) {
            jerry_call_function(init_cb_function, this_obj, NULL, 0);
        }
    }

    void connectionCallback(const Gap::ConnectionCallbackParams_t *params) {
        if (jerry_value_is_function(connect_cb_function)) {
            jerry_call_function(connect_cb_function, this_obj, NULL, 0);
        }
    }

    void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params) {
        if (jerry_value_is_function(disconnect_cb_function)) {
            jerry_call_function(disconnect_cb_function, this_obj, NULL, 0);
        }
    }

    jerry_value_t this_obj;
    jerry_value_t init_cb_function;
    jerry_value_t connect_cb_function;
    jerry_value_t disconnect_cb_function;

    BLE& ble;
};

#endif // _JERRYSCRIPT_MBED_BLE_BLEJS_H
