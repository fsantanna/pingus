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

#ifndef HEADER_PINGUS_PINGUS_SCREENS_GAME_SESSION_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_GAME_SESSION_HPP

#include <memory>

#include "engine/screen/gui_screen.hpp"
#include "pingus/action_name.hpp"
#include "pingus/pingus_level.hpp"

namespace Input {
struct ScrollEvent;
struct AxisEvent;
struct Event;
}

class PingusCounter;
class Playfield;
class TimeDisplay;
class SmallMap;
class ArmageddonButton;
class ForwardButton;
class PauseButton;
class Server;

/** You can use this class to start up a game session, which consist
    of a single level. */
class GameSession : public GUIScreen
{
public:
  /// The level data
  PingusLevel plf;

  bool show_result_screen;

  /// The server
  std::unique_ptr<Server> server;

  int world_delay; ///< how many milliseconds is the world behind the actual time

  // -- Client stuff
  bool is_finished;

public:
  GameSession(const PingusLevel& arg_plf, bool arg_show_result_screen);
  ~GameSession ();

  /** Pass a delta to the screen */
  void update_server(float delta);

  Server* get_server() { return server.get(); }

  void do_restart();
  bool finished();
  void set_finished();

  /** Update all parts of the world */
  void update (float delta);
  void update (const Input::Event& event);
  void draw_background (DrawingContext& gc);

  void on_startup ();
  void on_pause_press ();
  void on_single_step_press ();
  void on_fast_forward_press ();
  void on_fast_forward_release ();
  void on_armageddon_press ();
  void on_escape_press ();
  void on_action_axis_move (float);
  void resize(const Size&);

private:
  void process_scroll_event (const Input::ScrollEvent&);
  void process_axis_event (const Input::AxisEvent&);

private:
  GameSession (const GameSession&);
  GameSession& operator= (const GameSession&);
};

#endif

/* EOF */
