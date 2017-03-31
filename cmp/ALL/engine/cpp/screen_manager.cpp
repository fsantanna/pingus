void ScreenManager::update(float delta, const std::vector<Input::Event>& events) {
  ScreenPtr last_screen = get_current_screen();
  if (!last_screen) {
    return;
  }
  for(std::vector<Input::Event>::const_iterator i = events.begin(); i != events.end(); ++i) {
    if (i->type == Input::POINTER_EVENT_TYPE && i->pointer.name == Input::STANDARD_POINTER) {
      mouse_pos = Vector2i(static_cast<int>(i->pointer.x), static_cast<int>(i->pointer.y));
    }
    last_screen->update(*i);
  }
  last_screen->update(delta);
  get_current_screen()->draw(*display_gc);
  display_gc->render(*Display::get_framebuffer(), Rect(Vector2i(0,0), Size(Display::get_width(), Display::get_height())));
  display_gc->clear();
  Display::flip_display();
}
ScreenPtr ScreenManager::get_current_screen() {
  if (screens.empty()) {
    return ScreenPtr();
  } else {
    return screens.back();
  }
}
void ScreenManager::push_screen(ScreenPtr screen) {
  screens.push_back(screen);
  screen->on_startup();
}
void ScreenManager::pop_screen() {
  screens.pop_back();
  if (!screens.empty()) {
    if (screens.back()->get_size() != Display::get_size()) {
      screens.back()->resize(Display::get_size());
    }
    screens.back()->on_startup();
  }
}
void ScreenManager::pop_all_screens() {
  screens.clear();
}
void ScreenManager::replace_screen(ScreenPtr screen) {
  screens.back() = screen;
  if (screens.back()->get_size() != Display::get_size()) {
    screens.back()->resize(Display::get_size());
  }
  screens.back()->on_startup();
}
void ScreenManager::resize(const Size& size) {
  display_gc->set_rect(Rect(Vector2i(0, 0), size));
  get_current_screen()->resize(size);
}
