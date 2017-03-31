Slider::Slider (Pingu* p) :
  PinguAction(p),
  speed(10)
{
  sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() + "/slider/left");
  sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/slider/right");
}
void Slider::update () {
  sprite[pingu->direction].update();
  for (int i = 0; i < speed && rel_getpixel(1, 0) == Groundtype::GP_NOTHING; ++i) {
    pingu->set_x(pingu->get_x() + static_cast<float>(pingu->direction));
    if (rel_getpixel(0, -1) ==  Groundtype::GP_NOTHING) {
      speed = (speed > 5) ? 5 : speed;
      if (pingu->direction.is_right()) {
        pingu->set_velocity(pingu->get_velocity() + Vector3f(speed, 0.0));
      } else {
        pingu->set_velocity(pingu->get_velocity() + Vector3f(-speed, 0.0));
      }
      pingu->set_action(ActionName::FALLER);
      return;
    }
  }
  speed -= 7 * 0.025f;
  if (speed < 1) {
    pingu->set_action(ActionName::WALKER);
    return;
  }
}
void Slider::draw (SceneContext& gc) {
  gc.color().draw(sprite[pingu->direction], pingu->get_pos() + Vector3f(0, -2));
}
