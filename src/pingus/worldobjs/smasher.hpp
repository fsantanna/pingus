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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_SMASHER_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_SMASHER_HPP

#include "ceu_vars.h"
struct CEU_Smasher;

#include "math/vector3f.hpp"
#include "pingus/worldobj.hpp"

namespace WorldObjsData {
class SmasherData;
}

class Pingu;

namespace WorldObjs {

class Smasher : public WorldObj
{
private:

public:
  Smasher(const FileReader& reader);
  ~Smasher();
  CEU_Smasher* ceu;

  float get_z_pos () const;
  void set_pos(const Vector3f& p);
  Vector3f get_pos() const;

  void draw (SceneContext& gc);
  void on_startup ();
  void update ();

protected:

private:
  Smasher (const Smasher&);
  Smasher& operator= (const Smasher&);
};

} // namespace WorldObjs

#endif

/* EOF */
