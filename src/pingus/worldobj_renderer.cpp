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

#include "pingus/worldobj_renderer.hpp"

#include "math/vector3f.hpp"
#include "util/log.hpp"

WorldObjRenderer::WorldObjRenderer(Surface& output) :
  m_surface(output)
{
}

void
WorldObjRenderer::render_sprite(const ResDescriptor& desc,
                                const Vector3f& pos)
{
  Surface surface = Resource::load_surface(desc);

  // FIXME: hack, should take that info from the resource file
  m_surface.blit(surface, 
                 static_cast<int>(pos.x) - surface.get_width()/2, 
                 static_cast<int>(pos.y) - surface.get_height());    
}

void
WorldObjRenderer::render_surface(const ResDescriptor& desc,
                                 const Vector3f& pos,
                                 int repeat)
{
  Surface surface = Resource::load_surface(desc);
  for(int i = 0; i < repeat; ++i)
  {
    m_surface.blit(surface, 
                   static_cast<int>(pos.x) + surface.get_width()*i,
                   static_cast<int>(pos.y));
  }
}

void
WorldObjRenderer::process(const std::vector<FileReader>& readers)
{
  for(auto it = readers.begin(); it != readers.end(); ++it)
  {
    process(*it);
  }
}

void
WorldObjRenderer::process(const FileReader& reader)
{
  // FIXME: does not handle sprite alignment
  // FIXME: does not handle remove groundpieces
  // FIXME: does not handle liquid
  if (reader.get_name() == "surface-background" ||
      reader.get_name() == "starfield-background" ||
      reader.get_name() == "solidcolor-background")
  {
    // ignore
  }
  else if (reader.get_name() == "groundpiece" ||
           reader.get_name() == "liquid" ||
           reader.get_name() == "exit" ||
           reader.get_name() == "hotspot")
  {
    process_object_with_surface(reader);
  }
  else if (reader.get_name() == "entrance")
  {
    Vector3f pos;
    reader.read_vector("position", pos);
    render_sprite(ResDescriptor("entrances/generic"), pos);
  }
  else
  {
    log_error("unknown object type: " << reader.get_name());
  }
}

void
WorldObjRenderer::process_object_with_surface(const FileReader& reader)
{
  Vector3f pos;
  ResDescriptor desc;
    
  if (!(reader.read_vector("position", pos) &&
        reader.read_desc("surface", desc)))
  {
    log_error("object (" << reader.get_name() << ") does not have 'position' and 'surface'");
  }
  else
  {
    if (reader.get_name() == "exit" ||
        reader.get_name() == "hotspots")
    {
      render_sprite(desc, pos);
    }
    else if (reader.get_name() == "groundpiece")
    {
      std::string type;
      reader.read_string("type", type);
      if (type == "remove")
      {
        // FIXME: don't have blit_remove()
        render_surface(desc, pos);
      }
      else
      {
        render_surface(desc, pos);
      }
    }
    else
    {
      int repeat = 1;
      reader.read_int("repeat", repeat);
      render_surface(desc, pos, repeat);
    }
  }
}

/* EOF */