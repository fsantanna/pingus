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

#include "pingus/worldobjs/entrance.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/components/smallmap.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"

#include "ceu_vars.h"

namespace WorldObjs {

Entrance::Entrance(const FileReader& reader)
{
}

Entrance::~Entrance ()
{
}

int
Entrance::get_owner_id()
{
    return this->ceu->owner_id;
}

void
Entrance::set_pos (const Vector3f& p)
{
    CEU_Entrance_set_pos(NULL, this->ceu, (Vector3f*)&p);
}

Vector3f
Entrance::get_pos() const
{
  return CEU_Entrance_get_pos(NULL, this->ceu);
}

float
Entrance::get_z_pos () const
{
  return CEU_Entrance_get_z_pos(NULL, this->ceu);
}


void
Entrance::update ()
{
}

void
Entrance::draw (SceneContext& gc)
{
}

void
Entrance::draw_smallmap(SmallMap* smallmap)
{
}

} // namespace WorldObjs

/* EOF */
