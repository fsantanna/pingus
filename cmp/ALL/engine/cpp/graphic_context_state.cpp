GraphicContextState::GraphicContextState(int w, int h) : impl(new GraphicContextStateImpl())
{
  impl->rect       = Rect(Vector2i(0,0), Size(w, h));
  impl->offset     = Vector2i(0,0);
  impl->have_limit = false;
}
void GraphicContextState::set_limit(const Rect& limit) {
  impl->have_limit = true;
  impl->limit      = limit;
}
void GraphicContextState::set_unlimited() {
  impl->have_limit = false;
}
void GraphicContextState::set_size(int w, int h) {
  impl->rect = Rect(Vector2i(impl->rect.left, impl->rect.top), Size(w, h));
}
void GraphicContextState::push(DrawingContext& gc) {
  gc.push_modelview();
  gc.translate(impl->rect.left, impl->rect.top);
  gc.translate(impl->offset.x,  impl->offset.y);
}
void GraphicContextState::push(SceneContext& gc) {
  gc.push_modelview();
  gc.translate(impl->rect.left, impl->rect.top);
  gc.translate(impl->offset.x,  impl->offset.y);
}
void GraphicContextState::pop (SceneContext& gc) {
  gc.pop_modelview();
}
void GraphicContextState::pop (DrawingContext& gc) {
  gc.pop_modelview();
}
Rect GraphicContextState::get_clip_rect() {
  return Rect(-impl->offset, impl->rect.get_size());
}
void GraphicContextState::set_pos(const Vector2i& pos) {
  impl->offset.x = -pos.x + (get_width()/2);
  impl->offset.y = -pos.y + (get_height()/2);
  if (impl->have_limit) {
    if (-impl->offset.x < impl->limit.left) {
      impl->offset.x = -(impl->limit.left);
    } else if (-impl->offset.x + get_width() > impl->limit.right) {
      if (impl->limit.right - impl->limit.left > get_width()) {
        impl->offset.x = -(impl->limit.right - get_width());
      } else {
        impl->offset.x = -(impl->limit.left);
      }
    }
    if (-impl->offset.y < impl->limit.top) {
      impl->offset.y = -(impl->limit.top);
    } else if (-impl->offset.y + get_height() > impl->limit.bottom) {
      if (impl->limit.bottom - impl->limit.top > get_height()) {
        impl->offset.y = -(impl->limit.bottom - get_height());
      } else {
        impl->offset.y = -(impl->limit.top);
      }
    }
  }
}
Vector2i GraphicContextState::get_pos() const {
  return Vector2i(-impl->offset.x + (get_width()/2), -impl->offset.y + (get_height()/2));
}
Vector2i GraphicContextState::screen2world(const Vector2i& pos_) const {
  Vector2i pos(pos_.x - impl->rect.left, pos_.y - impl->rect.top);
  return pos - Vector2i(impl->rect.left, impl->rect.top) - impl->offset;
}
