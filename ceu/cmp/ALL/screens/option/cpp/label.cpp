Label::Label(const std::string& label_, const Rect& rect_) :
    RectComponent(rect_),
    label(label_)
{
}
void Label::draw(DrawingContext& gc) {
  gc.print_left(Fonts::chalk_normal, Vector2i(rect.left, rect.top), label);
}
