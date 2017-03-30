PingusCounter::PingusCounter(Server* s) :
  server(s),
  background("core/buttons/info")
{
  font = Fonts::pingus_small_fixnum;
}
void PingusCounter::draw(DrawingContext& gc) {
  char str[128];
  gc.draw(background, Vector2i(gc.get_width()/2, 0));
  World* world = server->get_world();
  snprintf(str, 128, _("Released:%3d/%d   Out:%3d   Saved:%3d/%d").c_str(), world->get_pingus()->get_number_of_released(), world->get_pingus()->get_number_of_allowed(), world->get_pingus()->get_number_of_alive(), world->get_pingus()->get_number_of_exited(), server->get_plf().get_number_to_save());
  gc.print_center(font, Vector2i(gc.get_width()/2, -2), str);
}
