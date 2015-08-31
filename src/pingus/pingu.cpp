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

#include "pingus/pingu.hpp"

#include <sstream>

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobj.hpp"
#include "pingus/pingu_enums.hpp"

#if 0
#include "pingus/actions/angel.hpp"
#include "pingus/actions/boarder.hpp"
#include "pingus/actions/climber.hpp"
#include "pingus/actions/laser_kill.hpp"
#include "pingus/actions/slider.hpp"
#include "pingus/actions/smashed.hpp"
#include "pingus/actions/superman.hpp"
#endif

#include "util/log.hpp"

#include "ceu_vars.h"

// Init a pingu at the given position while falling
Pingu::Pingu (int arg_id, const Vector3f& arg_pos, int owner) :
  id(arg_id),
  owner_id(owner),
/// TODO: exit
  status(PS_ALIVE),
  pos_x(arg_pos.x),
  pos_y(arg_pos.y)
{
}

Pingu::~Pingu ()
{
}

unsigned int
Pingu::get_id ()
{
  return id;
}

// TODO: pinguholder/world
Pingu::PinguStatus
Pingu::get_status (void) const
{
  return status;
}

std::string
Pingu::get_name()
{
  if (this->name.empty()) {
    return ActionName::to_screenname(current_action);
  } else {
    return this->name;
  }
}

ActionName::Enum
Pingu::get_action ()
{
  return current_action;
}

Vector3f
Pingu::get_pos () const
{
  return Vector3f(pos_x, pos_y, 0);
}

int
Pingu::get_owner ()
{
  return owner_id;
}

/* EOF */
