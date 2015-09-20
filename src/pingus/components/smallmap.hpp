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

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_SMALLMAP_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_SMALLMAP_HPP

#include "engine/display/sprite.hpp"
#include "engine/gui/rect_component.hpp"

class Playfield;
class Server;
class Vector3f;
class SmallMapImage;

/** This is the map that appears in the corner of the screen */
class SmallMap : public GUI::RectComponent
{
private:
  Playfield* playfield;

public:
  SmallMap(Server*, Playfield*, const Rect& rect);
  virtual ~SmallMap();

  /*{ @name Stuff called from the GUIManager */
  void on_primary_button_press(int x, int y);
  void on_primary_button_release(int x, int y);
  void on_pointer_move(int x, int y);

  // Events
  void on_pointer_enter ();
  void on_pointer_leave ();

  bool is_at (int x, int y);
  bool mouse_over();

/// TEMP
  void render(int x, int y, Framebuffer& fb);

  void draw(DrawingContext& gc);
  void update(float delta);
  /*}*/

private:
  SmallMap (const SmallMap&);
  SmallMap& operator= (const SmallMap&);
};

#endif

/* EOF */
