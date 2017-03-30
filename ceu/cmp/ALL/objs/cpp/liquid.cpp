Liquid::Liquid(const FileReader& reader) :
  old_width_handling(false),
  width(0),
{
  ResDescriptor desc;
  reader.read_vector("position", pos);
  reader.read_desc  ("surface",  desc);
  reader.read_int   ("repeat",   width);
  sur = Sprite(desc);
  if (!old_width_handling) {
    width = width * sur.get_width();
  }
}
void Liquid::on_startup () {
  CollisionMask mask("liquids/water_cmap");
  for(int i=0; i < width; ++i) {
    world->get_colmap()->put(mask, static_cast<int>(pos.x + static_cast<float>(i)), static_cast<int>(pos.y), Groundtype::GP_WATER);
  }
}
void Liquid::draw (SceneContext& gc) {
  for(int x = static_cast<int>(pos.x); x < static_cast<int>(pos.x) + width; x += sur.get_width()) {
    gc.color().draw(sur, Vector3f(static_cast<float>(x), pos.y));
  }
}
void Liquid::update() {
  sur.update(0.033f);
}
