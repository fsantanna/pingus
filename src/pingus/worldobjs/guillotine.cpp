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

#include "pingus/worldobjs/guillotine.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/world.hpp"

#include "ceu_vars.h"

namespace WorldObjs {

Guillotine::Guillotine(const FileReader& reader) :
  sprite_kill_right("traps/guillotinekill/right"),
  sprite_kill_left("traps/guillotinekill/left"),
  sprite_idle("traps/guillotineidle"),
  pos(),
  direction(),
  killing(false)
{
  reader.read_vector("position", pos);

  sprite_kill_right.set_play_loop(false);
  sprite_kill_left.set_play_loop(false);
  sprite_idle.set_play_loop(true);

  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_GUILLOTINE_NEW, &this_);
}

Guillotine::~Guillotine()
{
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_GUILLOTINE_DELETE, &this_);
}

void
Guillotine::draw (SceneContext& gc)
{
  if (killing) {
    if (direction.is_left())
      gc.color().draw (sprite_kill_left, pos);
    else
      gc.color().draw (sprite_kill_right, pos);
  } else {
    gc.color().draw (sprite_idle, pos);
  }
}

float
Guillotine::get_z_pos () const
{
  return pos.z;
}

void
Guillotine::update ()
{
}

void
Guillotine::catch_pingu (Pingu* pingu)
{
}

} // namespace WorldObjs

/* EOF */
