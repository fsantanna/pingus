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

#include "pingus/pingu.hpp"
#include "pingus/pingus_level.hpp"

#include "ceu_vars.h"

PinguHolder::PinguHolder(const PingusLevel& plf) :
  number_of_allowed(plf.get_number_of_pingus()),
  number_of_exited(0),
  all_pingus(),
  pingus()
{
  tceu__PinguHolder___int p = {this, number_of_allowed};
  ceu_sys_go(&CEU_APP, CEU_IN_PINGUHOLDER_NEW, &p);
}

PinguHolder::~PinguHolder()
{
  for(std::vector<Pingu*>::iterator i = all_pingus.begin();
      i != all_pingus.end(); ++i)
    delete *i;

  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_PINGUHOLDER_DELETE, this_);
}

Pingu*
PinguHolder::create_pingu (const Vector3f& pos, int owner_id)
{
  if (number_of_allowed > get_number_of_released())
  {
    // We use all_pingus.size() as pingu_id, so that id == array
    // index
    Pingu* pingu = new Pingu (static_cast<int>(all_pingus.size()), pos, owner_id);

    // This list will deleted
    all_pingus.push_back (pingu);

    // This list holds the active pingus
    pingus.push_back(pingu);

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


void
PinguHolder::draw (SceneContext& gc)
{
  gc.color().draw(*this, Vector2i(0,0));
}

void
PinguHolder::render(int x, int y, Framebuffer& fb)
{
  tceu__int__int__Framebuffer_ p = {x,y,&fb};
  ceu_sys_go(&CEU_APP, CEU_IN_PINGUHOLDER_RENDER, &p);
}

void
PinguHolder::update()
{
  ceu_sys_go(&CEU_APP, CEU_IN_PINGUHOLDER_UPDATE, NULL);
}

Pingu*
PinguHolder::get_pingu(unsigned int id_)
{
  if (id_ < all_pingus.size())
  {
    Pingu* pingu = all_pingus[id_];

    assert(pingu->get_id() == id_);

    if (pingu->get_status() == Pingu::PS_ALIVE)
      return pingu;
    else
      return 0;
  }
  else
  {
    return 0;
  }
}

float
PinguHolder::get_z_pos() const
{
  return 50;
}

int
PinguHolder::get_number_of_exited()
{
  tceu__PinguHolder___int_ p = {this, &number_of_exited};
  ceu_sys_go(&CEU_APP, CEU_IN_PINGUHOLDER_GET_NUMBER_OF_EXITED, &p);
  return number_of_exited;
}

int
PinguHolder::get_number_of_killed()
{
  int number_of_killed;
  tceu__PinguHolder___int_ p = {this, &number_of_killed};
  ceu_sys_go(&CEU_APP, CEU_IN_PINGUHOLDER_GET_NUMBER_OF_KILLED, &p);
  return number_of_killed;
}

int
PinguHolder::get_number_of_alive()
{
  int number_of_alive;
  tceu__PinguHolder___int_ p = {this, &number_of_alive};
  ceu_sys_go(&CEU_APP, CEU_IN_PINGUHOLDER_GET_NUMBER_OF_ALIVE, &p);
  return number_of_alive;
}

int
PinguHolder::get_number_of_released()
{
  int number_of_released;
  tceu__PinguHolder___int_ p = {this, &number_of_released};
  ceu_sys_go(&CEU_APP, CEU_IN_PINGUHOLDER_GET_NUMBER_OF_RELEASED, &p);
  return number_of_released;
}

int
PinguHolder::get_number_of_allowed()
{
  tceu__PinguHolder___int_ p = {this, &number_of_allowed};
  ceu_sys_go(&CEU_APP, CEU_IN_PINGUHOLDER_GET_NUMBER_OF_ALLOWED, &p);
  return number_of_allowed;
}

unsigned int
PinguHolder::get_end_id()
{
  return static_cast<unsigned int>(all_pingus.size());
}

/* EOF */
