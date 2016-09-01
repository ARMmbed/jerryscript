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

#include "mbed.h"
#include "rtos.h"

#include "jerry-core/jerry-api.h"

#include "jerryscript-mbed-event-loop/EventLoop.h"

#include "jerryscript-mbed-util/js_source.h"
#include "jerryscript-mbed-library-registry/registry.h"

#include "jerryscript-mbed-launcher/launcher.h"
#include "jerryscript-mbed-launcher/setup.h"

extern struct jsmbed_js_source_t jsmbed_js_code_storage[];

static int load_javascript (void)
{
  for (int src = 0; jsmbed_js_code_storage[src].source; src++)
  {
    LOG_PRINT("running js file %s\r\n", jsmbed_js_code_storage[src].name);
    jerry_value_t parsed_code = jerry_parse(reinterpret_cast<const jerry_char_t*>(jsmbed_js_code_storage[src].source), jsmbed_js_code_storage[src].length, false);

    if (jerry_value_has_error_flag(parsed_code)) {
      LOG_PRINT_ALWAYS ("jerry_parse failed [%s]\r\n", jsmbed_js_code_storage[src].name);
      jsmbed_js_exit ();
      return -1;
    }

    jerry_value_t returned_value = jerry_run(parsed_code);

    if (jerry_value_has_error_flag(returned_value)) {
      LOG_PRINT_ALWAYS ("jerry_run failed [%s]\r\n", jsmbed_js_code_storage[src].name);
      jsmbed_js_exit ();
      return -1;
    }

    jerry_release_value (parsed_code);
    jerry_release_value (returned_value);
  }

  return 0;
}

int jsmbed_js_init()
{
  jerry_init_flag_t flags = JERRY_INIT_EMPTY;
  jerry_init (flags);

  jsmbed_js_load_magic_strings ();
  mbed::js::LibraryRegistry::getInstance().register_all();

  return 0;
}

void jsmbed_js_exit (void)
{
  jerry_cleanup ();
}

void jsmbed_js_launch (void)
{
  Serial pc(USBTX, USBRX);
  pc.baud(115200);

  jsmbed_js_init();

  printf ("   JerryScript in mbed\r\n");
  printf ("   build date:  %s\r\n", __DATE__);

  if (load_javascript() == 0)
  {
    mbed::js::event_loop();
  }
}
