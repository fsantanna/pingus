//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/components/playfield.hpp"

#include "engine/display/display.hpp"
#include "pingus/globals.hpp"
#include "pingus/direction.hpp"
#include "pingus/screens/game_session.hpp"
#include "pingus/server.hpp"
#include "pingus/world.hpp"

#include "ceu_vars.h"

Playfield::Playfield(Server* server_, GameSession* session_, const Rect& rect_) :
  RectComponent(rect_),
  server(server_),
  session(session_),
  scene_context(new SceneContext(rect_)),
  state(rect_.get_width(), rect_.get_height()),
  clipping_rectangles(),
  mouse_pos()
{
}

Playfield::~Playfield() { }
void Playfield::update(float delta) { }
void Playfield::on_secondary_button_press(int x, int y) { }
void Playfield::on_secondary_button_release (int x, int y) { }
void Playfield::on_key_pressed(const Input::KeyboardEvent& ev) { }
void Playfield::on_primary_button_press(int x, int y) { }
void Playfield::on_pointer_move (int x, int y) { }

void
Playfield::draw(DrawingContext& gc)
{
  scene_context->clear();

  state.push(*scene_context);
  ceu_sys_go(&CEU_APP, CEU_IN_WORLD_DRAW, (void*)&scene_context);
  state.pop(*scene_context);

  gc.draw(new SceneContextDrawingRequest(scene_context.get(), Vector2i(0,0), -10000));

  gc.push_modelview();
  gc.translate(rect.left, rect.top);
  gc.pop_modelview();
}

void
Playfield::scroll (int x, int y)
{
  state.set_pos(state.get_pos() + Vector2i(x, y));
}

void
Playfield::update_layout()
{
  state.set_size(rect.get_width(), rect.get_height());
  scene_context->set_rect(rect);
}

/* EOF */
