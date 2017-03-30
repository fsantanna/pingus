StoryDot::StoryDot(const FileReader& reader) :
  m_story_dot_highlight("core/worldmap/story_dot_highlight"),
  m_story_dot("core/worldmap/story_dot"),
{
}
void StoryDot::draw(DrawingContext& gc) {
  gc.draw (m_story_dot, pos);
}
void StoryDot::draw_hover(DrawingContext& gc) {
  gc.draw (m_story_dot_highlight, pos);
  gc.print_center(Fonts::pingus_small, Vector2i(static_cast<int>(pos.x), static_cast<int>(pos.y) - 44), _(m_name), 10000);
}
void StoryDot::on_click() {
  try {
    FileReader reader = FileReader::parse(Pathname(m_story, Pathname::DATA_PATH));
    ScreenManager::instance()->push_screen(std::make_shared<StoryScreen>(reader, m_credits));
  } catch(const std::exception& err) {
    log_error("%1%", err.what());
  }
}
