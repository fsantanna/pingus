#ifndef HEADER_PINGUS_PINGUS_SCREENS_EMPTY_SESSION_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_EMPTY_SESSION_HPP

#include "engine/screen/screen_manager.hpp"
#include "engine/screen/gui_screen.hpp"
#include "pingus/pingus_level.hpp"

class EmptySession : public GUIScreen
{
public:
  EmptySession(const PingusLevel& arg_plf, bool arg_show_result_screen);
  ~EmptySession () {}
  void draw_background (DrawingContext& gc) { }
  void process_scroll_event (const Input::ScrollEvent& ev) { }
  void process_axis_event (const Input::AxisEvent& event) { }
  void on_action_axis_move (float move) { }
  void resize(const Size& size_) { }
  void on_startup () { }
  void update_server(float delta) { }
  void  on_escape_press () { }
  void  on_pause_press () { }
  void on_single_step_press () { }
  void on_fast_forward_press () { }
  void on_fast_forward_release () { }
  void on_armageddon_press () { }
};

#endif
