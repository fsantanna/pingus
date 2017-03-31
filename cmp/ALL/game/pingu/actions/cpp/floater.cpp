Floater::Floater(Pingu* p) :
  PinguAction(p),
{
  sprite = Sprite("pingus/player" + pingu->get_owner_str() + "/floater/left");
}
void Floater::update() {
  sprite.update ();
  pingu->set_velocity(Vector3f(0.0f, 1.0f));
  if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING) {
      pingu->set_y(pingu->get_y() + 1);
  } else {
    pingu->set_action (ActionName::WALKER);
  }
}
void Floater::draw (SceneContext& gc) {
  gc.color().draw(sprite, pingu->get_pos());
}
