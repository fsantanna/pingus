Miner::Miner (Pingu* p) :
  PinguAction(p),
  miner_radius("pingus/common/miner_radius_gfx", "pingus/common/miner_radius"),
  miner_radius_left("pingus/common/miner_radius_left_gfx", "pingus/common/miner_radius_left"),
  miner_radius_right("pingus/common/miner_radius_right_gfx", "pingus/common/miner_radius_right"),
  delay_count(0)
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/miner/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/miner/right"));
}
void Miner::update () {
  sprite[pingu->direction].update();
  delay_count += 1;
  if (delay_count % 4 == 0) {
    if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING) {
      mine(true);
      pingu->set_action(ActionName::WALKER);
    } else if (rel_getpixel(0, -1) == Groundtype::GP_SOLID || rel_getpixel(0, pingu_height) == Groundtype::GP_SOLID) {
      if (rel_getpixel(0, -1) == Groundtype::GP_SOLID) {
        Sound::PingusSound::play_sound("chink");
      }
      mine(true);
      pingu->direction.change();
      pingu->set_action(ActionName::WALKER);
    } else {
      mine(false);
      pingu->set_pos(pingu->get_xi() + pingu->direction, pingu->get_yi() + 1);
    }
  }
}
void Miner::mine(bool final) {
  if (!final) {
    if (delay_count % 2 == 0) {
      WorldObj::get_world()->remove(miner_radius, pingu->get_xi() - (miner_radius.get_width() / 2) + pingu->direction, pingu->get_yi() - miner_radius.get_height() + 2);
    }
  } else {
    CollisionMask& radius = (pingu->direction == Direction::LEFT) ? miner_radius_left : miner_radius_right;
    WorldObj::get_world()->remove(radius, pingu->get_xi() - (radius.get_width() / 2) + pingu->direction, pingu->get_yi() - radius.get_height() + 2);
  }
}
void Miner::draw (SceneContext& gc) {
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
