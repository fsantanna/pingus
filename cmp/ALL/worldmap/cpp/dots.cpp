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
  }
}
LevelDot::LevelDot(const FileReader& reader) :
  green_dot_sur("core/worldmap/dot_green"),
  red_dot_sur("core/worldmap/dot_red"),
  inaccessible_dot_sur("core/worldmap/dot_invalid"),
  highlight_green_dot_sur("core/worldmap/dot_green_hl"),
  highlight_red_dot_sur("core/worldmap/dot_red_hl"),
{
}
void LevelDot::draw(DrawingContext& gc) {
  Vector2i mpos = gc.screen_to_world(Vector2i(Input::Controller::current()->get_pointer(Input::STANDARD_POINTER)->get_pos()));
  float x = static_cast<float>(mpos.x) - pos.x;
  float y = static_cast<float>(mpos.y) - pos.y;
  bool highlight = false;
  if (Math::sqrt(x*x + y*y) < 30.0f) {
    highlight = true;
  }
  Savegame* savegame = SavegameManager::instance()->get(plf.get_resname());
  if (savegame && (savegame->get_status() == Savegame::FINISHED || savegame->get_status() == Savegame::ACCESSIBLE)) {
    if (savegame->get_status() == Savegame::FINISHED) {
      if (highlight) {
        gc.draw (highlight_green_dot_sur, pos);
      } else {
        gc.draw (green_dot_sur, pos);
      }
    } else {
      if (highlight) {
        gc.draw (highlight_red_dot_sur, pos);
      } else {
        gc.draw (red_dot_sur, pos);
      }
  } else {
    gc.draw (inaccessible_dot_sur, pos);
  }
}
void LevelDot::on_click() {
  ScreenManager::instance()->push_screen(std::make_shared<StartScreen>(plf));
}
void LevelDot::draw_hover(DrawingContext& gc) {
  if (accessible()) {
    gc.print_center(Fonts::pingus_small, Vector2i(static_cast<int>(pos.x), static_cast<int>(pos.y) - 44), _(get_plf().get_levelname()), 10000);
  } else {
    gc.print_center(Fonts::pingus_small, Vector2i(static_cast<int>(pos.x), static_cast<int>(pos.y) - 44), _("???"), 10000);
  }
  if (globals::developer_mode) {
    gc.print_center(Fonts::pingus_small, Vector2i(static_cast<int>(pos.x), static_cast<int>(pos.y) - 70), get_plf().get_resname(), 10000);
  }
}
