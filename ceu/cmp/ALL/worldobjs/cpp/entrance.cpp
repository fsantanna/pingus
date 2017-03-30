Entrance::Entrance(const FileReader& reader) :
  direction(MISC),
  release_rate(150),
  owner_id(0),
  smallmap_symbol("core/misc/smallmap_entrance"),
  last_direction(0)
{
  reader.read_int   ("owner-id",     owner_id);
  reader.read_vector("position",     pos);
  reader.read_int   ("release-rate", release_rate);
  if (owner_id < 0 || owner_id > 3) {
    owner_id = 0;
  }
  std::string direction_str;
  reader.read_string("direction", direction_str);
  if (direction_str == "left") {
    direction = LEFT;
  } else if (direction_str == "right") {
    direction = RIGHT;
  } else if (direction_str == "misc") {
    direction = MISC;
  else {
    log_error("unknown direction: '%1%'", direction_str);
    direction = MISC;
  }
  last_release = 150 - release_rate; // wait ~2sec at startup to allow a 'lets go' sound
}
void Entrance::create_pingu () {
  Direction d;
  Pingu* pingu = world->get_pingus()->create_pingu(pos, owner_id);
  if (pingu) {
    switch (direction) {
      case LEFT:
        d.left();
        pingu->set_direction(d);
        break;
      case MISC:
        if (last_direction) {
          d.left();
          last_direction = 0;
        } else {
          d.right();
          last_direction = 1;
        }
        pingu->set_direction(d);
        break;
      case RIGHT:
        d.right();
        pingu->set_direction(d);
        break;
      default:
        log_error("warning direction is wrong: %1%", direction);
        d.right();
        pingu->set_direction(d);
        break;
    }
}
void Entrance::update () {
  if (last_release + release_rate < (world->get_time())) {
    last_release = world->get_time();
    create_pingu();
  }
}
void Entrance::draw (SceneContext& gc) {
  if (!surface) {
    return;
  }
  gc.color().draw(surface, Vector3f(pos.x - 32, pos.y - 16));
}
void Entrance::draw_smallmap(SmallMap* smallmap) {
  smallmap->draw_sprite(smallmap_symbol, pos);
}
