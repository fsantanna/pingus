Exit::Exit(const FileReader& reader) :
  smallmap_symbol("core/misc/smallmap_exit")
{
  reader.read_vector("position", pos);
  reader.read_desc  ("surface",  desc);
  reader.read_int   ("owner-id", owner_id);
  if (owner_id < 0 || owner_id > 3) {
    owner_id = 0;
  }
  flag = Sprite("core/misc/flag" + StringUtil::to_string(owner_id));
  sprite = Sprite(desc);
}
void Exit::on_startup () {
  CollisionMask mask(desc);
  world->get_colmap()->remove(mask, static_cast<int>(pos.x) - sprite.get_width()/2, static_cast<int>(pos.y) - sprite.get_height());
}
void Exit::draw (SceneContext& gc) {
  gc.color().draw(sprite, pos);
  gc.color().draw(flag, pos + Vector3f(40, 0));
}
void Exit::draw_smallmap(SmallMap* smallmap) {
  smallmap->draw_sprite(smallmap_symbol, pos);
}
void Exit::update () {
  sprite.update();
  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin(); pingu != holder->end(); ++pingu) {
    if ((*pingu)->get_owner()  == owner_id) {
      if (   (*pingu)->get_pos().x > pos.x - 1 && (*pingu)->get_pos().x < pos.x + 1 && (*pingu)->get_pos().y > pos.y - 5 && (*pingu)->get_pos().y < pos.y + 5) {
        if (   (*pingu)->get_status() != Pingu::PS_EXITED && (*pingu)->get_status() != Pingu::PS_DEAD && (*pingu)->get_action() != ActionName::EXITER) {
          (*pingu)->set_action(ActionName::EXITER);
        }
      }
    }
  }
}
