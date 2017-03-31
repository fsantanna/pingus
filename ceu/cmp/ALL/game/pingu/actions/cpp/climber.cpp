Climber::Climber (Pingu* p) :
  PinguAction(p),
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/climber/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/climber/right"));
}
void Climber::update () {
  sprite[pingu->direction].update();
  if (   rel_getpixel(0, 1) == Groundtype::GP_NOTHING || rel_getpixel(0, 1) == Groundtype::GP_BRIDGE) {
    if (rel_getpixel(1, 1) != Groundtype::GP_NOTHING) {
      pingu->set_pos(pingu->get_x(), pingu->get_y() - 1);
      return;
    } else if (rel_getpixel(1, 1) ==  Groundtype::GP_NOTHING) {
      if (!head_collision_on_walk(pingu->direction, 1)) {
        pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction), pingu->get_y() - 1.0f);
      } else {
        pingu->direction.change();
      }
      pingu->set_action(ActionName::WALKER);
    }
  } else {
    pingu->direction.change();
    pingu->set_action(ActionName::WALKER);
  }
}
void Climber::draw (SceneContext& gc) {
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
