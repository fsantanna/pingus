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

Sprite::Sprite(const Sprite& that)
{
    *this = that;
    this->XXX_is_copy = true;
}

Sprite::Sprite() :
  impl()
{
//printf("1>>>>>[%p]\n", this);
//assert(0);
  void* this_ = this;
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_NEW_NONE, &this_);
}

Sprite::Sprite(const std::string& name) :
  impl()
{
  ///SpriteDescription* desc = Resource::load_sprite_desc(name);
  ///if (desc)
  ///{
    ///impl = std::make_shared<SpriteImpl>(*desc);
  ///}
  ///else
  ///{
    ///SpriteDescription desc_;
    ///desc_.filename = Pathname("images/core/misc/404.png", 
    //Pathname::DATA_PATH);
    ///impl = std::make_shared<SpriteImpl>(desc_);
  ///}

//printf("2>>>>>[%p]\n", this);
  char* str = (char*)name.c_str();
  tceu__Sprite___char_ p = {this, str};
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_NEW_NAME, &p);
}

Sprite::Sprite(const ResDescriptor& res_desc) :
  impl()
{
  ///SpriteDescription* desc = Resource::load_sprite_desc(res_desc.res_name);
  ///if (desc)
  ///{
    ///impl = std::make_shared<SpriteImpl>(*desc, res_desc.modifier);
  ///}
  ///else
  ///{
    ///SpriteDescription desc_;
    ///desc_.filename = Pathname("images/core/misc/404.png", //Pathname::DATA_PATH);
    ///impl = std::make_shared<SpriteImpl>(desc_);
  ///}

//printf("3>>>>>[%p]\n", this);
  tceu__Sprite___ResDescriptor_ p = {this, (ResDescriptor*)&res_desc};
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_NEW_RESDESCRIPTOR, &p);
}

Sprite::Sprite(const Surface& surface) :
  impl()
  ///impl(std::make_shared<SpriteImpl>(surface))
{
//printf("4>>>>>[%p]\n", this);
  tceu__Sprite___Surface_ p = {this, (Surface*)&surface};
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_NEW_SURFACE, &p);
}

Sprite::Sprite(const SpriteDescription& desc, ResourceModifier::Enum mod) :
  impl()
  ///impl(std::make_shared<SpriteImpl>(desc, mod))
{
//printf("5>>>>>[%p]\n", this);
  tceu__Sprite___SpriteDescription___int p = {this, (SpriteDescription*)&desc, mod};
  ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_NEW_SPRITEDESCRIPTION, &p);
}

Sprite::~Sprite()
{
//printf("->>>>>[%p]\n", this);
  if (! this->XXX_is_copy) {
    void* this_ = this;
    ceu_sys_go(&CEU_APP, CEU_IN_SPRITE_DELETE, &this_);
  } else {
    //printf("...\n");
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
  ///if (impl != NULL)
    impl->update(delta);
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
  ///if (impl != NULL)
    impl->restart();
}

void
Sprite::finish()
{
  ///if (impl != NULL)
    impl->finish();
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
