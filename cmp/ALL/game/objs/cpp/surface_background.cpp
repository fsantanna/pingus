SurfaceBackground::SurfaceBackground(const FileReader& reader) :
  para_x(0.5),
  para_y(0.5),
  scroll_x(0.0),
  scroll_y(0.0),
  color(0,0,0,0),
  stretch_x(false),
  stretch_y(false),
  keep_aspect(false),
  scroll_ox(0),
  scroll_oy(0)
{
  if (!reader.read_vector("position", pos)) {
    pos = Vector3f(0.f, 0.f, -150.f);
  }
  reader.read_desc("surface", desc);
  if (!reader.read_colori("colori", color)) {
    reader.read_colorf("color", color);
  }
  reader.read_float("para-x", para_x);
  reader.read_float("para-y", para_y);
  reader.read_float("scroll-x", scroll_x);
  reader.read_float("scroll-y", scroll_y);
  reader.read_bool("stretch-x", stretch_x);
  reader.read_bool("stretch-y", stretch_y);
  reader.read_bool("keep-aspect", keep_aspect);
  if (!stretch_x && !stretch_y && color.a == 0) {
    bg_sprite = Sprite(desc);
  } else {
    Surface surface = Resource::load_surface(desc);
    if (color.a != 0 && surface.is_indexed()) {
      if (surface.has_colorkey()) {
        surface = surface.convert_to_rgba();
      } else {
        surface = surface.convert_to_rgb();
      }
    }
    surface.fill(color);
    if (stretch_x && stretch_y) {
      surface = surface.scale(world->get_width(), world->get_height());
    } else if (stretch_x && !stretch_y) {
      if (keep_aspect) {
        float aspect = static_cast<float>(surface.get_height()) / static_cast<float>(surface.get_width());
        surface = surface.scale(world->get_width(), static_cast<int>(static_cast<float>(world->get_width()) * aspect));
      } else {
        surface = surface.scale(world->get_width(), surface.get_height());
      }
    } else if (!stretch_x && stretch_y) {
      if (keep_aspect) {
        float aspect = static_cast<float>(surface.get_width()) / static_cast<float>(surface.get_height());
        surface = surface.scale(static_cast<int>(static_cast<float>(world->get_height()) * aspect), world->get_height());
      } else {
        surface = surface.scale(surface.get_width(), world->get_height());
      }
    }
    bg_sprite = Sprite(surface);
  }
}
void SurfaceBackground::update() {
  bg_sprite.update();
  if (!bg_sprite || globals::static_graphics) {
    return;
  }
  if (scroll_x) {
    scroll_ox += scroll_x;
    if (scroll_ox > bg_sprite.get_width()) {
      scroll_ox -= static_cast<float>(bg_sprite.get_width());
    } else if (-scroll_ox > bg_sprite.get_width()) {
      scroll_ox += static_cast<float>(bg_sprite.get_width());
    }
  }
  if (scroll_y) {
    scroll_oy += scroll_y;
    if (scroll_oy > bg_sprite.get_height()) {
      scroll_oy -= static_cast<float>(bg_sprite.get_height());
    } else if (-scroll_oy > bg_sprite.get_height()) {
      scroll_oy += static_cast<float>(bg_sprite.get_height());
    }
  }
}
void SurfaceBackground::draw (SceneContext& gc) {
  if (!bg_sprite) {
    return;
  }
  Vector2i offset = gc.color().world_to_screen(Vector2i(0,0));
  offset.x -= gc.color().get_rect().left;
  offset.y -= gc.color().get_rect().top;
  int start_x = static_cast<int>((static_cast<float>(offset.x) * para_x) + scroll_ox);
  int start_y = static_cast<int>((static_cast<float>(offset.y) * para_y) + scroll_oy);
  if (start_x > 0) {
    start_x = (start_x % bg_sprite.get_width()) - bg_sprite.get_width();
  }
  if (start_y > 0) {
    start_y = (start_y % bg_sprite.get_height()) - bg_sprite.get_height();
  }
  for(int y = start_y; y < world->get_height(); y += bg_sprite.get_height()) {
    for(int x = start_x; x < world->get_width(); x += bg_sprite.get_width()) {
      gc.color().draw(bg_sprite, Vector2i(x - offset.x, y - offset.y), pos.z);
    }
  }
}
