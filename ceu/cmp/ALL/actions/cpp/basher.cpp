Basher::Basher (Pingu* p) :
  PinguAction(p),
  bash_radius("pingus/common/bash_radius_gfx", "pingus/common/bash_radius"),
  basher_c(0),
  first_bash(true),
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/basher/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/basher/right"));
  bash_reach = bash_radius.get_width();
  bash();
}
void Basher::draw (SceneContext& gc) {
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
void Basher::update () {
  sprite[pingu->direction].update();
  ++basher_c;
  if (basher_c % 3 == 0) {
    if (!walk_forward()) {
      return;
    }
    if (rel_getpixel(0, -1) == Groundtype::GP_WATER || rel_getpixel(0, -1) == Groundtype::GP_LAVA) {
      pingu->set_action(ActionName::DROWN);
    } else if (rel_getpixel(0, -1) != Groundtype::GP_NOTHING) {
      if (rel_getpixel(0, 0) == Groundtype::GP_SOLID || rel_getpixel(0, pingu_height) == Groundtype::GP_SOLID) {
        Sound::PingusSound::play_sound("chink");
        pingu->direction.change();
        pingu->set_action(ActionName::WALKER);
      } else if (have_something_to_dig()) {
        if (basher_c % 2 == 0) {
          bash();
        }
      } else if (static_cast<float>(sprite[pingu->direction].get_current_frame()) / static_cast<float>(sprite[pingu->direction].get_frame_count()) > 0.6f) {
        pingu->set_action(ActionName::WALKER);
      }
    }
  }
}
void Basher::bash() {
  WorldObj::get_world()->remove(bash_radius, pingu->get_xi() - bash_radius.get_width() / 2, pingu->get_yi() - bash_radius.get_height() + 1);
}
bool Basher::walk_forward() {
  int y_inc = 0;
  for (y_inc = 0; y_inc >= -max_steps_down; --y_inc) {
    if (rel_getpixel(0, y_inc - 1) != Groundtype::GP_NOTHING) {
      break;
    }
  }
  if (y_inc < -max_steps_down) {
    pingu->set_action(ActionName::FALLER);
    return false;
  } else {
    pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction), pingu->get_y() - static_cast<float>(y_inc));
  }
  return true;
}
bool Basher::have_something_to_dig() {
  if (first_bash) {
    first_bash = false;
    return true;
  } else {
    for(int x = 0; x <= bash_reach; ++x) {
      for (int y = min_bash_height; y <= max_bash_height; ++y) {
        if (rel_getpixel(x, y) == Groundtype::GP_GROUND) {
          log_debug("Basher: Found something to dig...");
          return true;
        }
      }
    }
    return false;
  }
}
