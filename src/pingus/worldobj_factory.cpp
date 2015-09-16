//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/worldobj_factory.hpp"

#include <stdexcept>

#include "pingus/prefab_file.hpp"
#if 0
#include "pingus/worldobjs/conveyor_belt.hpp"
#include "pingus/worldobjs/fake_exit.hpp"
#include "pingus/worldobjs/hammer.hpp"
#include "pingus/worldobjs/ice_block.hpp"
#include "pingus/worldobjs/rain_generator.hpp"
#include "pingus/worldobjs/snow_generator.hpp"
#include "pingus/worldobjs/solid_color_background.hpp"
#include "pingus/worldobjs/starfield_background.hpp"
#include "pingus/worldobjs/switch_door_door.hpp"
#include "pingus/worldobjs/switch_door_switch.hpp"
#include "pingus/worldobjs/teleporter.hpp"
#include "pingus/worldobjs/teleporter_target.hpp"
#endif
#include "math/vector3f.hpp"
#include "util/log.hpp"
#include "util/overrride_file_reader.hpp"

///using namespace WorldObjs;

/// TEMP
Vector3f WorldObjFactory::pos = Vector3f(0,0,0);

WorldObjFactory* WorldObjFactory::instance_ = 0;

#include "ceu_vars.h"

/** WorldObjAbstractFactory, interface for creating factories */
class WorldObjAbstractFactory
{
public:
  WorldObjAbstractFactory (const std::string& id) {
    WorldObjFactory::instance ()->register_factory (id, this);
  }

  virtual ~WorldObjAbstractFactory() {}

  virtual void create(const FileReader& reader) =0;

private:
  WorldObjAbstractFactory (const WorldObjAbstractFactory&);
  WorldObjAbstractFactory& operator= (const WorldObjAbstractFactory&);
};

/** Template to create factories, usage:
    new WorldObjFactoryImpl<"liquid", Liquid>; */
class WorldObjFactoryImpl : public WorldObjAbstractFactory
{
public:
  std::string id;
  WorldObjFactoryImpl (const std::string& id)
    : WorldObjAbstractFactory (id) {this->id = id;}

  void create(const FileReader& reader) {
    tceu__char___FileReader_ p = { (char*)id.c_str(), (FileReader*)&reader };
    ceu_sys_go(&CEU_APP, CEU_IN_WORLD_NEWOBJ, &p);
  }

private:
  WorldObjFactoryImpl (const WorldObjFactoryImpl&);
  WorldObjFactoryImpl& operator= (const WorldObjFactoryImpl&);
};

class WorldObjGroupFactory : public WorldObjAbstractFactory
{
public:
  WorldObjGroupFactory (const std::string& id) :
    WorldObjAbstractFactory(id)
  {}

  virtual ~WorldObjGroupFactory() {}

  virtual void create(const FileReader& reader) {
    FileReader objects = reader.read_section("objects");
    std::vector<FileReader> sections = objects.get_sections();
    for(auto it = sections.begin(); it != sections.end(); ++it)
    {
      WorldObjFactory::instance()->create(*it);
    }
  }

private:
  WorldObjGroupFactory (const WorldObjGroupFactory&);
  WorldObjGroupFactory& operator= (const WorldObjGroupFactory&);
};

class WorldObjPrefabFactory : public WorldObjAbstractFactory
{
public:
  WorldObjPrefabFactory (const std::string& id) :
    WorldObjAbstractFactory(id)
  { }

  virtual ~WorldObjPrefabFactory() {}

  virtual void create(const FileReader& reader) {
    std::string name;
    reader.read_string("name", name);

    Vector3f pos;
    reader.read_vector("position", pos);
    WorldObjFactory::pos += pos;

    PrefabFile prefab = PrefabFile::from_resource(name);
    FileReader overrides;
    reader.read_section("overrides", overrides);

    const std::vector<FileReader>& objects = prefab.get_objects();
    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
      OverrideFileReader override_reader(*it, overrides);

      WorldObjFactory::instance()->create(override_reader);
    }

    WorldObjFactory::pos -= pos;
  }

private:
  WorldObjPrefabFactory (const WorldObjPrefabFactory&);
  WorldObjPrefabFactory& operator= (const WorldObjPrefabFactory&);
};

WorldObjFactory::WorldObjFactory() :
  factories()
{
  // Register all WorldObj's
}

WorldObjFactory*
WorldObjFactory::instance()
{
  if ( ! instance_)
  {
    instance_ = new WorldObjFactory ();

    // Registring Factories
    new WorldObjGroupFactory("group");
    new WorldObjPrefabFactory("prefab");

    new WorldObjFactoryImpl("liquid");
    new WorldObjFactoryImpl("hotspot");
    new WorldObjFactoryImpl("entrance");
    new WorldObjFactoryImpl("exit");

    // traps
    ///new WorldObjFactoryImpl<FakeExit>("fake_exit");
    new WorldObjFactoryImpl("guillotine");
    ///new WorldObjFactoryImpl<Hammer>("hammer");
    new WorldObjFactoryImpl("laser_exit");
    new WorldObjFactoryImpl("smasher");
    new WorldObjFactoryImpl("spike");

    // Special Objects
    ///new WorldObjFactoryImpl<SwitchDoorSwitch>("switchdoor-switch");
    ///new WorldObjFactoryImpl<SwitchDoorDoor>("switchdoor-door");
    ///new WorldObjFactoryImpl<IceBlock>("iceblock");
    ///new WorldObjFactoryImpl<ConveyorBelt>("conveyorbelt");
    ///new WorldObjFactoryImpl<Teleporter>("teleporter");
    ///new WorldObjFactoryImpl<TeleporterTarget>("teleporter-target");

    // Backgrounds
    new WorldObjFactoryImpl("surface-background");
    ///new WorldObjFactoryImpl<StarfieldBackground>("starfield-background");
    ///new WorldObjFactoryImpl<SolidColorBackground>("solidcolor-background");

    // Weather
    ///new WorldObjFactoryImpl<SnowGenerator>("snow-generator");
    ///new WorldObjFactoryImpl<RainGenerator>("rain-generator");
    // Weather-Backward compability
    ///new WorldObjFactoryImpl<SnowGenerator>("snow");
    ///new WorldObjFactoryImpl<RainGenerator>("rain");

    // Groundpieces
    new WorldObjFactoryImpl("groundpiece");
  }

  return instance_;
}

void WorldObjFactory::deinit()
{
  if (instance_)
  {
    instance_->free_factories();
    delete instance_;
    instance_ = 0;
  }
}

void
WorldObjFactory::create(const FileReader& reader)
{
  std::map<std::string, WorldObjAbstractFactory*>::iterator it = factories.find(reader.get_name());

  if (it == factories.end())
  {
    log_error("invalid id: '%1%'", reader.get_name());
  }
  else
  {
    it->second->create(reader);
  }
}

void
WorldObjFactory::register_factory (const std::string& id,
                                   WorldObjAbstractFactory* factory)
{
  factories[id] = factory;
}

void
WorldObjFactory::free_factories()
{
  for (std::map<std::string, WorldObjAbstractFactory*>::iterator i = factories.begin(); i != factories.end(); ++i)
  {
    delete i->second;
  }
}

/* EOF */
