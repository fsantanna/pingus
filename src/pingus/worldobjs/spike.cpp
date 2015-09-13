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

namespace WorldObjs {

Spike::Spike (const FileReader& reader)
{
  tceu__WorldObjs__Spike___FileReader_ p = { this, (FileReader*)&reader };
  ceu_sys_go(&CEU_APP, CEU_IN_SPIKE_NEW, &p);
}

Spike::~Spike()
{
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_SPIKE_DELETE, &this_);
}

void
Spike::set_pos (const Vector3f& p)
{
    CEU_Spike_set_pos(NULL, this->ceu, (Vector3f*)&p);
}

Vector3f
Spike::get_pos() const
{
  return CEU_Spike_get_pos(NULL, this->ceu);
}

float
Spike::get_z_pos () const
{
  return CEU_Spike_get_z_pos(NULL, this->ceu);
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
