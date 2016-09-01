/*
 * Automatically generated JerryScript wrappers for C++ class DigitalOut.
 *
 * ************* WARNING **************
 * THIS CODE IS AUTOMATICALLY GENERATED FROM C++ HEADER FILES
 * YOU PROBABLY SHOULDN'T NEED TO MANUALLY MODIFY THIS. IT'S
 * BETTER TO MODIFY THE TEMPLATE, OR MODIFY THE HEADER FILE
 * ASSOCIATED WITH THIS FILE
 * ************************************
 *
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "jerryscript-mbed-util/logging.h"
#include "jerryscript-mbed-library-registry/wrap_tools.h"

#include "mbed.h"


DECLARE_CLASS_FUNCTION(DigitalOut, write) {
    // check and unbox arguments
    uintptr_t ptr_val;
    jerry_get_object_native_handle(this_obj, &ptr_val);
    DigitalOut* native_ptr = (DigitalOut*)ptr_val;

    jerry_value_t return_value;

    if (args_count == 1 && jerry_value_is_number(args[0])) {
        int arg0 = int(jerry_get_number_value(args[0]));

        native_ptr->write(arg0);
        return_value = jerry_create_undefined();

    }
    else {
        LOG_PRINT_ALWAYS("invalid arguments in method write");
        return jerry_create_undefined();
    }

    return return_value;
}

DECLARE_CLASS_FUNCTION(DigitalOut, read) {
    // check and unbox arguments
    uintptr_t ptr_val;
    jerry_get_object_native_handle(this_obj, &ptr_val);
    DigitalOut* native_ptr = (DigitalOut*)ptr_val;

    jerry_value_t return_value;

    if (args_count == 0) {

        int result = native_ptr->read();
        // now wrap up the value in a jerry_value_t
        return_value = jerry_create_number(double(result));


    }
    else {
        LOG_PRINT_ALWAYS("invalid arguments in method read");
        return jerry_create_undefined();
    }

    return return_value;
}

DECLARE_CLASS_FUNCTION(DigitalOut, is_connected) {
    // check and unbox arguments
    uintptr_t ptr_val;
    jerry_get_object_native_handle(this_obj, &ptr_val);
    DigitalOut* native_ptr = (DigitalOut*)ptr_val;

    jerry_value_t return_value;

    if (args_count == 0) {

        int result = native_ptr->is_connected();
        // now wrap up the value in a jerry_value_t
        return_value = jerry_create_number(double(result));


    }
    else {
        LOG_PRINT_ALWAYS("invalid arguments in method is_connected");
        return jerry_create_undefined();
    }

    return return_value;
}



void js_DigitalOut_destructor(const uintptr_t native_handle) {
    delete (DigitalOut*)native_handle;
}

DECLARE_CLASS_CONSTRUCTOR(DigitalOut) {
    // check and unbox arguments
    uintptr_t native_ptr;

    if (args_count == 1 && jerry_value_is_number(args[0])) {
        PinName arg0 = PinName(jerry_get_number_value(args[0]));

        native_ptr = (uintptr_t) new DigitalOut(arg0);
    }
    else if (args_count == 2 && jerry_value_is_number(args[0]) && jerry_value_is_number(args[1])) {
        PinName arg0 = PinName(jerry_get_number_value(args[0]));
        int arg1 = int(jerry_get_number_value(args[1]));

        native_ptr = (uintptr_t) new DigitalOut(arg0, arg1);
    }
    else {
        LOG_PRINT_ALWAYS("invalid constructor arguments");
        return jerry_create_undefined();
    }

    // create the jerryscript object
    jerry_value_t js_object = jerry_create_object();
    jerry_set_object_native_handle(js_object, native_ptr, js_DigitalOut_destructor);

    // attach methods
    // register method 'write'
    {
        ATTACH_CLASS_FUNCTION(js_object, DigitalOut, write);
    }

    // register method 'read'
    {
        ATTACH_CLASS_FUNCTION(js_object, DigitalOut, read);
    }

    // register method 'is_connected'
    {
        ATTACH_CLASS_FUNCTION(js_object, DigitalOut, is_connected);
    }

    return js_object;
}
