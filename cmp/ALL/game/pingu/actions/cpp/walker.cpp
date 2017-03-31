Walker::Walker (Pingu* p) :
  PinguAction(p),
{
  walker.load(Direction::LEFT, "pingus/player" + pingu->get_owner_str() + "/walker/left");
  walker.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/walker/right");
  floaterlayer.load(Direction::LEFT,  "other/floaterlayer/left");
  floaterlayer.load(Direction::RIGHT, "other/floaterlayer/right");
  pingu->set_velocity(Vector3f());
}
void Walker::update () {
  walker[pingu->direction].update(0.033f);
  floaterlayer[pingu->direction].update(0.033f);
  Vector3f last_pos = pingu->get_pos();
  if (rel_getpixel(1, 0) == Groundtype::GP_OUTOFSCREEN) {
    pingu->set_x(pingu->get_x() + static_cast<float>(pingu->direction));
    return;
  }
  if (rel_getpixel(0, -1) ==  Groundtype::GP_WATER) {
    pingu->set_action(ActionName::DROWN);
    return;
  }
  if (rel_getpixel(0, -1) ==  Groundtype::GP_NOTHING) {
    bool found_ground = false;
    int i;
    for (i = -2; i > -5; --i) {
      if (!(rel_getpixel(0, i) == Groundtype::GP_NOTHING)) {
        found_ground = true;
        break;
      }
    }
    if (found_ground) {
      pingu->set_y(pingu->get_y() - static_cast<float>(i));
    } else {
      pingu->set_action(ActionName::FALLER);
      return;
    }
  }
  if (rel_getpixel(1, 0) == Groundtype::GP_BRIDGE && !head_collision_on_walk(1, 1)) {
    pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction), pingu->get_y() - 1.0f); // pingus 'float' through bridges
  } else {
    int y_inc = 0;
    int possible_y_step = 0;
    bool found_next_step = false;
    for (y_inc = -max_steps; y_inc <= max_steps; ++y_inc) {// up/down-hill scan
      if ((  rel_getpixel(1, y_inc)     == Groundtype::GP_NOTHING || rel_getpixel(1, y_inc)     == Groundtype::GP_BRIDGE) && rel_getpixel(1, y_inc - 1) != Groundtype::GP_NOTHING) { // FIXME:
        found_next_step = true;
        possible_y_step = y_inc;
      }
    }
    if (found_next_step) {
      pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction), pingu->get_y() - static_cast<float>(possible_y_step));
    } else {
      if (rel_getpixel(1, 0) !=  Groundtype::GP_NOTHING) {
        if (pingu->request_wall_action()) {
          log_debug("Pingu: We are in front of a wall, setting persistant action");
          return;
        }
        pingu->direction.change();
      } else {
        pingu->set_x(pingu->get_x() + static_cast<float>(pingu->direction));
        pingu->set_action(ActionName::FALLER);
        return;
      }
    }
  }
  if (head_collision_on_walk(0, 0)) {
    log_debug("Pingu: Head collision");
    pingu->direction.change();
    pingu->set_pos(last_pos);
    return;
  }
}
void Walker::draw (SceneContext& gc) {
  gc.color().draw(walker[pingu->direction], pingu->get_pos());
  if (pingu->get_fall_action() && pingu->get_fall_action()->get_type() == ActionName::FLOATER) {
    gc.color().draw(floaterlayer[pingu->direction], pingu->get_pos());
  }
}
