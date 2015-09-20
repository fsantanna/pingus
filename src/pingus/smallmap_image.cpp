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

#include "pingus/smallmap_image.hpp"

#include "pingus/collision_map.hpp"
#include "pingus/server.hpp"
#include "pingus/world.hpp"

SmallMapImage::SmallMapImage(Server* s, int width, int height)
  : server(s),
    canvas(width, height),
    sur(),
    colmap_serial(0)
{
  update_surface();
}

Sprite
SmallMapImage::get_surface ()
{
  if (sur)
  {
    return sur;
  }
  else
  {
    update_surface();
    return sur;
  }
}

void
SmallMapImage::update_surface()
{
}

/* EOF */
