class WorldObjPrefabFactory : public WorldObjAbstractFactory
{
  virtual std::vector<WorldObj*> create(const FileReader& reader) {
    std::string name;
    reader.read_string("name", name);
    Vector3f pos;
    reader.read_vector("position", pos);
    PrefabFile prefab = PrefabFile::from_resource(name);
    FileReader overrides;
    reader.read_section("overrides", overrides);
    std::vector<WorldObj*> group;
    const std::vector<FileReader>& objects = prefab.get_objects();
    for(auto it = objects.begin(); it != objects.end(); ++it) {
      OverrideFileReader override_reader(*it, overrides);
      std::vector<WorldObj*> objs = WorldObjFactory::instance()->create(override_reader);
      for(auto obj = objs.begin(); obj != objs.end(); ++obj) {
        if (*obj) {
          (*obj)->set_pos((*obj)->get_pos() + pos);
          group.push_back(*obj);
        }
      }
    }
    return group;
  }
};
WorldObjFactory* WorldObjFactory::instance() {
  if ( ! instance_) {
    instance_ = new WorldObjFactory ();
    new WorldObjGroupFactory("group");
    new WorldObjPrefabFactory("prefab");
    new WorldObjFactoryImpl<Liquid>("liquid");
    new WorldObjFactoryImpl<Hotspot>("hotspot");
    new WorldObjFactoryImpl<Entrance>("entrance");
    new WorldObjFactoryImpl<Exit>("exit");
    new WorldObjFactoryImpl<Guillotine>("guillotine");
    new WorldObjFactoryImpl<Smasher>("smasher");
    new WorldObjFactoryImpl<Spike>("spike");
    new WorldObjFactoryImpl<SurfaceBackground>("surface-background");
    new WorldObjFactoryImpl<Groundpiece>("groundpiece");
  }
  return instance_;
}
void WorldObjFactory::deinit() {
  if (instance_) {
    instance_->free_factories();
    delete instance_;
    instance_ = 0;
  }
}
std::vector<WorldObj*> WorldObjFactory::create(const FileReader& reader) {
  std::map<std::string, WorldObjAbstractFactory*>::iterator it = factories.find(reader.get_name());
  if (it == factories.end()) {
    return std::vector<WorldObj*>();
  } else {
    return it->second->create(reader);
  }
}
void WorldObjFactory::register_factory (const std::string& id, WorldObjAbstractFactory* factory) {
  factories[id] = factory;
}
void WorldObjFactory::free_factories() {
  for (std::map<std::string, WorldObjAbstractFactory*>::iterator i = factories.begin(); i != factories.end(); ++i) {
    delete i->second;
  }
}
