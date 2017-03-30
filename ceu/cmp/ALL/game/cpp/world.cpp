World::World(const PingusLevel& plf) :
  ambient_light(Color(plf.get_ambient_light())),
  gfx_map(new GroundMap(plf.get_size().width, plf.get_size().height)),
  game_time(0),
  do_armageddon(false),
  armageddon_count(0),
  pingus(new PinguHolder(plf)),
  colmap(gfx_map->get_colmap()),
  gravitational_acceleration(0.2f)
{
  WorldObj::set_world(this);
  pingu_particle_holder = new Particles::PinguParticleHolder();
  rain_particle_holder  = new Particles::RainParticleHolder();
  smoke_particle_holder = new Particles::SmokeParticleHolder();
  snow_particle_holder  = new Particles::SnowParticleHolder();
  world_obj.push_back(gfx_map);
  world_obj.push_back(pingu_particle_holder);
  world_obj.push_back(rain_particle_holder);
  world_obj.push_back(smoke_particle_holder);
  world_obj.push_back(snow_particle_holder);
  init_worldobjs(plf);
}
void World::add_object (WorldObj* obj) {
  world_obj.push_back(obj);
}
void World::init_worldobjs(const PingusLevel& plf) {
  const std::vector<FileReader>& objects = plf.get_objects();
  for (std::vector<FileReader>::const_iterator i = objects.begin(); i != objects.end (); ++i) {
    std::vector<WorldObj*> objs = WorldObjFactory::instance()->create(*i);
    for(auto obj = objs.begin(); obj != objs.end(); ++obj) {
      if (*obj) {
        add_object(*obj);
      }
    }
  }
  world_obj.push_back(pingus);
  std::stable_sort (world_obj.begin (), world_obj.end (), WorldObj_less);
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj) {
    (*obj)->on_startup();
  }
}
World::~World() {
  for (WorldObjIter it = world_obj.begin(); it != world_obj.end(); ++it) {
    delete *it;
  }
}
void World::draw (SceneContext& gc) {
  WorldObj::set_world(this);
  gc.light().fill_screen(Color(ambient_light));
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj) {
    (*obj)->draw(gc);
  }
}
void World::draw_smallmap(SmallMap* smallmap) {
  WorldObj::set_world(this);
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj) {
    (*obj)->draw_smallmap (smallmap);
  }
}
void World::update() {
  WorldObj::set_world(this);
  game_time += 1;
  if (do_armageddon) {
    if (game_time % 4 == 0) {
      while (armageddon_count < pingus->get_end_id()) {
        Pingu* pingu = pingus->get_pingu(armageddon_count);
        if (pingu && pingu->get_status() == Pingu::PS_ALIVE) {
          pingu->request_set_action(ActionName::BOMBER);
          break;
        } else {
          ++armageddon_count;
        }
      }
      ++armageddon_count;
    }
  }
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj) {
    (*obj)->update();
  }
}
void World::armageddon(void) {
  Sound::PingusSound::play_sound("goodidea");
  do_armageddon = true;
  armageddon_count = 0;
}
Pingu* World::get_pingu (const Vector3f& pos) {
  Pingu* current_pingu = 0;
  float distance = -1.0;
  for (PinguIter i = pingus->begin (); i != pingus->end (); ++i) {
    if ((*i)->is_over(int(pos.x), int(pos.y))) {
      if (distance == -1.0f || distance >= (*i)->dist(static_cast<int>(pos.x), static_cast<int>(pos.y))) {
        current_pingu = (*i);
        distance = (*i)->dist(static_cast<int>(pos.x), static_cast<int>(pos.y));
      }
    }
  }
  return current_pingu;
}
void World::put(const CollisionMask& mask, int x, int y, Groundtype::GPType type) {
  gfx_map->put(mask.get_surface(), x, y);
  colmap->put(mask, x, y, type);
}
void World::remove(const CollisionMask& mask, int x, int y) {
  gfx_map->remove(mask.get_surface(), x, y);
  colmap->remove(mask, x, y);
}
WorldObj* World::get_worldobj(const std::string& id) {
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj) {
    if ((*obj)->get_id() == id) {
      return *obj;
    }
  }
  return 0;
}
Vector2i World::get_start_pos(int player_id) {
  Vector2i pos;
  int num_entrances = 0;
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj) {
    WorldObjs::Entrance* entrance = dynamic_cast<WorldObjs::Entrance*>(*obj);
    if (entrance && entrance->get_owner_id() == player_id) {
      pos += Vector2i(static_cast<int>(entrance->get_pos().x), static_cast<int>(entrance->get_pos().y));
      num_entrances += 1;
    }
  }
  if (num_entrances > 0) {
    pos.x /= num_entrances;
    pos.y /= num_entrances;
    pos.y += 100;
  }
  return pos;
}
