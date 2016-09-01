/* Copyright 2015 Samsung Electronics Co., Ltd.
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

#ifndef __JSMBED_WRAP_TOOLS_H__
#define __JSMBED_WRAP_TOOLS_H__

#include <stdlib.h>

#include "jerry-core/jerry-api.h"

#include "jerryscript-mbed-util/wrappers.h"

inline uint32_t jsmbed_wrap_unbox_uint32(const jerry_value_t val_p)
{
  if (jerry_value_is_number(val_p))
  {
    return (uint32_t)jerry_get_number_value(val_p);
  }
  printf("ERROR: Attempted to get uint32 from non-uint32 bearing value: 0x%lu\n", val_p);
  return 0;
}
inline uint32_t jsmbed_wrap_unbox_int(const jerry_value_t val_p)
{
  if (jerry_value_is_number(val_p))
  {
    return (int)jerry_get_number_value(val_p);
  }
  printf("ERROR: Attempted to get uint32 from non-uint32 bearing value: 0x%lu\n", val_p);
  return 0;
}
inline float jsmbed_wrap_unbox_float32(const jerry_value_t val_p)
{
  if (jerry_value_is_number(val_p))
  {
    return (float)jerry_get_number_value(val_p);
  }
  printf("ERROR: Attempted to get float32 from non-float32 bearing value: 0x%lu\n", val_p);
  return 0.0f;
}
inline double jsmbed_wrap_unbox_float64(const jerry_value_t val_p)
{
  if (jerry_value_is_number(val_p))
  {
    return (double)jerry_get_number_value(val_p);
  }
  printf("ERROR: Attempted to get float64 from non-float64 bearing value: 0x%lu\n", val_p);
  return 0.0;
}
inline double jsmbed_wrap_unbox_number(const jerry_value_t val_p)
{
  return (double)jerry_get_number_value(val_p);
}
inline bool jsmbed_wrap_unbox_boolean(const jerry_value_t val_p)
{
  if (jerry_value_is_boolean(val_p))
  {
    return jerry_get_boolean_value(val_p);
  }
  printf("ERROR: Attempted to get boolean from non-boolean bearing value: 0x%lu\n", val_p);
  return false;
}

//
// Extras
//
inline bool jsmbed_wrap_value_is_function(const jerry_value_t val_p)
{
  return jerry_value_is_function(val_p);
}

inline void jsmbed_wrap_acquire_object(jerry_value_t obj_p)
{
  jerry_acquire_value(obj_p);
}

inline void jsmbed_wrap_release_object(jerry_value_t obj_p)
{
  jerry_release_value(obj_p);
}

inline jerry_value_t jsmbed_wrap_create_object()
{
  return jerry_create_object();
}

inline void jsmbed_wrap_link_objects(jerry_value_t js_obj, uintptr_t native_obj, void (*free_function)(uintptr_t))
{
  jerry_set_object_native_handle(js_obj, native_obj, (jerry_object_free_callback_t) free_function);
}

inline uintptr_t jsmbed_wrap_get_native_handle(const jerry_value_t val_p)
{
  uintptr_t handle;
  if (!jerry_value_is_object(val_p))
  {
    printf("ERROR: Cannot get native handle from non-object-bearing value.\n");
    return 0;
  }
  if (!jerry_get_object_native_handle(val_p, &handle))
  {
    printf("ERROR: Failed to get handle from supposedly natively-mapped object.\n");
    return 0;
  }
  return handle;
}


//
// Functions used by the wrapper registration API.
//

bool
jsmbed_wrap_register_global_function (const char* name,
                          jerry_external_handler_t handler);

bool
jsmbed_wrap_register_class_constructor (const char* name,
                            jerry_external_handler_t handler);

bool
jsmbed_wrap_register_class_function (jerry_value_t this_obj_p,
                         const char* name,
                         jerry_external_handler_t handler);

#endif
