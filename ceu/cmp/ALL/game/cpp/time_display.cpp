TimeDisplay::TimeDisplay (GameSession* c) :
  server(c->get_server()),
{
  font = Fonts::pingus_small_fixnum;
}
void TimeDisplay::draw (DrawingContext& gc) {
  int level_time = server->get_plf().get_time();
  if (level_time >= 0 || globals::developer_mode) {
    int time_value = server->get_world()->get_time();
    if (!globals::developer_mode) {
      time_value = std::max(0, level_time - time_value);
    }
    std::string time_string = GameTime::ticks_to_realtime_string(time_value);
    gc.print_right(font, Vector2i(Display::get_width() - 30, 3), time_string, 150);
  }
}
