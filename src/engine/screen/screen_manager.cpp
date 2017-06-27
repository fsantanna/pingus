//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/screen/screen_manager.hpp"

#include <iostream>

#include "engine/input/event.hpp"
#include "engine/display/display.hpp"
#include "engine/display/framebuffer.hpp"
#include "engine/display/sdl_framebuffer_surface_impl.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "external/tinygettext/include/tinygettext/log.hpp"
using tinygettext::Log;
#include "external/tinygettext/include/tinygettext/log_stream.hpp"

#include "_ceu_app.c.h"

extern void SDL_Driver_update (float delta);

ScreenManager* ScreenManager::instance_ = 0;

ScreenManager::ScreenManager()
{
  assert(instance_ == 0);
  instance_ = this;
}

ScreenManager::~ScreenManager() {
  instance_ = 0;
}

static int is_terminating = 0;
tceu_callback_ret ceu_callback_pingus (int cmd, tceu_callback_arg p1, tceu_callback_arg p2, const char* file, u32 line) {
    tceu_callback_ret ret = { .is_handled=1 };
    switch (cmd) {
        case CEU_CALLBACK_TERMINATING:
            is_terminating = 1;
            break;
        case CEU_CALLBACK_WCLOCK_DT:
            ret.value.num  = CEU_WCLOCK_INACTIVE;
            break;
        case CEU_CALLBACK_ABORT:
            abort();
            break;
        case CEU_CALLBACK_LOG: {
            switch (p1.num) {
                case 0:
                    printf("%s", (char*)p2.ptr);
                    break;
                case 1:
                    printf("%p", p2.ptr);
                    break;
                case 2:
                    printf("%d", p2.num);
                    break;
            }
            break;
        }
        case CEU_CALLBACK_REALLOC:
            ret.value.ptr = realloc(p1.ptr, p2.size);
        default:
            ret.is_handled = 0;
    }
    return ret;
}


void
ScreenManager::display(CommandLineOptions* cmd_options)
{
  Uint32 last_ticks = SDL_GetTicks();
  float previous_frame_time;

  tceu_callback cb = { &ceu_callback_pingus, NULL };
  ceu_start(&cb, 0, NULL);
  ceu_input(CEU_INPUT_MAIN, &cmd_options);

  while (!is_terminating)
  {
    Uint32 dt;

    // Get time and update Input::Events
    {
      // Get Time
      Uint32 ticks = SDL_GetTicks();
      previous_frame_time  = float(ticks - last_ticks)/1000.0f;
      dt = ticks - last_ticks;
      last_ticks = ticks;
      SDL_Driver_update(previous_frame_time);
    }

    {
      s32 dt_us = 1000*dt;
      if (dt_us > 0) {
        ceu_input(CEU_INPUT__WCLOCK, &dt_us);
        ceu_input(CEU_INPUT_SDL_DT,  &dt);
      }
    }

    // previous frame took more than one second
    if (previous_frame_time > 1.0)
    {
      if (globals::developer_mode)
        log_warn("ScreenManager: previous frame took longer than 1 second (%1% sec.), ignoring and doing frameskip", previous_frame_time);
    }
    else
    {
////
{
  ceu_input(CEU_INPUT_SDL_REDRAW, NULL);
  Display::flip_display();
  ceu_input(CEU_INPUT__ASYNC, NULL);    /// TODO: remove
}
////

      // cap the framerate at the desired value
      // figure out how long this frame took
      float current_frame_time = float(SDL_GetTicks() - last_ticks) / 1000.0f;
      // idly delay if this frame didn't last long enough to
      // achieve <desired_fps> frames per second
      if (current_frame_time < 1.0f / globals::desired_fps) {
        Uint32 sleep_time = static_cast<Uint32>(1000 *((1.0f / globals::desired_fps) - current_frame_time));
        //SDL_Delay(sleep_time);
      }
    }
  }
  ceu_stop();
}

ScreenManager* ScreenManager::instance() {
  return instance_;
}

/* EOF */
