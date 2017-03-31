PinguHolder::PinguHolder(const PingusLevel& plf) :
  number_of_allowed(plf.get_number_of_pingus()),
  number_of_exited(0),
{
}
PinguHolder::~PinguHolder() {
  for(std::vector<Pingu*>::iterator i = all_pingus.begin(); i != all_pingus.end(); ++i) {
    delete *i;
  }
}
Pingu* PinguHolder::create_pingu (const Vector3f& pos, int owner_id) {
  if (number_of_allowed > get_number_of_released()) {
    Pingu* pingu = new Pingu (static_cast<int>(all_pingus.size()), pos, owner_id);
    all_pingus.push_back (pingu);
    pingus.push_back(pingu);
    return pingu;
  } else {
    return 0;
  }
}
void PinguHolder::draw (SceneContext& gc) {
  for(std::list<Pingu*>::iterator pingu = pingus.begin(); pingu != pingus.end(); ++pingu) {
      (*pingu)->draw (gc);
  }
}
void PinguHolder::update() {
  PinguIter pingu = pingus.begin();
  while(pingu != pingus.end()) {
    (*pingu)->update();
    if ((*pingu)->get_status() == Pingu::PS_DEAD) {
      pingu = pingus.erase(pingu);
    } else if ((*pingu)->get_status() == Pingu::PS_EXITED) {
      number_of_exited += 1;
      pingu = pingus.erase(pingu);
    } else {
      ++pingu;
    }
  }
}
Pingu* PinguHolder::get_pingu(unsigned int id_) {
  if (id_ < all_pingus.size()) {
    Pingu* pingu = all_pingus[id_];
    assert(pingu->get_id() == id_);
    if (pingu->get_status() == Pingu::PS_ALIVE) {
      return pingu;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}
int PinguHolder::get_number_of_exited() {
  return number_of_exited;
}
int PinguHolder::get_number_of_killed() {
  return static_cast<int>(all_pingus.size()) - static_cast<int>(pingus.size()) - get_number_of_exited();
}
int PinguHolder::get_number_of_alive() {
  return static_cast<int>(pingus.size());
}
int PinguHolder::get_number_of_released() {
  return static_cast<int>(all_pingus.size());
}
int PinguHolder::get_number_of_allowed() {
  return number_of_allowed;
}
