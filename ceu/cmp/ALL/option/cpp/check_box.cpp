CheckBox::CheckBox(const Rect& rect_) :
    RectComponent(rect_),
    m_checkbox_marked("core/menu/checkbox_marked_small"),
    m_checkbox("core/menu/checkbox_small"),
    state(false),
{
}
void CheckBox::on_primary_button_press (int x, int y) {
  state = !state;
  on_change(state);
}
void CheckBox::draw(DrawingContext& gc) {
  if (state) {
    gc.draw(m_checkbox_marked, Vector2i(rect.left, rect.top));
  } else {
    gc.draw(m_checkbox, Vector2i(rect.left, rect.top));
  }
}
void CheckBox::set_state(bool v, bool send_signal) {
  state = v;
  if (send_signal) {
    on_change(state);
  }
}
