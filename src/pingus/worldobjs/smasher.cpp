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

#include "pingus/worldobjs/smasher.hpp"

#include <assert.h>

#include "engine/display/scene_context.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"

namespace WorldObjs {

Smasher::Smasher(const FileReader& reader)
{
  tceu__WorldObjs__Smasher___FileReader_ p = { this, (FileReader*)&reader };
  ceu_sys_go(&CEU_APP, CEU_IN_SMASHER_NEW, &p);
}

Smasher::~Smasher () {
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_SMASHER_DELETE, &this_);
}

void
Smasher::set_pos (const Vector3f& p)
{
    CEU_Smasher_set_pos(NULL, this->ceu, (Vector3f*)&p);
}

Vector3f
Smasher::get_pos() const
{
  return CEU_Smasher_get_pos(NULL, this->ceu);
}

float
Smasher::get_z_pos () const
{
  return CEU_Smasher_get_z_pos(NULL, this->ceu);
}

void
Smasher::update ()
{
}

void
Smasher::on_startup ()
{
  CEU_Smasher_on_startup(NULL, this->ceu);
}

void
Smasher::draw (SceneContext& gc)
{
}

} // namespace WorldObjs

/* EOF */
