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

#include "pingus/screens/game_session.hpp"

#include "engine/display/display.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/server.hpp"

#include "engine/sound/sound.hpp"
///#include "pingus/components/button_panel.hpp"
///#include "pingus/components/pingus_counter.hpp"
///#include "pingus/components/playfield.hpp"
///#include "pingus/components/time_display.hpp"
#include "pingus/globals.hpp"
#include "pingus/savegame_manager.hpp"
#include "pingus/screens/result_screen.hpp"
#include "util/log.hpp"

#include "ceu_vars.h"

Server* GLOBAL_SERVER = NULL;
GameSession* GLOBAL_SESSION = NULL;
CEU_World* GLOBAL_CEU_WORLD = NULL;

GameSession::GameSession(const PingusLevel& arg_plf, bool arg_show_result_screen) :
  plf(arg_plf),
  show_result_screen(arg_show_result_screen),
  server(),
  world_delay(),
  is_finished  (false)
{
  GLOBAL_SESSION = this;
  server = std::unique_ptr<Server>(new Server(plf, true));
  GLOBAL_SERVER = server.get();
  // the world is initially on time
  world_delay = 0;
  log_debug("GameSession");
  void* p = &get_server()->plf;
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_NEW, &p);
}

GameSession::~GameSession()
{
}

void
GameSession::update_server(float delta)
{
  if (server->is_finished())
  {
    CEU_PinguHolder* pingu_holder = CEU_World_get_pingus(NULL,GLOBAL_CEU_WORLD);
    Result result;

    result.plf    = server->get_plf();

    result.saved  = pingu_holder->number_of_exited;
    result.killed = pingu_holder->number_of_killed;
    result.total  = server->get_plf().get_number_of_pingus();

    result.needed = server->get_plf().get_number_to_save();

    result.max_time  = server->get_plf().get_time();
    result.used_time = CEU_World_get_time(NULL, GLOBAL_CEU_WORLD);

    { // Write the savegame
      Savegame savegame(result.plf.get_resname(),
                        (result.saved >= result.needed) ? Savegame::FINISHED : Savegame::ACCESSIBLE,
                        result.used_time,
                        result.saved);
      SavegameManager::instance()->store(savegame);
    }

    if (show_result_screen)
      ScreenManager::instance()->replace_screen(std::make_shared<ResultScreen>(result));
    else
      ScreenManager::instance()->pop_screen();

  }
}

void
GameSession::draw_background (DrawingContext& gc)
{
#if 0
  Rect rect = playfield->get_rect();

  if (rect != Rect(Vector2i(0,0), Size(Display::get_width(), Display::get_height())))
  { // Draw a black border around the playfield when the playfield is smaller then the screen
    Color border_color(0, 0, 0);
    // top
    gc.draw_fillrect(Rect(0, 0, Display::get_width(), rect.top),
                     border_color);
    // bottom
    gc.draw_fillrect(Rect(0, rect.bottom, Display::get_width(), Display::get_height()),
                     border_color);
    // left
    gc.draw_fillrect(Rect(0, rect.top, rect.left, rect.bottom),
                     border_color);
    // right
    gc.draw_fillrect(Rect(rect.right, rect.top, Display::get_width(), rect.bottom),
                     border_color);
  }
#endif
}

void
GameSession::update(float delta)
{
  ///update_server(delta);
  GUIScreen::update(delta);
}

void
GameSession::update(const Input::Event& event)
{
  GUIScreen::update(event);
  switch (event.type)
  {
    case Input::AXIS_EVENT_TYPE:
      // ???
      process_axis_event (event.axis);
      break;
    case Input::SCROLLER_EVENT_TYPE:
      process_scroll_event(event.scroll);
      break;
  }
}

void
GameSession::process_scroll_event (const Input::ScrollEvent& ev)
{
  //playfield->scroll(static_cast<int>(-ev.x_delta),
                    //static_cast<int>(-ev.y_delta));
}

void
GameSession::process_axis_event (const Input::AxisEvent& event)
{
  // log_info("GameSession::process_axis_event ()");
}

void
GameSession::do_restart()
{
  server->send_finish_event();
}

bool
GameSession::finished()
{
  return is_finished;
}

void
GameSession::set_finished()
{
  is_finished = true;
  server->send_finish_event();
}

void GameSession:: on_escape_press () { }
void GameSession:: on_pause_press () { }
void GameSession::on_single_step_press () { }
void GameSession::on_fast_forward_press () { }
void GameSession::on_fast_forward_release () { }
void GameSession::on_armageddon_press () { }

void
GameSession::on_action_axis_move (float move)
{
#if 0
  if (move > 0)
    button_panel->next_action ();
  else if (move < 0)
    button_panel->previous_action ();
#endif
}

void
GameSession::on_startup ()
{
  is_finished = false;

  if (globals::developer_mode)
    log_info("Starting Music: %1%", server->get_plf().get_music());

  if (server->get_plf().get_music() == "none" ||
      server->get_plf().get_music().empty())
  {
    Sound::PingusSound::stop_music();
  }
  else
  {
    Sound::PingusSound::play_music(server->get_plf().get_music());
  }
}

void
GameSession::resize(const Size& size_)
{
  GUIScreen::resize(size_);

  int world_width  = CEU_World_get_width(NULL, GLOBAL_CEU_WORLD);
  int world_height = CEU_World_get_height(NULL, GLOBAL_CEU_WORLD);

assert(!"RESIZE");

#if 0
  playfield->set_rect(Rect(Vector2i(Math::max((size.width  - world_width)/2,  0),
                                    Math::max((size.height - world_height)/2, 0)),
                           Size(Math::min(size.width,  world_width),
                                Math::min(size.height, world_height))));

  armageddon_button->set_rect(Rect(Vector2i(size.width - 40, size.height - 62),
                                   Size(38, 60)));
  forward_button->set_rect(Rect(Vector2i(size.width - 40*2, size.height - 62),
                                Size(38, 60)));
  pause_button->set_rect(Rect(Vector2i(size.width - 40*3, size.height - 62),
                              Size(38, 60)));
  button_panel->set_pos(Vector2i(0, (size.height - 150)/2));
#endif
}

/* EOF */
