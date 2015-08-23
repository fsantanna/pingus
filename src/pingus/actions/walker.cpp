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

#include "pingus/actions/walker.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/globals.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/pingu.hpp"
#include "util/log.hpp"

#include "ceu_vars.h"

namespace Actions {

Walker::Walker (Pingu* p) :
  PinguAction(p),
  walker(),
  floaterlayer()
{
  walker.load(Direction::LEFT, "pingus/player" + pingu->get_owner_str() + "/walker/left");
  walker.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/walker/right");

  floaterlayer.load(Direction::LEFT,  "other/floaterlayer/left");
  floaterlayer.load(Direction::RIGHT, "other/floaterlayer/right");

  // Reset the velocity
  pingu->set_velocity(Vector3f());
}

void
Walker::update ()
{
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_WALKER_UPDATE, &this_);
}

void
Walker::draw (SceneContext& gc)
{
  gc.color().draw(walker[pingu->direction], pingu->get_pos());

  if (pingu->get_fall_action() == ActionName::FLOATER)
  {
    gc.color().draw(floaterlayer[pingu->direction], pingu->get_pos());
  }
}

} // namespace Actions

/* EOF */
