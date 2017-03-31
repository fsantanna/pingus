Playfield::Playfield(Server* server_, GameSession* session_, const Rect& rect_) :
  RectComponent(rect_),
  server(server_),
  session(session_),
  current_pingu(0),
  scene_context(new SceneContext(rect_)),
  state(rect_.get_width(), rect_.get_height()),
  capture_rectangle(session),
{
  mouse_scrolling    = false;
  state.set_limit(Rect(Vector2i(0, 0), Size(server->get_world()->get_width(), server->get_world()->get_height())));
  state.set_pos(server->get_world()->get_start_pos(0));
}
void Playfield::draw(DrawingContext& gc) {
  scene_context->clear();
  state.push(*scene_context);
  capture_rectangle.set_pingu(current_pingu);
  capture_rectangle.draw(*scene_context);
  server->get_world()->draw(*scene_context);
  state.pop(*scene_context);
  gc.draw(new SceneContextDrawingRequest(scene_context.get(), Vector2i(0,0), -10000));
  gc.push_modelview();
  gc.translate(rect.left, rect.top);
  if (mouse_scrolling && !globals::drag_drop_scrolling) {
    gc.draw_line(mouse_pos, scroll_center - Vector2i(0, 15), Color(0, 255, 0));
    gc.draw_line(mouse_pos, scroll_center + Vector2i(0, 15), Color(0, 0, 255));
    gc.draw_line(mouse_pos, scroll_center + Vector2i(15, 0), Color(0, 255, 255));
    gc.draw_line(mouse_pos, scroll_center - Vector2i(15, 0), Color(255, 255, 0));
    gc.draw_line(mouse_pos, scroll_center, Color(255, 0, 0));
  }
  gc.pop_modelview();
}
Pingu* Playfield::current_pingu_find(const Vector2f& pos) {
  float min_dist = 500.0;
  float dist;
  Pingu* c_pingu = 0;
  for (PinguIter pingu = server->get_world()->get_pingus()->begin(); pingu != server->get_world()->get_pingus()->end(); ++pingu) {
    if ((*pingu)->is_over(static_cast<int>(pos.x), static_cast<int>(pos.y))) {
      dist = (*pingu)->dist(static_cast<int>(pos.x), static_cast<int>(pos.y));
      if (dist < min_dist) {
        min_dist = dist;
        c_pingu = *pingu;
      }
    }
  }
  return c_pingu;
}
void Playfield::update(float delta) {
  if (!mouse_scrolling) {
    current_pingu = current_pingu_find(state.screen2world(mouse_pos));
    capture_rectangle.set_pingu(current_pingu);
  } else {
    if (globals::drag_drop_scrolling) {
      state.set_pos(old_state_pos + (scroll_center - mouse_pos));
    } else {
      state.set_pos(Vector2i(state.get_pos().x - static_cast<int>(static_cast<float>(scroll_center.x - mouse_pos.x) * 0.2f), state.get_pos().y - static_cast<int>(static_cast<float>(scroll_center.y - mouse_pos.y) * 0.2f)));
    }
  }
  if (globals::auto_scrolling && (Display::is_fullscreen() || SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON)) {
    scroll_speed = static_cast<int>(800 * delta);
    if (mouse_pos.x < 10) {
      state.set_pos(state.get_pos() - Vector2i(scroll_speed, 0));
    } else if (mouse_pos.x > Display::get_width() - 10) {
      state.set_pos(state.get_pos() + Vector2i(scroll_speed, 0));
    }
    if (mouse_pos.y < 10) {
      state.set_pos(state.get_pos() - Vector2i(0, scroll_speed));
    } else if (mouse_pos.y > Display::get_height() - 10) {
      state.set_pos(state.get_pos() + Vector2i(0, scroll_speed));
    }
  }
}
void Playfield::on_key_pressed(const Input::KeyboardEvent& ev) {
  if (globals::developer_mode) {
    bool spawn = ev.keysym.sym == SDLK_l || ev.keysym.sym == SDLK_r;
    if (spawn) {
      Vector2i n = state.screen2world(mouse_pos);
      Vector3f p = Vector3f(n.x,n.y,0);
      Pingu* pingu = server->get_world()->get_pingus()->create_pingu(p, 0);
      if (pingu) {
        Direction d;
        ev.keysym.sym == SDLK_l ? d.left() : d.right();
        pingu->set_direction(d);
      }
    }
  }
}
void Playfield::on_primary_button_press(int x, int y) {
  x -= rect.left;
  y -= rect.top;
  if (session) {
    current_pingu = current_pingu_find(state.screen2world(Vector2i(x,y)));
    if (current_pingu) {
      server->send_pingu_action_event(current_pingu, session->get_action_name());
    }
  }
}
void Playfield::on_secondary_button_press(int x, int y) {
  x -= rect.left;
  y -= rect.top;
  mouse_scrolling = true;
  scroll_center.x = x;
  scroll_center.y = y;
  old_state_pos = state.get_pos();
}
void Playfield::on_secondary_button_release (int x, int y) {
  x -= rect.left;
  y -= rect.top;
  mouse_scrolling = false;
}
void Playfield::on_pointer_move (int x, int y) {
  x -= rect.left;
  y -= rect.top;
  mouse_pos.x = x;
  mouse_pos.y = y;
  if (globals::developer_mode) { // Some fun stuff that lets you draw directly on the level
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_DELETE]) {
      CollisionMask mask("other/bash_radius_gfx");
      Vector2i p = state.screen2world(mouse_pos);
      server->get_world()->remove(mask, p.x - mask.get_width()/2, p.y - mask.get_height()/2);
    } else if (keystate[SDLK_INSERT]) {
      CollisionMask mask("other/bash_radius_gfx");
      Vector2i p = state.screen2world(mouse_pos);
      server->get_world()->put(mask, p.x - mask.get_width()/2, p.y - mask.get_height()/2, Groundtype::GP_GROUND);
    } else if (keystate[SDLK_HOME]) {
      CollisionMask mask("other/bash_radius_gfx");
      Vector2i p = state.screen2world(mouse_pos);
      server->get_world()->put(mask, p.x - mask.get_width()/2, p.y - mask.get_height()/2, Groundtype::GP_BRIDGE);
    }
  }
}
Vector2i Playfield::get_pos() const {
  return Vector2i(static_cast<int>(state.get_pos().x), static_cast<int>(state.get_pos().y));
}
void Playfield::set_viewpoint(int x, int y) {
  state.set_pos(Vector2i(x, y));
}
void Playfield::scroll (int x, int y) {
  state.set_pos(state.get_pos() + Vector2i(x, y));
}
void Playfield::update_layout() {
  state.set_size(rect.get_width(), rect.get_height());
  scene_context->set_rect(rect);
}
