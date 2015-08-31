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

#include "action_name.hpp"

#include "gettext.h"

namespace ActionName {

char* to_screenname(Enum action)
{
  switch (action)
  {
    case ANGEL          : return ("Angel");
    case BASHER         : return ("Basher");
    case BLOCKER        : return ("Blocker");
    case BOARDER        : return ("Boarder");
    case BOMBER         : return ("Bomber");
    case BRIDGER        : return ("Bridger");
    case CLIMBER        : return ("Climber");
    case DIGGER         : return ("Digger");
    case DROWN          : return ("Drown");
    case EXITER         : return ("Exiter");
    case FALLER         : return ("Faller");
    case FLOATER        : return ("Floater");
    case JUMPER         : return ("Jumper");
    case LASERKILL      : return ("Laserkill");
    case MINER          : return ("Miner");
    case SLIDER         : return ("Slider");
    case SMASHED        : return ("Smashed");
    case SPLASHED       : return ("Splashed");
    case SUPERMAN       : return ("Superman");
    case TELEPORTED     : return ("Teleported");
    case WAITER         : return ("Waiter");
    case WALKER         : return ("Walker");
    default             : return "Unknown ActionName";
  }
}

std::string to_string(Enum action)
{
  switch (action)
  {
    case ANGEL          : return "angel";
    case BASHER         : return "basher";
    case BLOCKER        : return "blocker";
    case BOARDER        : return "boarder";
    case BOMBER         : return "bomber";
    case BRIDGER        : return "bridger";
    case CLIMBER        : return "climber";
    case DIGGER         : return "digger";
    case DROWN          : return "drown";
    case EXITER         : return "exiter";
    case FALLER         : return "faller";
    case FLOATER        : return "floater";
    case JUMPER         : return "jumper";
    case LASERKILL      : return "laserkill";
    case MINER          : return "miner";
    case SLIDER         : return "slider";
    case SMASHED        : return "smashed";
    case SPLASHED       : return "splashed";
    case SUPERMAN       : return "superman";
    case TELEPORTED     : return "teleported";
    case WAITER         : return "waiter";
    case WALKER         : return "walker";
    default             : return "Unknown ActionName";
  }
}

Enum from_string(const std::string& action)
{
  if (action == "angel")          return ANGEL;
  if (action == "basher")         return BASHER;
  if (action == "blocker")        return BLOCKER;
  if (action == "boarder")        return BOARDER;
  if (action == "bomber")         return BOMBER;
  if (action == "bridger")        return BRIDGER;
  if (action == "climber")        return CLIMBER;
  if (action == "digger")         return DIGGER;
  if (action == "drown")          return DROWN;
  if (action == "exiter")         return EXITER;
  if (action == "faller")         return FALLER;
  if (action == "floater")        return FLOATER;
  if (action == "jumper")         return JUMPER;
  if (action == "laserkill")      return LASERKILL;
  if (action == "miner")          return MINER;
  if (action == "slider")         return SLIDER;
  if (action == "smashed")        return SMASHED;
  if (action == "splashed")       return SPLASHED;
  if (action == "superman")       return SUPERMAN;
  if (action == "teleported")     return TELEPORTED;
  if (action == "waiter")         return WAITER;
  if (action == "walker")         return WALKER;

  return WALKER;
}

} // namespace ActionName

/* EOF */
