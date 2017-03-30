Hotspot::Hotspot(const FileReader& reader) :
{
  ResDescriptor desc;
  reader.read_vector("position", pos);
  reader.read_desc  ("surface",  desc);
  reader.read_float ("parallax", para);
  sprite = Sprite(desc);
}
void Hotspot::update() {
  sprite.update();
}
void Hotspot::draw (SceneContext& gc) {
  gc.color().draw (sprite, pos);
}
