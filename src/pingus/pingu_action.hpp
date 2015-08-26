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

#ifndef HEADER_PINGUS_PINGUS_PINGU_ACTION_HPP
#define HEADER_PINGUS_PINGUS_PINGU_ACTION_HPP

#include "math/vector3f.hpp"
#include "pingus/action_name.hpp"

class ActionHolder;
class SceneContext;
class Pingu;

/** This class provides an abstract interface for pingu actions. It is
    used to inherit classes which represent the actions. The actions
    are stored in a seperate library, have a look in actions/ for some
    examples. */
class PinguAction
{
protected:
  /** A pointer to the pingu, which hold the action. */
  Pingu*      pingu;

  ActionName::Enum action_name;  ///
  std::string      name;         ///


public:
  PinguAction(Pingu* p);
  PinguAction(Pingu* p, ActionName::Enum action_name); ///
  virtual ~PinguAction();

  /// Gives the PinguAction class access to the data of the Pingu.
  void set_pingu (Pingu*);

  /** Get the pixel from the colmap, relative to the pingu position.

      @param x x is relative to the direction, so 1 is the pixel
      infront of the pingu

      @param y 1 is up, -1 is down
  */
  int  rel_getpixel (int x, int y);

  /// The "AI" of the pingu.
  virtual void update (); /// = 0;

  /** Draws the action */
  virtual void draw (SceneContext& gc); /// =0;

  virtual Vector3f get_center_pos() const;

  /** The name of the action, this is used in the CaputreRectangle, so
      it can contain more than just the name */
  virtual std::string get_name () const;

  /// The type of the action
  ///virtual ActionName::Enum get_type () const =0;
  virtual ActionName::Enum get_type () const { return action_name; }

  /// Catch another pingu and act on it (see blocker.hxx)
  virtual bool  need_catch();

  /// The time the action needs to get activated (see bomber.cxx)
  virtual int   activation_time () { return -1; };

  /// True if Pingu in specified position would bang its head if it were walking
  bool head_collision_on_walk (int x, int y);

  /// True if Pingu in specified position would have a collision if it were walking
  bool collision_on_walk (int x, int y);

  /** Move Pingu according to the forces applied to it */
  void move_with_forces ();

private:
  //PinguAction (const PinguAction&);
  //PinguAction& operator= (const PinguAction&);

  PinguAction(PinguAction const &) = delete;
  void operator=(PinguAction const &t) = delete;
  PinguAction(PinguAction &&) = delete;
};

#endif /* PINGU_ACTION_HH */

/* EOF */
