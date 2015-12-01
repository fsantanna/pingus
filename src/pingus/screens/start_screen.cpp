//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/screens/start_screen.hpp"

#include "engine/display/display.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/gui/surface_button.hpp"
#include "engine/screen/screen_manager.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/fonts.hpp"
#include "pingus/game_time.hpp"
#include "pingus/gettext.h"
#include "pingus/globals.hpp"
#include "pingus/string_format.hpp"
#include "util/string_util.hpp"

#include "pingus/screens/empty_session.hpp"
#include "ceu_vars.h"

class StartScreenComponent : public GUI::Component
{
private:
  PingusLevel plf;

public:
  StartScreenComponent(const PingusLevel& plf);
  void draw(DrawingContext& gc);
  virtual ~StartScreenComponent() {}

private:
  const std::string& format_description(int length);
};

class StartScreenAbortButton
  : public GUI::SurfaceButton
{
private:
  StartScreen* parent;

public:
  StartScreenAbortButton(StartScreen* p)
    : GUI::SurfaceButton(Display::get_width()/2 - 300,
                         Display::get_height()/2 + 200,
                         "core/start/back",
                         "core/start/back_clicked",
                         "core/start/back_hover"),
      parent(p)
  {
  }

  void draw(DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, Vector2i(x_pos + 55, y_pos), _("Back"));
  }

  void on_click() {
    parent->cancel_game();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }

private:
  StartScreenAbortButton(const StartScreenAbortButton&);
  StartScreenAbortButton & operator=(const StartScreenAbortButton&);
};

StartScreenComponent::StartScreenComponent(const PingusLevel& p) : plf(p) { }
void StartScreenComponent::draw(DrawingContext& gc) { }
const std::string& StartScreenComponent::format_description(int length) { }

StartScreen::StartScreen(const PingusLevel& arg_plf) :
  plf(arg_plf),
  abort_button()
{
  StartScreenComponent* comp = new StartScreenComponent(plf);
  gui_manager->add(comp);
  gui_manager->add(abort_button = new StartScreenAbortButton(this));

  tceu__StartScreen___PingusLevel_ p = { this, &plf };
  ceu_sys_go(&CEU_APP, CEU_IN_STARTSCREEN_NEW, &p);
}

StartScreen::~StartScreen() { }

void
StartScreen::on_fast_forward_press()
{
  start_game();
}

void
StartScreen::on_pause_press ()
{
  start_game();
}

void
StartScreen::on_escape_press()
{
  cancel_game();
}

void
StartScreen::start_game()
{
  //ceu_sys_go(&CEU_APP, CEU_IN_STARTSCREEN_DELETE, NULL);
  ScreenManager::instance()->replace_screen(std::make_shared<EmptySession>(plf, true));
  tceu__PingusLevel___bool p = { &plf, true };
  ceu_sys_go(&CEU_APP, CEU_IN_GAMESESSION_NEW, &p);
}

void
StartScreen::cancel_game()
{
  ScreenManager::instance()->pop_screen();
}

void
StartScreen::resize(const Size& size_)
{
  GUIScreen::resize(size_);

  abort_button->set_pos(size.width /2 - 300, size.height/2 + 200);
}

/* EOF */
