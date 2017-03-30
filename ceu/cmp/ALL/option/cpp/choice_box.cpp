ChoiceBox::ChoiceBox(const Rect& rect_) :
    RectComponent(rect_),
    current_choice(0),
{
}
void ChoiceBox::draw(DrawingContext& gc) {
  if (globals::developer_mode) {
    gc.draw_rect(rect, Color(0, 255, 255));
  }
  if (!choices.empty()) {
    if (current_choice >= 0 && current_choice < int(choices.size())) {
      gc.print_left(Fonts::chalk_normal,  Vector2i(rect.left, rect.top), "<");
      gc.print_right(Fonts::chalk_normal, Vector2i(rect.right, rect.top), ">");
      gc.print_center(Fonts::chalk_normal, Vector2i(rect.left + rect.get_width()/2, rect.top), choices[current_choice]);
    }
  }
}
void ChoiceBox::on_primary_button_press(int x, int y) {
  if (!choices.empty()) {
    int last_current_choice = current_choice;
    x -= rect.left;
    if (x > rect.get_width()/2) {
      current_choice += 1;
      if (current_choice >= int(choices.size())) {
        current_choice = 0;
      }
    } else {
      current_choice -= 1;
      if (current_choice < 0) {
        current_choice = static_cast<int>(choices.size()) - 1;
      }
    }
    if (last_current_choice != current_choice) {
      on_change(choices[current_choice]);
    }
  }
}
void ChoiceBox::add_choice(const std::string& str) {
  choices.push_back(str);
}
void ChoiceBox::set_current_choice(int choice) {
  if (choice >= 0 && choice < int(choices.size())) {
    current_choice = choice;
  }
}
