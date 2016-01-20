//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldmap/worldmap.hpp"

#include <iostream>

#include "engine/sound/sound.hpp"
#include "pingus/gettext.h"
#include "pingus/globals.hpp"
#include "pingus/stat_manager.hpp"
#include "pingus/worldmap/drawable_factory.hpp"
#include "pingus/worldmap/level_dot.hpp"
//#include "pingus/worldmap/pingus.hpp"
#include "util/log.hpp"
#include "util/sexpr_file_writer.hpp"
#include "util/pathname.hpp"

namespace WorldmapNS {

Worldmap* Worldmap::current_ = 0;

Worldmap::Worldmap(const Pathname& filename) :
  worldmap(),
  default_node(),
  final_node(),
  //pingus(),
  gc_state(),
  path_graph(),
  drawables(),
  objects(),
  mouse_x(0),
  mouse_y(0)
{
  current_ = this;

  worldmap = PingusWorldmap(filename);

  FileReader path_graph_reader = worldmap.get_graph();
  path_graph.reset(new PathGraph(this, path_graph_reader));

  default_node = path_graph->lookup_node(worldmap.get_default_node());
  final_node   = path_graph->lookup_node(worldmap.get_final_node());

  //pingus = new Pingus(path_graph.get());
  set_starting_node();
  //add_drawable(pingus);

  gc_state.set_limit(Rect(Vector2i(0, 0), Size(worldmap.get_width(), worldmap.get_height())));
}

Worldmap::~Worldmap() { }
void Worldmap::draw(DrawingContext& gc) { }
void Worldmap::update(float delta) { }
void Worldmap::add_drawable(Drawable* drawable) { }
void Worldmap::on_pointer_move(int x, int y) { }
void Worldmap::on_primary_button_press(int x, int y) { }
void Worldmap::on_secondary_button_press(int x, int y) { }

void Worldmap::on_startup() {
  update_locked_nodes();
}

void Worldmap::enter_level() {
#if 0
  NodeId node = get_pingus()->get_node();

  if (node != NoNode)
  {
    Dot* dot = path_graph->get_dot(node);
    if (dot)
    {
      dot->on_click();
    }
  }
  else
  {
    if (globals::developer_mode)
      log_info("Worldmap: Pingus not on level");
  }
#endif
}

struct unlock_nodes
{
  PathGraph* path_graph;

  unlock_nodes(PathGraph* g) :
    path_graph(g)
  {
  }

  void operator()(Node<Dot*>& node)
  {
    if (node.data->finished())
    {
      //log_info("Unlocking neightbours of: " << node.data);
      for (std::vector<EdgeId>::iterator i = node.next.begin(); i != node.next.end(); ++i)
      {
        Edge<Path*>& edge = path_graph->graph.resolve_edge(*i);

        // FIXME: This should be identical to node.data->unlock(), but not sure
        path_graph->graph.resolve_node(edge.source).data->unlock();
        path_graph->graph.resolve_node(edge.destination).data->unlock();
      }
    }
  }
};

void
Worldmap::update_locked_nodes()
{
  // FIXME: This shouldn't be a polling function
  path_graph->graph.for_each_node(unlock_nodes(path_graph.get()));

#if 0
  bool credits_unlocked = false;
  StatManager::instance()->get_bool(worldmap.get_short_name() + "-endstory-seen", credits_unlocked);

  if (!credits_unlocked)
  {
    // See if the last level is finished
    Dot* dot = path_graph->get_dot(final_node);
    if (dot)
    {
      if (dot->finished())
      {
        ScreenManager::instance()->push_screen(new StoryScreen(worldmap.get_end_story()));
      }
    }
    else
    {
      log_info("Error: Worldmap: Last level missing");
    }
  }
#endif
}

void Worldmap::set_starting_node() {
  NodeId id;
  std::string node_name;

  if (StatManager::instance()->get_string(worldmap.get_short_name() + "-current-node", node_name))
  {
    // Just in case that level doesn't exist, look it up.
    id = path_graph->lookup_node(node_name);
    if (id == NoNode)
      id = default_node;
  }
  else
    id = default_node;

  LevelDot* leveldot = dynamic_cast<LevelDot*>(path_graph->get_dot(id));
  if (leveldot) {
    leveldot->unlock();
  }
}

bool Worldmap::is_final_map() {
  return false;
  // TODO: return pingus->get_node() == final_node;
}

int Worldmap::get_width()  const {
  return worldmap.get_width();
}

int Worldmap::get_height() const {
  return worldmap.get_height();
}

} // namespace WorldmapNS

/* EOF */
