SliderBox::SliderBox(const Rect& rect_, int steps)
  : RectComponent(rect_),
    m_steps(steps),
    value(steps),
    drag_drop(false),
{
}
void SliderBox::draw(DrawingContext& gc) {
  if (globals::developer_mode) {
    gc.draw_rect(rect, Color(0, 255, 255));
  }
  if (value == 0) {
    gc.print_center(Fonts::chalk_normal, Vector2i(rect.left + rect.get_width()/2, rect.top), "off");
  } else {
    for(int i = 0; i < m_steps; ++i) {
      if (i < value) {
        gc.print_left(Fonts::chalk_normal, Vector2i(rect.left + i*(rect.get_width()-12)/m_steps + 6, rect.top), "|");
      }
    }
  }
  gc.print_left(Fonts::chalk_normal, Vector2i(rect.left, rect.top), "[");
  gc.print_right(Fonts::chalk_normal, Vector2i(rect.right, rect.top), "]");
}
void SliderBox::on_primary_button_press(int x, int y) {
  drag_drop = true;
  on_pointer_move(x, y);
}
void SliderBox::on_primary_button_release(int x, int y) {
  drag_drop = false;
  on_pointer_move(x, y);
}
void SliderBox::on_pointer_move(int x, int y) {
  if (drag_drop) {
    int old_value = value;
    x -= rect.left;
    value = m_steps * x / (rect.get_width() - 12);
    value = Math::clamp(0, value, m_steps);
    if (value != old_value) {
      on_change(value*5); // scale to [0,100]
    }
  }
}
void SliderBox::set_value(int v) {
  value = v * m_steps / 100; // FIXME: rounding errors
}
