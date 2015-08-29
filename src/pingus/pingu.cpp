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
  status(PS_ALIVE),
  pos_x(arg_pos.x),
  pos_y(arg_pos.y),
  direction()
{
  direction.left ();
}

Pingu::~Pingu ()
{
}

unsigned int
Pingu::get_id ()
{
  return id;
}

void
Pingu::set_x (float x)
{
  pos_x = x;
}

void
Pingu::set_y (float y)
{
  pos_y = y;
}

void
Pingu::set_pos (float x, float y)
{
  set_x (x);
  set_y (y);
}

void
Pingu::set_pos (const Vector3f& arg_pos)
{
  set_x (arg_pos.x);
  set_y (arg_pos.y);
}

// Set the action of the pingu (bridger, blocker, bomber, etc.)
// This function is used by external stuff, like the ButtonPanel, etc
// When you select a function on the button panel and click on a
// pingu, this action will be called with the action name
bool
Pingu::request_set_action(ActionName::Enum action_name)
{
  bool ret_val = false;

  tceu__Pingu___bool___ActionName__Enum p = {this, &ret_val, action_name};
  ceu_sys_go(&CEU_APP, CEU_IN_PINGU_REQUEST_SET_ACTION, &p);

  return ret_val;
}

// TODO: pinguholder/world
Pingu::PinguStatus
Pingu::get_status (void) const
{
  return status;
}

/// SPIKE, GUILLOTINE
Pingu::PinguStatus
Pingu::set_status (PinguStatus s)
{
  return (status = s);
}

// Returns true if the given koordinates are above the pingu
bool
Pingu::is_over (int x, int y)
{
  Vector3f center = get_center_pos ();

  return (center.x + 16 > x && center.x - 16 < x &&
          center.y + 16 > y && center.y - 16 < y);
}

bool
Pingu::is_inside (int x1, int y1, int x2, int y2)
{
  assert (x1 < x2);
  assert (y1 < y2);

  return (pos_x > x1 && pos_x < x2
          &&
          pos_y > y1 && pos_y < y2);
}

// Returns the distance between the Pingu and a given coordinate
float
Pingu::dist(int x, int y)
{
  Vector3f p = get_center_pos ();

  return Math::sqrt(((p.x - static_cast<float>(x)) * (p.x - static_cast<float>(x)) +
                     (p.y - static_cast<float>(y)) * (p.y - static_cast<float>(y))));
}

int
Pingu::rel_getpixel(int x, int y)
{
  return WorldObj::get_world()->get_colmap()->getpixel(static_cast<int>(pos_x + static_cast<float>(x * direction)),
                                                       static_cast<int>(pos_y - static_cast<float>(y)));
}

bool
Pingu::head_collision_on_walk (int x, int y)
{
  int pixel = rel_getpixel(x, y + pingu_height);

  if (pixel != Groundtype::GP_NOTHING && !(pixel & Groundtype::GP_BRIDGE))
    return true;

  return false;
}

void
Pingu::set_direction (Direction d)
{
  direction = d;
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

Vector3f
Pingu::get_center_pos () const
{
  return this->get_pos() + Vector3f(0, -16);
}

int
Pingu::get_owner ()
{
  return owner_id;
}

/* EOF */
