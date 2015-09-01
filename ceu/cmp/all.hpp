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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_BASHER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_BASHER_HPP

#include "pingus/collision_mask.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

class Basher : public PinguAction
{
private:
  StateSprite   sprite;
  CollisionMask bash_radius;
  int  basher_c;
  bool first_bash;

  /** The no. of pixels ahead that a Basher checks for something bashable.
      This is initialised using the size of the bash_radius surface. */
  int bash_reach;

  /** Defines the minimum "wall" height needed for a bash to happen. */
  static const int min_bash_height = 5;

  /** Defines the maximum height up to which a check is made to see if there
      is anything to bash.  Best to make this at least (min_bash_height +
      pingu_height). */
  static const int max_bash_height = 33;

  /** Defines the maximum no. of steps down a Basher can go down before it
      stops being a Basher and turns into a Faller. */
  static const int max_steps_down = 3;

public:
  Basher (Pingu* p);

  ActionName::Enum get_type () const { return ActionName::BASHER; }

  void draw (SceneContext& gc);
  void update ();

  bool have_something_to_dig ();
  bool walk_forward ();
  void bash ();

private:
  Basher (const Basher&);
  Basher& operator= (const Basher&);
};

} // namespace Actions

#endif

/* EOF */
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_BLOCKER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_BLOCKER_HPP

#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

class Blocker : public PinguAction
{
private:
  StateSprite sprite;

public:
  Blocker (Pingu* p);

  ActionName::Enum get_type() const { return ActionName::BLOCKER; }

  void  draw (SceneContext& gc);
  void  update();

private:
  bool  standing_on_ground();
  void  catch_pingu(Pingu* pingu);

  Blocker (const Blocker&);
  Blocker& operator= (const Blocker&);
};

} // namespace Actions

#endif

/* EOF */
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_BOMBER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_BOMBER_HPP

#include "pingus/collision_mask.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

/** An action with lets the Pingu explode. After the explosion the the
    Pingu leaves a hole inside the ground. */
class Bomber : public PinguAction
{
private:
  bool particle_thrown;
  bool sound_played;
  bool gfx_exploded;
  bool colmap_exploded;

  CollisionMask bomber_radius;
  StateSprite sprite;

  Sprite   explo_surf;

public:
  Bomber (Pingu* p);

  ActionName::Enum get_type() const { return ActionName::BOMBER; }

  bool change_allowed (ActionName::Enum action) { return false; }

  void draw (SceneContext& gc);
  void update();

private:
  Bomber (const Bomber&);
  Bomber& operator= (const Bomber&);
};

} // namespace Actions

#endif

/* EOF */
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_BRIDGER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_BRIDGER_HPP

#include "math/vector3f.hpp"
#include "pingus/collision_mask.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

class Bridger : public PinguAction
{
private:
  enum Mode { B_WALKING, B_BUILDING } mode;
  enum { MAX_BRICKS = 15 };
  enum { brick_length = 16 };

private:
  StateSprite walk_sprite;
  StateSprite build_sprite;
  CollisionMask brick_l;
  CollisionMask brick_r;

  int bricks;
  //int step;
  //int do_steps;
  bool block_build;

  std::string name;

public:
  Bridger(Pingu*);

  std::string get_name () const;
  ActionName::Enum get_type () const { return ActionName::BRIDGER; }

  void   update ();
  void   update_build ();
  void   update_walk ();

  void   draw (SceneContext& gc);

  bool   way_is_free ();
  bool   brick_placement_allowed (void);
  void   place_a_brick ();
  void   walk_one_step_up ();

private:
  Bridger (const Bridger&);
  Bridger& operator= (const Bridger&);
};

} // namespace Actions

#endif

/* EOF */
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_DIGGER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_DIGGER_HPP

#include "pingus/collision_mask.hpp"
#include "engine/display/sprite.hpp"
#include "pingus/pingu_action.hpp"

namespace Actions {

class Digger : public PinguAction
{
private:
  CollisionMask digger_radius;
  CollisionMask digger_radius_final;
  Sprite sprite;
  int delay_count;

public:
  Digger(Pingu*);

  ActionName::Enum get_type() const { return ActionName::DIGGER; }

  bool have_something_to_dig();
  void dig(bool final);

  void draw(SceneContext& gc);
  void update();

private:
  Digger (const Digger&);
  Digger& operator= (const Digger&);
};

} // namespace Actions

#endif

/* EOF */
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_DROWN_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_DROWN_HPP

#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

class Drown : public PinguAction
{
private:
  StateSprite sprite;

public:
  Drown (Pingu* p);

  ActionName::Enum get_type () const { return ActionName::DROWN; }

  void draw (SceneContext& gc);
  void update ();

  bool catchable () { return false; }

private:
  Drown (const Drown&);
  Drown& operator= (const Drown&);
};

} // namespace Actions

#endif

/* EOF */
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_EXITER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_EXITER_HPP

#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

