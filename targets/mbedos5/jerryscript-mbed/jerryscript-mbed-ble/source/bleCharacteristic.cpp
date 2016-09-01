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
#include "jerry-core/jerry-api.h"

#include "jerryscript-mbed-event-loop/EventLoop.h"
#include "jerryscript-mbed-ble/ble-js.h"
#include "jerryscript-mbed-ble/BLEJS.h"

#include "ble/BLE.h"

#include "Callback.h"

void BLECharacteristic__destructor(uintptr_t native_ptr) {
    LOG_PRINT_ALWAYS("BLECharacteristic: destructor\r\n");
    GattCharacteristic *data = (GattCharacteristic*)native_ptr;
    delete data;
}

DECLARE_CLASS_FUNCTION(BLECharacteristic, write) {
    printf("BLECharacteristic.write\r\n");

    CHECK_ARGUMENT_COUNT(BLECharacteristic, write, args_count==1);
    CHECK_ARGUMENT_TYPE_ALWAYS(BLECharacteristic, write, 0, array);

    // get the native pointer
    uintptr_t native_handle;
    jerry_get_object_native_handle(this_obj, &native_handle);

    GattCharacteristic *native_ptr = (GattCharacteristic*)native_handle;

    // get the length of the array
    size_t data_length = jerry_get_array_length(args[0]);
    uint8_t *buffer = (uint8_t*)malloc(data_length);

    for (size_t i = 0; i < data_length; i++) {
        jerry_value_t val = jerry_get_property_by_index(args[0], i);
        uint8_t byte_val = uint8_t(jerry_get_number_value(val));
        jerry_release_value(val);
        buffer[i] = byte_val;
    }

    BLE::Instance().gattServer().write(native_ptr->getValueHandle(), buffer, data_length);

    free(buffer);

    return jerry_create_undefined();
}

/**
 * GattCharacteristic:
 * - uuid
 * - properties (read/write/notify etc.)
 * - max length (bytes)
 * - initial value
 */
DECLARE_CLASS_CONSTRUCTOR(BLECharacteristic) {
    CHECK_ARGUMENT_COUNT(BLECharacteristic, __constructor, (args_count == 3 || args_count == 4));
    CHECK_ARGUMENT_TYPE_ALWAYS(BLECharacteristic, __constructor, 0, string);
    CHECK_ARGUMENT_TYPE_ALWAYS(BLECharacteristic, __constructor, 1, array);
    CHECK_ARGUMENT_TYPE_ALWAYS(BLECharacteristic, __constructor, 2, number);
    CHECK_ARGUMENT_TYPE_ON_CONDITION(BLECharacteristic, __constructor, 3, array, args_count == 4);

    jerry_value_t service_uuid = args[0];

    // unwrap the uuid
    char uuid_buf[4] = {0};
    jerry_string_to_char_buffer(service_uuid, (jerry_char_t*)uuid_buf, 4);
    jerry_release_value(service_uuid);
    uint16_t uuid = hex_str_to_u16(uuid_buf, sizeof(uuid_buf));

    uint8_t props = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE;

    // properties: read, writeWithoutResponse, write, notify, indicate
    uint32_t property_length = jerry_get_array_length(args[1]);
    for (uint32_t i = 0; i < property_length; i++) {
        char property[20] = {0};
        jerry_value_t obj = jerry_get_property_by_index(args[1], i);
        jerry_string_to_char_buffer(obj, (jerry_char_t*)property, sizeof(property));

        // just check the necessary characters to disambiguate the words,
        // since this will be faster.
        if (property[0] == 'r') {
            props |= GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ;
        } else if (property[0] == 'w') {
            if (property[5] == 'W') {
                props |= GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE;
            } else {
                props |= GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE;
            }
        } else if (property[0] == 'n') {
            props |= GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY;
        } else if (property[0] == 'i') {
            props |= GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE;
        }
    }

    // unwrap the buffer size
    size_t buffer_size = 23;

    uint8_t *buffer = (uint8_t*)calloc(buffer_size, 1);
    GattCharacteristic *characteristic = new GattCharacteristic(uuid, buffer, buffer_size, buffer_size, props);
    //char_data_t *char_data = new char_data_t { buffer, buffer_size, characteristic };
    uintptr_t native_ptr = (uintptr_t)characteristic;

    // create the jerryscript object
    jerry_value_t js_object = jerry_create_object();
    jerry_set_object_native_handle(js_object, native_ptr, NULL);

    ATTACH_CLASS_FUNCTION(js_object, BLECharacteristic, write);

    return js_object;
}
