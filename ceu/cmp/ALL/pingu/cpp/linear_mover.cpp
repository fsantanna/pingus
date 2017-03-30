LinearMover::LinearMover(World* const world_arg, const Vector3f& pos_arg) : Mover(world_arg, pos_arg)
{
}
void LinearMover::update(const Vector3f& move, const Collider& collision_check) {
  Vector3f target_pos = pos + move;
  Vector3f step_vector = move;
  int move_length = static_cast<int>(move.length());
  step_vector.normalize();
  collision = false;
  for (int i = 0; i < move_length && !collision; ++i) {
    collision = collision_check(world, pos, step_vector);
    pos += step_vector;
  }
  if (collision) {
    pos -= step_vector;
  }
  remaining_move = target_pos - pos;
}
