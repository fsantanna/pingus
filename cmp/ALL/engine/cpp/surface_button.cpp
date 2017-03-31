SurfaceButton::SurfaceButton (int arg_x_pos, int arg_y_pos, const std::string& arg_button_surface, const std::string& arg_button_pressed_surface, const std::string& arg_button_mouse_over_surface) :
  x_pos (arg_x_pos),
  y_pos (arg_y_pos),
  pressed (false),
  mouse_over (false)
{
  button_surface            = Sprite(arg_button_surface);
  button_pressed_surface    = Sprite(arg_button_pressed_surface);
  button_mouse_over_surface = Sprite(arg_button_mouse_over_surface);
}
void SurfaceButton::draw (DrawingContext& gc) {
  if (pressed && mouse_over) {
    gc.draw(button_pressed_surface, Vector2i(x_pos, y_pos));
  } else if (!pressed && mouse_over) {
    gc.draw(button_mouse_over_surface, Vector2i(x_pos, y_pos));
  } else {
    gc.draw(button_surface, Vector2i(x_pos, y_pos));
  }
}
bool SurfaceButton::is_at (int x, int y) {
  return x > x_pos && x < x_pos + int(button_surface.get_width ()) && y > y_pos && y < y_pos + int(button_surface.get_height ());
}
void SurfaceButton::on_primary_button_press (int x, int y) {
  pressed = true;
}
void SurfaceButton::on_primary_button_release (int x, int y) {
  pressed = false;
}
void SurfaceButton::on_pointer_enter () {
  mouse_over = true;
}
void SurfaceButton::on_pointer_leave () {
  mouse_over = false;
}
void SurfaceButton::on_primary_button_click (int x, int y) {
  on_click();
}
