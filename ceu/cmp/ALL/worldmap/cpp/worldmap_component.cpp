WorldmapComponent::WorldmapComponent(WorldmapScreen* worldmap_screen_) :
  scene_context(new SceneContext),
  worldmap_screen(worldmap_screen_),
  m_fast_forward(false)
{
}
void WorldmapComponent::draw (DrawingContext& gc) {
  Worldmap* worldmap = worldmap_screen->get_worldmap();
  Rect cliprect = worldmap_screen->get_trans_rect();
  scene_context->set_rect(cliprect);
  scene_context->clear();
  scene_context->push_modelview();
  worldmap->draw(scene_context->color());
  gc.draw(new SceneContextDrawingRequest(scene_context.get(), Vector2i(0,0), -1000));
  scene_context->pop_modelview();
  if (cliprect != Rect(Vector2i(0,0), Size(Display::get_width(), Display::get_height()))) {
    Color border_color(0, 0, 0);
    gc.draw_fillrect(Rect(0, 0, Display::get_width(), cliprect.top), border_color);
    gc.draw_fillrect(Rect(0, cliprect.bottom, Display::get_width(), Display::get_height()), border_color);
    gc.draw_fillrect(Rect(0, cliprect.top, cliprect.left, cliprect.bottom), border_color);
    gc.draw_fillrect(Rect(cliprect.right, cliprect.top, Display::get_width(), cliprect.bottom), border_color);
  }
}
void WorldmapComponent::update(float delta) {
  if (m_fast_forward) {
    for (int i = 0; i < globals::fast_forward_time_scale; ++i) {
      worldmap_screen->get_worldmap()->update(delta);
    }
  } else {
    worldmap_screen->get_worldmap()->update(delta);
  }
}
void WorldmapComponent::on_primary_button_press (int x, int y) {
  Rect cliprect = worldmap_screen->get_trans_rect();
  worldmap_screen->get_worldmap()->on_primary_button_press(x - cliprect.left, y - cliprect.top);
}
void WorldmapComponent::on_pointer_move (int x, int y) {
  Rect cliprect = worldmap_screen->get_trans_rect();
  worldmap_screen->get_worldmap()->on_pointer_move(x - cliprect.left, y - cliprect.top);
}
void WorldmapComponent::on_secondary_button_press (int x, int y) {
  Rect cliprect = worldmap_screen->get_trans_rect();
  worldmap_screen->get_worldmap()->on_secondary_button_press(x - cliprect.left, y - cliprect.top);
}
void WorldmapComponent::on_fast_forward_press() {
  m_fast_forward = true;
}
void WorldmapComponent::on_fast_forward_release() {
  m_fast_forward = false;
}
