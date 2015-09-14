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

#include "pingus/worldobjs/liquid.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/world.hpp"

namespace WorldObjs {

Liquid::Liquid(const FileReader& reader)
{
}

void
Liquid::on_startup ()
{
}

void
Liquid::draw (SceneContext& gc)
{
}

void
Liquid::update()
{
  sur.update(0.033f);
}

void
Liquid::set_pos (const Vector3f& p)
{
    CEU_Liquid_set_pos(NULL, this->ceu, (Vector3f*)&p);
}

Vector3f
Liquid::get_pos() const
{
  return CEU_Liquid_get_pos(NULL, this->ceu);
}

float
Liquid::get_z_pos () const
{
  return CEU_Liquid_get_z_pos(NULL, this->ceu);
}

} // namespace WorldObjs

/* EOF */
