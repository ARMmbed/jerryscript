/* Copyright 2014-2015 Samsung Electronics Co., Ltd.
 * Copyright (c) 2016 ARM Limited. All rights reserved.
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
#include "jerryscript-mbed-util/logging.h"
#include "jerryscript-mbed-drivers/I2C-js.h"
#include "jerryscript-mbed-library-registry/wrap_tools.h"

#include "mbed.h"

DECLARE_CLASS_FUNCTION(I2C, frequency)
{
  CHECK_ARGUMENT_COUNT(Ticker, frequency, (args_count == 1));
  CHECK_ARGUMENT_TYPE_ALWAYS(Ticker, frequency, 0, number);
  int hz = jsmbed_wrap_unbox_number(args[0]);
  uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);
  I2C* native_ptr = (I2C*)native_handle;
  native_ptr->frequency(hz);
  return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(I2C, read)
{
  if (args_count == 1)
  {
    CHECK_ARGUMENT_TYPE_ALWAYS(I2C, read, 0, number);
    uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);
    int data = jsmbed_wrap_unbox_number(args[0]);
    I2C *native_ptr = (I2C*)native_handle;
    int result = native_ptr->read(data);

    return jerry_create_number(double(result));
  }
  else if (args_count == 3 || args_count == 4)
  {
    CHECK_ARGUMENT_TYPE_ALWAYS(I2C, read, 0, number);
    CHECK_ARGUMENT_TYPE_ALWAYS(I2C, read, 1, array);
    CHECK_ARGUMENT_TYPE_ALWAYS(I2C, read, 2, number);

    CHECK_ARGUMENT_TYPE_ON_CONDITION(I2C, read, 3, boolean, (args_count == 4));

    uintptr_t native_handle;
    jerry_get_object_native_handle (this_obj, &native_handle);

    const uint32_t data_len = jerry_get_array_length(args[1]);

    int address = int(jerry_get_number_value(args[0]));
    int length = int(jerry_get_number_value(args[2]));
    char *data = (char*)malloc(data_len * sizeof(char));

    bool repeated = false;
    if (args_count == 4)
    {
      repeated = jerry_get_boolean_value(args[3]);
    }

    I2C *native_ptr = (I2C*)native_handle;

    int result = native_ptr->read(address, data, length, repeated);

    jerry_value_t out_array = jerry_create_array(data_len);

    for (uint32_t i = 0; i < data_len; i++) {
      jerry_value_t val = jerry_create_number(double(data[i]));
      jerry_set_property_by_index(out_array, i, val);
      jerry_release_value(val);
    }

    free(data);

    if (result == 0) {
      // ACK
      return out_array;
    } else {
      // NACK
      jerry_release_value(out_array);
      return jerry_create_error(JERRY_ERROR_COMMON, reinterpret_cast<const jerry_char_t *>("NACK received from I2C bus"));
    }
  }

  printf("ERROR: Unexpected number of arguments to I2C.read, expected 1, 3 or 4.");
  return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(I2C, write)
{
  if (args_count == 1)
  {
    CHECK_ARGUMENT_TYPE_ALWAYS(I2C, write, 0, number);
    uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);
    int data = int(jerry_get_number_value(args[0]));
    I2C *native_ptr = (I2C*)native_handle;
    int result = native_ptr->write(data);
    return jerry_create_number(result);
  }
  else if (args_count == 3 || args_count == 4)
  {
    CHECK_ARGUMENT_TYPE_ALWAYS(I2C, write, 0, number);
    CHECK_ARGUMENT_TYPE_ALWAYS(I2C, write, 1, array);
    CHECK_ARGUMENT_TYPE_ALWAYS(I2C, write, 2, number);
    CHECK_ARGUMENT_TYPE_ON_CONDITION(I2C, write, 3, boolean, (args_count == 4));

    uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);

    int address = int(jerry_get_number_value(args[0]));
    const uint32_t data_len = jerry_get_array_length(args[1]);

    char *data = (char*)malloc(data_len * sizeof(char));

    for (uint32_t i = 0; i < data_len; i++) {
      data[i] = char(jerry_get_property_by_index(args[1], i));
    }

    int length = int(jerry_get_number_value(args[2]));
    bool repeated = false;

    if (args_count == 4)
    {
      repeated = jerry_get_boolean_value(args[3]);
    }

    I2C *native_ptr = (I2C*)native_handle;
    int result = native_ptr->write(address, (const char*) data, length, repeated);

    free(data);

    return jerry_create_number(double(result));
  }
  printf("ERROR: Unexpected number of arguments to I2C.write, expected 1, 3 or 4.");
  return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(I2C, start)
{
  CHECK_ARGUMENT_COUNT(I2C, start, (args_count == 0));
  uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);

  I2C *native_ptr = (I2C*)native_handle;
  native_ptr->start();
  return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(I2C, stop)
{
  CHECK_ARGUMENT_COUNT(I2C, stop, (args_count == 0));
  uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);
  I2C *native_ptr = (I2C*)native_handle;
  native_ptr->stop();
  return jerry_create_undefined();
}

void NAME_FOR_CLASS_NATIVE_DESTRUCTOR(I2C) (uintptr_t handle) {
    delete (I2C*) handle;
}

DECLARE_CLASS_CONSTRUCTOR(I2C)
{
  CHECK_ARGUMENT_COUNT(I2C, __constructor, (args_count == 2));
  CHECK_ARGUMENT_TYPE_ALWAYS(I2C, __constructor, 0, number);
  CHECK_ARGUMENT_TYPE_ALWAYS(I2C, __constructor, 1, number);

  int sda = jsmbed_wrap_unbox_number(args[0]);
  int scl = jsmbed_wrap_unbox_number(args[1]);

  uintptr_t native_handle = (uintptr_t)new I2C((PinName)sda, (PinName)scl);

  jerry_value_t js_object = jerry_create_object();
  jerry_set_object_native_handle(js_object, native_handle, NAME_FOR_CLASS_NATIVE_DESTRUCTOR(I2C));

  ATTACH_CLASS_FUNCTION(js_object, I2C, frequency);
  ATTACH_CLASS_FUNCTION(js_object, I2C, read);
  ATTACH_CLASS_FUNCTION(js_object, I2C, write);
  ATTACH_CLASS_FUNCTION(js_object, I2C, start);
  ATTACH_CLASS_FUNCTION(js_object, I2C, stop);

  return js_object;
}
