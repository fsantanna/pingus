Exiter::Exiter (Pingu* p) :
  PinguAction(p),
  sound_played(false)
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/exit/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/exit/right"));
}
void Exiter::update () {
  sprite[pingu->direction].update();
  if (!sound_played) {
    sound_played = true;
    Sound::PingusSound::play_sound("yipee");
  }
  if (sprite[pingu->direction].is_finished()) {
    if (pingu->get_status() != Pingu::PS_EXITED) {
      pingu->set_status(Pingu::PS_EXITED);
    }
  }
}
void Exiter::draw (SceneContext& gc) {
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
