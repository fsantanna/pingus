Sprite::Sprite(const std::string& name) :
  impl()
{
  SpriteDescription* desc = Resource::load_sprite_desc(name);
  if (desc)
  {
    impl = std::make_shared<SpriteImpl>(*desc);
  }
  else
  {
    SpriteDescription desc_;
    desc_.filename = Pathname("images/core/misc/404.png", Pathname::DATA_PATH);
    impl = std::make_shared<SpriteImpl>(desc_);
  }
}

Sprite::Sprite(const ResDescriptor& res_desc) :
  impl()
{
  SpriteDescription* desc = Resource::load_sprite_desc(res_desc.res_name);
  if (desc)
  {
    impl = std::make_shared<SpriteImpl>(*desc, res_desc.modifier);
  }
  else
  {
    SpriteDescription desc_;
    desc_.filename = Pathname("images/core/misc/404.png", Pathname::DATA_PATH);
    impl = std::make_shared<SpriteImpl>(desc_);
  }
}

Sprite::Sprite(const Surface& surface) :
  impl(std::make_shared<SpriteImpl>(surface))
{
}

Sprite::Sprite(const SpriteDescription& desc, ResourceModifier::Enum mod) :
  impl(std::make_shared<SpriteImpl>(desc, mod))
{
}

Sprite::~Sprite()
{
}

void
Sprite::render(int x, int y, Framebuffer& fb)
{
  if (impl.get())
    impl->render(x, y, fb);
}

int
Sprite::get_width() const
{
  if (impl.get())
    return impl->frame_size.width;
  else
    return 0;
}

int
Sprite::get_height() const
{
  if (impl.get())
    return impl->frame_size.height;
  else
    return 0;
}

Sprite::operator bool() const
{
  return (impl.get() != 0);
}

void
Sprite::update(float delta)
{
  if (impl.get())
    impl->update(delta);
}

void
Sprite::set_frame(int i)
{
  if (impl.get())
    impl->frame = i;
}

int
Sprite::get_frame_count() const
{
  if (impl.get())
    return (impl->array.width * impl->array.height);
  else
    return 0;
}

bool
Sprite::is_finished() const
{
  if (impl.get())
    return impl->finished;
  else
    return true;
}

bool
Sprite::is_looping() const
{
  if (impl.get())
    return impl->loop_last_cycle;
  else
    return false;
}

void
Sprite::set_play_loop(bool loop)
{
  if (impl.get())
    impl->loop = loop;
}

int
Sprite::get_current_frame() const
{
  if (impl.get())
    return impl->frame;
  else
    return 0;
}

void
Sprite::restart()
{
  if (impl.get())
    impl->restart();
}

void
Sprite::finish()
{
  if (impl.get())
    impl->finish();
}

Vector2i
Sprite::get_offset() const
{
  if (impl.get())
    return impl->offset;
  else
    return Vector2i();
}

void
Sprite::set_hotspot(Origin origin, int x, int y)
{
  if (impl.get())
  {
    // FIXME: offset and other stuff should be member of the Sprite, not the SpriteImpl
    impl->offset = calc_origin(origin, impl->frame_size) - Vector2i(x, y);
  }
}

/* EOF */
