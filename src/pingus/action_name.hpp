//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2009 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_ACTION_NAME_HPP
#define HEADER_PINGUS_PINGUS_ACTION_NAME_HPP

#include <string>

namespace ActionName {

/** The order of the actions here are the same as the order of actions
    in the buttonpanel, so if the order in the buttonpanel is not what
    it should be it needs to be changed here. */
enum Enum
{
  NONE=0,
  DEAD,
  ANGEL,
  BASHER,
  BLOCKER,
  BOARDER,
  BOMBER,
  BRIDGER,
  CLIMBER,
  DIGGER,
  DROWN,
  EXITER,
  FALLER,
  FLOATER,
  JUMPER,
  LASERKILL,
  MINER,
  SLIDER,
  SMASHED,
  SPLASHED,
  SUPERMAN,
  TELEPORTED,
  WAITER,
  WALKER,
  MAX
};

static char CATCHABLE[MAX] = {
    1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,0,0,1,1,1,1   // (drown,laserkill,smashed,splashed)
};

static char PERSISTENT_CHAR[MAX] = {
    0,0,0,0,0,0,0,0,'c',0,0,0,0,'f',0,0,0,0,0,0,0,0,0,0   // (climber,floater)
};

static char CHANGE_ALLOWED[MAX][MAX] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // NONE
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // DEAD
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // ANGEL
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // BASHER
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // BLOCKER
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // BOARDER
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // BOMBER
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // BRIDGER
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // CLIMBER
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // DIGGER
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // DROWN
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // EXITER
    {0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0},    // FALLER(bomber,climber,floater)
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // FLOATER(bomber)
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // JUMPER
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // LASERKILL
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // MINER
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // SLIDER
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // SMASHED
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // SPLASHED
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // SUPERMAN
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // TELEPORTED
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},    // WAITER
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}     // WALKER
};

/** Converts a action enum into its internal string representation, as
    used in the xml or the resource files Bomber => "bomber" */
std::string to_string(Enum action);

/** Converts a internal action string back into its action enum
    representation  "bomber" => Bomber */
Enum from_string(const std::string& action);

/** Convert a action enum into its visual representation, aka the
    thing that is seen on the screen for the user Bomber => "Bomber" */
std::string to_screenname(Enum action);

} // namespace ActionName

#endif

/* EOF */
