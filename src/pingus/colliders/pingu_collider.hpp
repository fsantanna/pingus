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

#ifndef HEADER_PINGUS_PINGUS_COLLIDERS_PINGU_COLLIDER_HPP
#define HEADER_PINGUS_PINGUS_COLLIDERS_PINGU_COLLIDER_HPP

#include "pingus/collision_map.hpp"
//#include "pingus/collider.hpp"

class Vector3f;

namespace Colliders {

class PinguCollider
{
public:
  /** Constructor */
  PinguCollider();

  /** Destructor */
  ~PinguCollider();

  /** Find out if a Pingu at the specified position is colliding with
      something */
  bool operator() (CollisionMap* colmap, Vector3f current_pos, const Vector3f& step_vector) const;

  int getpixel(CollisionMap* colmap, const Vector3f& pos) const;

private:
  /** Pingu could be on its belly.  Therefore, this is the current height of
      the Pingu. */
  int height;
};

} // namespace Colliders

#endif

/* EOF */
