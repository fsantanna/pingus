class RectComponent : public Component {
  virtual bool is_at (int x, int y) {
    return rect.contains(Vector2i(x, y));
  }
  void set_rect(const Rect& rect_) {
    rect = rect_;
    update_layout();
  }
};
