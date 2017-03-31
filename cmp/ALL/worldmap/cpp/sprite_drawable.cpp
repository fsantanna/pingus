SpriteDrawable::SpriteDrawable(const FileReader& reader) :
  Drawable(reader),
{
  auto_uncover = false;
  reader.read_desc  ("surface", desc);
  reader.read_vector("position", pos);
  reader.read_bool  ("auto-uncover", auto_uncover);
  surface = Sprite(desc);
}
void SpriteDrawable::draw(DrawingContext& gc) {
  if (surface) {
    if (auto_uncover) {
      Vector3f pingus_pos = Worldmap::current()->get_pingus()->get_pos();
      if (!(pingus_pos.x > pos.x && pingus_pos.x < pos.x + static_cast<float>(surface.get_width()) && pingus_pos.y > pos.y && pingus_pos.y < pos.y + static_cast<float>(surface.get_height()))) {
        gc.draw(surface, pos);
      }
      else if (pingus_pos.z > pos.z + 1000)
      { // FIXME: Hack for the 0.6.0 release/tutorial world remove later
        gc.draw(surface, pos);
      }
    } else {
      gc.draw(surface, pos);
    }
  }
}
