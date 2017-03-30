Groundpiece::Groundpiece(const FileReader& reader) :
{
  reader.read_vector("position", pos);
  reader.read_desc  ("surface",  desc);
  gptype = Groundtype::GP_GROUND;
  reader.read_enum("type", gptype, &Groundtype::string_to_type);
}
void Groundpiece::on_startup () {
  CollisionMask mask(desc);
  if (gptype == Groundtype::GP_REMOVE) {
    get_world()->remove(mask, static_cast<int>(pos.x), static_cast<int>(pos.y));
  } else {
    get_world()->put(mask, static_cast<int>(pos.x), static_cast<int>(pos.y), gptype);
  }
}