class Exiter : public PinguAction
{
private:
  StateSprite sprite;
  bool sound_played;

public:
  Exiter(Pingu*);
  void init(void);
  ActionName::Enum get_type() const { return ActionName::EXITER; }

  void draw (SceneContext& gc);
  void update();

private:
  Exiter (const Exiter&);
  Exiter& operator= (const Exiter&);
};

} // namespace Actions

#endif

/* EOF */
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_FALLER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_FALLER_HPP

#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

class Faller : public PinguAction
{
private:
  StateSprite faller;
  StateSprite tumbler;

public:
  Faller(Pingu*);
  virtual ~Faller();

  void  draw (SceneContext& gc);
  void  update();

  bool change_allowed (ActionName::Enum new_action);

  ActionName::Enum get_type() const { return ActionName::FALLER; }
  bool is_tumbling () const;

private:
  Faller (const Faller&);
  Faller& operator= (const Faller&);
};

} // namespace Actions

#endif

/* EOF */
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_FLOATER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_FLOATER_HPP

#include "engine/display/sprite.hpp"
#include "pingus/pingu_action.hpp"

namespace Actions {

class Floater : public PinguAction
{
private:
  int falling_depth;
  int step;
  Sprite sprite;

public:
  Floater(Pingu* p);

  ActionName::Enum get_type() const { return ActionName::FLOATER; }

  void init(void);

  void draw (SceneContext& gc);
  void update();

  char get_persistent_char () { return 'f'; }
  bool change_allowed (ActionName::Enum new_action);

private:
  Floater (const Floater&);
  Floater& operator= (const Floater&);
};

} // namespace Actions

#endif

/* EOF */
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_JUMPER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_JUMPER_HPP

#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

class Jumper : public PinguAction
{
private:
  StateSprite sprite;

public:
  Jumper(Pingu*);

  ActionName::Enum get_type() const { return ActionName::JUMPER; }

  void  draw (SceneContext& gc);
  void  update();

private:
  Jumper (const Jumper&);
  Jumper& operator= (const Jumper&);
};

} // namespace Actions

#endif

/* EOF */
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_MINER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_MINER_HPP

#include "pingus/collision_mask.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

class Miner : public PinguAction
{
private:
  CollisionMask miner_radius;
  CollisionMask miner_radius_left;
  CollisionMask miner_radius_right;

  StateSprite sprite;
  int delay_count;

public:
  Miner (Pingu* p);
  virtual ~Miner () {}

  ActionName::Enum get_type () const { return ActionName::MINER; }

  void draw (SceneContext& gc);
  void update ();

private:
  void mine(bool final);

private:
  Miner (const Miner&);
  Miner& operator= (const Miner&);
};

} // namespace Actions

#endif

/* EOF */
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_SPLASHED_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_SPLASHED_HPP

#include "engine/display/sprite.hpp"
#include "pingus/pingu_action.hpp"

namespace Actions {

class Splashed : public PinguAction
{
private:
  bool particle_thrown;
  bool sound_played;
  Sprite sprite;

public:
  Splashed (Pingu*);

  ActionName::Enum get_type () const { return ActionName::SPLASHED; }

  void draw (SceneContext& gc);
  void update ();

  bool catchable () { return false; }
  bool change_allowed (ActionName::Enum ) { return false; }

private:
  Splashed (const Splashed&);
  Splashed& operator= (const Splashed&);
};

} // namespace Actions

#endif

/* EOF */
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_WAITER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_WAITER_HPP

#include "engine/display/sprite.hpp"
#include "pingus/pingu_action.hpp"

namespace Actions {

/** A Waiting action for the bridger, it gets activated when the
    bridger is out of bridges. It then waits two seconds (meanwhile doing a
    funny animation) and then he changes back to a normal walker. */
class Waiter : public PinguAction
{
private:
  float countdown;
  Sprite sprite;

public:
  Waiter (Pingu*);

  ActionName::Enum get_type () const { return ActionName::WAITER; }

  void draw (SceneContext& gc);
  void update ();

private:
  Waiter (const Waiter&);
  Waiter& operator= (const Waiter&);
};

} // namespace Actions

#endif

/* EOF */
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_ACTIONS_WALKER_HPP
#define HEADER_PINGUS_PINGUS_ACTIONS_WALKER_HPP

#include "pingus/pingu_action.hpp"
#include "pingus/state_sprite.hpp"

namespace Actions {

class Walker : public PinguAction
{
private:
  StateSprite walker;
  StateSprite floaterlayer;

  enum { max_steps = 5 }; // max nr. of pixels that pingu can walk up/down

public:
  Walker (Pingu*);

  void draw (SceneContext& gc);
  void update ();

  ActionName::Enum get_type () const { return ActionName::WALKER; }

private:
  Walker (const Walker&);
  Walker& operator= (const Walker&);
};

} // namespace Actions

#endif

/* EOF */
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

enum ActionType
{
  INSTANT,
  WALL_TRIGGERED,
  FALL_TRIGGERED,
  COUNTDOWN_TRIGGERED
};

