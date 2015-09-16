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

#ifndef HEADER_PINGUS_PINGUS_PINGU_HOLDER_HPP
#define HEADER_PINGUS_PINGUS_PINGU_HOLDER_HPP

#include <list>

#include "ceu_vars.h"

#include "pingus/worldobj.hpp"

class PingusLevel;
class Vector3f;
class Pingu;
struct CEU_PinguHolder;

/** This class holds all the penguins in the world */
class PinguHolder : public WorldObj
{
private:
  CEU_PinguHolder* ceu;

public:
  PinguHolder(const PingusLevel&);
  ~PinguHolder();

  void draw (SceneContext& gc);

  void update();
  float get_z_pos() const;
  void set_pos(const Vector3f& p) { }
  Vector3f get_pos() const { return Vector3f(); }
  unsigned int get_end_id();

private:
  PinguHolder (const PinguHolder&);
  PinguHolder& operator= (const PinguHolder&);
};

#endif

/* EOF */
