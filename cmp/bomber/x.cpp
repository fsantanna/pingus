Bomber::Bomber (Pingu* p) :
  PinguAction(p),
  particle_thrown(false),
  sound_played(false),
  gfx_exploded(false),
  colmap_exploded(false),
  bomber_radius("other/bomber_radius_gfx", "other/bomber_radius"),
  explo_surf(Sprite("pingus/player" + pingu->get_owner_str() + "/explo"))
{
  sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() + "/bomber/left");
  sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/bomber/right");
  WorldObj::get_world()->play_sound("ohno", pingu->get_pos ());
}
void Bomber::draw (SceneContext& gc) {
  if (sprite[pingu->direction].get_current_frame() >= 13 && !gfx_exploded) {
    gc.color().draw (explo_surf, Vector3f(pingu->get_x () - 32, pingu->get_y () - 48));
    gfx_exploded = true;
  }
  gc.color().draw(sprite[pingu->direction], pingu->get_pos ());
}
void Bomber::update () {
  sprite.update ();
  Movers::LinearMover mover(WorldObj::get_world(), pingu->get_pos());
  Vector3f velocity = pingu->get_velocity();
  mover.update(velocity, Colliders::PinguCollider(pingu_height));
  pingu->set_pos(mover.get_pos());
  if (sprite[pingu->direction].get_current_frame() <= 9 && (rel_getpixel(0, -1) == Groundtype::GP_WATER || rel_getpixel(0, -1) == Groundtype::GP_LAVA)) {
    pingu->set_action(ActionName::DROWN);
    return;
  }
  if (sprite[pingu->direction].get_current_frame () <= 9 && rel_getpixel(0, -1) != Groundtype::GP_NOTHING && velocity.y > deadly_velocity) {
    pingu->set_action(ActionName::SPLASHED);
    return;
  }
  if (sprite[pingu->direction].get_current_frame () > 9 && !sound_played) {
    WorldObj::get_world()->play_sound("plop", pingu->get_pos ());
    sound_played = true;
  }
  if (sprite[pingu->direction].get_current_frame () > 12 && !particle_thrown) {
    particle_thrown = true;
    WorldObj::get_world()->get_pingu_particle_holder()->add_particle(static_cast<int>(pingu->get_x()), static_cast<int>(pingu->get_y()) - 5);
  }
  if (sprite[pingu->direction].get_current_frame () >= 13 && !colmap_exploded) {
    colmap_exploded = true;
    WorldObj::get_world()->remove(bomber_radius, static_cast<int>(static_cast<int>(pingu->get_x()) - (bomber_radius.get_width()/2)), static_cast<int>(static_cast<int>(pingu->get_y()) - 16 - (bomber_radius.get_width()/2)));
  }
  if (sprite[pingu->direction].is_finished ()) {
    pingu->set_status(Pingu::PS_DEAD);
  }
}
