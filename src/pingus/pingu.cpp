#include "pingus/pingu.hpp"

#include <sstream>

#include "engine/display/scene_context.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobj.hpp"
#include "pingus/pingu_enums.hpp"
#include "util/log.hpp"
#include "ceu_vars.h"

// Init a pingu at the given position while falling
Pingu::Pingu (int arg_id, const Vector3f& arg_pos, int owner) :
  id(arg_id),
/// TODO: exit
  status(PS_ALIVE)
{}
Pingu::~Pingu () { }

unsigned int
Pingu::get_id ()
{
  return id;
}

// TODO: pinguholder/world
Pingu::PinguStatus Pingu::get_status (void) const {
  return status;
}

std::string Pingu::get_name() {
  if (this->name.empty()) {
    return ActionName::to_screenname(current_action);
  } else {
    return this->name;
  }
}

ActionName::Enum Pingu::get_action () {
  return current_action;
}
