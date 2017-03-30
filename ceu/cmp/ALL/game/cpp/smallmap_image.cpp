SmallMapImage::SmallMapImage(Server* s, int width, int height)
  : server(s),
    canvas(width, height),
    update_count(0),
    colmap_serial(0)
{
  update_surface();
}
void SmallMapImage::update (float delta) {
  float smallmap_update_time = 1.0f;
  update_count += delta;
  if (update_count > smallmap_update_time) {
    update_count = 0.0f;
    CollisionMap* colmap = server->get_world()->get_colmap();
    if (colmap_serial != colmap->get_serial()) {
      update_surface();
    }
  }
}
Sprite SmallMapImage::get_surface () {
  if (sur) {
    return sur;
  } else {
    update_surface();
    return sur;
  }
}
void SmallMapImage::update_surface() {
  unsigned char* cbuffer;
  CollisionMap* colmap = server->get_world()->get_colmap();
  colmap_serial = colmap->get_serial();
  canvas.lock();
  cbuffer = static_cast<unsigned char*>(canvas.get_data());
  int cmap_width  = colmap->get_width();
  int cmap_height = colmap->get_height();
  int width  = canvas.get_width();
  int height = canvas.get_height();
  int pitch  = canvas.get_pitch();
  assert(width < cmap_width && height < cmap_height);
  const int red   = 0;
  const int green = 1;
  const int blue  = 2;
  const int alpha = 3;
  for(int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int i = y * pitch + 4 * x;
      switch (colmap->getpixel_fast(x * cmap_width  / width, y * cmap_height / height)) {
        case Groundtype::GP_NOTHING:
          cbuffer[i + red]   = 0;
          cbuffer[i + green] = 0;
          cbuffer[i + blue]  = 0;
          cbuffer[i + alpha] = 255;
          break;
        case Groundtype::GP_BRIDGE:
          cbuffer[i + red]   =   0;
          cbuffer[i + green] = 255;
          cbuffer[i + blue]  = 100;
          cbuffer[i + alpha] = 255;
          break;
        case Groundtype::GP_SOLID:
          cbuffer[i + red]   = 100;
          cbuffer[i + green] = 100;
          cbuffer[i + blue]  = 125;
          cbuffer[i + alpha] = 255;
          break;
        case Groundtype::GP_WATER:
        case Groundtype::GP_LAVA:
          cbuffer[i + red]   = 0;
          cbuffer[i + green] = 0;
          cbuffer[i + blue]  = 200;
          cbuffer[i + alpha] = 255;
          break;
        default:
          cbuffer[i + red]   = 200;
          cbuffer[i + green] = 200;
          cbuffer[i + blue]  = 200;
          cbuffer[i + alpha] = 255;
          break;
      }
    }
  }
  canvas.unlock();
  sur = Sprite(canvas.clone());
}
