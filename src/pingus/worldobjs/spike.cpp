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

#include "pingus/worldobjs/spike.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"

#include "ceu_vars.h"

namespace WorldObjs {

Spike::Spike (const FileReader& reader) :
  pos()
{
  reader.read_vector("position", pos);
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_SPIKE_NEW, &this_);
}

Spike::~Spike()
{
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_SPIKE_DELETE, &this_);
}

float
Spike::get_z_pos () const
{
  return pos.z;
}

void
Spike::draw (SceneContext& gc)
{
}

void
Spike::update()
{
}

} // namespace WorldObjs

/* EOF */
