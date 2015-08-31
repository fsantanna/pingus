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
