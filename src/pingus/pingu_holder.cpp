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

///
#include "engine/display/scene_context.hpp"

#include "pingus/pingu_holder.hpp"

#include "pingus/pingus_level.hpp"

PinguHolder::PinguHolder(const PingusLevel& plf) :
  number_of_allowed(plf.get_number_of_pingus()),
  number_of_exited(0),
  XXX_n(0)
{
}

PinguHolder::~PinguHolder()
{
}

void*
PinguHolder::create_pingu (const Vector3f& pos, int owner_id)
{
  if (number_of_allowed > get_number_of_released())
  {
    void* pingu = NULL;

    tceu__void____int__Vector3f___int p = { &pingu, XXX_n++, (Vector3f*)&pos, owner_id };
    ceu_sys_go(&CEU_APP, CEU_IN_PINGUHOLDER_CREATE_PINGU, &p);

    return pingu;
  }
  else
  {
    return 0;
  }
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

void PinguHolder::draw (SceneContext& gc) {}

void
PinguHolder::update()
{
}

float
PinguHolder::get_z_pos() const
{
  return 50;
}

int
PinguHolder::get_number_of_exited()
{
  return this->ceu->number_of_exited;
}

int
PinguHolder::get_number_of_killed()
{
  return this->ceu->number_of_killed;
}

int
PinguHolder::get_number_of_alive()
{
  return this->ceu->number_of_alive;
}

int
PinguHolder::get_number_of_released()
{
  return this->ceu->number_of_released;
}

int
PinguHolder::get_number_of_allowed()
{
  return this->ceu->number_of_allowed;
}

/* EOF */
