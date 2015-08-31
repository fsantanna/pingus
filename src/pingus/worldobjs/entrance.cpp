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

#include "pingus/worldobjs/entrance.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/components/smallmap.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"

#include "ceu_vars.h"

namespace WorldObjs {

Entrance::Entrance(const FileReader& reader) :
  direction(MISC),
  pos(),
  release_rate(150),
  owner_id(0),
  smallmap_symbol("core/misc/smallmap_entrance"),
  surface(),
  last_release(),
  last_direction(0)
{
  reader.read_int   ("owner-id",     owner_id);
  reader.read_vector("position",     pos);
  reader.read_int   ("release-rate", release_rate);

  // Set default owner ID to 0
  if (owner_id < 0 || owner_id > 3) owner_id = 0;

  std::string direction_str;
  reader.read_string("direction", direction_str);

  if (direction_str == "left")
    direction = LEFT;
  else if (direction_str == "right")
    direction = RIGHT;
  else if (direction_str == "misc")
    direction = MISC;
  else
  {
    log_error("unknown direction: '%1%'", direction_str);
    direction = MISC;
  }

  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_ENTRANCE_NEW, &this_);
}

Entrance::~Entrance ()
{
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_ENTRANCE_DELETE, &this_);
}

float
Entrance::get_z_pos () const
{
  return pos.z;
}

void
Entrance::update ()
{
}

void
Entrance::draw (SceneContext& gc)
{
  if (!surface)
  {
    // Entrances have only a surface for historical reasons
    // log_error("entrance without a surface?!");
    return;
  }

  // FIXME: Why do we still have these hardcoded offsets?!
  gc.color().draw(surface, Vector3f(pos.x - 32, pos.y - 16));
}

void
Entrance::draw_smallmap(SmallMap* smallmap)
{
  smallmap->draw_sprite(smallmap_symbol, pos);
}

} // namespace WorldObjs

/* EOF */
