class LevelMenuAbortButton : public GUI::SurfaceButton {
  LevelMenuAbortButton(LevelMenu* p, int x, int y) : GUI::SurfaceButton(x, y, "core/start/back", "core/start/back_clicked", "core/start/back_hover"),
      parent(p)
  {
  }
  void draw(DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, Vector2i(x_pos + 55, y_pos), _("Back"));
  }
  void on_click() {
    parent->on_escape_press();
  }
  void on_pointer_enter() {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
};
class LevelScrollButton : public GUI::SurfaceButton {
  LevelScrollButton(int x, int y, const std::string& str, std::function<void (void)> callback_)
    : GUI::SurfaceButton(x, y, str, str + "_pressed", str + "_hover"),
      callback(callback_)
  {
  }
  void on_click() {
    callback();
  }
  void on_pointer_enter() {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound("tick");
  }
};
struct LevelsetPrioritySorter {
  bool operator()(Levelset* lhs, Levelset* rhs) const {
    return lhs->get_priority() > rhs->get_priority();
  }
};
class LevelsetSelector : public GUI::RectComponent {
  LevelsetSelector(LevelMenu* level_menu_, const Rect& rect_) :
    RectComponent(rect_),
    level_menu(level_menu_),
    page(0),
    item_height(95),
    items_per_page(4),
    list_rect(30, 70, 680 - 90, 70 + items_per_page * item_height)
  {
    marker      = Sprite("core/menu/marker");
    auto directory = Pathname("levelsets", Pathname::DATA_PATH).opendir("*.levelset");
    for(auto i = directory.begin(); i != directory.end(); ++i) {
      try {
        std::unique_ptr<Levelset> levelset = Levelset::from_file(*i);
        if (!levelset->get_developer_only() || globals::developer_mode) {
          levelsets.push_back(levelset.release());
        }
      } catch(const std::exception& err) {
        log_error("%1%", err.what());
      }
    }
    if (globals::developer_mode) {
      levelsets.push_back(Levelset::from_directory(_("Under Construction"), _("Untested, unpolished and broken levels"), "levelsets/underconstruction", Pathname("levels", Pathname::DATA_PATH)).release());
    }
    std::sort(levelsets.begin(), levelsets.end(), LevelsetPrioritySorter());
  }
  ~LevelsetSelector() {
    for(Levelsets::iterator i = levelsets.begin(); i != levelsets.end(); ++i) {
      delete *i;
    }
  }
  void draw(DrawingContext& gc) {
    gc.push_modelview();
    gc.translate(rect.left, rect.top);
    gc.print_center(Fonts::chalk_large, Vector2i(rect.get_width()/2, 10), _("Levelsets"));
    int y = list_rect.top;
    for(int i = page; (i < (page+items_per_page)) && (i < int(levelsets.size())); ++i) {
      Levelset* levelset = levelsets[i];
      if (levelset == current_levelset) {
        gc.draw(marker, Vector2i(15, y - 5));
      }
      gc.draw(levelset->get_image(), Vector2i(list_rect.left + 10, y));
      gc.print_left(Fonts::chalk_normal, Vector2i(list_rect.left + 105, 15 + y), _(levelset->get_title()));
      gc.print_left(Fonts::chalk_small,  Vector2i(list_rect.left + 105, 40 + y), _(levelset->get_description()));
      gc.print_right(Fonts::chalk_normal, Vector2i(list_rect.right, 15 + y), (boost::format("%1% %2%%%") % _("Solved:") % levelset->get_completion()).str());
      gc.print_right(Fonts::chalk_small,  Vector2i(list_rect.right, 40 + y), (boost::format("%1% %2%") % levelset->get_level_count() % _("levels")).str());
      y += item_height;
    }
    gc.pop_modelview();
  }
  void next_page() {
    page += 1;
    if (page >= (static_cast<int>(levelsets.size()) - items_per_page)) {
      page = std::max(0, (static_cast<int>(levelsets.size())) - items_per_page);
    }
  }
  void prev_page() {
    page -= 1;
    if (page < 0) {
      page = 0;
    }
  }
  void on_pointer_leave() {
    current_levelset = 0;
  }
  void on_pointer_move(int x, int y) {
    x -= rect.left;
    y -= rect.top;
    if (!list_rect.contains(Vector2i(x, y))) {
      current_levelset = NULL;
    } else {
      x -= list_rect.left;
      y -= list_rect.top;
      if (!levelsets.empty()) {
        int i = y / item_height + page;
        if (i >= 0 && i < static_cast<int>(levelsets.size())) {
          current_levelset = levelsets[i];
        } else {
          current_levelset = NULL;
        }
      }
    }
  }
  void on_primary_button_press (int x, int y) {
    on_pointer_move(x, y);
    if (current_levelset) {
      level_menu->set_levelset(current_levelset);
    }
  }
};
class LevelSelector : public GUI::RectComponent {
  LevelSelector(LevelMenu* level_menu_, const Rect& rect_) :
    RectComponent(rect_),
    level_menu(level_menu_),
    levelset(0),
    current_level(-1),
    page(0),
    item_height(32),
    items_per_page(10),
    list_rect(50, 112, 680 - 90, 112 + items_per_page * item_height)
  {
    marker        = Sprite("core/menu/marker2");
    m_checkbox_marked = Sprite("core/menu/checkbox_marked_small");
    m_checkbox_locked = Sprite("core/menu/locked_small");
    m_checkbox = Sprite("core/menu/checkbox_small");
  }
  void draw(DrawingContext& gc) {
    gc.push_modelview();
    gc.translate(rect.left, rect.top);
    Sprite sprite = levelset->get_image();
    gc.draw(sprite, Vector2i(rect.get_width()/2 - sprite.get_width()/2 - 275, 15));
    gc.draw(sprite, Vector2i(rect.get_width()/2 - sprite.get_width()/2 + 275, 15));
    gc.print_center(Fonts::chalk_large, Vector2i(rect.get_width()/2, 10), _(levelset->get_title()));
    gc.print_center(Fonts::chalk_normal,  Vector2i(rect.get_width()/2, 62), _(levelset->get_description()));
    if (levelset) {
      levelset->refresh(); // should be better placed in on_startup() or so
      int y = list_rect.top;
      for(int i = page; i < (page + items_per_page) && i < levelset->get_level_count(); ++i) {
        if (levelset->get_level(i)->accessible && i == current_level) {
          gc.draw(marker, Vector2i(20, y));
        }
        if (globals::developer_mode) {
          gc.print_left(Fonts::chalk_normal, Vector2i(list_rect.left + 40, y+4), levelset->get_level(i)->plf.get_resname());
        } else {
          gc.print_left(Fonts::chalk_normal, Vector2i(list_rect.left + 40, y+4), _(levelset->get_level(i)->plf.get_levelname()));
        }
        if (!levelset->get_level(i)->accessible) {
          gc.draw(m_checkbox_locked, Vector2i(list_rect.left + 0, y));
        } else if (levelset->get_level(i)->finished) {
          gc.draw(m_checkbox_marked, Vector2i(list_rect.left + 0, y));
        } else {
          gc.draw(m_checkbox, Vector2i(list_rect.left + 0, y));
        }
        y += item_height;
      }
    }
    gc.pop_modelview();
  }
  void prev_page() {
    page -= 1;
    if (page < 0) {
      page = 0;
    }
  }
  void next_page() {
    page += 1;
    if (page >= (levelset->get_level_count() - items_per_page)) {
      page = std::max(0, (levelset->get_level_count()) - items_per_page);
    }
  }
  void set_levelset(Levelset* levelset_) {
    page = 0;
    levelset = levelset_;
  }
  void on_pointer_leave() {
    current_level = -1;
  }
  int get_current_level(int x, int y) {
    int cl = current_level = y / 32 + page;
    if (cl < 0 || cl >= levelset->get_level_count()) {
      cl = -1;
    }
    return cl;
  }
  void on_pointer_move(int x, int y) {
    x -= rect.left;
    y -= rect.top;
    if (!list_rect.contains(Vector2i(x, y))) {
      current_level = -1;
    } else {
      x -= list_rect.left;
      y -= list_rect.top;
      current_level = y / item_height + page;
      if (current_level < 0 || current_level >= levelset->get_level_count()) {
        current_level = -1;
      }
    }
  }
  void on_primary_button_press (int x, int y) {
    on_pointer_move(x, y);
    if (current_level != -1) {
      if (levelset->get_level(current_level)->accessible) {
        ScreenManager::instance()->push_screen(std::make_shared<StartScreen>(levelset->get_level(current_level)->plf));
      }
    }
  }
};
LevelMenu::LevelMenu() :
  x_pos((Display::get_width()  - globals::default_screen_width)/2),
  y_pos((Display::get_height() - globals::default_screen_height)/2),
  background("core/menu/wood"),
  blackboard("core/menu/blackboard"),
{
  ok_button  = Sprite("core/start/ok");
  levelset_selector = new LevelsetSelector(this, Rect());
  level_selector    = new LevelSelector(this, Rect());
  gui_manager->add(levelset_selector);
  gui_manager->add(level_selector);
  gui_manager->add(prev_button = new LevelScrollButton(Display::get_width()/2  + 280, Display::get_height()/2 - 150, "core/menu/arrow_up", std::bind(&LevelMenu::prev_page, this)));
  gui_manager->add(next_button = new LevelScrollButton(Display::get_width()/2  + 280, Display::get_height()/2 + 70, "core/menu/arrow_down", std::bind(&LevelMenu::next_page, this)));
  gui_manager->add(abort_button = new LevelMenuAbortButton(this, Display::get_width()/2 - 300, Display::get_height()/2 + 144));
  level_selector->hide();
  resize(Display::get_size());
}
void LevelMenu::draw_background(DrawingContext& gc)
{
  for(int y = 0; y < gc.get_height(); y += background.get_height()) {
    for(int x = 0; x < gc.get_width(); x += background.get_width()) {
      gc.draw(background, Vector2i(x, y));
    }
  }
  gc.draw(blackboard, Vector2i(gc.get_width()/2, gc.get_height()/2));
} 
void LevelMenu::on_escape_press() {
  if (level_selector->is_visible()) {
    levelset_selector->show();
    level_selector->hide();
  } else {
    ScreenManager::instance()->pop_screen();
  }
}
void LevelMenu::on_action_up_press() {
  if (level_selector->is_visible()) {
    level_selector->prev_page();
  } else {
    levelset_selector->prev_page();
  }
}
void LevelMenu::on_action_down_press() {
  if (level_selector->is_visible()) {
    level_selector->next_page();
  } else {
    levelset_selector->next_page();
  }
}
void LevelMenu::next_page() {
  if (level_selector->is_visible()) {
    level_selector->next_page();
  } else {
    levelset_selector->next_page();
  }
}
void LevelMenu::prev_page() {
  if (level_selector->is_visible()) {
    level_selector->prev_page();
  } else {
    levelset_selector->prev_page();
  }
}
void LevelMenu::set_levelset(Levelset* levelset) {
  if (levelset) {
    level_selector->set_levelset(levelset);
    levelset_selector->hide();
    level_selector->show();
  } else {
    levelset_selector->show();
    level_selector->hide();
  }
}
void LevelMenu::resize(const Size& size_) {
  GUIScreen::resize(size_);
  x_pos = (size.width  - globals::default_screen_width)/2;
  y_pos = (size.height - globals::default_screen_height)/2;
  levelset_selector->set_rect(Rect(Vector2i(x_pos + 60, y_pos + 50), Size(680, 500)));
  level_selector   ->set_rect(Rect(Vector2i(x_pos + 60, y_pos + 50), Size(680, 500)));
  prev_button->set_pos(size.width/2  + 280, size.height/2 - 48 - 12);
  next_button->set_pos(size.width/2  + 280, size.height/2 + 12);
  abort_button->set_pos(size.width /2 - 300, size.height/2 + 200);
}
