Faller::Faller (Pingu* p) :
  PinguAction(p),
{
  faller.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/faller/left"));
  faller.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/faller/right"));
  tumbler.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/tumbler/left"));
  tumbler.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/tumbler/right"));
}
void Faller::update () {
  if (is_tumbling()) {
    tumbler[pingu->direction].update();
  } else {
    faller[pingu->direction].update();
  }
  if (pingu->get_velocity().y > 5.0 && pingu->request_fall_action()) {
    return;
  }
  pingu->set_velocity(pingu->get_velocity() + Vector3f(0.0f, WorldObj::get_world()->get_gravity()) );
  Vector3f velocity = pingu->get_velocity();
  Vector3f move = velocity;
  bool collided;
  Movers::LinearMover mover(WorldObj::get_world(), pingu->get_pos());
  mover.update(move, Colliders::PinguCollider(pingu_height));
  pingu->set_pos(mover.get_pos());
  collided = mover.collided();
  if (collided) {
    if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING && rel_getpixel(0, -2) == Groundtype::GP_NOTHING && rel_getpixel(0, -3) == Groundtype::GP_NOTHING && rel_getpixel(1, 0)  != Groundtype::GP_NOTHING && rel_getpixel(1, 0)  != Groundtype::GP_BRIDGE) {
      velocity.x = -(velocity.x / 3.0f);
      if (velocity.x > 0.0f) {
        pingu->direction.right();
      } else {
        pingu->direction.left();
      }
      pingu->set_velocity(velocity);
    } else if (velocity.y > 0.0f) {
        if (pingu->get_previous_action() == ActionName::BLOCKER) {
          pingu->set_action(pingu->get_previous_action());
        } else {
          pingu->set_action(ActionName::WALKER);
        }
    } else {
      velocity.y = 0;
      velocity.x = (velocity.x / 3.0f);
      pingu->set_velocity(velocity);
    }
  }
}
void Faller::draw (SceneContext& gc) {
  if (is_tumbling()) {
    gc.color().draw(tumbler[pingu->direction], pingu->get_pos ());
  } else {
    gc.color().draw(faller[pingu->direction], pingu->get_pos ());
  }
}
bool Faller::is_tumbling () const {
  return (Math::abs(pingu->get_velocity().x) > deadly_velocity || Math::abs(pingu->get_velocity().y) > deadly_velocity);
}
bool Faller::change_allowed (ActionName::Enum new_action) {
  return new_action == ActionName::FLOATER || new_action == ActionName::CLIMBER || new_action == ActionName::BOMBER;
}
