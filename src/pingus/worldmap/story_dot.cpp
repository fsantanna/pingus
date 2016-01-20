//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/display/drawing_context.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
///#include "pingus/screens/story_screen.hpp"
#include "util/file_reader.hpp"
#include "util/file_reader.hpp"
#include "util/log.hpp"
#include "util/pathname.hpp"

namespace WorldmapNS {

StoryDot::StoryDot(const FileReader& reader) :
  Dot(reader.read_section("dot"))
{
}

void StoryDot::draw(DrawingContext& gc) { }
void StoryDot::draw_hover(DrawingContext& gc) { }
void StoryDot::update(float delta) { }
void StoryDot::on_click() { }

} // namespace WorldmapNS

/* EOF */
