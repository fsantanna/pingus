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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_SURFACE_BACKGROUND_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_SURFACE_BACKGROUND_HPP

#include "math/vector3f.hpp"
#include "math/color.hpp"
#include "pingus/worldobj.hpp"

#include "ceu_vars.h"
struct CEU_SurfaceBackground;

namespace WorldObjsData {
class SurfaceBackgroundData;
}

namespace WorldObjs {

class SurfaceBackground : public WorldObj
{
private:
public:
  SurfaceBackground(const FileReader& reader);
  CEU_SurfaceBackground* ceu;

  float get_z_pos () const;
  void set_pos(const Vector3f& p);
  Vector3f get_pos() const;

  void update ();
  void draw (SceneContext& gc);

private:
  SurfaceBackground (const SurfaceBackground&);
  SurfaceBackground& operator= (const SurfaceBackground&);
};

} // namespace WorldObjs

#endif

/* EOF */
