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
  server()
{
  GLOBAL_SESSION = this;
  server = std::unique_ptr<Server>(new Server(arg_plf, true));
  GLOBAL_SERVER = server.get();
  log_debug("GameSession");
  tceu__PingusLevel___bool p = { &get_server()->plf, arg_show_result_screen };
  ceu_sys_go(&CEU_APP, CEU_IN_GAMESESSION_NEW, &p);
}

void GameSession::draw_background (DrawingContext& gc) { }
void GameSession::process_scroll_event (const Input::ScrollEvent& ev) { }
void GameSession::process_axis_event (const Input::AxisEvent& event) { }
void GameSession::on_action_axis_move (float move) { }
void GameSession::resize(const Size& size_) { }
GameSession::~GameSession() { }
void GameSession::on_startup () { }
void GameSession::update_server(float delta) { }
void GameSession:: on_escape_press () { }
void GameSession:: on_pause_press () { }
void GameSession::on_single_step_press () { }
void GameSession::on_fast_forward_press () { }
void GameSession::on_fast_forward_release () { }
void GameSession::on_armageddon_press () { }

/* EOF */
