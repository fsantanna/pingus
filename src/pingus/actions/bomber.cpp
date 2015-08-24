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

#include "pingus/actions/bomber.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/colliders/pingu_collider.hpp"
#include "pingus/movers/linear_mover.hpp"
#include "pingus/particles/pingu_particle_holder.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_enums.hpp"
#include "pingus/world.hpp"

#include "ceu_vars.h"

namespace Actions {

Bomber::Bomber (Pingu* p) :
  PinguAction(p),
  ///particle_thrown(false),
  ///sound_played(false),
  gfx_exploded(false),
  ///colmap_exploded(false),
  bomber_radius("other/bomber_radius_gfx", "other/bomber_radius"),
  sprite(),
  explo_surf(Sprite("pingus/player" + pingu->get_owner_str() + "/explo"))
{
  sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() + "/bomber/left");
  sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/bomber/right");

  WorldObj::get_world()->play_sound("ohno", pingu->get_pos ());
}

void
Bomber::draw (SceneContext& gc)
{
  if (sprite[pingu->direction].get_current_frame() >= 13 && !gfx_exploded)
  {
    gc.color().draw (explo_surf, Vector3f(pingu->get_x () - 32, pingu->get_y () - 48));
    gfx_exploded = true;
  }

  gc.color().draw(sprite[pingu->direction], pingu->get_pos ());
}

void
Bomber::update ()
{
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_BOMBER_UPDATE, &this_);
}

} // namespace Actions

/* EOF */
