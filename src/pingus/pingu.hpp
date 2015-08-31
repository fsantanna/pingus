#ifndef HEADER_PINGUS_PINGUS_PINGU_HPP
#define HEADER_PINGUS_PINGUS_PINGU_HPP
#include <memory>
#include "math/vector3f.hpp"
#include "pingus/action_name.hpp"

class ActionHolder;
class PinguAction;
class SceneContext;

class Pingu {
public:
  enum PinguStatus { PS_NONE, PS_ALIVE, PS_EXITED, PS_DEAD };

private:
  PinguStatus status;

public:
  Pingu (int arg_id, const Vector3f& pos, int owner);
  ~Pingu ();

  void* ceu;

  PinguStatus get_status (void) const;
/// TODO: pingu_holder

  float get_z_pos () const { return 0; }

private:
  Pingu(Pingu const &) = delete;
  void operator=(Pingu const &t) = delete;
  Pingu(Pingu &&) = delete;
};

#endif
