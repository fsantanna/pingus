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

#include "pingus/worldobjs/exit.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/components/smallmap.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"
#include "util/string_util.hpp"

#include "ceu_vars.h"

namespace WorldObjs {

Exit::Exit(const FileReader& reader) :
  smallmap_symbol("core/misc/smallmap_exit")
{
  reader.read_desc  ("surface",  desc);

  tceu__WorldObjs__Exit___FileReader_ p = { this, (FileReader*)&reader };
  ceu_sys_go(&CEU_APP, CEU_IN_EXIT_NEW, &p);

}

Exit::~Exit ()
{
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_ENTRANCE_DELETE, &this_);
}

void
Exit::on_startup ()
{
  CEU_Exit_on_startup(NULL, this->ceu);
}

void
Exit::draw (SceneContext& gc)
{
}

void
Exit::draw_smallmap(SmallMap* smallmap)
{
  CEU_Exit_draw_smallmap(NULL, this->ceu, smallmap);
}

void
Exit::update ()
{
}

void
Exit::set_pos (const Vector3f& p)
{
    CEU_Exit_set_pos(NULL, this->ceu, (Vector3f*)&p);
}

Vector3f
Exit::get_pos() const
{
  return CEU_Exit_get_pos(NULL, this->ceu);
}

float
Exit::get_z_pos () const
{
  return CEU_Exit_get_z_pos(NULL, this->ceu);
}

} // namespace WorldObjs

/* EOF */
