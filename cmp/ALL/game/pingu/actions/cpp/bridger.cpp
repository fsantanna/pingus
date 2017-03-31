Bridger::Bridger (Pingu* p) :
  PinguAction(p),
  mode(B_BUILDING),
  brick_l("other/brick_left"),
  brick_r("other/brick_right"),
  bricks(MAX_BRICKS),
  block_build(false),
  name(_("Bridger") + std::string(" (" + StringUtil::to_string(bricks) + ")"))
{
  walk_sprite.load (Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/bridger_walk/left"));
  walk_sprite.load (Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/bridger_walk/right"));
  build_sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/bridger/left"));
  build_sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/bridger/right"));
}
void Bridger::draw(SceneContext& gc) {
  switch (mode) {
    case B_BUILDING:
      gc.color().draw(build_sprite[pingu->direction], Vector3f(pingu->get_pos().x - static_cast<float>(x_offset * pingu->direction), pingu->get_pos().y + static_cast<float>(y_offset)));
      break;
    case B_WALKING:
      gc.color().draw(walk_sprite[pingu->direction], Vector3f(pingu->get_pos().x - static_cast<float>(x_offset * pingu->direction), pingu->get_pos().y + static_cast<float>(y_offset)));
      break;
  }
}
void Bridger::update() {
  switch (mode) {
    case B_BUILDING:
      update_build ();
      break;
    case B_WALKING:
      update_walk ();
      break;
  }
}
void Bridger::update_walk () {
  if (walk_sprite[pingu->direction].is_finished ()) {
    if (way_is_free()) {
      mode = B_BUILDING;
      block_build = false;
      walk_sprite[pingu->direction].restart();
      walk_one_step_up();
    } else {
      pingu->direction.change ();
      pingu->set_action (ActionName::WALKER);
      return;
    }
  } else {
    walk_sprite.update ();
  }
}
void Bridger::update_build () {
  build_sprite[pingu->direction].update();
  if (build_sprite[pingu->direction].get_current_frame () >= 7 && !block_build) {
    block_build = true;
    if (bricks > 0) {
      if (brick_placement_allowed()) {
        place_a_brick();
      } else {
        pingu->direction.change ();
        pingu->set_action (ActionName::WALKER);
        return;
      }
    } else {
      pingu->set_action(ActionName::WAITER);
      return;
    }
  }
  if (build_sprite[pingu->direction].is_finished ()) {
    mode = B_WALKING;
    build_sprite[pingu->direction].restart();
  }
}
bool Bridger::way_is_free() {
  bool way_free = true;
  for (int x_inc = 1; x_inc <= 4; x_inc++) {
    if (rel_getpixel(x_inc, 2) != Groundtype::GP_NOTHING || head_collision_on_walk(x_inc, 2)) {
      way_free = false;
      break;
    }
  }
  return way_free;
}
bool Bridger::brick_placement_allowed(void) {
  return !head_collision_on_walk(0, 2);
}
void Bridger::place_a_brick() {
  bricks--;
  name = _("Bridger") + std::string(" (") + StringUtil::to_string(bricks) + ")";
  if (bricks < 4) {
    Sound::PingusSound::play_sound("ting");
  }
  if (pingu->direction.is_right()) {
    WorldObj::get_world()->put(brick_r, static_cast<int>(pingu->get_pos().x + 10.0f - static_cast<float>(brick_r.get_width())), static_cast<int>(pingu->get_pos().y), Groundtype::GP_BRIDGE);
  } else {
    WorldObj::get_world()->put(brick_l, static_cast<int>(pingu->get_pos().x - 10.0f), static_cast<int>(pingu->get_pos().y), Groundtype::GP_BRIDGE);
  }
}
void Bridger::walk_one_step_up() {
  pingu->set_pos(pingu->get_pos().x + (4.0f * static_cast<float>(pingu->direction)), pingu->get_pos().y - 2);
}
