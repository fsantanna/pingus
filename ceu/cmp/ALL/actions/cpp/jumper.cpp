Jumper::Jumper (Pingu* p) :
  PinguAction(p),
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/jumper/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/jumper/right"));
}
void Jumper::draw (SceneContext& gc) {
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
void Jumper::update () {
  if ((pingu->get_previous_action() == ActionName::CLIMBER)) {
    pingu->direction.change();
  }
  if (pingu->direction.is_left()) {
    pingu->set_velocity(pingu->get_velocity() + Vector3f(-4.58f, -4.58f));
  } else {
    pingu->set_velocity(pingu->get_velocity() + Vector3f(4.58f, -4.58f));
  }
  pingu->set_y(pingu->get_y() - 1);
  pingu->set_action (ActionName::FALLER);
}
