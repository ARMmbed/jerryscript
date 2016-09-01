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
#include "jerryscript-mbed-ble/blejs_types.h"

#include "ble/BLE.h"

#include "Callback.h"

DECLARE_CLASS_FUNCTION(BLEDevice, startAdvertising) {
    CHECK_ARGUMENT_COUNT(BLEDevice, startAdvertising,
                        (args_count == 2 || args_count == 0));

    uintptr_t native_handle;
    jerry_get_object_native_handle(this_obj, &native_handle);

    BLEJS *ble = (BLEJS*)native_handle;

    // allow ble.startAdvertising() to be called to restart
    // advertising, without any configuration
    if (args_count == 0) {
        ble->startAdvertising();
        return jerry_create_undefined();
    }

    CHECK_ARGUMENT_TYPE_ALWAYS(BLEDevice, startAdvertising, 0, string);
    CHECK_ARGUMENT_TYPE_ALWAYS(BLEDevice, startAdvertising, 1, array);

    ble->startAdvertising(args[0], args[1]);

    return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(BLEDevice, isConnected) {
    CHECK_ARGUMENT_COUNT(BLEDevice, isConnected, (args_count == 0));

    uintptr_t native_handle;
    jerry_get_object_native_handle(this_obj, &native_handle);

    BLEJS *ble = (BLEJS*)native_handle;

    return jerry_create_boolean(ble->isConnected());
}

DECLARE_CLASS_FUNCTION(BLEDevice, stopAdvertising) {
    CHECK_ARGUMENT_COUNT(BLEDevice, stopAdvertising, (args_count == 0));

    uintptr_t native_handle;
    jerry_get_object_native_handle(this_obj, &native_handle);

    BLEJS *this_ble = (BLEJS*)native_handle;
    this_ble->stopAdvertising();

    return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(BLEDevice, ready) {
    CHECK_ARGUMENT_COUNT(BLEDevice, ready, (args_count == 1));
    CHECK_ARGUMENT_TYPE_ALWAYS(BLEDevice, ready, 0, function);

    uintptr_t native_handle;
    jerry_get_object_native_handle(this_obj, &native_handle);

    jerry_value_t f = args[0];
    jerry_acquire_value(f);

    BLEJS *this_ble = (BLEJS*)native_handle;
    this_ble->init(f);

    return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(BLEDevice, addServices) {
    CHECK_ARGUMENT_COUNT(BLEDevice, addServices, (args_count == 1));
    CHECK_ARGUMENT_TYPE_ALWAYS(BLEDevice, addServices, 0, array);

    uintptr_t native_handle;
    jerry_get_object_native_handle(this_obj, &native_handle);
    BLEJS *this_ble = (BLEJS*)native_handle;

    size_t service_count = jerry_get_array_length(args[0]);
    for (size_t i = 0; i < service_count; i++) {
        // we'll hope and hope and hope that this is a Service :')
        jerry_value_t service = jerry_get_property_by_index(args[0], i);

        uintptr_t service_native_handle;
        jerry_get_object_native_handle(service, &service_native_handle);

        js_ble_service_data_t *service_data = (js_ble_service_data_t*)service_native_handle;
        GattService *service_ptr = service_data->service;

        this_ble->addService(service_ptr);

        jerry_release_value(service);
    }

    return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(BLEDevice, onConnection) {
    CHECK_ARGUMENT_COUNT(BLEDevice, ready, (args_count == 1));
    CHECK_ARGUMENT_TYPE_ALWAYS(BLEDevice, ready, 0, function);

    uintptr_t native_handle;
    jerry_get_object_native_handle(this_obj, &native_handle);

    jerry_value_t f = args[0];
    jerry_acquire_value(f);

    BLEJS *this_ble = (BLEJS*)native_handle;
    this_ble->onConnection(f);

    return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(BLEDevice, onDisconnection) {
    CHECK_ARGUMENT_COUNT(BLEDevice, ready, (args_count == 1));
    CHECK_ARGUMENT_TYPE_ALWAYS(BLEDevice, ready, 0, function);

    uintptr_t native_handle;
    jerry_get_object_native_handle(this_obj, &native_handle);

    jerry_value_t f = args[0];
    jerry_acquire_value(f);

    BLEJS *this_ble = (BLEJS*)native_handle;
    this_ble->onDisconnection(f);

    return jerry_create_undefined();
}

DECLARE_CLASS_CONSTRUCTOR(BLEDevice) {
    CHECK_ARGUMENT_COUNT(BLEDevice, __constructor, (args_count == 0));
    // check and unbox arguments
    BLEJS *ble = new BLEJS(BLE::Instance());
    uintptr_t native_ptr = (uintptr_t)ble;

    // create the jerryscript object
    jerry_value_t js_object = jerry_create_object();
    jerry_set_object_native_handle(js_object, native_ptr, NULL);

    ATTACH_CLASS_FUNCTION(js_object, BLEDevice, startAdvertising);
    ATTACH_CLASS_FUNCTION(js_object, BLEDevice, stopAdvertising);
    ATTACH_CLASS_FUNCTION(js_object, BLEDevice, onConnection);
    ATTACH_CLASS_FUNCTION(js_object, BLEDevice, onDisconnection);
    ATTACH_CLASS_FUNCTION(js_object, BLEDevice, addServices);
    ATTACH_CLASS_FUNCTION(js_object, BLEDevice, ready);
    ATTACH_CLASS_FUNCTION(js_object, BLEDevice, isConnected);

    return js_object;
}
