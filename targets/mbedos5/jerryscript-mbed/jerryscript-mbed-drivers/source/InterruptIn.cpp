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
#include "jerryscript-mbed-event-loop/EventLoop.h"
#include "jerryscript-mbed-library-registry/wrap_tools.h"

#include "mbed.h"

DECLARE_CLASS_FUNCTION(InterruptIn, rise)
{
  CHECK_ARGUMENT_COUNT(InterruptIn, rise, (args_count == 1));
  // Special case for rise(null), which means "detach the rise callback"
  if (jerry_value_is_null(args[1]))
  {
    uintptr_t native_handle;
    jerry_get_object_native_handle(this_obj, &native_handle);

    InterruptIn *this_interruptin = (InterruptIn*) native_handle;

    jerry_value_t property_name = jerry_create_string((const jerry_char_t*)"cb_rise");
    jerry_value_t cb_func = jerry_get_property(this_obj, property_name);
    jerry_release_value(property_name);

    mbed::js::EventLoop::getInstance().dropCallback(cb_func);

    this_interruptin->rise(0);

    return jerry_create_undefined();
  }

  // Assuming we actually have a callback now...
  CHECK_ARGUMENT_TYPE_ALWAYS(InterruptIn, rise, 0, function);
  uintptr_t native_handle;
  jerry_get_object_native_handle(this_obj, &native_handle);

  jerry_value_t f = args[0];
  jerry_acquire_value(f);

  InterruptIn *this_interruptin = (InterruptIn*) native_handle;
  mbed::Callback<void()> cb = mbed::js::EventLoop::getInstance().wrapFunction(f);
  this_interruptin->rise(cb);

  jerry_value_t property_name = jerry_create_string((const jerry_char_t*)"cb_rise");
  jerry_set_property(this_obj, property_name, f);
  jerry_release_value(property_name);


  return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(InterruptIn, fall)
{
  CHECK_ARGUMENT_COUNT(InterruptIn, fall, (args_count == 1));
  // Special case for fall(null), which means "detach the fall callback"
  if (jerry_value_is_null(args[1]))
  {
    uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);
    InterruptIn *this_interruptin = (InterruptIn*) native_handle;

    jerry_value_t property_name = jerry_create_string((const jerry_char_t*)"cb_fall");
    jerry_value_t cb_func = jerry_get_property(this_obj, property_name);
    jerry_release_value(property_name);

    mbed::js::EventLoop::getInstance().dropCallback(cb_func);

    this_interruptin->rise(0);
    return jerry_create_undefined();
  }

  // Assuming we actually have a callback now...
  CHECK_ARGUMENT_TYPE_ALWAYS(InterruptIn, fall, 0, function);

  uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);

  jerry_value_t f = args[0];
  jerry_acquire_value(f);

  InterruptIn *this_interruptin = (InterruptIn*) native_handle;
  mbed::Callback<void()> cb = mbed::js::EventLoop::getInstance().wrapFunction(f);
  this_interruptin->rise(cb);

  jerry_value_t property_name = jerry_create_string((const jerry_char_t*)"cb_fall");
  jerry_set_property(this_obj, property_name, f);
  jerry_release_value(property_name);

  return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(InterruptIn, mode)
{
  CHECK_ARGUMENT_COUNT(InterruptIn, mode, (args_count == 1));
  CHECK_ARGUMENT_TYPE_ALWAYS(InterruptIn, mode, 0, number);

  uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);

  int pull = jsmbed_wrap_unbox_number(args[0]);
  InterruptIn *native_ptr = (InterruptIn*)native_handle;
  native_ptr->mode((PinMode)pull);

  return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(InterruptIn, disable_irq)
{
  CHECK_ARGUMENT_COUNT(InterruptIn, disable_irq, (args_count == 0));
  uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);
  InterruptIn *native_ptr = (InterruptIn*)native_handle;
  native_ptr->disable_irq();
  return jerry_create_undefined();
}

DECLARE_CLASS_FUNCTION(InterruptIn, enable_irq)
{
  CHECK_ARGUMENT_COUNT(InterruptIn, enable_irq, (args_count == 0));
  uintptr_t native_handle = jsmbed_wrap_get_native_handle(this_obj);
  InterruptIn *native_ptr = (InterruptIn*)native_handle;
  native_ptr->enable_irq();
  return jerry_create_undefined();
}

void NAME_FOR_CLASS_NATIVE_DESTRUCTOR(InterruptIn) (uintptr_t handle)
{
  ((InterruptIn*) handle)->rise(0);
  ((InterruptIn*) handle)->fall(0);
  delete (InterruptIn*) handle;
}


DECLARE_CLASS_CONSTRUCTOR(InterruptIn)
{
  CHECK_ARGUMENT_COUNT(InterruptIn, __constructor, (args_count == 1));
  CHECK_ARGUMENT_TYPE_ALWAYS(InterruptIn, __constructor, 0, number);
  int pin = jsmbed_wrap_unbox_number(args[0]);

  uintptr_t native_handle = (uintptr_t)new InterruptIn((PinName)pin);
  jerry_value_t js_object = jerry_create_object();

  jerry_set_object_native_handle(js_object, native_handle, NAME_FOR_CLASS_NATIVE_DESTRUCTOR(InterruptIn));

  ATTACH_CLASS_FUNCTION(js_object, InterruptIn, rise);
  ATTACH_CLASS_FUNCTION(js_object, InterruptIn, fall);
  ATTACH_CLASS_FUNCTION(js_object, InterruptIn, mode);
  ATTACH_CLASS_FUNCTION(js_object, InterruptIn, enable_irq);
  ATTACH_CLASS_FUNCTION(js_object, InterruptIn, disable_irq);

  return js_object;
}