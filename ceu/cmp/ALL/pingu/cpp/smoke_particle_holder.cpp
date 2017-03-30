SmokeParticleHolder::SmokeParticle::SmokeParticle (float x, float y, float vel_x, float vel_y) :
  pos(Vector3f(x,y)),
  velocity(Vector3f(vel_x, vel_y))
{
  time = livetime = 25 + (rand() % 10);
  use_surf2 = rand() % 2;
}
SmokeParticleHolder::SmokeParticleHolder ()
  : surf1("particles/smoke"),
    surf2("particles/smoke2"),
{
}
void SmokeParticleHolder::add_particle (float x, float y, float vel_x, float vel_y) {
  for (std::vector<SmokeParticle>::iterator it=particles.begin(); it != particles.end(); ++it) {
    if (!it->livetime) {
      *it = SmokeParticle(x, y, vel_x, vel_y);
      return;
    }
  }
  particles.push_back(SmokeParticle(x, y, vel_x, vel_y));
}
void SmokeParticleHolder::update () {
  for (std::vector<SmokeParticle>::iterator it=particles.begin(); it != particles.end(); ++it) {
    if (!it->livetime) {
      continue;
    }
    it->pos.x += it->velocity.x;
    it->pos.y += it->velocity.y;
    it->pos.z = get_z_pos();
    --it->livetime;
  }
}
void SmokeParticleHolder::draw (SceneContext& gc) {
  for (std::vector<SmokeParticle>::iterator it=particles.begin(); it != particles.end(); ++it) {
    if (!it->livetime) {
      continue;
    }
    if (!it->use_surf2) {
      gc.color().draw(surf1, it->pos);
    } else {
      gc.color().draw(surf2, it->pos);
    }
  }
}
