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
struct CEU_PinguHolder;

///namespace Particles {
///class RainParticleHolder;
///class SnowParticleHolder;
///}

/** The World holds all objects of the pingu enviroment.

    It holds the pingus, traps, exits, entrances, etc.. It keeps
    control that all objects become time to move and can catch each
    other. */
class World
{
private:
  CEU_World* ceu;

  Color ambient_light;

  /** groundmap for the pingus */
  GroundMap* gfx_map;

  /** manager class for the time in the pingus world */
  int game_time;

  /** set to true once an armageddon got started */
  bool do_armageddon;

  ///Particles::RainParticleHolder*  rain_particle_holder;
  ///Particles::SnowParticleHolder*  snow_particle_holder;
  CEU_PinguHolder* pingus;

  // Pointers which are references to objects from other classes
  CollisionMap*  colmap;

  /** Acceleration due to gravity in the world */
  const float gravitational_acceleration;

public:
  World(const PingusLevel& level);
  virtual ~World();

/// TEMP
  void render(int x, int y, Framebuffer& fb);

  /** Draw the world onto the given SceneContext */
  void    draw (SceneContext& gc);

  /** Draw the world onte the given SmallMap*/
  void    draw_smallmap(SmallMap* smallmap);

  /** Update the World */
  void    update ();

  /** Issue an armageddon, all Pingus will explode in some seconds. */
  void    armageddon ();

  /** @return The absolute height of the world. */
  int     get_height ();
  /** @return The absolute width of the world */
  int     get_width();

  /** @return A pointer to the collision map used in this world */
  CollisionMap* get_colmap();

  /** @return A pointer to the gfx map used in this world */
  GroundMap* get_gfx_map();

  void put(int x, int y, Groundtype::GPType p = Groundtype::GP_GROUND);
  void put(const CollisionMask&, int x, int y, Groundtype::GPType);

  void remove(const CollisionMask&, int x, int y);

  /** @return true if the world is currently doing an armageddon */
  bool check_armageddon() { return do_armageddon; }

  /** Play a sound as if it would have been generated at the given
      position, adjust panning and volume by the position relative to
      the center of the screen
      @param name The name of the sound to play ("ohno", not "sounds/ohno.wav")
      @param pos Position from which the sound seems to come (z-pos is
      going to be ignored) void play_sound (std::string name, const
      @param volume The volume of the sound
  */
  void play_sound (std::string name, const Vector3f& pos, float volume = 0.5f);

  /** Get the acceleration due to gravity in the world */
  float get_gravity();

  /** Returns the start pos for the given player */
  Vector2i get_start_pos(int player_id);

private:
  World (const World&);
  World& operator= (const World&);
};

#endif

/* EOF */
