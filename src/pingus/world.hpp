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

#ifndef HEADER_PINGUS_PINGUS_WORLD_HPP
#define HEADER_PINGUS_PINGUS_WORLD_HPP

#include <string>
#include <vector>

#include "math/vector2i.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/groundtype.hpp"

class Vector3f;
class GroundMap;
class PinguHolder;
class PingusLevel;
class CollisionMap;
class Pingu;
class SmallMap;
class SceneContext;

#include "ceu_vars.h"
struct CEU_World;

class World
{
private:
  CEU_World* ceu;
public:
  World(const PingusLevel& level);
  virtual ~World();
private:
  World (const World&);
  World& operator= (const World&);
};

#endif

/* EOF */
