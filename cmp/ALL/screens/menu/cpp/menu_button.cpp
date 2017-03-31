MenuButton::MenuButton(PingusMenu* menu_, const Vector2i& pos_, const std::string& text_, const std::string& desc_) :
  menu(menu_),
{
  surface_p = Sprite("core/menu/menuitem");
  highlight = Sprite("core/menu/menuitem_highlight");
  text = text_;
  desc  = desc_;
  x_pos = pos_.x;
  y_pos = pos_.y;
  font       = Fonts::pingus_small;
  font_large = Fonts::chalk_large;
  mouse_over = false;
  pressed    = false;
}
void MenuButton::on_click () {
  menu->on_click(this);
}
void MenuButton::draw (DrawingContext& gc) {
  if (mouse_over) {
    gc.draw(surface_p,Vector2i(x_pos, y_pos));
    gc.draw(highlight, Vector2i(x_pos, y_pos));
    gc.print_center(font_large, Vector2i(x_pos, y_pos - 28), text);
  } else {
    gc.draw(surface_p, Vector2i(x_pos, y_pos));
    gc.print_center(font_large, Vector2i(x_pos, y_pos - 28), text);
  }
}
void MenuButton::on_pointer_enter () {
  mouse_over = true;
  Sound::PingusSound::play_sound ("tick");
  menu->set_hint(desc);
}
void MenuButton::on_pointer_leave () {
  mouse_over = false;
  menu->set_hint("");
}
void MenuButton::on_pointer_press () {
  pressed = true;
}
void MenuButton::on_pointer_release () {
  pressed = false;
}
bool MenuButton::is_at(int x, int y) {
  return (x > x_pos - int(surface_p.get_width()) / 2 && x < x_pos + int(surface_p.get_width()) / 2 && y > y_pos - int(surface_p.get_height()) / 2 && y < y_pos + int(surface_p.get_height()) / 2);
}
void MenuButton::set_pos(int x, int y) {
  x_pos = x;
  y_pos = y;
}
