Splashed::Splashed (Pingu* p) :
  PinguAction(p),
  particle_thrown(false),
{
  sprite = Sprite("pingus/player" + pingu->get_owner_str() + "/splat");
}
void Splashed::update () {
  sprite.update();
  if (!particle_thrown) {
    particle_thrown = true;
    WorldObj::get_world()->play_sound("splash", pingu->get_pos());
  }
  if (sprite.is_finished()) {
    pingu->set_status(Pingu::PS_DEAD);
  }
}
void Splashed::draw (SceneContext& gc) {
  gc.color().draw(sprite, pingu->get_pos ());
}
