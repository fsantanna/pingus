class OptionMenuCloseButton : public GUI::SurfaceButton {
  OptionMenuCloseButton(OptionMenu* p, int x, int y) : GUI::SurfaceButton(x, y, "core/start/ok", "core/start/ok_clicked", "core/start/ok_hover"),
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
OptionMenu::OptionMenu() :
  m_background("core/menu/wood"),
  m_blackboard("core/menu/blackboard"),
{
  gui_manager->add(ok_button = new OptionMenuCloseButton(this, Display::get_width()/2 + 245, Display::get_height()/2 + 150));
  x_pos = 0;
  y_pos = 0;
  ChoiceBox* resolution_box = new ChoiceBox(Rect()); {
    std::vector<Size> resolutions = Display::get_fullscreen_video_modes();
    Size fullscreen = config_manager.get_fullscreen_resolution();
    int choice = static_cast<int>(resolutions.size()) - 1;
    for (auto it = resolutions.begin(); it != resolutions.end(); ++it) {
      std::ostringstream ostr;
      ostr << it->width << "x" << it->height;
      resolution_box->add_choice(ostr.str());
      if (fullscreen == *it) {
        choice = static_cast<int>(it - resolutions.begin());
      }
    }
    resolution_box->set_current_choice(choice);
  }
  ChoiceBox* renderer_box = new ChoiceBox(Rect());
  renderer_box->add_choice("sdl");
  renderer_box->add_choice("delta");
#ifdef HAVE_OPENGL
  renderer_box->add_choice("opengl");
#endif
  switch(config_manager.get_renderer()) {
    case SDL_FRAMEBUFFER:
      renderer_box->set_current_choice(0);
      break;
    case DELTA_FRAMEBUFFER:
      renderer_box->set_current_choice(1);
      break;
#ifdef HAVE_OPENGL
    case OPENGL_FRAMEBUFFER:
      renderer_box->set_current_choice(2);
      break;
#endif
  }
  m_language = dictionary_manager.get_language();
  ChoiceBox* language_box = new ChoiceBox(Rect());
  {
    std::set<tinygettext::Language> languages = dictionary_manager.get_languages();
    languages.insert(tinygettext::Language::from_name("en"));
    std::vector<tinygettext::Language> langs(languages.begin(), languages.end());
    std::sort(langs.begin(), langs.end(), LanguageSorter());
    for (auto i = langs.begin(); i != langs.end(); ++i) {
      m_language_map[i->get_name()] = *i;
      language_box->add_choice(i->get_name());
      if (m_language == *i) {
        language_box->set_current_choice(static_cast<int>(i - langs.begin()));
      }
    }
  }
  ChoiceBox* scroll_box = new ChoiceBox(Rect());
  scroll_box->add_choice("Drag&Drop");
  scroll_box->add_choice("Rubberband");
  software_cursor_box = new CheckBox(Rect());
  fullscreen_box      = new CheckBox(Rect());
  autoscroll_box      = new CheckBox(Rect());
  dragdrop_scroll_box = new CheckBox(Rect());
  mousegrab_box       = new CheckBox(Rect());
  printfps_box        = new CheckBox(Rect());
  master_volume_box = new SliderBox(Rect(), 25);
  sound_volume_box  = new SliderBox(Rect(), 25);
  music_volume_box  = new SliderBox(Rect(), 25);
  master_volume_box->set_value(config_manager.get_master_volume());
  sound_volume_box->set_value(config_manager.get_sound_volume());
  music_volume_box->set_value(config_manager.get_music_volume());
  C(software_cursor_box->on_change.connect(std::bind(&OptionMenu::on_software_cursor_change, this, std::placeholders::_1)));
  C(fullscreen_box->on_change.connect(std::bind(&OptionMenu::on_fullscreen_change, this, std::placeholders::_1)));
  C(autoscroll_box->on_change.connect(std::bind(&OptionMenu::on_autoscroll_change, this, std::placeholders::_1)));
  C(dragdrop_scroll_box->on_change.connect(std::bind(&OptionMenu::on_drag_drop_scrolling_change, this, std::placeholders::_1)));
  C(mousegrab_box->on_change.connect(std::bind(&OptionMenu::on_mousegrab_change, this, std::placeholders::_1)));
  C(printfps_box->on_change.connect(std::bind(&OptionMenu::on_printfps_change, this, std::placeholders::_1)));
  C(master_volume_box->on_change.connect(std::bind(&OptionMenu::on_master_volume_change, this, std::placeholders::_1)));
  C(sound_volume_box->on_change.connect(std::bind(&OptionMenu::on_sound_volume_change, this, std::placeholders::_1)));
  C(music_volume_box->on_change.connect(std::bind(&OptionMenu::on_music_volume_change, this, std::placeholders::_1)));
  C(language_box->on_change.connect(std::bind(&OptionMenu::on_language_change, this, std::placeholders::_1)));
  C(resolution_box->on_change.connect(std::bind(&OptionMenu::on_resolution_change, this, std::placeholders::_1)));
  C(renderer_box->on_change.connect(std::bind(&OptionMenu::on_renderer_change, this, std::placeholders::_1)));
  x_pos = 0;
  y_pos = 0;
  add_item(_("Fullscreen"), fullscreen_box);
  add_item(_("Mouse Grab"), mousegrab_box);
  y_pos += 1;
  add_item(_("Software Cursor"), software_cursor_box);
  add_item(_("Autoscrolling"), autoscroll_box);
  add_item(_("Drag&Drop Scrolling"), dragdrop_scroll_box);
  y_pos += 1;
  add_item(_("Print FPS"), printfps_box);
  x_pos = 1;
  y_pos = 0;
  add_item(_("Resolution:"),    resolution_box);
  add_item(_("Renderer:"),      renderer_box);
  y_pos += 1;
  add_item(_("Language:"),        language_box);
  y_pos += 1;
  add_item(_("Master Volume:"),   master_volume_box);
  add_item(_("Sound Volume:"),    sound_volume_box);
  add_item(_("Music Volume:"),    music_volume_box);
  mousegrab_box->set_state(config_manager.get_mouse_grab(), false);
  C(config_manager.on_mouse_grab_change.connect(std::bind(&CheckBox::set_state, mousegrab_box, std::placeholders::_1, false)));
  printfps_box->set_state(config_manager.get_print_fps(), false);
  C(config_manager.on_print_fps_change.connect(std::bind(&CheckBox::set_state, printfps_box, std::placeholders::_1, false)));
  fullscreen_box->set_state(config_manager.get_fullscreen(), false);
  C(config_manager.on_fullscreen_change.connect(std::bind(&CheckBox::set_state, fullscreen_box, std::placeholders::_1, false)));
  software_cursor_box->set_state(config_manager.get_software_cursor(), false);
  C(config_manager.on_software_cursor_change.connect(std::bind(&CheckBox::set_state, software_cursor_box, std::placeholders::_1, false)));
  autoscroll_box->set_state(config_manager.get_auto_scrolling(), false);
  C(config_manager.on_auto_scrolling_change.connect(std::bind(&CheckBox::set_state, autoscroll_box, std::placeholders::_1, false)));
  dragdrop_scroll_box->set_state(config_manager.get_drag_drop_scrolling(), false);
  C(config_manager.on_drag_drop_scrolling_change.connect(std::bind(&CheckBox::set_state, dragdrop_scroll_box, std::placeholders::_1, false)));
}
void OptionMenu::add_item(const std::string& label, GUI::RectComponent* control) {
  int x_offset = (Display::get_width()  - 800) / 2;
  int y_offset = (Display::get_height() - 600) / 2;
  Rect rect(Vector2i(80 + x_offset + x_pos * 320, 140 + y_offset + y_pos * 32), Size(320, 32));
  Rect left(rect.left, rect.top, rect.right - 180, rect.bottom);
  Rect right(rect.left + 140, rect.top, rect.right, rect.bottom);
  Label* label_component = new Label(label, Rect());
  if (dynamic_cast<ChoiceBox*>(control)) {
    label_component->set_rect(left);
    control->set_rect(right);
  } else if (dynamic_cast<SliderBox*>(control)) {
    label_component->set_rect(left);
    control->set_rect(right);
  } else if (dynamic_cast<CheckBox*>(control)) {
    control->set_rect(Rect(Vector2i(rect.left, rect.top), Size(32, 32)));
    label_component->set_rect(Rect(rect.left + 40,  rect.top, rect.right, rect.bottom));
  }
  gui_manager->add(label_component);
  gui_manager->add(control);
  options.push_back(Option(label_component, control));
  y_pos += 1;
}
OptionMenu::~OptionMenu() {
  for(Connections::iterator i = connections.begin(); i != connections.end(); ++i) {
    (*i).disconnect();
  }
}
void OptionMenu::draw_background(DrawingContext& gc)
{
  for(int y = 0; y < gc.get_height(); y += m_background.get_height()) {
    for(int x = 0; x < gc.get_width(); x += m_background.get_width()) {
      gc.draw(m_background, Vector2i(x, y));
    }
  }
  gc.draw(m_blackboard, Vector2i(gc.get_width()/2, gc.get_height()/2));
  gc.print_center(Fonts::chalk_large, Vector2i(gc.get_width()/2, gc.get_height()/2 - 240), _("Option Menu"));
  gc.print_center(Fonts::chalk_normal, Vector2i(gc.get_width()/2 + 245 + 30, gc.get_height()/2 + 150 - 20), _("Close"));
  gc.print_left(Fonts::chalk_normal, Vector2i(gc.get_width()/2 - 320, gc.get_height()/2 + 200), _("Some options require a restart of the game to take effect."));
}
void OptionMenu::on_escape_press() {
  ScreenManager::instance()->pop_screen();
  Pathname cfg_filename(System::get_userdir() + "config", Pathname::SYSTEM_PATH);
  config_manager.get_options().save(cfg_filename);
}
void OptionMenu::resize(const Size& size_) {
  Size old_size = size;
  GUIScreen::resize(size_);
  if (ok_button) {
    ok_button->set_pos(size.width/2 + 245, size.height/2 + 150);
  }
  if (options.empty()) {
    return;
  }
  int x_diff = (size.width  - old_size.width) / 2;
  int y_diff = (size.height - old_size.height) / 2;
  Rect rect;
  for(std::vector<Option>::iterator i = options.begin(); i != options.end(); ++i) {
    if ((*i).label) {
      rect = (*i).label->get_rect();
      (*i).label->set_rect(Rect(Vector2i(rect.left + x_diff, rect.top + y_diff), rect.get_size()));
    }
    rect = (*i).control->get_rect();
    (*i).control->set_rect(Rect(Vector2i(rect.left + x_diff, rect.top + y_diff), rect.get_size()));
  }
}
void OptionMenu::on_software_cursor_change(bool v) {
  config_manager.set_software_cursor(v);
}
void OptionMenu::on_fullscreen_change(bool v) {
  config_manager.set_fullscreen(v);
}
void OptionMenu::on_autoscroll_change(bool v) {
  config_manager.set_auto_scrolling(v);
}
void OptionMenu::on_drag_drop_scrolling_change(bool v) {
  config_manager.set_drag_drop_scrolling(v);
}
void OptionMenu::on_mousegrab_change(bool v) {
  config_manager.set_mouse_grab(v);
}
void OptionMenu::on_printfps_change(bool v) {
  config_manager.set_print_fps(v);
}
void OptionMenu::on_master_volume_change(int v) {
  config_manager.set_master_volume(v);
}
void OptionMenu::on_sound_volume_change(int v) {
  config_manager.set_sound_volume(v);
}
void OptionMenu::on_music_volume_change(int v) {
  config_manager.set_music_volume(v);
}
void OptionMenu::on_language_change(const std::string &str) {
  auto it = m_language_map.find(str);
  if (it != m_language_map.end()) {
    m_language = it->second;
    config_manager.set_language(it->second);
  }
}
void OptionMenu::on_resolution_change(const std::string& str) {
  Size size_;
    config_manager.set_fullscreen_resolution(size_);
}
void OptionMenu::on_renderer_change(const std::string& str) {
  config_manager.set_renderer(framebuffer_type_from_string(str));
}
