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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJ_FACTORY_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJ_FACTORY_HPP

#include <map>
#include <vector>

#include "util/file_reader.hpp"

class WorldObj;
class WorldObjAbstractFactory;

class WorldObjFactory
{
private:
  static WorldObjFactory* instance_;
  WorldObjFactory ();

public:

  /// TEMP
  static Vector3f pos;

  static WorldObjFactory* instance ();
  void create(const FileReader& reader);

private:
  WorldObjFactory (const WorldObjFactory&);
  WorldObjFactory& operator= (const WorldObjFactory&);
};

#endif

/* EOF */
