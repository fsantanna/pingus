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

WorldObjFactory::WorldObjFactory() {}

WorldObjFactory* WorldObjFactory::instance() {
  if ( ! instance_)
  {
    instance_ = new WorldObjFactory ();
    // traps
    ///new WorldObjFactoryImpl<FakeExit>("fake_exit");
    ///new WorldObjFactoryImpl<Hammer>("hammer");
    // Special Objects
    ///new WorldObjFactoryImpl<SwitchDoorSwitch>("switchdoor-switch");
    ///new WorldObjFactoryImpl<SwitchDoorDoor>("switchdoor-door");
    ///new WorldObjFactoryImpl<IceBlock>("iceblock");
    ///new WorldObjFactoryImpl<ConveyorBelt>("conveyorbelt");
    ///new WorldObjFactoryImpl<Teleporter>("teleporter");
    ///new WorldObjFactoryImpl<TeleporterTarget>("teleporter-target");
    // Backgrounds
    ///new WorldObjFactoryImpl<StarfieldBackground>("starfield-background");
    ///new WorldObjFactoryImpl<SolidColorBackground>("solidcolor-background");
    // Weather
    ///new WorldObjFactoryImpl<SnowGenerator>("snow-generator");
    ///new WorldObjFactoryImpl<RainGenerator>("rain-generator");
    // Weather-Backward compability
    ///new WorldObjFactoryImpl<SnowGenerator>("snow");
    ///new WorldObjFactoryImpl<RainGenerator>("rain");
  }
  return instance_;
}

void WorldObjFactory::create(const FileReader& reader) {
    if (reader.get_name()=="prefab") {
        std::string name;
        reader.read_string("name", name);

        Vector3f pos;
        reader.read_vector("position", pos);
        WorldObjFactory::pos += pos;

        PrefabFile prefab = PrefabFile::from_resource(name);
        FileReader overrides;
        reader.read_section("overrides", overrides);

        const std::vector<FileReader>& objects = prefab.get_objects();
        for(auto it = objects.begin(); it != objects.end(); ++it) {
          OverrideFileReader override_reader(*it, overrides);

          WorldObjFactory::instance()->create(override_reader);
        }

        WorldObjFactory::pos -= pos;
    } else if (reader.get_name()=="group") {
      FileReader objects = reader.read_section("objects");
      std::vector<FileReader> sections = objects.get_sections();
      for(auto it = sections.begin(); it != sections.end(); ++it) {
        WorldObjFactory::instance()->create(*it);
      }
    } else {
      tceu__char___FileReader_ p = { (char*)reader.get_name().c_str(), (FileReader*)&reader };
      ceu_sys_go(&CEU_APP, CEU_IN_WORLD_NEWOBJ, &p);
    }
}

/* EOF */
