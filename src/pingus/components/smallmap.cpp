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
  playfield(playfield_)
{
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
}

void
SmallMap::update (float delta)
{
}

bool
SmallMap::is_at (int x, int y)
{
  return false;
}

void
SmallMap::on_pointer_move (int x, int y)
{
}

void
SmallMap::on_primary_button_press (int x, int y)
{
}

void
SmallMap::on_primary_button_release(int x, int y)
{
}

void
SmallMap::on_pointer_enter ()
{
}

void
SmallMap::on_pointer_leave ()
{
}

/* EOF */
