GUIManager::GUIManager ()
  : GroupComponent(Rect(0, 0, Display::get_width(), Display::get_height()), false),
    mouse_pos(400,300)
{
}
GUIManager::GUIManager(const Rect& rect_)
  : GroupComponent(rect_),
    mouse_pos(400,300)
{
}
void GUIManager::update(float delta) {
  GroupComponent::update(delta);
}
void GUIManager::update(const Input::Event& event) {
  switch (event.type) {
    case Input::POINTER_EVENT_TYPE:
      mouse_pos.x = int(event.pointer.x);
      mouse_pos.y = int(event.pointer.y);
      on_pointer_move(mouse_pos.x, mouse_pos.y);
      break;
    case Input::BUTTON_EVENT_TYPE:
      if (event.button.name == PRIMARY_BUTTON) {
        if (event.button.state == Input::BUTTON_PRESSED) {
          on_primary_button_press(mouse_pos.x, mouse_pos.y);
        } else if (event.button.state == Input::BUTTON_RELEASED) {
          on_primary_button_release(mouse_pos.x, mouse_pos.y);
        }
      } else if (event.button.name == SECONDARY_BUTTON) {
        if (event.button.state == Input::BUTTON_PRESSED) {
          on_secondary_button_press(mouse_pos.x, mouse_pos.y);
        } else if (event.button.state == Input::BUTTON_RELEASED) {
          on_secondary_button_release(mouse_pos.x, mouse_pos.y);
        }
      }
      break;
    case Input::AXIS_EVENT_TYPE:
      log_debug("GUIManager: AxisEvent: %1%", event.axis.dir);
      break;
    case Input::KEYBOARD_EVENT_TYPE:
      if (event.keyboard.state) {
        on_key_pressed(event.keyboard);
      }
      break;
    case Input::SCROLLER_EVENT_TYPE:
      on_scroller_move(event.scroll.x_delta, event.scroll.y_delta);
      break;
    default:
      log_warn("unhandled event type %1%", event.type);
      break;
  }
}
