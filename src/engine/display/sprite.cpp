//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/display/sprite.hpp"

#include "engine/display/sprite_description.hpp"
#include "engine/display/sprite_impl.hpp"
#include "pingus/resource.hpp"
#include "util/log.hpp"

#include "ceu_vars.h"

int Sprite::NEW     = 0;
int Sprite::NEW_DEL = 0;
int Sprite::CPY     = 0;
int Sprite::CPY_DEL = 0;

Sprite::Sprite(const Sprite& that)
{
  *this = that;
  this->XXX_is_copy = true;
  Sprite::CPY++;
}
Sprite::Sprite(const Sprite&& that)
{
  *this = that;
  this->XXX_is_copy = true;
  Sprite::CPY++;
}
#if 0
#endif
Sprite& Sprite::operator=(Sprite const &that)
{
  this->impl = that.impl;
  this->XXX_is_copy = true;
  Sprite::CPY++;
  return *this;
}

Sprite::Sprite() :
  impl()
{
  Sprite::NEW++;
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_NEW_NONE, &this_);
}

Sprite::Sprite(const std::string& name) :
  impl()
{
  Sprite::NEW++;
  char* str = (char*)name.c_str();
  if (!XXX_FROM_CEU) {
    tceu__Sprite___char_ p = {this, str};
    ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_NEW_NAME, &p);
  }
}

Sprite::Sprite(const ResDescriptor& res_desc) :
  impl()
{
  Sprite::NEW++;
  tceu__Sprite___ResDescriptor_ p = {this, (ResDescriptor*)&res_desc};
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_NEW_RESDESCRIPTOR, &p);
}

Sprite::Sprite(const Surface& surface) :
  impl()
{
  Sprite::NEW++;
  tceu__Sprite___Surface_ p = {this, (Surface*)&surface};
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_NEW_SURFACE, &p);
}

Sprite::Sprite(const SpriteDescription& desc, ResourceModifier::Enum mod) :
  impl()
{
  Sprite::NEW++;
  tceu__Sprite___SpriteDescription___int p = {this, (SpriteDescription*)&desc, mod};
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_NEW_SPRITEDESCRIPTION, &p);
}

Sprite::~Sprite()
{
  if (! this->XXX_is_copy) {
    Sprite::NEW_DEL++;
    void* this_ = this;
    ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_DELETE, &this_);
  } else {
    Sprite::CPY_DEL++;
  }
}

void
Sprite::render(int x, int y, Framebuffer& fb)
{
  ///if (impl != NULL)
    impl->render(x, y, fb);
}

int
Sprite::get_width() const
{
  ///if (impl != NULL)
    return impl->frame_size.width;
  ///else
    ///return 0;
}

int
Sprite::get_height() const
{
  ///if (impl != NULL)
    return impl->frame_size.height;
  ///else
    ///return 0;
}

Sprite::operator bool() const
{
  ///return (impl != NULL != 0);
  return true;
}

void
Sprite::update(float delta)
{
  tceu__SpriteImpl___float p = {impl.get(), delta};
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_IMPL_UPDATE, &p);
}

void
Sprite::set_frame(int i)
{
  ///if (impl != NULL)
    impl->frame = i;
}

int
Sprite::get_frame_count() const
{
  ///if (impl != NULL)
    return (impl->array.width * impl->array.height);
  ///else
    ///return 0;
}

bool
Sprite::is_finished() const
{
  ///if (impl != NULL)
    return impl->finished;
  ///else
    ///return true;
}

bool
Sprite::is_looping() const
{
  ///if (impl != NULL)
    return impl->loop_last_cycle;
  ///else
    ///return false;
}

void
Sprite::set_play_loop(bool loop)
{
  ///if (impl != NULL)
    impl->loop = loop;
}

int
Sprite::get_current_frame() const
{
  ///if (impl != NULL)
    return impl->frame;
  ///else
    ///return 0;
}

void
Sprite::restart()
{
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_IMPL_RESTART, &impl);
}

void
Sprite::finish()
{
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_IMPL_FINISH, &impl);
}

Vector2i
Sprite::get_offset() const
{
  ///if (impl != NULL)
    return impl->offset;
  ///else
    ///return Vector2i();
}

void
Sprite::set_hotspot(Origin origin, int x, int y)
{
  ///if (impl != NULL)
  ///{
    // FIXME: offset and other stuff should be member of the Sprite, not the SpriteImpl
    impl->offset = calc_origin(origin, impl->frame_size) - Vector2i(x, y);
  ///}
}

/* EOF */
