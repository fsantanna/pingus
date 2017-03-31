Pingus::Pingus (PathGraph* arg_path) :
  Drawable("pingus"),
  path(arg_path),
  sprite_standing("core/worldmap/pingus_standing"),
  arrow("core/worldmap/arrow"),
{
  sprite.load(Direction::LEFT,  Sprite("core/worldmap/pingus/left"));
  sprite.load(Direction::RIGHT, Sprite("core/worldmap/pingus/right"));
  final_target_node = NoNode;
  current_node = NoNode;
}
void Pingus::draw (DrawingContext& gc) {
  if (final_target_node != NoNode && current_node == NoNode) {
    gc.draw(arrow, path->get_dot(final_target_node)->get_pos() + Vector3f(0, 0, 10));
  }
  if (!is_walking()) {
    gc.draw(sprite_standing, pos);
  } else {
    float direction = get_direction();
    if (direction >= 0 && direction < 180) {
      gc.draw(sprite[Direction::RIGHT], pos + Vector3f(0, 0, 10));
    } else {
      gc.draw(sprite[Direction::LEFT], pos + Vector3f(0, 0, 10));
    }
  }
}
void Pingus::update (float delta) {
  float direction = get_direction();
  if (direction >= 0 && direction < 180) {
    sprite[Direction::RIGHT].update(delta);
  }
  else {
    sprite[Direction::LEFT].update(delta);
  }
  if (is_walking()) {
    update_walk(delta);
  }
}
void Pingus::update_walk (float delta) {
  float velocity = 70.0f;
  edge_path_position += velocity * delta;
  if (edge_path_position > edge_path.length()) {
    if (node_path.empty ()) {
      current_node = target_node;
      final_target_node = NoNode;
    } else {
      update_edge_path();
    }
  }
  last_pos = pos;
  pos = calc_pos ();
}
float Pingus::get_direction() const {
  return (Math::atan2(last_pos.x - pos.x, -(last_pos.y - pos.y)) / static_cast<float>(Math::pi) * 180.0f) + 180.0f;
}
bool Pingus::walk_to_node (NodeId target) {
  final_target_node = target;
  if (current_node == target) {
    return true;
  } else if (current_node != NoNode) {
    const PathfinderResult& res = path->get_path (current_node, target);
    if (res.path.empty()) {
      return false;
    } else {
      node_path = res.path;
      target_node = node_path.back(); // equal to current_node;
      node_path.pop_back();
      update_edge_path();
      current_node = NoNode;
      return true;
    }
  } else {
    if (target_node == target) {
      node_path.clear();
      return true;
    } else if (source_node == target) {
      std::swap(target_node, source_node);
      edge_path.reverse();
      edge_path_position = edge_path.length() - edge_path_position;
      node_path.clear();
      return true;
    } else {
      const PathfinderResult& node_path1 = path->get_path (source_node, target);
      const PathfinderResult& node_path2 = path->get_path (target_node, target);
      if (node_path1.path.empty()) {
        if (node_path2.path.empty()) {
          return false;
        } else {
          node_path = node_path2.path;
        }
      } else {
        if (node_path1.cost + edge_path_position < node_path2.cost + (edge_path.length() - edge_path_position)) { // walk to source node, which means to reverse the pingu
          node_path = node_path1.path;
          std::swap(target_node, source_node);
          edge_path.reverse();
          edge_path_position = edge_path.length() - edge_path_position;
        } else { // walk to target_node
          node_path = node_path2.path;
        }
      }
      node_path.pop_back();
      return true;
    }
  }
}
Vector3f Pingus::calc_pos () {
  if (current_node != NoNode) {
    return path->graph.resolve_node(current_node).data->get_pos ();
  } else {
    return edge_path.at(edge_path_position);
  }
}
void Pingus::set_position (NodeId node) {
  pos = path->get_dot(node)->get_pos();
  current_node = node;
}
void Pingus::update_edge_path()
{
  source_node = target_node;
  target_node = node_path.back ();
  node_path.pop_back (); // remove target node from list of nodes
  edge_path_position = 0.0f;
  edge_path.clear();
  Path* partial_path = path->graph.resolve_edge(source_node, target_node).data;
  edge_path.push_back(path->graph.resolve_node(source_node).data->get_pos());
  edge_path.reverse_insert(*partial_path);
  edge_path.push_back(path->graph.resolve_node(target_node).data->get_pos());
}
bool Pingus::is_walking() {
  return current_node == NoNode;
}
