Waiter::Waiter (Pingu* p) :
  PinguAction(p),
  countdown(2.0f),
{
  sprite = Sprite("pingus/player" + pingu->get_owner_str() + "/waiter/left");
}
void Waiter::update () {
  sprite.update();
  if (countdown < 0) {
    pingu->set_action(ActionName::WALKER);
    return;
  }
  countdown -= 0.025f;
}
void Waiter::draw (SceneContext& gc) {
  gc.color().draw(sprite, pingu->get_pos ());
}
