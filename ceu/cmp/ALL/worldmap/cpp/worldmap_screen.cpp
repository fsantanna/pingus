WorldmapScreenCreditsButton::WorldmapScreenCreditsButton(WorldmapScreen* worldmap_screen_) : GUI::SurfaceButton(Display::get_width() - 150, 0, "core/worldmap/credits_button_normal", "core/worldmap/credits_button_pressed", "core/worldmap/credits_button_hover"),
  worldmap_screen(worldmap_screen_)
{
}
void WorldmapScreenCreditsButton::on_pointer_enter() {
  SurfaceButton::on_pointer_enter();
  Sound::PingusSound::play_sound ("tick");
}
void WorldmapScreenCreditsButton::draw (DrawingContext& gc) {
  SurfaceButton::draw(gc);
  gc.print_center(Fonts::chalk_small, Vector2i(Display::get_width() - 59 - 24, 2), _("Show Ending?"));
}
void WorldmapScreenCreditsButton::on_click() {
  worldmap_screen->show_end_story();
}
WorldmapScreenCloseButton::WorldmapScreenCloseButton(WorldmapScreen* worldmap_screen_) :
  GUI::SurfaceButton(0, Display::get_height() - 37, "core/worldmap/leave_button_normal", "core/worldmap/leave_button_pressed", "core/worldmap/leave_button_hover"),
  worldmap_screen(worldmap_screen_)
{
}
void WorldmapScreenCloseButton::on_pointer_enter() {
  SurfaceButton::on_pointer_enter();
  Sound::PingusSound::play_sound ("tick");
}
void WorldmapScreenCloseButton::draw (DrawingContext& gc) {
  SurfaceButton::draw(gc);
  gc.print_center(Fonts::chalk_small, Vector2i(44, Display::get_height() - 25), _("Leave?"));
}
void WorldmapScreenCloseButton::on_click() {
  ScreenManager::instance ()->pop_screen ();
}
WorldmapScreen::WorldmapScreen() :
  is_init(false),
  exit_worldmap(false),
{
  gui_manager->add(m_worldmap_component = new WorldmapComponent(this));
  gui_manager->add(close_button = new WorldmapScreenCloseButton(this));
}
void WorldmapScreen::load(const Pathname& filename) {
  worldmap = std::unique_ptr<Worldmap>(new Worldmap(filename));
  bool credits_unlocked = false;
  if (credits_unlocked) {
    gui_manager->add(new WorldmapScreenCreditsButton(this));
  }
}
void WorldmapScreen::on_startup () {
  exit_worldmap = false;
  Sound::PingusSound::stop_music();
  if (worldmap.get()) {
    worldmap->on_startup ();
  }
}
void WorldmapScreen::on_escape_press () {
  exit_worldmap = true;
}
void WorldmapScreen::update (float delta) {
  GUIScreen::update(delta);
  if (exit_worldmap) {
    ScreenManager::instance ()->pop_screen();
  }
  if (new_worldmap.get()) {
    worldmap = std::move(new_worldmap);
  }
}
void WorldmapScreen::show_intro_story() {
  ScreenManager::instance()->push_screen(std::make_shared<StoryScreen>(worldmap->get_worldmap().get_intro_story()));
}
void WorldmapScreen::show_end_story() {
  ScreenManager::instance()->push_screen(std::make_shared<StoryScreen>(worldmap->get_worldmap().get_end_story()));
}
void WorldmapScreen::resize(const Size& size_) {
  GUIScreen::resize(size_);
  close_button->set_pos(0, size.height - 37);
}
