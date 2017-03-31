class StoryScreenContinueButton : public GUI::SurfaceButton {
  StoryScreenContinueButton(StoryScreenComponent* arg_story_comp, int x, int y) : GUI::SurfaceButton(x, y, "core/misc/next", "core/misc/next", "core/misc/next_hover"),
      story_comp(arg_story_comp)
  {
  }
  void on_pointer_enter() {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
  void on_click() {
    story_comp->next_text();
  }
};
class StoryScreenSkipButton : public GUI::SurfaceButton {
  StoryScreenSkipButton(StoryScreenComponent* arg_story_comp, int x, int y) : GUI::SurfaceButton(x, y, "", "", ""), // FIXME: Little fugly way to do a text button story_comp(arg_story_comp)
  {
  }
  void draw (DrawingContext& gc) {
    gc.print_right(Fonts::chalk_small, Vector2i(x_pos, y_pos), _("skip"));
  }
  bool is_at(int x, int y) {
    return x > x_pos - static_cast<int>(Fonts::chalk_small.get_width(_("skip"))) && x < x_pos && y > y_pos && y < y_pos + static_cast<int>(Fonts::chalk_small.get_height());
  }
  void on_pointer_enter() {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
  void on_click() {
    story_comp->skip_story();
  }
};
StoryScreen::StoryScreen(const FileReader& reader, bool credits) :
  story(new WorldmapNS::WorldmapStory(reader)),
  continue_button(0),
  skip_button(0),
  m_credits(credits)
{
  story_comp = new StoryScreenComponent(story.get(), m_credits);
  gui_manager->add(story_comp);
  gui_manager->add(continue_button = new StoryScreenContinueButton(story_comp, Display::get_width()/2 + 220 + 40, Display::get_height()/2 + 180 + 32));
  if (globals::developer_mode) {
    gui_manager->add(skip_button     = new StoryScreenSkipButton(story_comp, Display::get_width() - 4, Display::get_height() - 26));
  }
}
StoryScreenComponent::StoryScreenComponent (WorldmapNS::WorldmapStory *arg_story, bool credits) :
  m_credits(credits)
{
  page_displayed_completly = false;
  time_passed  = 0;
  pages = story->get_pages();
  current_page = pages.back();
  page_surface = Sprite(current_page.image);
  background = Sprite("core/menu/wood");
  blackboard = Sprite("core/menu/blackboard");
}
void StoryScreenComponent::draw (DrawingContext& gc) {
  for(int y = 0; y < gc.get_height(); y += background.get_height()) {
    for(int x = 0; x < gc.get_width(); x += background.get_width()) {
      gc.draw(background, Vector2i(x, y));
    }
  }
  gc.draw(blackboard, Vector2i(gc.get_width()/2, gc.get_height()/2));
  gc.print_center(Fonts::chalk_large, Vector2i(gc.get_width()/2, gc.get_height()/2 - 200), story->get_title());
  gc.draw(page_surface, Vector2i(gc.get_width()/2, gc.get_height()/2 - 65));
  gc.print_left(Fonts::chalk_normal, Vector2i(gc.get_width()/2  - 280, gc.get_height()/2 + 35), display_text);
}
void StoryScreenComponent::update(float delta) {
  time_passed += delta;
  if (!page_displayed_completly) {
    std::string::size_type len = static_cast<std::string::size_type>(20.0f * time_passed);
    std::string::size_type text_len = UTF8::length(current_page.text);
    display_text = UTF8::substr(current_page.text, 0, Math::min(text_len, len));
    if (text_len < len) {
      page_displayed_completly = true;
    }
  }
}
void StoryScreen::on_fast_forward_press () {
  story_comp->next_text();
}
void StoryScreen::on_escape_press () {
  ScreenManager::instance()->pop_screen();
}
void StoryScreen::on_startup() {
  Sound::PingusSound::play_music(story_comp->get_story()->get_music(), .7f);
}
void StoryScreenComponent::skip_story() {
  page_displayed_completly = true;
  while (pages.size() > 1) {
    pages.pop_back();
  }
  next_text();
}
void StoryScreenComponent::next_text() {
  if (!page_displayed_completly) {
    page_displayed_completly = true;
    display_text = current_page.text;
  } else {
    pages.pop_back();
    if (!pages.empty()) {
      current_page = pages.back();
      page_surface = Sprite(current_page.image);
      display_text = "";
      time_passed = 0;
      page_displayed_completly = false;
    } else {
      if (m_credits) {
        ScreenManager::instance()->replace_screen (std::make_shared<Credits>(Pathname("credits/pingus.credits", Pathname::DATA_PATH)));
      } else {
        ScreenManager::instance()->pop_screen();
      }
    }
  }
}
void StoryScreen::resize(const Size& size_) {
  GUIScreen::resize(size_);
  continue_button->set_pos(size.width/2 + 220 + 40, size.height/2 + 180 +32);
  if (skip_button) {
    skip_button->set_pos(size.width  - 4, size.height - 26);
  }
}
