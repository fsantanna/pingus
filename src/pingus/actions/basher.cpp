//  Pingus - A free Lemmings clone
//  Copyright (C) 1999-2007 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/actions/basher.hpp"

#include "engine/display/scene_context.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/globals.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_enums.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobj.hpp"
#include "util/log.hpp"

#include "ceu_vars.h"

namespace Actions {

Basher::Basher (Pingu* p) :
  PinguAction(p),
  sprite(),
  bash_radius("pingus/common/bash_radius_gfx", "pingus/common/bash_radius"),
  ///basher_c(0),
  ///first_bash(true),
  bash_reach()
{
  assert(bash_radius.get_width() % 2 == 0);

  sprite.load(Direction::LEFT,  Sprite("pingus/player" +
                                       pingu->get_owner_str() + "/basher/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" +
                                       pingu->get_owner_str() + "/basher/right"));

  bash_reach = bash_radius.get_width();

  // Start a bash even so the action will stops instantly after the
  // first bash
  bash();
}

void
Basher::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}

void
Basher::update ()
{
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_BASHER_UPDATE, &this_);
}

void
Basher::bash()
{
  WorldObj::get_world()->remove(bash_radius,
                                pingu->get_xi() - bash_radius.get_width() / 2,
                                pingu->get_yi() - bash_radius.get_height() + 1);
}

bool
Basher::walk_forward()
{
  int y_inc = 0;

  // Find the correct y position to go to next
  for (y_inc = 0; y_inc >= -max_steps_down; --y_inc)
  {
    // If there is something below, get out of this loop
    if (rel_getpixel(0, y_inc - 1) != Groundtype::GP_NOTHING)
      break;
  }

  if (y_inc < -max_steps_down)
  {
    // The step down is too much.  So stop being a Basher and be a Faller.
    pingu->set_action(ActionName::FALLER);
    return false;
  }
  else
  {
    // Note that Pingu::set_pos() is the 'reverse' of the y co-ords of
    // rel_getpixel()
    pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction),
                   pingu->get_y() - static_cast<float>(y_inc));
  }

  return true;
}

bool
Basher::have_something_to_dig()
{
  {
    // Check that there is something "within" the Basher's reach
    for(int x = 0; x <= bash_reach; ++x)
    {
      for (int y = min_bash_height; y <= max_bash_height; ++y)
      {
        if (rel_getpixel(x, y) == Groundtype::GP_GROUND)
        {
          log_debug("Basher: Found something to dig...");
          return true;
        }
      }
    }

    return false;
  }
}

} // namespace Actions

/* EOF */
