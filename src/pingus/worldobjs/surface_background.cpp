//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldobjs/surface_background.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/globals.hpp"
#include "pingus/resource.hpp"
#include "pingus/world.hpp"

namespace WorldObjs {

SurfaceBackground::SurfaceBackground(const FileReader& reader)
{
#if 0
  if (!stretch_x && !stretch_y && color.a == 0)
  {
  }
  else
  {
NOT PORTED: is it used?
    Surface surface = Resource::load_surface(desc);

    if (color.a != 0 && surface.is_indexed())
    {
      if (surface.has_colorkey())
      {
        surface = surface.convert_to_rgba();
      }
      else
      {
        surface = surface.convert_to_rgb();
      }
    }

    surface.fill(color);

    // Scaling Code
    if (stretch_x && stretch_y)
    {
      surface = surface.scale(world->get_width(), world->get_height());
    }
    else if (stretch_x && !stretch_y)
    {
      if (keep_aspect)
      {
        float aspect = static_cast<float>(surface.get_height()) / static_cast<float>(surface.get_width());
        surface = surface.scale(world->get_width(), static_cast<int>(static_cast<float>(world->get_width()) * aspect));
      }
      else
      {
        surface = surface.scale(world->get_width(), surface.get_height());
      }
    }
    else if (!stretch_x && stretch_y)
    {
      if (keep_aspect)
      {
        float aspect = static_cast<float>(surface.get_width()) / static_cast<float>(surface.get_height());
        surface = surface.scale(static_cast<int>(static_cast<float>(world->get_height()) * aspect), world->get_height());
      }
      else
      {
        surface = surface.scale(surface.get_width(), world->get_height());
      }
    }

    bg_sprite = Sprite(surface);
  }
#endif
}

void
SurfaceBackground::update()
{
}

void
SurfaceBackground::draw (SceneContext& gc)
{
}

void
SurfaceBackground::set_pos (const Vector3f& p)
{
    CEU_SurfaceBackground_set_pos(NULL, this->ceu, (Vector3f*)&p);
}

Vector3f
SurfaceBackground::get_pos() const
{
  return CEU_SurfaceBackground_get_pos(NULL, this->ceu);
}

float
SurfaceBackground::get_z_pos () const
{
  return CEU_SurfaceBackground_get_z_pos(NULL, this->ceu);
}

} // namespace WorldObjs

/* EOF */
