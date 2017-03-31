CaptureRectangle::CaptureRectangle(GameSession* session_) :
  session(session_),
  pingu(0),
  owner_id(0),
  good("game/cursors/capgood"),
  bad("game/cursors/capbad"),
  arrow_left("game/cursors/arrow_left"),
  arrow_right("game/cursors/arrow_right"),
  font(Fonts::courier_small)
{
}
void CaptureRectangle::draw(SceneContext& sc) {
  if (pingu && pingu->catchable()) {
    if (session && pingu->change_allowed(session->get_action_name())) {
      sc.color().draw(good, pingu->get_center_pos() + Vector3f(0, 0, 1000));
    } else {
      sc.color().draw(bad, pingu->get_center_pos() + Vector3f(0, 0, 1000));
    }
    if (pingu->direction.is_left()) {
      sc.color().draw(arrow_left, pingu->get_center_pos() + Vector3f(0, 2, 1000));
    } else {
      sc.color().draw(arrow_right, pingu->get_center_pos() + Vector3f(0, 2, 1000));
    }
    sc.color().print_center(font, Vector2i(static_cast<int>(pingu->get_center_pos().x), static_cast<int>(pingu->get_center_pos().y - 46)), action_str, 1000);
  }
}
void CaptureRectangle::set_pingu (Pingu* p) {
  pingu = p;
  if (pingu) {
    action_str = pingu->get_name();
    if (pingu->get_wall_action() || pingu->get_fall_action()) {
      action_str += "[";
      if (pingu->get_wall_action()) {
        action_str += pingu->get_wall_action()->get_persistent_char();
      }
      if (pingu->get_fall_action()) {
        action_str += pingu->get_fall_action()->get_persistent_char();
      }
      action_str += "]";
    }
  }
}
