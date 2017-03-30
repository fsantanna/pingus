const float x_collision_decrease = 0.3f;
const float y_collision_decrease = 0.6f;
PinguParticleHolder::PinguParticle::PinguParticle (int x, int y)
  : livetime(50 + (rand() % 75)),
    use_frame2((rand() % 5) == 0),
    pos(Vector3f(static_cast<float>(x), static_cast<float>(y))),
    velocity(Vector3f(Math::frand() * 7 - 3.5f, Math::frand() * -9))
{
}
PinguParticleHolder::PinguParticleHolder () :
  surface("particles/pingu_explo"),
  particles()
{
}
void PinguParticleHolder::add_particle (int x, int y) {
  int i = 0;
  for (std::vector<PinguParticle>::iterator it=particles.begin(); it != particles.end(); ++it) {
    if (!it->livetime) {
      *it = PinguParticle(x, y);
      ++i;
    }
  }
  particles.reserve(particles.size() + 50 - i);
  for (; i < 50; ++i) {
    particles.push_back(PinguParticle(x, y));
  }
}
void PinguParticleHolder::update () {
  for (std::vector<PinguParticle>::iterator it=particles.begin(); it != particles.end(); ++it) {
    if (!it->livetime) {
      continue;
    }
    float tmp_x_add = 0.0f;
    float tmp_y_add = 0.0f;
    it->velocity.y += WorldObj::get_world()->get_gravity();
    if (it->velocity.y > 0) {
      for (tmp_y_add = it->velocity.y; tmp_y_add >= 1.0; --tmp_y_add) {
        if (world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y))) {
          it->velocity.y *= -y_collision_decrease;
          tmp_y_add = -tmp_y_add;
          --it->pos.y;
          break;
        }
        ++it->pos.y;
      }
      it->pos.y += tmp_y_add;
    } else {
      for (tmp_y_add = it->velocity.y; tmp_y_add <= -1.0; ++tmp_y_add) {
        if (world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y))) {
          it->velocity.y *= -y_collision_decrease;
          tmp_y_add = -tmp_y_add;
          ++it->pos.y;
          break;
        }
        --it->pos.y;
      }
      it->pos.y += tmp_y_add;
    }
    if (it->velocity.x > 0) {
      for (tmp_x_add = it->velocity.x; tmp_x_add >= 1.0; --tmp_x_add) {
        if (world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y))) {
          it->velocity.x *= -x_collision_decrease;
          tmp_x_add = -tmp_x_add;
          --it->pos.x;
          break;
        }
        ++it->pos.x;
      }
      it->pos.x += tmp_x_add;
    } else {
      for (tmp_x_add = it->velocity.x; tmp_x_add <= -1.0; ++tmp_x_add) {
        if (world->get_colmap()->getpixel(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y))) {
          it->velocity.x *= -x_collision_decrease;
          tmp_x_add = -tmp_x_add;
          ++it->pos.x;
          break;
        }
        --it->pos.x;
      }
      it->pos.x += tmp_x_add;
    }
    --it->livetime;
  }
}
void PinguParticleHolder::draw (SceneContext& gc) {
  for (std::vector<PinguParticle>::iterator it=particles.begin(); it != particles.end(); ++it) {
    if (!it->livetime) {
      continue;
    }
    gc.color().draw(surface, it->pos);
  }
}
