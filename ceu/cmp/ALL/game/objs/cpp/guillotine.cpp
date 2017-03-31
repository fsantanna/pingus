Guillotine::Guillotine(const FileReader& reader) :
  sprite_kill_right("traps/guillotinekill/right"),
  sprite_kill_left("traps/guillotinekill/left"),
  sprite_idle("traps/guillotineidle"),
  killing(false)
{
  reader.read_vector("position", pos);
  sprite_kill_right.set_play_loop(false);
  sprite_kill_left.set_play_loop(false);
  sprite_idle.set_play_loop(true);
}
void Guillotine::draw (SceneContext& gc) {
  if (killing) {
    if (direction.is_left()) {
      gc.color().draw (sprite_kill_left, pos);
    } else {
      gc.color().draw (sprite_kill_right, pos);
    }
  } else {
    gc.color().draw (sprite_idle, pos);
  }
}
void Guillotine::update () {
  if (sprite_kill_left.is_finished()) {
    killing = false;
  }
  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu) {
    catch_pingu(*pingu);
  }
  if (killing) {
    sprite_kill_left.update();
    sprite_kill_right.update();
    if (sprite_kill_left.get_current_frame() == 7) {
      WorldObj::get_world()->play_sound("splash", pos);
    }
  } else {
    sprite_idle.update();
  }
}
void Guillotine::catch_pingu (Pingu* pingu) {
  if (!killing) {
    if (pingu->is_inside (static_cast<int>(pos.x + 38), static_cast<int>(pos.y + 90), static_cast<int>(pos.x + 42), static_cast<int>(pos.y + 98))) {
      killing = true;
      pingu->set_status(Pingu::PS_DEAD);
      direction = pingu->direction;
      sprite_kill_left.restart();
      sprite_kill_right.restart();
    }
  }
}
