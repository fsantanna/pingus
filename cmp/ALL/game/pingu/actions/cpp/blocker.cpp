Blocker::Blocker(Pingu* p) :
  PinguAction(p),
{
  sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() + "/blocker/left");
  sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/blocker/right");
}
void Blocker::update() {
  if (!standing_on_ground()) {
    pingu->set_action(ActionName::FALLER);
    return;
  } else {
    PinguHolder* pingus = WorldObj::get_world()->get_pingus();
    for(PinguIter i = pingus->begin(); i != pingus->end(); ++i) {
      catch_pingu(*i);
    }
  }
  sprite.update();
}
void Blocker::draw (SceneContext& gc) {
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
bool Blocker::standing_on_ground() {
  return (rel_getpixel(0,-1) !=  Groundtype::GP_NOTHING);
}
void Blocker::catch_pingu(Pingu* target) {
  if (target != pingu) {
    if (target->get_x () > pingu->get_x () - 16 && target->get_x () < pingu->get_x () + 16 && target->get_y () > pingu->get_y () - 32 && target->get_y () < pingu->get_y () + 5) {
      if (target->get_x () > pingu->get_x ()) {
        target->direction.right();
      } else {
        target->direction.left();
      }
    }
  }
}
