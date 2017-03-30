Pingu::Pingu (int arg_id, const Vector3f& arg_pos, int owner) :
  previous_action(ActionName::FALLER),
  id(arg_id),
  action_time(-1),
  owner_id(owner),
  status(PS_ALIVE),
  pos_x(arg_pos.x),
  pos_y(arg_pos.y),
  velocity(0, 0, 0),
{
  direction.left ();
  action = create_action(ActionName::FALLER);
}
void Pingu::set_velocity (const Vector3f& velocity_) {
  velocity = velocity_;
  velocity.x = Math::clamp(-terminal_velocity, velocity.x, terminal_velocity);
  velocity.y = Math::clamp(-terminal_velocity, velocity.y, terminal_velocity);
}
bool Pingu::request_set_action(ActionName::Enum action_name) {
  bool ret_val = false;
  if (status == PS_DEAD) {
    ret_val =  false;
  } else {
    switch (PinguAction::get_activation_mode(action_name)) {
      case INSTANT:
        if (action_name == action->get_type()) {
          ret_val = false;
        } else if (action->change_allowed(action_name)) {
          set_action(action_name);
          ret_val = true;
        } else {
          ret_val = false;
        }
        break;
      case WALL_TRIGGERED:
        if (wall_action && wall_action->get_type() == action_name) {
          ret_val = false;
        } else {
          wall_action = create_action(action_name);
          ret_val = true;
        }
        break;
      case FALL_TRIGGERED:
        if (fall_action && fall_action->get_type() == action_name) {
          ret_val = false;
        } else {
          fall_action = create_action(action_name);
          ret_val = true;
        }
        break;
      case COUNTDOWN_TRIGGERED:
          if (countdown_action && countdown_action->get_type() == action_name) {
            ret_val = false;
            break;
          }
          std::shared_ptr<PinguAction> act = create_action(action_name);
          action_time = act->activation_time();
          countdown_action = act;
          ret_val = true;
        }
        break;
      default:
        ret_val = false;
        break;
    }
  }
  return ret_val;
}
void Pingu::set_action(std::shared_ptr<PinguAction> act) {
  previous_action = action->get_type();
  action = act;
}
bool Pingu::request_fall_action () {
  if (fall_action) {
    set_action(fall_action);
    return true;
  }
  return false;
}
bool Pingu::request_wall_action () {
  if (wall_action) {
    set_action(wall_action);
    return true;
  }
  return false;
}
bool Pingu::is_over (int x, int y) {
  Vector3f center = get_center_pos ();
  return (center.x + 16 > x && center.x - 16 < x && center.y + 16 > y && center.y - 16 < y);
}
bool Pingu::is_inside (int x1, int y1, int x2, int y2) {
  return (pos_x > x1 && pos_x < x2 && pos_y > y1 && pos_y < y2);
}
float Pingu::dist(int x, int y) {
  Vector3f p = get_center_pos ();
  return Math::sqrt(((p.x - static_cast<float>(x)) * (p.x - static_cast<float>(x)) + (p.y - static_cast<float>(y)) * (p.y - static_cast<float>(y))));
}
void Pingu::update() {
  if (status == PS_DEAD) {
    return;
  }
  if (rel_getpixel(0, -1) == Groundtype::GP_OUTOFSCREEN) {
    status = PS_DEAD;
    return;
  }
  if (action_time > -1) {
    --action_time;
  }
  if (action_time == 0 && countdown_action) {
    set_action(countdown_action);
    countdown_action = std::shared_ptr<PinguAction>();
    action_time = -1;
    return;
  }
  action->update();
}
void Pingu::draw(SceneContext& gc) {
  char str[16];
  action->draw(gc);
  if (action_time != -1) {
    snprintf(str, 16, "%d", action_time/3);
    gc.color().print_center(Fonts::chalk_normal, Vector2i(static_cast<int>(pos_x), static_cast<int>(pos_y) - 48), str);
  }
}
int Pingu::rel_getpixel(int x, int y) {
  return WorldObj::get_world()->get_colmap()->getpixel(static_cast<int>(pos_x + static_cast<float>(x * direction)), static_cast<int>(pos_y - static_cast<float>(y)));
}
bool Pingu::is_alive (void) {
  return (status != PS_DEAD && status != PS_EXITED);
}
std::shared_ptr<PinguAction> Pingu::create_action(ActionName::Enum action_) {
  switch(action_) {
    case ActionName::ANGEL:
        return std::make_shared<Angel>(this);
    case ActionName::BASHER:
        return std::make_shared<Basher>(this);
    case ActionName::BLOCKER:
        return std::make_shared<Blocker>(this);
    case ActionName::BOARDER:
        return std::make_shared<Boarder>(this);
    case ActionName::BOMBER:
        return std::make_shared<Bomber>(this);
    case ActionName::BRIDGER:
        return std::make_shared<Bridger>(this);
    case ActionName::CLIMBER:
        return std::make_shared<Climber>(this);
    case ActionName::DIGGER:
        return std::make_shared<Digger>(this);
    case ActionName::DROWN:
        return std::make_shared<Drown>(this);
    case ActionName::EXITER:
        return std::make_shared<Exiter>(this);
    case ActionName::FALLER:
        return std::make_shared<Faller>(this);
    case ActionName::FLOATER:
        return std::make_shared<Floater>(this);
    case ActionName::JUMPER:
        return std::make_shared<Jumper>(this);
    case ActionName::LASERKILL:
        return std::make_shared<LaserKill>(this);
    case ActionName::MINER:
        return std::make_shared<Miner>(this);
    case ActionName::SLIDER:
        return std::make_shared<Slider>(this);
    case ActionName::SMASHED:
        return std::make_shared<Smashed>(this);
    case ActionName::SPLASHED:
        return std::make_shared<Splashed>(this);
    case ActionName::SUPERMAN:
        return std::make_shared<Superman>(this);
    case ActionName::WAITER:
        return std::make_shared<Waiter>(this);
    case ActionName::WALKER:
        return std::make_shared<Walker>(this);
  }
}
bool PinguAction::head_collision_on_walk (int x, int y) {
  int pixel = rel_getpixel(x, y + pingu_height);
  if (pixel != Groundtype::GP_NOTHING && !(pixel & Groundtype::GP_BRIDGE)) {
    return true;
  }
  return false;
}
bool PinguAction::collision_on_walk (int x, int y) {
  bool collision = false;
  int pixel = Groundtype::GP_NOTHING;
  for (int pingu_y = 0; pingu_y <= pingu_height; ++pingu_y) {
    pixel = rel_getpixel(x, y + pingu_y);
    if (pixel != Groundtype::GP_NOTHING && pixel != Groundtype::GP_BRIDGE) {
      collision = true;
      break;
    }
  }
  return collision;
}
