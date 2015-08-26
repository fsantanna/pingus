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

#ifndef HEADER_PINGUS_PINGUS_PINGU_HPP
#define HEADER_PINGUS_PINGUS_PINGU_HPP

#include <memory>

#include "math/vector3f.hpp"
#include "pingus/direction.hpp"
#include "pingus/action_name.hpp"

class ActionHolder;
class PinguAction;
class SceneContext;

/** The class for managing one of the many penguins which are walking
    around in the World. All actions are handled by PinguAction
    objects. */
class Pingu
{
public:
  /** The Pingus Status shows the current status of a Pingu, as
      displayed in the PingusCounter pannel. PS_DEAD are pingus that got
      killed, PS_ALIVE are pingus that are still active in the world and
      PS_EXITED are pingus that successfully finished a level

      FIXME: different subvalues of PS_DEAD might be usefull (drowned,
      FIXME: splashed, smashed, etc.) */
  enum PinguStatus { PS_ALIVE, PS_EXITED, PS_DEAD };

    ///
  ActionName::Enum current_action;
  ActionName::Enum wall_action;
  ActionName::Enum fall_action;


private:
  /** The primary action which is currently in use */
  std::shared_ptr<PinguAction> action;

  /** The uniq id of the Pingu, this is used to refer to the Pingu in
      a demo file or in a network connection */
  unsigned int id;

  /** The id of the owner of the pingus, used in multiplayer matches */
  int owner_id;

  /** The stat of the pingu, these can be modified by PinguActions */
  PinguStatus status;

  Vector3f velocity;

public:

  float pos_x;
  float pos_y;

  //FIXME make me private
  Direction direction;

  /** Creates a new Pingu at the given coordinates
      @param arg_id The uniq id of the pingu
      @param pos The start position of the pingu
      @param owner The owner id of the pingu (used for multiplayer) */
  Pingu (int arg_id, const Vector3f& pos, int owner);

  /** Destruct the pingu... */
  ~Pingu ();

  /** Return the logical pingus position, this is the position which
      is used for collision detection to the ground (the pingus
      feet) */
  Vector3f get_pos () const;

  /** Returns the visible position of the pingu, the graphical center
      of the pingu. */
  Vector3f get_center_pos () const;

  /** Returns the x position of the pingu
   * For backward comp. only
   */
  const float& get_x () const { return pos_x; }

  /** Returns the y position of the pingu
      For backward comp. only */
  const float& get_y () const { return pos_y; }

  int get_xi () const { return static_cast<int>(pos_x); }
  int get_yi () const { return static_cast<int>(pos_y); }

  /// Check if the pingu is still alive
  bool is_alive (void);

  /// Return the status of the pingu
  PinguStatus get_status (void) const;

  PinguStatus set_status (PinguStatus);

  /** The descriptive name of the action, this is used in the
      CaputreRectangle, so it can contain more than just the name
      (number of blocks, etc.) */
  std::string get_name();

  /// Returns the unique id of the pingu
  unsigned int  get_id (void);

  /// Set the pingu to the given coordinates
  void set_pos (float x, float y);
  void set_pos (int x, int y) { set_pos(static_cast<float>(x), static_cast<float>(y)); }

  void set_x (float x);

  void set_y (float y);

  /// Set the pingu to the given coordinates
  void set_pos (const Vector3f& arg_pos);

  Vector3f get_velocity () const { return velocity; }

  void set_velocity (const Vector3f& velocity_);

  // Set the pingu in the gives direction
  void set_direction (Direction d);

  /** Request an action to be set to the pingu, if its a persistent
      action, it will be hold back for later execution, same with a
      timed action, normal action will be applied if the current
      action allows that. */
  bool request_set_action (ActionName::Enum action_name);

  /** Set an action without any checking, the action will take
      instantly control. */
  void set_action (ActionName::Enum action_name);

  /// get the wall action if we have one
  ActionName::Enum request_wall_action ();

  /// get the fall action if we have one
  ActionName::Enum request_fall_action ();

  /** Returns the `color' of the colmap in the walking direction
      Examples:
      (0, -1) is the pixel under the pingu
      (1, 0)  is the pixel in front of the pingu
  */
  int  rel_getpixel (int x, int y);

  /** Returns true if the pingu needs to catch another pingu */
  bool need_catch ();

  void draw (SceneContext& gc);
  void apply_force (Vector3f);

  void update();

  /** Indicate if the pingu's speed is above the deadly velocity */
  //bool is_tumbling () const;

  float get_z_pos () const { return 0; }

  /** @return The owner_id of the owner, only used in multiplayer
      configurations, ought to be 0 in single player */
  int get_owner ();

  /** @return The owner_id as a string. Only used in multiplayer
      configurations, ought to be "0" in single player */
  std::string get_owner_str ();

  bool   is_over (int x, int y);

  bool   is_inside (int x1, int y1, int x2, int y2);

  float dist (int x, int y);

  /** @return the name of the action the Pingu currently has */
  ActionName::Enum get_action ();

private:
  ///Pingu (const Pingu&);
  ///Pingu& operator= (const Pingu&);

  Pingu(Pingu const &) = delete;
  void operator=(Pingu const &t) = delete;
  Pingu(Pingu &&) = delete;
};

#endif

/* EOF */
