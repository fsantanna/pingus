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

#include "engine/display/display.hpp"
#include "engine/display/drawing_context.hpp"
#include "engine/display/framebuffer.hpp"
#include "engine/input/manager.hpp"
#include "engine/input/event.hpp"
///#include "engine/screen/screen.hpp"
///#include "pingus/fps_counter.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"

#include "ceu_vars.h"

using namespace Input;

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

ScreenManager::ScreenManager(Input::Manager& arg_input_manager,
                             Input::ControllerPtr arg_input_controller) :
  input_manager(arg_input_manager),
  input_controller(arg_input_controller),
  display_gc(new DrawingContext()),
  ///fps_counter(),
  cursor(),
  screens(),
  mouse_pos()
{
  assert(instance_ == 0);
  instance_ = this;

  cursor = Sprite("core/cursors/animcross");
  ///fps_counter = std::unique_ptr<FPSCounter>(new FPSCounter());
}

ScreenManager::~ScreenManager()
{
  instance_ = 0;
}

void
ScreenManager::display()
{
  show_software_cursor(globals::software_cursor);

  Uint32 last_ticks = SDL_GetTicks();
  float previous_frame_time;
  std::vector<Input::Event> events;

  while (CEU_APP.isAlive)
  {
    Uint32 dt;

    events.clear();

    // Get time and update Input::Events
    {
      // Get Time
      Uint32 ticks = SDL_GetTicks();
      previous_frame_time  = float(ticks - last_ticks)/1000.0f;
      dt = ticks - last_ticks;
      last_ticks = ticks;

      // Update InputManager and get Events
      input_manager.update(previous_frame_time);
      input_controller->poll_events(events);
    }

    {
      s32 dt_us = 1000*dt;
      ceu_out_go(&CEU_APP, CEU_IN__WCLOCK, &dt_us);
      ceu_out_go(&CEU_APP, CEU_IN_SDL_DT,  &dt);
    }

    if (globals::software_cursor)
      cursor.update(previous_frame_time);

    // previous frame took more than one second
    if (previous_frame_time > 1.0)
    {
      if (globals::developer_mode)
        log_warn("ScreenManager: previous frame took longer than 1 second (%1% sec.), ignoring and doing frameskip", previous_frame_time);
    }
    else
    {
      update(previous_frame_time, events);

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

    {
      static int FRAMES_N      = 0;
      static int FRAMES_LAST_t = 0;
      static int FRAMES_LAST_n = 0;
      int cur = SDL_GetTicks();
      FRAMES_N++;
      if (cur >= FRAMES_LAST_t+5000) {
        FRAMES_LAST_t = cur;
        int diff = FRAMES_N - FRAMES_LAST_n;
        ///printf("[5s] %6d FPS=%3d\n", diff, diff/5);
        FRAMES_LAST_n = FRAMES_N;
      }
    }
  }
}

void
ScreenManager::update(float delta, const std::vector<Input::Event>& events)
{
  for(std::vector<Input::Event>::const_iterator i = events.begin(); i != events.end(); ++i)
  {
    if (i->type == Input::POINTER_EVENT_TYPE && i->pointer.name == Input::STANDARD_POINTER) {
      mouse_pos = Vector2i(static_cast<int>(i->pointer.x),
                           static_cast<int>(i->pointer.y));
    }

////
      const Input::Event& event = *i;
      switch (event.type)
      {
        case Input::POINTER_EVENT_TYPE:
          mouse_pos.x = int(event.pointer.x);
          mouse_pos.y = int(event.pointer.y);
          //on_pointer_move(mouse_pos.x, mouse_pos.y);
          {
            tceu__int__int p = { mouse_pos.x, mouse_pos.y };
            ceu_sys_go(&CEU_APP, CEU_IN_ON_POINTER_MOVE, &p);
          }
          break;

        case Input::BUTTON_EVENT_TYPE:
          if (event.button.name == PRIMARY_BUTTON)
          {
            if (event.button.state == Input::BUTTON_PRESSED) {
              //on_primary_button_press(mouse_pos.x, mouse_pos.y);
              tceu__int__int p = { mouse_pos.x, mouse_pos.y };
              ceu_sys_go(&CEU_APP, CEU_IN_ON_PRIMARY_BUTTON_PRESSED, &p);
            } else if (event.button.state == Input::BUTTON_RELEASED) {
              //on_primary_button_release(mouse_pos.x, mouse_pos.y);
              tceu__int__int p = { mouse_pos.x, mouse_pos.y };
              ceu_sys_go(&CEU_APP, CEU_IN_ON_PRIMARY_BUTTON_RELEASED, &p);
            }
          }
          else if (event.button.name == SECONDARY_BUTTON)
          {
            if (event.button.state == Input::BUTTON_PRESSED) {
              //on_secondary_button_press(mouse_pos.x, mouse_pos.y);
              tceu__int__int p = { mouse_pos.x, mouse_pos.y };
              ceu_sys_go(&CEU_APP, CEU_IN_ON_SECONDARY_BUTTON_PRESSED, &p);
            } else if (event.button.state == Input::BUTTON_RELEASED) {
              //on_secondary_button_release(mouse_pos.x, mouse_pos.y);
              tceu__int__int p = { mouse_pos.x, mouse_pos.y };
              ceu_sys_go(&CEU_APP, CEU_IN_ON_SECONDARY_BUTTON_RELEASED, &p);
            }
          }
          break;

        case Input::AXIS_EVENT_TYPE:
          // AxisEvents can be ignored in the GUI, they are handled elsewhere
          log_debug("GUIManager: AxisEvent: %1%", event.axis.dir);

          break;

        case Input::KEYBOARD_EVENT_TYPE:
          if (event.keyboard.state)
          {
            //on_key_pressed(event.keyboard);
          }
          else
          {
            //FIXME: implement this on_key_release(event.keyboard);
          }
          break;

        case Input::SCROLLER_EVENT_TYPE:
          //on_scroller_move(event.scroll.x_delta, event.scroll.y_delta);
          break;

        default:
          log_warn("unhandled event type %1%", event.type);
          break;
      }
      Input::Event* p_event = (Input::Event*) &event;
      ceu_sys_go(&CEU_APP, CEU_IN_ON_INPUT_EVENT, &p_event);
////
  }

  ceu_sys_go(&CEU_APP, CEU_IN_SCREENMANAGER_UPDATE, &delta);
  ceu_sys_go(&CEU_APP, CEU_IN__ASYNC, NULL);    /// TODO: remove
  ceu_sys_go(&CEU_APP, CEU_IN_SCREENMANAGER_DRAW, &display_gc);

  // Render the DrawingContext to the screen
  display_gc->render(*Display::get_framebuffer(), Rect(Vector2i(0,0), Size(Display::get_width(),
                                                                           Display::get_height())));
  display_gc->clear();

  // Draw the mouse pointer
  if (globals::software_cursor)
    cursor.render(mouse_pos.x, mouse_pos.y, *Display::get_framebuffer());

  // Draw FPS Counter
  ceu_sys_go(&CEU_APP, CEU_IN_FPS_DRAW, &display_gc);
  if (globals::print_fps)
  {
    ///fps_counter->draw();
    if (globals::developer_mode)
    {
      Fonts::pingus_small.render(origin_center, Display::get_width()/2, 60,
                                 "Developer Mode", *Display::get_framebuffer());
    }
  }
  else if (globals::developer_mode)
  {
    Fonts::pingus_small.render(origin_center, Display::get_width()/2, 35,
                               "Developer Mode", *Display::get_framebuffer());
  }

  Display::flip_display();
}

ScreenManager* ScreenManager::instance() {
  return instance_;
}

#if 0
void
ScreenManager::fade_over(ScreenPtr old_screen, ScreenPtr new_screen)
{
  if (!old_screen.get() || !new_screen.get())
    return;

  Uint32 last_ticks = SDL_GetTicks();
  float progress = 0.0f;
  Framebuffer& fb = *Display::get_framebuffer();
  while (progress <= 1.0f)
  {
    int border_x = static_cast<int>(static_cast<float>(Display::get_width()/2)  * (1.0f - progress));
    int border_y = static_cast<int>(static_cast<float>(Display::get_height()/2) * (1.0f - progress));

    old_screen->draw(*display_gc);
    display_gc->render(fb, Rect(Vector2i(0,0), Size(Display::get_width(),
                                                    Display::get_height())));
    display_gc->clear();

    fb.push_cliprect(Rect(Vector2i(0 + border_x, 0 + border_y),
                          Size(Display::get_width()  - 2*border_x,
                               Display::get_height() - 2*border_y)));

    new_screen->draw(*display_gc);
    display_gc->render(*Display::get_framebuffer(), Rect(Vector2i(0,0), Size(Display::get_width(),
                                                                            Display::get_height())));
    display_gc->clear();

    fb.pop_cliprect();
    fb.flip();
    display_gc->clear();

    progress = static_cast<float>(SDL_GetTicks() - last_ticks)/1000.0f * 2.0f;
  }

  input_manager.refresh();
}
#endif

void
ScreenManager::show_software_cursor(bool visible)
{
  globals::software_cursor = visible;

  if (globals::software_cursor)
  {
    SDL_ShowCursor(SDL_DISABLE);
  }
  else
  {
    SDL_ShowCursor(SDL_ENABLE);
  }
}

/* EOF */
