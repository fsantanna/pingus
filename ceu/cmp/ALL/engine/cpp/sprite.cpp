Sprite::Sprite(const std::string& name) :
{
  SpriteDescription* desc = Resource::load_sprite_desc(name);
  if (desc) {
    impl = std::make_shared<SpriteImpl>(*desc);
  } else {
    SpriteDescription desc_;
    desc_.filename = Pathname("images/core/misc/404.png", Pathname::DATA_PATH);
    impl = std::make_shared<SpriteImpl>(desc_);
  }
}
Sprite::Sprite(const ResDescriptor& res_desc) :
{
  SpriteDescription* desc = Resource::load_sprite_desc(res_desc.res_name);
  if (desc) {
    impl = std::make_shared<SpriteImpl>(*desc, res_desc.modifier);
  } else {
    SpriteDescription desc_;
    desc_.filename = Pathname("images/core/misc/404.png", Pathname::DATA_PATH);
    impl = std::make_shared<SpriteImpl>(desc_);
  }
}
