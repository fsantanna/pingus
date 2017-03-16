ArmageddonButton::ArmageddonButton(Server* s, int x, int y) :
  RectComponent(Rect(Vector2i(x, y), Size(38, 60))),
  server(s),
  background  ("core/buttons/hbuttonbgb"),
  backgroundhl("core/buttons/hbuttonbg")
{
  pressed = false;
  sprite  = Sprite("core/buttons/armageddon_anim");
}
void ArmageddonButton::draw (DrawingContext& gc) {
  Vector2i pos(rect.left, rect.top);
  if (server->get_world()->check_armageddon ()) {
    gc.draw(backgroundhl, pos);
    gc.draw(sprite, pos);
  } else {
    gc.draw(background, pos);
    sprite.set_frame(7);
    gc.draw(sprite, pos);
  }
}
void ArmageddonButton::update (float delta) {
  sprite.update(delta);
  if (pressed) {
    press_time += delta;
    if (press_time > 1.0f) {
      press_time = 0;
      pressed = false;
    }
  }
  else {
    pressed = false;
    press_time = 0;
  }
}
void ArmageddonButton::on_primary_button_click (int x, int y) {
  if (pressed) {
    server->send_armageddon_event();
  } else {
    pressed = true;
  }
}
