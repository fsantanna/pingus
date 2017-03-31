class CreditsOkButton : public GUI::SurfaceButton {
  CreditsOkButton(Credits* p) : GUI::SurfaceButton(Display::get_width()/2  + 260, Display::get_height()/2 + 170, "core/start/ok", "core/start/ok_clicked", "core/start/ok_hover"),
      parent(p)
  {
  }
  void on_pointer_enter () {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound("tick");
  }
  void on_click() {
    parent->on_escape_press();
    Sound::PingusSound::play_sound("yipee");
  }
};
Credits::Credits(const Pathname& filename) :
  fast_scrolling(false),
  background("core/menu/wood"),
  blackboard("core/menu/blackboard"),
  pingu("core/misc/creditpingu"),
{
  scene_context.reset(new SceneContext);
  fast_scrolling = false;
  gui_manager->add(new CreditsOkButton(this));
  font       = Fonts::chalk_normal;
  font_small = Fonts::chalk_large;
  { // read credit information from filename
    std::ifstream in(filename.get_sys_path());
    if (!in) {
      log_error("couldn't open %1%", filename);
      std::ostringstream out;
      out << "couldn't open " << filename;
      credits.push_back(out.str());
    } else {
      std::string line;
      while(std::getline(in, line))
      {
        credits.push_back(line);
      }
    }
  }
  end_offset = -static_cast<float>(Display::get_height())/2 - 50; // screen height + grace time
  for (std::vector<std::string>::iterator i = credits.begin(); i != credits.end(); ++i) {
    switch ((*i)[0]) {
      case '-':
        end_offset += static_cast<float>(font.get_height() + 5);
        break;
      case '_':
        end_offset += static_cast<float>(font_small.get_height() + 5);
        break;
      case 'n':
        end_offset += 50;
        break;
      default:
        log_error("Credits: Syntax error: Unknown format: '%1%'", (*i)[0]);
        break;
    }
  }
  end_offset = -end_offset;
}
Credits::~Credits () {
  StatManager::instance()->set_bool("credits-seen", true);
}
void Credits::update (float delta) {
  if (offset < end_offset) {
    //ScreenManager::instance()->pop_screen ();
  } else {
    if (fast_scrolling) {
      offset -= 450.0f * delta;
    }
    else {
      offset -= 35.0f * delta;
    }
  }
}
void Credits::draw_background (DrawingContext& gc) {
  {
    for(int y = 0; y < gc.get_height(); y += background.get_height()) {
      for(int x = 0; x < gc.get_width(); x += background.get_width()) {
        gc.draw(background, Vector2i(x, y));
      }
    }
  }
  x = Display::get_width()/2;
  y = static_cast<int>(offset);
  gc.draw(blackboard, Vector2i(gc.get_width()/2, gc.get_height()/2));
  gc.draw(pingu, Vector2i(gc.get_width()/2, gc.get_height()/2 - 20));
  yof = 0;
  scene_context->clear();
  scene_context->set_cliprect(Rect(gc.get_width()/2 - 685/2, gc.get_height()/2 - 250, gc.get_width()/2 + 685/2, gc.get_height()/2 + 250));
  for (std::vector<std::string>::iterator i = credits.begin(); i != credits.end(); ++i) {
    switch ((*i)[0]) {
      case '-':
        scene_context->color().print_center(font, Vector2i(x, (y + yof)), i->substr(1));
        yof += font.get_height() + 5;
        break;
      case '_':
        scene_context->color().print_center(font_small, Vector2i(x, (y + yof)), i->substr(1));
        yof += font_small.get_height() + 5;
        break;
      case 'n':
        yof += 50;
        break;
      default:
        log_error("Credits: Syntax error: Unknown format: '%1%'", (*i)[0]);
        break;
    }
  }
  gc.draw(new SceneContextDrawingRequest(scene_context.get(), Vector2i(0,0), 100));
}
void Credits::on_startup () {
  offset = static_cast<float>(Display::get_height()) - 50;
  //PingusSound::play_music("pingus-5.it");
}
void Credits::on_fast_forward_press () {
  fast_scrolling = ! fast_scrolling;
}
void Credits::on_escape_press () {
  bool credits_seen = false;
  StatManager::instance()->get_bool("credits-seen", credits_seen);
  if (!credits_seen) {
    ScreenManager::instance ()->pop_screen ();
  }
  ScreenManager::instance ()->pop_screen ();
}
