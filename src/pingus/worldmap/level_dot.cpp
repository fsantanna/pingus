//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingus/worldmap/level_dot.hpp"

#include "pingus/globals.hpp"
#include "pingus/plf_res_mgr.hpp"
#include "pingus/savegame_manager.hpp"

namespace WorldmapNS {

LevelDot::LevelDot(const FileReader& reader) :
  Dot(reader.read_section("dot")),
  plf()
{
  std::string resname;
  reader.read_string("levelname", resname); 

  plf = PLFResMgr::load_plf(resname);
}

bool
LevelDot::finished()
{
  Savegame* savegame = SavegameManager::instance()->get(plf.get_resname());
  if (savegame && savegame->get_status() == Savegame::FINISHED)
    return true;
  else
    return false;
}

bool
LevelDot::accessible()
{
  Savegame* savegame = SavegameManager::instance()->get(plf.get_resname());
  if (savegame && savegame->get_status() != Savegame::NONE)
    return true;
  else
    return false;
}

void
LevelDot::unlock()
{
  Savegame* savegame = SavegameManager::instance()->get(plf.get_resname());
  if (savegame == 0 || savegame->get_status() == Savegame::NONE)
  {
    Savegame savegame_(plf.get_resname(),
                       Savegame::ACCESSIBLE,
                       0,
                       0);
    SavegameManager::instance()->store(savegame_);
  }
  else
  {
  }
}

} // namespace WorldmapNS

/* EOF */
