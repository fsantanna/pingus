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

#include "pingus/worldobjs/laser_exit.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"

#include "ceu_vars.h"

namespace WorldObjs {

LaserExit::LaserExit(const FileReader& reader) :
  surface("traps/laser_exit"),
  pos(),
  killing(false)
{
  reader.read_vector("position", pos);

  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_LASEREXIT_NEW, &this_);
}

/// TODO ~

float
LaserExit::get_z_pos () const
{
  return pos.z;
}

void
LaserExit::draw (SceneContext& gc)
{
  gc.color().draw (surface, pos);
}

void
LaserExit::update ()
{
}

} // namespace WorldObjs

/* EOF */
