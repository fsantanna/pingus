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
#include "engine/display/drawing_context.hpp"
#include "engine/display/framebuffer.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"

#include "ceu_vars.h"

extern void SDLDriver_update (float delta);

FramebufferSurface* load_framebuffer_sdl_surface(const Pathname& filename, ResourceModifier::Enum modifier)
{
  // FIXME: Implement proper cache
  try
  {
    Surface surface(filename);
    if (modifier != ResourceModifier::ROT0)
    {
      surface = surface.mod(modifier);
    }
    return new FramebufferSurface(new SDLFramebufferSurfaceImpl(surface.get_surface()));
  }
  catch(const std::exception& err)
  {
    // return a dummy surface for cases where the image file can't be found
    log_error("%1%", err.what());
    Surface surface(Pathname("images/core/misc/404.png", Pathname::DATA_PATH));
    return new FramebufferSurface(new SDLFramebufferSurfaceImpl(surface.get_surface()));
  }
}

ScreenManager* ScreenManager::instance_ = 0;

ScreenManager::ScreenManager():
  display_gc(new DrawingContext())
{
  assert(instance_ == 0);
  instance_ = this;
}

ScreenManager::~ScreenManager() {
  instance_ = 0;
}

void
ScreenManager::display()
{
  Uint32 last_ticks = SDL_GetTicks();
  float previous_frame_time;

  while (CEU_APP.isAlive)
  {
    Uint32 dt;

    // Get time and update Input::Events
    {
      // Get Time
      Uint32 ticks = SDL_GetTicks();
      previous_frame_time  = float(ticks - last_ticks)/1000.0f;
      dt = ticks - last_ticks;
      last_ticks = ticks;
      SDLDriver_update(previous_frame_time);
    }

    {
      s32 dt_us = 1000*dt;
      ceu_out_go(&CEU_APP, CEU_IN__WCLOCK, &dt_us);
      ceu_out_go(&CEU_APP, CEU_IN_SDL_DT,  &dt);
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
  ceu_sys_go(&CEU_APP, CEU_IN_SCREENMANAGER_DRAW, &display_gc);

  // Render the DrawingContext to the screen
  display_gc->render(*Display::get_framebuffer(), Rect(Vector2i(0,0), Size(Display::get_width(),
                                                                           Display::get_height())));
  ceu_sys_go(&CEU_APP, CEU_IN_SDL_REDRAW, NULL);
  ceu_sys_go(&CEU_APP, CEU_IN__ASYNC, NULL);    /// TODO: remove
  ceu_sys_go(&CEU_APP, CEU_IN__ASYNC, NULL);    /// TODO: remove

  display_gc->clear();
  Display::flip_display();
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
}

ScreenManager* ScreenManager::instance() {
  return instance_;
}

/* EOF */