/** This class provides an abstract interface for pingu actions. It is
    used to inherit classes which represent the actions. The actions
    are stored in a seperate library, have a look in actions/ for some
    examples. */
class PinguAction
{
protected:
  /** A pointer to the pingu, which hold the action. */
  Pingu*      pingu;

public:
  PinguAction(Pingu* p);
  virtual ~PinguAction();

  /// Gives the PinguAction class access to the data of the Pingu.
  void set_pingu (Pingu*);

  /** Get the pixel from the colmap, relative to the pingu position.

      @param x x is relative to the direction, so 1 is the pixel
      infront of the pingu

      @param y 1 is up, -1 is down
  */
  int  rel_getpixel (int x, int y);

  /** Checks if this action allows to be overwritten with the given new action */
  virtual bool change_allowed (ActionName::Enum action) { return true; }

  /// The "AI" of the pingu.
  virtual void update () = 0;

  /** Draws the action */
  virtual void draw (SceneContext& gc) =0;

  virtual Vector3f get_center_pos() const;

  /** The name of the action, this is used in the CaputreRectangle, so
      it can contain more than just the name */
  virtual std::string get_name () const;

  /// The type of the action
  virtual ActionName::Enum get_type () const =0;

  /** Return the character that is shown when a persitent action is
      activated in the CaptureRectangle. */
  virtual char get_persistent_char ();

  /// Catch another pingu and act on it (see blocker.hxx)
  virtual bool  need_catch();

  ///
  virtual void  catch_pingu (Pingu*) { /* do nothing */}

  /// The time the action needs to get activated (see bomber.cxx)
  virtual int   activation_time () { return -1; };

  /** Return true if the pingu can be caught with the mouse and
      another action can be applied, false otherwise (exiter,
      splashed, etc.) */
  virtual bool catchable () { return true; }

  /// True if Pingu in specified position would bang its head if it were walking
  bool head_collision_on_walk (int x, int y);

  /// True if Pingu in specified position would have a collision if it were walking
  bool collision_on_walk (int x, int y);

  /** Move Pingu according to the forces applied to it */
  void move_with_forces ();

public:
  static ActionType get_activation_mode(ActionName::Enum action_name);

private:
  PinguAction (const PinguAction&);
  PinguAction& operator= (const PinguAction&);
};

#endif /* PINGU_ACTION_HH */

/* EOF */
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

private:
  /** The primary action which is currently in use */
  std::shared_ptr<PinguAction> action;

  /** A secondary action which will turn active after a given amount of time
      The only example is currently the bomber. */
  std::shared_ptr<PinguAction> countdown_action;

  /** the action that gets triggered when the pingu hits a wall */
  std::shared_ptr<PinguAction> wall_action;

  /** the action that gets triggered when the pingu falls */
  std::shared_ptr<PinguAction> fall_action;

  /** The previous_action contains the action type that was in action
      before action got applied, its here to enable action to behave
      differently depending on the previous action */
  ActionName::Enum previous_action;

  /** The uniq id of the Pingu, this is used to refer to the Pingu in
      a demo file or in a network connection */
  unsigned int id;

  /** Countdown till countdown_action is triggered (-1 for no active countdown) */
  int action_time;

  /** The id of the owner of the pingus, used in multiplayer matches */
  int owner_id;

  /** The stat of the pingu, these can be modified by PinguActions */
  PinguStatus status;

  float pos_x;
  float pos_y;

  Vector3f velocity;

private:
  void set_action(std::shared_ptr<PinguAction>);

  std::shared_ptr<PinguAction> create_action(ActionName::Enum action);

public:

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

  /** Checks if this action allows to be overwritten with the given new action */
  bool change_allowed (ActionName::Enum new_action);

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

  /// set the wall action if we have one
  bool request_wall_action ();

  /// set the fall action if we have one
  bool request_fall_action ();

  PinguAction* get_wall_action () { return wall_action.get(); }

  PinguAction* get_fall_action () { return fall_action.get(); }

  /** Returns the `color' of the colmap in the walking direction
      Examples:
      (0, -1) is the pixel under the pingu
      (1, 0)  is the pixel in front of the pingu
  */
  int  rel_getpixel (int x, int y);

  /** Let the pingu catch another pingu, so that an action can be
      applied (i.e. let a blocker change the direction f another
      pingu) */
  void catch_pingu (Pingu* pingu);

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

  /** Return true if the pingu can be caught with the mouse and
      another action can be applied, false otherwise (exiter,
      splashed, etc.) */
  bool catchable ();

  /** @return the name of the action the Pingu currently has */
  ActionName::Enum get_action ();

  /** @return the action that was active before the action returned by
      get_action() took place. This is used in a few situations where
      an action needs to now what the Pingu was doing before the
      action took place (faller->bomber translation is different
      walker->bomber, etc.). */
  ActionName::Enum get_previous_action() const { return previous_action; }

private:
  Pingu (const Pingu&);
  Pingu& operator= (const Pingu&);
};

#endif

/* EOF */
