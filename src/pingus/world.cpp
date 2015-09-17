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
#include "pingus/pingus_level.hpp"
#include "util/log.hpp"

World::World(const PingusLevel& plf)
{
  tceu__World___PingusLevel_ p = { this, (PingusLevel*)&plf };
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_NEW, &p);
}

World::~World() {
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_DELETE, &this_);
}

void World::draw (SceneContext& gc) {
  SceneContext* p = &gc;
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_DRAW, &p);
}

void World::render(int x, int y, Framebuffer& fb) {
  tceu__int__int__Framebuffer_ p = {x,y,&fb};
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_RENDER, &p);
}


void World::draw_smallmap(SmallMap* smallmap) {
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_DRAW_SMALLMAP, &smallmap);
}

void World::update() {
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_UPDATE, NULL);
}

void World::armageddon(void) {
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_ARMAGEDDON, NULL);
}

bool World::check_armageddon (void) {
    CEU_World_check_armageddon(NULL, this->ceu);
}

void World::play_sound(std::string name, const Vector3f& pos, float volume) {
    CEU_World_play_sound(NULL, this->ceu, &name, (Vector3f*)&pos, volume);
}

float World::get_gravity() {
    CEU_World_get_gravity(NULL, this->ceu);
}

void World::put(int x, int y, Groundtype::GPType p) {
    assert(!"IS THIS USED?");
}

void World::put(const CollisionMask& mask, int x, int y, Groundtype::GPType type) {
    CEU_World_put(NULL, this->ceu, (CollisionMask*)&mask, x, y, type);
}

void World::remove(const CollisionMask& mask, int x, int y) {
    CEU_World_remove(NULL, this->ceu, (CollisionMask*)&mask, x, y);
}

Vector2i World::get_start_pos(int player_id) {
  return CEU_World_get_start_pos(NULL, this->ceu);
}

/// TODO: move to proper place!
#include "engine/display/sdl_framebuffer_surface_impl.hpp"
FramebufferSurface* load_framebuffer_sdl_surface(const Pathname& filename, 
ResourceModifier::Enum modifier)
{
  // FIXME: Implement proper cache
  try
  {
    Surface surface(filename);
    if (modifier != ResourceModifier::ROT0)
    {
      surface = surface.mod(modifier);
    }
    return new FramebufferSurface(new SDLFramebufferSurfaceImpl(surface.get_surface()));
  }
  catch(const std::exception& err)
  {
    // return a dummy surface for cases where the image file can't be found
    log_error("%1%", err.what());
    Surface surface(Pathname("images/core/misc/404.png", Pathname::DATA_PATH));
    return new FramebufferSurface(new SDLFramebufferSurfaceImpl(surface.get_surface()));
  }
}

/* EOF */
