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

#include "pingus/world.hpp"

World::World(const PingusLevel& plf)
{
  tceu__World___PingusLevel_ p = { this, (PingusLevel*)&plf };
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_NEW, &p);
}

World::~World() {
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_DELETE, &this_);
}

/// TODO: move to proper place!
#include "engine/display/sdl_framebuffer_surface_impl.hpp"
FramebufferSurface* load_framebuffer_sdl_surface(const Pathname& filename, 
ResourceModifier::Enum modifier)
{
  // FIXME: Implement proper cache
  try
  {
    Surface surface(filename);
    if (modifier != ResourceModifier::ROT0)
    {
      surface = surface.mod(modifier);
    }
    return new FramebufferSurface(new SDLFramebufferSurfaceImpl(surface.get_surface()));
  }
  catch(const std::exception& err)
  {
    // return a dummy surface for cases where the image file can't be found
    log_error("%1%", err.what());
    Surface surface(Pathname("images/core/misc/404.png", Pathname::DATA_PATH));
    return new FramebufferSurface(new SDLFramebufferSurfaceImpl(surface.get_surface()));
  }
}

/* EOF */
