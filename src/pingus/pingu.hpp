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
  ActionName::Enum current_action;
  std::string      name = "";

private:
  unsigned int id;
  PinguStatus status;

public:
  float pos_x;
  float pos_y;

  Pingu (int arg_id, const Vector3f& pos, int owner);
  ~Pingu ();

  void* ceu;

  Vector3f get_pos () const;
/// TODO: worldobjs

  PinguStatus get_status (void) const;
/// TODO: pingu_holder

  std::string get_name();
/// TODO: char[] in functions

  /// Returns the unique id of the pingu
  unsigned int  get_id (void);

  float get_z_pos () const { return 0; }

  /** @return the name of the action the Pingu currently has */
  ActionName::Enum get_action ();

private:
  Pingu(Pingu const &) = delete;
  void operator=(Pingu const &t) = delete;
  Pingu(Pingu &&) = delete;
};

#endif
