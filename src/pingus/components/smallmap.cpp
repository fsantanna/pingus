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

#include "pingus/components/smallmap.hpp"

#include "pingus/components/playfield.hpp"
///#include "pingus/pingu_holder.hpp"
#include "pingus/server.hpp"
///#include "pingus/smallmap_image.hpp"
#include "pingus/world.hpp"

#include "ceu_vars.h"

SmallMap::SmallMap(Server* server_, Playfield* playfield_, const Rect& rect_) :
  RectComponent(rect_),
  server(server_),
  playfield(playfield_),
  scroll_mode(),
  has_focus(),
  gc_ptr(0)
{
  scroll_mode = false;
}

SmallMap::~SmallMap()
{
}

void
SmallMap::render(int x, int y, Framebuffer& fb) {
  tceu__int__int__Rect___Framebuffer_ p = {x,y,&rect,&fb};
  ceu_sys_go(&CEU_APP, CEU_IN_SMALLMAP_RENDER, &p);
}

void
SmallMap::draw(DrawingContext& gc)
{
  DrawingContext* p = &gc;
  ceu_sys_go(&CEU_APP, CEU_IN_SMALLMAP_DRAW, &p);
}

void
SmallMap::update (float delta)
{
}

bool
SmallMap::is_at (int x, int y)
{
  return (x > rect.left && x < rect.left + static_cast<int>(rect.get_width())
          && y > rect.top && y < rect.top + static_cast<int>(rect.get_height()));
}

void
SmallMap::on_pointer_move (int x, int y)
{
  int cx, cy;
  World* world = server->get_world();

  if (scroll_mode)
  {
    cx = (x - rect.left) * static_cast<int>(CEU_World_get_width(NULL,world->ceu)  / rect.get_width());
    cy = (y - rect.top) * static_cast<int>(CEU_World_get_height(NULL,world->ceu) / rect.get_height());

    playfield->set_viewpoint(cx, cy);
  }
}

void
SmallMap::on_primary_button_press (int x, int y)
{
  scroll_mode = true;

  // set view to the given COs
  int cx, cy;
  World* world = server->get_world();
  cx = (x - rect.left) * int(CEU_World_get_width(NULL,world->ceu)) / rect.get_width();
  cy = (y - rect.top) * int(CEU_World_get_height(NULL,world->ceu)) / rect.get_height();
  playfield->set_viewpoint(cx, cy);
}

void
SmallMap::on_primary_button_release(int x, int y)
{
  scroll_mode = false;
}

void
SmallMap::on_pointer_enter ()
{
  has_focus = true;
}

void
SmallMap::on_pointer_leave ()
{
  has_focus = false;
}

/* EOF */
