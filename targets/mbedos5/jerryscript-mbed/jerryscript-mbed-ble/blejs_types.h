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
#ifndef _JERRYSCRIPT_MBED_BLE_BLEJS_TYPES_H
#define _JERRYSCRIPT_MBED_BLE_BLEJS_TYPES_H

#include "ble/BLE.h"

typedef struct {
    GattService *service;
    GattCharacteristic **characteristics;
    size_t characteristics_count;
} js_ble_service_data_t;

#endif // _JERRYSCRIPT_MBED_BLE_BLEJS_TYPES_H
