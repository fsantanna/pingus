GUIScreen::GUIScreen() :
  Screen(Display::get_size()),
  gui_manager(new GUI::GUIManager())
{
}
void GUIScreen::draw(DrawingContext& gc) {
  draw_background(gc);
  gui_manager->draw(gc);
  draw_foreground(gc);
}
void GUIScreen::update(float delta) {
  gui_manager->update(delta);
}
void GUIScreen::update (const Input::Event& event) {
  gui_manager->update(event);
  switch (event.type) {
    case Input::BUTTON_EVENT_TYPE:
      process_button_event (event.button);
    break;
    case Input::AXIS_EVENT_TYPE:
      if (event.axis.name == Input::ACTION_AXIS) {
        on_action_axis_move(event.axis.dir);
      }
    }
    break;
    default:
      log_error("unhandled event type: %1%", event.type);
      break;
  }
}
void GUIScreen::process_button_event (const Input::ButtonEvent& event) {
  if (event.state == Input::BUTTON_PRESSED) {
    switch (event.name) {
      case Input::PAUSE_BUTTON:
        on_pause_press ();
        break;
      case Input::SINGLE_STEP_BUTTON:
        on_single_step_press();
        break;
      case Input::FAST_FORWARD_BUTTON:
        on_fast_forward_press ();
        break;
      case Input::ARMAGEDDON_BUTTON:
        on_armageddon_press ();
        break;
      case Input::ESCAPE_BUTTON:
        on_escape_press ();
        break;
    }
  } else if (event.state == Input::BUTTON_RELEASED) {
    switch (event.name) {
      case Input::PAUSE_BUTTON:
        on_pause_release ();
        break;
      case Input::SINGLE_STEP_BUTTON:
        on_single_step_release();
        break;
      case Input::FAST_FORWARD_BUTTON:
        on_fast_forward_release ();
        break;
      case Input::ARMAGEDDON_BUTTON:
        on_armageddon_release ();
        break;
      case Input::ESCAPE_BUTTON:
        on_escape_release ();
        break;
    }
  }
}
void GUIScreen::resize(const Size& size_) {
  Screen::resize(size_);
  gui_manager->set_rect(Rect(Vector2i(0, 0), size));
}
