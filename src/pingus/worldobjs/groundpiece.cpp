//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldobjs/groundpiece.hpp"

#include "pingus/world.hpp"

namespace WorldObjs {

Groundpiece::Groundpiece(const FileReader& reader) :
  desc(),
  gptype()
{
  reader.read_desc  ("surface",  desc);

  gptype = Groundtype::GP_GROUND;
  reader.read_enum("type", gptype, &Groundtype::string_to_type);

  tceu__WorldObjs__GroundPiece___FileReader_ p = { this, (FileReader*)&reader };
  ceu_sys_go(&CEU_APP, CEU_IN_GROUNDPIECE_NEW, &p);
}

void
Groundpiece::on_startup ()
{
  CEU_GroundPiece_on_startup(NULL, this->ceu);
}

void
Groundpiece::draw (SceneContext& gc)
{
}

void
Groundpiece::set_pos (const Vector3f& p)
{
    CEU_GroundPiece_set_pos(NULL, this->ceu, (Vector3f*)&p);
}

Vector3f
Groundpiece::get_pos() const
{
  return CEU_GroundPiece_get_pos(NULL, this->ceu);
}

float
Groundpiece::get_z_pos () const
{
  return CEU_GroundPiece_get_z_pos(NULL, this->ceu);
}

} // namespace WorldObjs

/* EOF */
