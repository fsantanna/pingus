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

#include "pingus/colliders/pingu_collider.hpp"

#include "math/vector3f.hpp"
#include "pingus/groundtype.hpp"
#include "pingus/pingu_enums.hpp"

namespace Colliders {

PinguCollider::PinguCollider() { }
PinguCollider::~PinguCollider() { }

int PinguCollider::getpixel(CollisionMap* colmap, const Vector3f& pos) const {
  return colmap->getpixel(
            static_cast<int>(pos.x),
            static_cast<int>(pos.y));
}

bool PinguCollider::operator() (CollisionMap* colmap, Vector3f current_pos, const Vector3f& step_vector) const
{
  Vector3f new_pos = current_pos + step_vector;
  int pixel;
  bool falling = false;
  bool collided = false;

  if (step_vector.y > 0.0f)
    falling = true;

  // If the Pingu is going to move sideways to the next pixel...
  if (static_cast<int>(new_pos.x) != static_cast<int>(current_pos.x))
  {
    float top_of_pingu = new_pos.y - static_cast<float>(pingu_height);

    for (; new_pos.y >= top_of_pingu; --new_pos.y)
    {
      pixel = getpixel(colmap, new_pos);

      // If there is something in the way, then Pingu has collided with
      // something.  However, if not falling and colliding with a
      // Bridge, allow Pingu to go through it.
      if (pixel != Groundtype::GP_NOTHING
          && !(pixel == Groundtype::GP_BRIDGE && !falling))
      {
        collided = true;
        break;
      }
    }
  }
  // If the Pingu is not falling...
  else if (!falling)
  {
    pixel = getpixel(colmap, Vector3f(new_pos.x, new_pos.y - static_cast<float>(pingu_height)));

    // If the top of the Pingu has hit something except a bridge...
    if (pixel != Groundtype::GP_NOTHING && pixel != Groundtype::GP_BRIDGE)
    {
      collided = true;
    }
  }
  // If the Pingu's "feet" has hit something...
  else if (getpixel(colmap, new_pos) != Groundtype::GP_NOTHING)
  {
    collided = true;
  }

  return collided;
}

} // namespace Colliders

/* EOF */
