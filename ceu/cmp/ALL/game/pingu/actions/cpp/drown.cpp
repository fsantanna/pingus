Drown::Drown (Pingu* p) :
  PinguAction(p),
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/drownfall/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/drownfall/right"));
}
void Drown::draw (SceneContext& gc) {
  gc.color().draw(sprite[pingu->direction], pingu->get_pos ());
}
void Drown::update () {
  sprite[pingu->direction].update();
  if (sprite[pingu->direction].is_finished()) {
    pingu->set_status(Pingu::PS_DEAD);
  }
}
