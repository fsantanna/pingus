Smasher::Smasher(const FileReader& reader) :
  sprite("traps/smasher"),
  smashing(false),
  downwards(false),
  count(0)
{
  reader.read_vector("position", pos);
}
void Smasher::update () {
  PinguHolder* holder = world->get_pingus();
  for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu) {
    catch_pingu(*pingu);
  }
  if (smashing) {
    sprite.set_frame(count);
    if (downwards) {
      if (count >= 5) {
        downwards = false;
        --count;
        Sound::PingusSound::play_sound("tenton");
        for(int i=0; i < 20; ++i) {
          world->get_smoke_particle_holder()-> add_particle(pos.x + 20 + float(rand() % 260), pos.y + 180, Math::frand()-0.5f, Math::frand()-0.5f);
        }
        for (PinguIter pingu = holder->begin (); pingu != holder->end (); ++pingu) {
          if ((*pingu)->is_inside(static_cast<int>(pos.x + 30), static_cast<int>(pos.y + 90), static_cast<int>(pos.x + 250), static_cast<int>(pos.y + 190))) {
            if ((*pingu)->get_action() != ActionName::SPLASHED) {
              (*pingu)->set_action(ActionName::SPLASHED);
            }
          }
        }
      } else {
        ++count;
      }
    } else {
      if (count <= 0) {
        count = 0;
        smashing = false;
      } else {
        --count;
      }
    }
  }
}
void Smasher::on_startup () {
  log_info("Drawing colmap entry");
  CollisionMask buf("traps/smasher_cmap");
  world->put(buf, static_cast<int>(pos.x), static_cast<int>(pos.y), Groundtype::GP_SOLID);
}
void Smasher::draw (SceneContext& gc) {
  gc.color().draw(sprite, pos);
}
void Smasher::catch_pingu (Pingu* pingu) {
  if ((   pingu->direction.is_left() && pingu->get_pos().x > pos.x + 65 && pingu->get_pos().x < pos.x + 85) || (   pingu->direction.is_right() && pingu->get_pos().x > pos.x + 190 && pingu->get_pos().x < pos.x + 210)) {
    if (pingu->get_action() != ActionName::SPLASHED) {
      if (!smashing) {
        count = 0;
        downwards = true;
        smashing = true;
      }
    }
  }
}
