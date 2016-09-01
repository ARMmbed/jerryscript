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
#include "jerryscript-mbed-drivers/assert-js.h"

DECLARE_GLOBAL_FUNCTION(assert)
{
  CHECK_ARGUMENT_COUNT(global, assert, (args_count == 1));
  CHECK_ARGUMENT_TYPE_ALWAYS(global, assert, 0, boolean);

  if (!jsmbed_wrap_unbox_boolean(args[0]))
  {
    printf("ERROR: Script assertion failed\n");
    exit(1);
    return jerry_create_undefined();
  }

  return jerry_create_undefined();
}
