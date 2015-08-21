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

#include "pingus/actions/floater.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/pingu.hpp"

#include "ceu_vars.h"

namespace Actions {

Floater::Floater(Pingu* p) :
  PinguAction(p),
  falling_depth(0),
  step(0),
  sprite()
{
  sprite = Sprite("pingus/player" + pingu->get_owner_str() + "/floater/left");

  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_FLOATER_NEW, &this_);
}

void
Floater::update()
{
#if 0
  sprite.update ();

  pingu->set_velocity(Vector3f(0.0f, 1.0f));

  if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING)
  {
    ++step;
    if (step > 0)
    {
      pingu->set_y(pingu->get_y() + 1);
      step = 0;
    }
  }
  else
  {
    pingu->set_action (ActionName::WALKER);
  }
#endif

  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_FLOATER_UPDATE, &this_);
}

void
Floater::draw (SceneContext& gc)
{
  gc.color().draw(sprite, pingu->get_pos());
}

bool
Floater::change_allowed(ActionName::Enum action)
{
  return action == ActionName::BOMBER;
}

} // namespace Actions

/* EOF */
