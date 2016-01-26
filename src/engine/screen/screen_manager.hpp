//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_SCREEN_SCREEN_MANAGER_HPP
#define HEADER_PINGUS_ENGINE_SCREEN_SCREEN_MANAGER_HPP

#include <memory>
#include <vector>

#include "math/vector2f.hpp"

namespace Input {
class Manager;
struct Event;
class Controller;
}

///class FPSCounter;
class Size;
class DrawingContext;
class Screen;

typedef std::shared_ptr<Screen> ScreenPtr;

class ScreenManager
{
private:
  static ScreenManager* instance_;

public:
  ScreenManager();
  ~ScreenManager();
  void resize(const Size& size);
  void display();
  void update(float delta, const std::vector<Input::Event>& events);
  static ScreenManager* instance();

  ScreenManager (const ScreenManager&);
  ScreenManager& operator= (const ScreenManager&);
};

#endif

/* EOF */
