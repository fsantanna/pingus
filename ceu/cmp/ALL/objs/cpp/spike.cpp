Spike::Spike (const FileReader& reader) :
  surface("traps/spike"),
  killing(false)
{
  reader.read_vector("position", pos);
}
void Spike::draw (SceneContext& gc) {
  if (killing) {
    gc.color().draw (surface, pos);
  }
}
void Spike::update() {
  if (killing) {
    surface.update();
  }
  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu) {
    catch_pingu(*pingu);
  }
  if (surface.get_current_frame() == surface.get_frame_count() - 1) {
    killing = false;
  }
}
void Spike::catch_pingu (Pingu* pingu) {
  if (!killing) {
    if ( pingu->get_pos().x > pos.x + 16 - 5 && pingu->get_pos().x < pos.x + 16 + 5 && pingu->get_pos().y > pos.y          && pingu->get_pos().y < pos.y + 32) {
      surface.restart();
      killing = true;
    }
  } else {
    if (surface.get_current_frame() == 3 && pingu->get_pos().x > pos.x +16 - 12 && pingu->get_pos().x < pos.x + 16 + 12 && pingu->get_pos().y > pos.y          && pingu->get_pos().y < pos.y + 32) {
      pingu->set_status(Pingu::PS_DEAD);
    }
  }
}
