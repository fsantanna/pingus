FPSCounter::FPSCounter() :
{
  start_time = SDL_GetTicks();
  fps_count = 0;
}
void FPSCounter::draw() {
  update_fps_counter();
  if (odd_frame) {
    Fonts::pingus_small.render(origin_center, Display::get_width()/2, 35, fps_string, *Display::get_framebuffer());
    odd_frame = false;
  } else {
    Fonts::pingus_small.render(origin_center, Display::get_width()/2, 35, "+ " + fps_string + " +", *Display::get_framebuffer());
    odd_frame = true;
  }
}
void FPSCounter::update_fps_counter() {
  unsigned int current_time = SDL_GetTicks();
  fps_count++;
  if (start_time + 1000 < current_time) {
    current_fps = fps_count * 1000 / (current_time - start_time);
    fps_count = 0;
    start_time = SDL_GetTicks();
    str << current_fps << " fps";
    fps_string = str.str();
  }
}
