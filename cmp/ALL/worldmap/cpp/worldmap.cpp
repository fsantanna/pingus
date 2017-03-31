Worldmap* Worldmap::current_ = 0;
Worldmap::Worldmap(const Pathname& filename) :
  mouse_x(0),
  mouse_y(0)
{
  current_ = this;
  worldmap = PingusWorldmap(filename);
  const std::vector<FileReader>& object_reader = worldmap.get_objects();
  for(std::vector<FileReader>::const_iterator i = object_reader.begin(); i != object_reader.end(); ++i) {
    Drawable* drawable = DrawableFactory::create(*i);
    if (drawable) {
      objects.push_back(drawable);
      drawables.push_back(drawable);
    }
  }
  FileReader path_graph_reader = worldmap.get_graph();
  path_graph.reset(new PathGraph(this, path_graph_reader));
  default_node = path_graph->lookup_node(worldmap.get_default_node());
  final_node   = path_graph->lookup_node(worldmap.get_final_node());
  pingus = new Pingus(path_graph.get());
  set_starting_node();
  add_drawable(pingus);
  gc_state.set_limit(Rect(Vector2i(0, 0), Size(worldmap.get_width(), worldmap.get_height())));
}
Worldmap::~Worldmap() {
  for (auto i = drawables.begin (); i != drawables.end (); ++i) {
    delete (*i);
  }
}
void Worldmap::draw(DrawingContext& gc)
{
  Vector2i pingu_pos(static_cast<int>(pingus->get_pos().x), static_cast<int>(pingus->get_pos().y));
  int min, max;
  int width  = worldmap.get_width();
  int height = worldmap.get_height();
  if (width >= gc.get_width()) {
    min = gc.get_width()/2;
    max = width - gc.get_width()/2;
  } else {
    min = width - gc.get_width()/2;
    max = gc.get_width()/2;
  }
  pingu_pos.x = Math::clamp(min, pingu_pos.x, max);
  if (height >= gc.get_height()) {
    min = gc.get_height()/2;
    max = height - gc.get_height()/2;
  } else {
    min = height - gc.get_height()/2;
    max = gc.get_height()/2;
  }
  pingu_pos.y = Math::clamp(min, pingu_pos.y, max);
  gc_state.set_size(gc.get_width(), gc.get_height());
  gc_state.set_pos(Vector2i(pingu_pos.x, pingu_pos.y));
  gc_state.push(gc);
  for (DrawableLst::iterator i = drawables.begin (); i != drawables.end (); ++i) {
    (*i)->draw(gc);
  }
  Vector2f mpos = gc_state.screen2world(Vector2i(mouse_x, mouse_y));
  Dot* dot = path_graph->get_dot(mpos.x, mpos.y);
  if (dot) {
    dot->draw_hover(gc);
  }
  gc_state.pop(gc);
}
void Worldmap::update(float delta) {
  for (DrawableLst::iterator i = drawables.begin (); i != drawables.end (); ++i) {
    (*i)->update (delta);
  }
}
void Worldmap::on_startup() {
  Sound::PingusSound::play_music(worldmap.get_music());
  update_locked_nodes();
}
void Worldmap::add_drawable(Drawable* drawable) {
  drawables.push_back(drawable);
}
void Worldmap::on_pointer_move(int x, int y) {
  mouse_x = x;
  mouse_y = y;
}
void Worldmap::on_primary_button_press(int x, int y) {
  Vector2f click_pos = gc_state.screen2world(Vector2i(x, y));
  Dot* dot = path_graph->get_dot(click_pos.x, click_pos.y);
  if (dot) {
    if (path_graph->lookup_node(dot->get_name()) == pingus->get_node()) {
      dot->on_click();
    } else {
      if (dot->accessible()) {
        if (!pingus->walk_to_node(path_graph->lookup_node(dot->get_name()))) {
        } else {
          StatManager::instance()->set_string(worldmap.get_short_name() + "-current-node", dot->get_name());
        }
      } else {
        Sound::PingusSound::play_sound("chink");
      }
    }
  }
}
void Worldmap::on_secondary_button_press(int x, int y) {
  if (globals::developer_mode) {
    Vector3f click_pos = gc_state.screen2world(Vector2i(x, y));
    Dot* dot = path_graph->get_dot(click_pos.x, click_pos.y);
    if (dot) { // FIXME: Dot NodeID missmatch...
      NodeId id = path_graph->get_id(dot);
      pingus->set_position(id);
    }
  }
}
void Worldmap::enter_level() {
  NodeId node = get_pingus()->get_node();
  if (node != NoNode) {
    Dot* dot = path_graph->get_dot(node);
    if (dot) {
      dot->on_click();
    }
  }
}
struct unlock_nodes {
  void operator()(Node<Dot*>& node) {
    if (node.data->finished()) {
      for (std::vector<EdgeId>::iterator i = node.next.begin(); i != node.next.end(); ++i) {
        Edge<Path*>& edge = path_graph->graph.resolve_edge(*i);
        path_graph->graph.resolve_node(edge.source).data->unlock();
        path_graph->graph.resolve_node(edge.destination).data->unlock();
      }
    }
  }
};
void Worldmap::update_locked_nodes() {
  path_graph->graph.for_each_node(unlock_nodes(path_graph.get()));
}
void Worldmap::set_starting_node() {
  NodeId id;
  std::string node_name;
  if (StatManager::instance()->get_string(worldmap.get_short_name() + "-current-node", node_name)) {
    id = path_graph->lookup_node(node_name);
    if (id == NoNode){
      id = default_node;
    }
  } else {
    id = default_node;
  }
  pingus->set_position(id);
  LevelDot* leveldot = dynamic_cast<LevelDot*>(path_graph->get_dot(id));
  if (leveldot) {
    leveldot->unlock();
  }
}
