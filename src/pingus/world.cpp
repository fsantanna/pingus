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

#include "pingus/world.hpp"

#include <algorithm>

#include "engine/display/scene_context.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/ground_map.hpp"
///#include "pingus/particles/rain_particle_holder.hpp"
///#include "pingus/particles/snow_particle_holder.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/worldobj_factory.hpp"
#include "pingus/worldobjs/entrance.hpp"
#include "util/log.hpp"

static
bool WorldObj_less (WorldObj* a, WorldObj* b)
{
  return a->get_z_pos () < b->get_z_pos ();
}

World::World(const PingusLevel& plf) :
  ambient_light(Color(plf.get_ambient_light())),
  gfx_map(new GroundMap(plf.get_size().width, plf.get_size().height)),
  game_time(0),
  do_armageddon(false),
  ///rain_particle_holder(),
  ///snow_particle_holder(),
  colmap(gfx_map->get_colmap()),
  gravitational_acceleration(0.2f)
{
  log_debug("create particle holder");

  pingus = new PinguHolder(plf);

  tceu__World___PingusLevel_ p = { this, (PingusLevel*)&plf };
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_NEW, &p);

  const std::vector<FileReader>& objects = plf.get_objects();
  for (std::vector<FileReader>::const_iterator i = objects.begin();
       i != objects.end ();
       ++i)
  {
    std::vector<WorldObj*> objs = WorldObjFactory::instance()->create(*i);
  }
}

World::~World()
{
  delete(gfx_map);

  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_DELETE, &this_);
}

void
World::draw (SceneContext& gc)
{
  gc.light().fill_screen(Color(ambient_light));

  gfx_map->draw(gc);
  pingus->draw(gc);

  SceneContext* p = &gc;
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_DRAW, &p);
}

void
World::draw_smallmap(SmallMap* smallmap)
{
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_DRAW_SMALLMAP, &smallmap);
}

void
World::update()
{
  game_time += 1;
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_UPDATE, &game_time);
}

PinguHolder*
World::get_pingus()
{
  return pingus;
}

int
World::get_width()
{
  assert(gfx_map);
  return gfx_map->get_width();
}

int
World::get_height()
{
  assert(gfx_map);
  return gfx_map->get_height();
}

int
World::get_time()
{
  return game_time;
}

void
World::armageddon(void)
{
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_ARMAGEDDON, NULL);
}

CollisionMap*
World::get_colmap()
{
  return colmap;
}

GroundMap*
World::get_gfx_map()
{
  return gfx_map;
}

void
World::play_sound(std::string name, const Vector3f& pos, float volume)
{
  float panning = 0.0f;
  Sound::PingusSound::play_sound(name, volume, panning);
}

float World::get_gravity()
{
  return gravitational_acceleration;
}

void
World::put(int x, int y, Groundtype::GPType p)
{
}

void
World::put(const CollisionMask& mask, int x, int y, Groundtype::GPType type)
{
  gfx_map->put(mask.get_surface(), x, y);
  colmap->put(mask, x, y, type);
}

void
World::remove(const CollisionMask& mask, int x, int y)
{
  gfx_map->remove(mask.get_surface(), x, y);
  colmap->remove(mask, x, y);
}

Vector2i
World::get_start_pos(int player_id)
{
  return CEU_World_get_start_pos(NULL, this->ceu);
}

/* EOF */
