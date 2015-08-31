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
#include "pingus/particles/smoke_particle_holder.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"

#include "ceu_vars.h"

namespace WorldObjs {

Smasher::Smasher(const FileReader& reader) :
  sprite("traps/smasher"),
  pos(),
  smashing(false),
  downwards(false),
  count(0)
{
  assert(sprite.get_frame_count() == 6);

  reader.read_vector("position", pos);

  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_SMASHER_NEW, &this_);
}

Smasher::~Smasher () {
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_SMASHER_DELETE, &this_);
}

float
Smasher::get_z_pos () const
{
  return pos.z;
}

void
Smasher::update ()
{
}

void
Smasher::on_startup ()
{
  log_info("Drawing colmap entry");
  CollisionMask buf("traps/smasher_cmap");
  world->put(buf,
             static_cast<int>(pos.x),
             static_cast<int>(pos.y),
             Groundtype::GP_SOLID);
}

void
Smasher::draw (SceneContext& gc)
{
  gc.color().draw(sprite, pos);
}

void
Smasher::catch_pingu (Pingu* pingu)
{
}

} // namespace WorldObjs

/* EOF */
