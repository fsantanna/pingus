//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingus/worldmap/story_dot.hpp"

#include "util/file_reader.hpp"
#include "util/file_reader.hpp"
#include "util/log.hpp"
#include "util/pathname.hpp"

namespace WorldmapNS {

StoryDot::StoryDot(const FileReader& reader) :
  Dot(reader.read_section("dot")),
  m_name(),
  m_story(),
  m_credits(false)
{
  reader.read_string("name", m_name);
  reader.read_string("story", m_story);
  reader.read_bool("credits", m_credits);
}

} // namespace WorldmapNS

/* EOF */
