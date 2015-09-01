Basher::Basher (Pingu* p) :
    PinguAction(p),
    bash_radius("pingus/common/bash_radius_gfx", "pingus/common/bash_radius"),
    basher_c(0),
    first_bash(true)
{
    assert(bash_radius.get_width() % 2 == 0);
    sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/basher/left"));
    sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/basher/right"));
    bash_reach = bash_radius.get_width();
    bash();
}
void Basher::draw (SceneContext& gc) {
    gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
void Basher::update () {
    sprite[pingu->direction].update();
    ++basher_c;
    if (basher_c % 3 == 0) {
        if (!walk_forward()) {
            return;
        }
        if (rel_getpixel(0, -1) == Groundtype::GP_WATER
                || rel_getpixel(0, -1) == Groundtype::GP_LAVA) {
            pingu->set_action(ActionName::DROWN);
        } else if (rel_getpixel(0, -1) != Groundtype::GP_NOTHING) {
            if (rel_getpixel(0, 0) == Groundtype::GP_SOLID
                    || rel_getpixel(0, pingu_height) == Groundtype::GP_SOLID) {
                Sound::PingusSound::play_sound("chink");
                pingu->direction.change();
                pingu->set_action(ActionName::WALKER);
            } else if (first_bash || have_something_to_dig()) {
                first_bash = true;
                if (basher_c % 2 == 0) {
                    bash();
                }
            } else if (static_cast<float>(sprite[pingu->direction].get_current_frame()) // FIXME: Game logic must be separate from Sprite
                       / static_cast<float>(sprite[pingu->direction].get_frame_count()) > 0.6f) {
                pingu->set_action(ActionName::WALKER);
            }
        }
    }
}
void Basher::bash() {
    WorldObj::get_world()->remove(bash_radius,
                                  pingu->get_xi() - bash_radius.get_width() / 2,
                                  pingu->get_yi() - bash_radius.get_height() + 1);
}
bool Basher::walk_forward() {
    int y_inc = 0;
    for (y_inc = 0; y_inc >= -max_steps_down; --y_inc) {
        if (rel_getpixel(0, y_inc - 1) != Groundtype::GP_NOTHING) {
            break;
        }
    }
    if (y_inc < -max_steps_down) {
        pingu->set_action(ActionName::FALLER);
        return false;
    } else {
        pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction),
                       pingu->get_y() - static_cast<float>(y_inc));
    }
    return true;
}
bool Basher::have_something_to_dig() {
    for(int x = 0; x <= bash_reach; ++x) {
        for (int y = min_bash_height; y <= max_bash_height; ++y) {
            if (rel_getpixel(x, y) == Groundtype::GP_GROUND) {
                log_debug("Basher: Found something to dig...");
                return true;
            }
        }
    }
    return false;
}
Blocker::Blocker(Pingu* p) :
    PinguAction(p),
{
    sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() + "/blocker/left");
    sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/blocker/right");
    if (   rel_getpixel(0,-1)  ==  Groundtype::GP_NOTHING
            && rel_getpixel(0, -2) ==  Groundtype::GP_GROUND) {
        pingu->set_y(pingu->get_y() + 1);
    } else if (   rel_getpixel(0,-1) ==  Groundtype::GP_NOTHING
                  && rel_getpixel(0,-2) ==  Groundtype::GP_NOTHING
                  && rel_getpixel(0,-3) ==  Groundtype::GP_GROUND) {
        pingu->set_y(pingu->get_y() + 2);
    }
}
void Blocker::update() {
    if (!standing_on_ground()) {
        pingu->set_action(ActionName::FALLER);
        return;
    } else {
        PinguHolder* pingus = WorldObj::get_world()->get_pingus();
        for(PinguIter i = pingus->begin(); i != pingus->end(); ++i) {
            catch_pingu(*i);
        }
    }
    sprite.update();
}
void Blocker::draw (SceneContext& gc) {
    gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
bool Blocker::standing_on_ground() {
    return (rel_getpixel(0,-1) !=  Groundtype::GP_NOTHING);
}
void Blocker::catch_pingu(Pingu* target) {
    if (target != pingu) { // avoid 'self' catch
        if (target->get_x () > pingu->get_x () - 16
                && target->get_x () < pingu->get_x () + 16
                && target->get_y () > pingu->get_y () - 32
                && target->get_y () < pingu->get_y () + 5
           ) {
            if (target->get_x () > pingu->get_x ()) {
                target->direction.right();
            } else {
                target->direction.left();
            }
        }
    }
}
Bomber::Bomber (Pingu* p) :
    PinguAction(p),
    particle_thrown(false),
    sound_played(false),
    gfx_exploded(false),
    colmap_exploded(false),
    bomber_radius("other/bomber_radius_gfx", "other/bomber_radius"),
    explo_surf(Sprite("pingus/player" + pingu->get_owner_str() + "/explo"))
{
    sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() + "/bomber/left");
    sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/bomber/right");
    WorldObj::get_world()->play_sound("ohno", pingu->get_pos ());
}
void Bomber::draw (SceneContext& gc) {
    if (sprite[pingu->direction].get_current_frame() >= 13 && !gfx_exploded) {
        gc.color().draw (explo_surf, Vector3f(pingu->get_x () - 32, pingu->get_y () - 48));
        gfx_exploded = true;
    }
    gc.color().draw(sprite[pingu->direction], pingu->get_pos ());
}
void Bomber::update () {
    sprite.update ();
    Movers::LinearMover mover(WorldObj::get_world(), pingu->get_pos());
    Vector3f velocity = pingu->get_velocity();
    mover.update(velocity, Colliders::PinguCollider(pingu_height));
    pingu->set_pos(mover.get_pos());
    if (sprite[pingu->direction].get_current_frame() <= 9 && (rel_getpixel(0, -1) == Groundtype::GP_WATER
            || rel_getpixel(0, -1) == Groundtype::GP_LAVA)) {
        pingu->set_action(ActionName::DROWN);
        return;
    }
    if (sprite[pingu->direction].get_current_frame () <= 9 && rel_getpixel(0, -1) != Groundtype::GP_NOTHING
            && velocity.y > deadly_velocity) {
        pingu->set_action(ActionName::SPLASHED);
        return;
    }
    if (sprite[pingu->direction].get_current_frame () > 9 && !sound_played) {
        WorldObj::get_world()->play_sound("plop", pingu->get_pos ());
        sound_played = true;
    }
    if (sprite[pingu->direction].get_current_frame () > 12 && !particle_thrown) {
        particle_thrown = true;
        WorldObj::get_world()->get_pingu_particle_holder()->add_particle(static_cast<int>(pingu->get_x()),
                static_cast<int>(pingu->get_y()) - 5);
    }
    if (sprite[pingu->direction].get_current_frame () >= 13 && !colmap_exploded) {
        colmap_exploded = true;
        WorldObj::get_world()->remove(bomber_radius,
                                      static_cast<int>(static_cast<int>(pingu->get_x()) - (bomber_radius.get_width()/2)),
                                      static_cast<int>(static_cast<int>(pingu->get_y()) - 16 - (bomber_radius.get_width()/2)));
    }
    if (sprite[pingu->direction].is_finished ()) {
        pingu->set_status(Pingu::PS_DEAD);
    }
}
Bridger::Bridger (Pingu* p) :
    PinguAction(p),
    mode(B_BUILDING),
    brick_l("other/brick_left"),
    brick_r("other/brick_right"),
    bricks(MAX_BRICKS),
    block_build(false),
    name(_("Bridger") + std::string(" (" + StringUtil::to_string(bricks) + ")"))
{
    walk_sprite.load (Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/bridger_walk/left"));
    walk_sprite.load (Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/bridger_walk/right"));
    build_sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/bridger/left"));
    build_sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/bridger/right"));
}
void Bridger::draw(SceneContext& gc) {
    int x_offset = 6;
    int y_offset = 4;
    if (bricks == MAX_BRICKS) {
        x_offset = -4;
        y_offset = 0;
    } else if (bricks == MAX_BRICKS - 1) {
        x_offset = 0;
        y_offset = 1;
    } else if (bricks == MAX_BRICKS - 2) {
        x_offset = 3;
        y_offset = 2;
    }
    switch (mode) {
    case B_BUILDING:
        gc.color().draw(build_sprite[pingu->direction], Vector3f(pingu->get_pos().x - static_cast<float>(x_offset * pingu->direction),
                        pingu->get_pos().y + static_cast<float>(y_offset)));
        break;
    case B_WALKING:
        gc.color().draw(walk_sprite[pingu->direction], Vector3f(pingu->get_pos().x - static_cast<float>(x_offset * pingu->direction),
                        pingu->get_pos().y + static_cast<float>(y_offset)));
        break;
    }
}
void Bridger::update() {
    switch (mode) {
    case B_BUILDING:
        update_build ();
        break;
    case B_WALKING:
        update_walk ();
        break;
    }
}
void Bridger::update_walk () {
    if (walk_sprite[pingu->direction].is_finished ()) { // FIXME: Dangerous! might not be fixed timing
        if (way_is_free()) {
            mode = B_BUILDING;
            block_build = false;
            walk_sprite[pingu->direction].restart();
            walk_one_step_up();
        } else { // We reached a wall...
            pingu->direction.change ();
            pingu->set_action (ActionName::WALKER);
            return;
        }
    } else {
        walk_sprite.update ();
    }
}
void Bridger::update_build () {
    build_sprite[pingu->direction].update();
    if (build_sprite[pingu->direction].get_current_frame () >= 7 && !block_build) {
        block_build = true;
        if (bricks > 0) {
            if (brick_placement_allowed()) {
                place_a_brick();
            } else {
                pingu->direction.change ();
                pingu->set_action (ActionName::WALKER);
                return;
            }
        } else { // Out of bricks
            pingu->set_action(ActionName::WAITER);
            return;
        }
    }
    if (build_sprite[pingu->direction].is_finished ()) {
        mode = B_WALKING;
        build_sprite[pingu->direction].restart();
    }
}
bool Bridger::way_is_free() {
    bool way_free = true;
    for (int x_inc = 1; x_inc <= 4; x_inc++) {
        if (rel_getpixel(x_inc, 2) != Groundtype::GP_NOTHING
                || head_collision_on_walk(x_inc, 2)) {
            way_free = false;
            break;
        }
    }
    return way_free;
}
bool Bridger::brick_placement_allowed(void) {
    return !head_collision_on_walk(0, 2);
}
void Bridger::place_a_brick() {
    bricks--;
    name = _("Bridger") + std::string(" (") + StringUtil::to_string(bricks) + ")";
    if (bricks < 4) {
        Sound::PingusSound::play_sound("ting");
    }
    if (pingu->direction.is_right()) {
        WorldObj::get_world()->put(brick_r,
                                   static_cast<int>(pingu->get_pos().x + 10.0f - static_cast<float>(brick_r.get_width())),
                                   static_cast<int>(pingu->get_pos().y),
                                   Groundtype::GP_BRIDGE);
    } else {
        WorldObj::get_world()->put(brick_l,
                                   static_cast<int>(pingu->get_pos().x - 10.0f),
                                   static_cast<int>(pingu->get_pos().y),
                                   Groundtype::GP_BRIDGE);
    }
}
void Bridger::walk_one_step_up() {
    pingu->set_pos(pingu->get_pos().x + (4.0f * static_cast<float>(pingu->direction)),
                   pingu->get_pos().y - 2);
}
Digger::Digger(Pingu* p) :
    PinguAction(p),
    digger_radius("pingus/common/digger_radius_gfx", "pingus/common/digger_radius"),
    digger_radius_final("pingus/common/digger_radius_final_gfx", "pingus/common/digger_radius_final_gfx"),
    delay_count(0)
{
    sprite = Sprite("pingus/player" + pingu->get_owner_str() + "/digger/left");
}
void Digger::update() {
    sprite.update();
    delay_count += 1;
    if (rel_getpixel(0, -1) ==  Groundtype::GP_WATER
            || rel_getpixel(0, -1) ==  Groundtype::GP_LAVA) {
        pingu->set_action(ActionName::DROWN);
    } else {
        if (!have_something_to_dig()) {
            dig(true);
            pingu->set_action(ActionName::WALKER);
        } else if (delay_count % 4 == 0) {
            dig(false);
        }
    }
}
bool Digger::have_something_to_dig() {
    if (rel_getpixel(0, -1) !=  Groundtype::GP_NOTHING) {
        if (rel_getpixel(0, -1) ==  Groundtype::GP_SOLID) {
            Sound::PingusSound::play_sound("chink");
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}
void Digger::dig(bool final) {
    if (!final) {
        WorldObj::get_world()->remove(digger_radius,
                                      pingu->get_xi() - digger_radius.get_width() / 2,
                                      pingu->get_yi() - digger_radius.get_height() + 2);
    } else {
        WorldObj::get_world()->remove(digger_radius_final,
                                      pingu->get_xi() - digger_radius.get_width() / 2,
                                      pingu->get_yi() - digger_radius.get_height() + 2);
    }
    pingu->set_pos(pingu->get_xi(), pingu->get_yi() + 1);
}
void Digger::draw(SceneContext& gc) {
    gc.color().draw(sprite, pingu->get_pos());
}
Drown::Drown (Pingu* p) :
    PinguAction(p)
{
    sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/drownfall/left"));
    sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/drownfall/right"));
}
void Drown::draw (SceneContext& gc) {
    gc.color().draw(sprite[pingu->direction], pingu->get_pos ());
}
void Drown::update () {
    sprite[pingu->direction].update();
    if (sprite[pingu->direction].is_finished()) {
        pingu->set_status(Pingu::PS_DEAD);
    }
}
Exiter::Exiter (Pingu* p) :
    PinguAction(p),
    sound_played(false)
{
    sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/exit/left"));
    sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/exit/right"));
}
void Exiter::update () {
    sprite[pingu->direction].update();
    if (!sound_played) {
        sound_played = true;
        Sound::PingusSound::play_sound("yipee");
    }
    if (sprite[pingu->direction].is_finished()) {
        if (pingu->get_status() != Pingu::PS_EXITED) {
            pingu->set_status(Pingu::PS_EXITED);
        }
    }
}
void Exiter::draw (SceneContext& gc) {
    gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
Faller::Faller (Pingu* p) :
    PinguAction(p)
{
    faller.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/faller/left"));
    faller.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/faller/right"));
    tumbler.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/tumbler/left"));
    tumbler.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/tumbler/right"));
}
Faller::~Faller () { }
void Faller::update () {
    if (is_tumbling()) {
        tumbler[pingu->direction].update();
    } else {
        faller[pingu->direction].update();
    }
    if (pingu->get_velocity().y > 5.0 && pingu->request_fall_action()) {
        return;
    }
    pingu->set_velocity(pingu->get_velocity()
                        + Vector3f(0.0f, WorldObj::get_world()->get_gravity()) );
    Vector3f velocity = pingu->get_velocity();
    Vector3f move = velocity;
    bool collided;
    Movers::LinearMover mover(WorldObj::get_world(), pingu->get_pos());
    mover.update(move, Colliders::PinguCollider(pingu_height));
    pingu->set_pos(mover.get_pos());
    collided = mover.collided();
    if (collided) {
        if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING &&
                rel_getpixel(0, -2) == Groundtype::GP_NOTHING &&
                rel_getpixel(0, -3) == Groundtype::GP_NOTHING &&
                rel_getpixel(1, 0)  != Groundtype::GP_NOTHING &&
                rel_getpixel(1, 0)  != Groundtype::GP_BRIDGE) {
            velocity.x = -(velocity.x / 3.0f);
            if (velocity.x > 0.0f) {
                pingu->direction.right();
            } else {
                pingu->direction.left();
            }
            pingu->set_velocity(velocity);
        } else if (velocity.y > 0.0f) { // If the Pingu collided into something while moving down...
            if (rel_getpixel(0, -1) == Groundtype::GP_WATER
                    || rel_getpixel(0, -1) == Groundtype::GP_LAVA) {
                pingu->set_action(ActionName::DROWN);
            }
            else if (Math::abs(pingu->get_velocity().y) > deadly_velocity) {
                pingu->set_action(ActionName::SPLASHED);
            } else {
                if (pingu->get_previous_action() == ActionName::BLOCKER) {
                    pingu->set_action(pingu->get_previous_action());
                } else {
                    pingu->set_action(ActionName::WALKER);
                }
            }
        }
        else { // if (velocity.y < 0.0f)
            velocity.y = 0;
            velocity.x = (velocity.x / 3.0f);
            pingu->set_velocity(velocity);
        }
    }
}
void Faller::draw (SceneContext& gc) {
    if (is_tumbling()) {
        gc.color().draw(tumbler[pingu->direction], pingu->get_pos ());
    } else {
        gc.color().draw(faller[pingu->direction], pingu->get_pos ());
    }
}
bool Faller::is_tumbling () const {
    return (Math::abs(pingu->get_velocity().x) > deadly_velocity
            || Math::abs(pingu->get_velocity().y) > deadly_velocity);
}
Floater::Floater(Pingu* p) :
    PinguAction(p),
    falling_depth(0),
    step(0),
{
    sprite = Sprite("pingus/player" + pingu->get_owner_str() + "/floater/left");
}
void Floater::update() {
    sprite.update ();
    pingu->set_velocity(Vector3f(0.0f, 1.0f));
    if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING) {
        ++step;
        if (step > 0) {
            pingu->set_y(pingu->get_y() + 1);
            step = 0;
        }
    } else {
        pingu->set_action (ActionName::WALKER);
    }
}
void Floater::draw (SceneContext& gc) {
    gc.color().draw(sprite, pingu->get_pos());
}
Jumper::Jumper (Pingu* p) :
    PinguAction(p),
{
    sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/jumper/left"));
    sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/jumper/right"));
}
void Jumper::draw (SceneContext& gc) {
    gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
void Jumper::update () {
    if ((pingu->get_previous_action() == ActionName::CLIMBER)) {
        pingu->direction.change();
    }
    if (pingu->direction.is_left()) {
        pingu->set_velocity(pingu->get_velocity() + Vector3f(-4.58f, -4.58f));
    } else { // if (pingu->direction.is_right())
        pingu->set_velocity(pingu->get_velocity() + Vector3f(4.58f, -4.58f));
    }
    pingu->set_y(pingu->get_y() - 1);
    pingu->set_action (ActionName::FALLER);
}
Miner::Miner (Pingu* p) :
    PinguAction(p),
    miner_radius("pingus/common/miner_radius_gfx", "pingus/common/miner_radius"),
    miner_radius_left("pingus/common/miner_radius_left_gfx", "pingus/common/miner_radius_left"),
    miner_radius_right("pingus/common/miner_radius_right_gfx", "pingus/common/miner_radius_right"),
    delay_count(0)
{
    sprite.load(Direction::LEFT,  Sprite("pingus/player" + pingu->get_owner_str() + "/miner/left"));
    sprite.load(Direction::RIGHT, Sprite("pingus/player" + pingu->get_owner_str() + "/miner/right"));
}
void Miner::update () {
    sprite[pingu->direction].update();
    delay_count += 1;
    if (delay_count % 4 == 0) {
        if (rel_getpixel(0, -1) == Groundtype::GP_NOTHING) {
            mine(true);
            pingu->set_action(ActionName::WALKER);
        } else if (rel_getpixel(0, -1) == Groundtype::GP_SOLID ||
                   rel_getpixel(0, pingu_height) == Groundtype::GP_SOLID) {
            if (rel_getpixel(0, -1) == Groundtype::GP_SOLID) {
                Sound::PingusSound::play_sound("chink");
            }
            mine(true);
            pingu->direction.change();
            pingu->set_action(ActionName::WALKER);
        } else {
            mine(false);
            pingu->set_pos(pingu->get_xi() + pingu->direction,
                           pingu->get_yi() + 1);
        }
    }
}
void Miner::mine(bool final) {
    if (!final) {
        if (delay_count % 2 == 0) {
            WorldObj::get_world()->remove(miner_radius,
                                          pingu->get_xi() - (miner_radius.get_width() / 2) + pingu->direction,
                                          pingu->get_yi() - miner_radius.get_height() + 2);
        }
    } else {
        CollisionMask& radius = (pingu->direction == Direction::LEFT) ? miner_radius_left : miner_radius_right;
        WorldObj::get_world()->remove(radius,
                                      pingu->get_xi() - (radius.get_width() / 2) + pingu->direction,
                                      pingu->get_yi() - radius.get_height() + 2);
    }
}
void Miner::draw (SceneContext& gc) {
    gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}
Splashed::Splashed (Pingu* p) :
    PinguAction(p),
    particle_thrown(false),
    sound_played(false),
{
    sprite = Sprite("pingus/player" + pingu->get_owner_str() + "/splat");
}
void Splashed::update () {
    sprite.update();
    if (!particle_thrown) {
        particle_thrown = true;
        WorldObj::get_world()->play_sound("splash", pingu->get_pos());
    }
    if (sprite.is_finished()) {
        pingu->set_status(Pingu::PS_DEAD);
    }
}
void Splashed::draw (SceneContext& gc) {
    gc.color().draw(sprite, pingu->get_pos ());
}
Waiter::Waiter (Pingu* p) :
    PinguAction(p),
    countdown(2.0f),
{
    sprite = Sprite("pingus/player" + pingu->get_owner_str() + "/waiter/left");
}
void Waiter::update () {
    sprite.update();
    if (countdown < 0) {
        pingu->set_action(ActionName::WALKER);
        return;
    }
    countdown -= 0.025f;
}
void Waiter::draw (SceneContext& gc) {
    gc.color().draw(sprite, pingu->get_pos ());
}
Walker::Walker (Pingu* p) :
    PinguAction(p),
{
    walker.load(Direction::LEFT, "pingus/player" + pingu->get_owner_str() + "/walker/left");
    walker.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/walker/right");
    floaterlayer.load(Direction::LEFT,  "other/floaterlayer/left");
    floaterlayer.load(Direction::RIGHT, "other/floaterlayer/right");
    pingu->set_velocity(Vector3f());
}
void Walker::update () {
    walker[pingu->direction].update(0.033f);
    floaterlayer[pingu->direction].update(0.033f);
    Vector3f last_pos = pingu->get_pos();
    if (rel_getpixel(1, 0) == Groundtype::GP_OUTOFSCREEN) {
        pingu->set_x(pingu->get_x() + static_cast<float>(pingu->direction));
        return;
    }
    if (rel_getpixel(0, -1) ==  Groundtype::GP_WATER) {
        pingu->set_action(ActionName::DROWN);
        return;
    }
    if (rel_getpixel(0, -1) ==  Groundtype::GP_NOTHING) {
        bool found_ground = false;
        int i;
        for (i = -2; i > -5; --i) {
            if (!(rel_getpixel(0, i) == Groundtype::GP_NOTHING)) {
                found_ground = true;
                break;
            }
        }
        if (found_ground) {
            pingu->set_y(pingu->get_y() - static_cast<float>(i));
        } else {
            pingu->set_action(ActionName::FALLER);
            return;
        }
    }
    if (rel_getpixel(1, 0) == Groundtype::GP_BRIDGE
            && !head_collision_on_walk(1, 1)) { // bridge
        pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction),
                       pingu->get_y() - 1.0f); // pingus 'float' through bridges
    } else {
        int y_inc = 0;
        int possible_y_step = 0;
        bool found_next_step = false;
        for (y_inc = -max_steps; y_inc <= max_steps; ++y_inc) {
            if ((  rel_getpixel(1, y_inc)     == Groundtype::GP_NOTHING
                    || rel_getpixel(1, y_inc)     == Groundtype::GP_BRIDGE) // FIXME: This causes a rather huge step
                    && rel_getpixel(1, y_inc - 1) != Groundtype::GP_NOTHING) {
                found_next_step = true;
                possible_y_step = y_inc;
            }
        }
        if (found_next_step) {
            pingu->set_pos(pingu->get_x() + static_cast<float>(pingu->direction),
                           pingu->get_y() - static_cast<float>(possible_y_step));
        } else {
            if (rel_getpixel(1, 0) !=  Groundtype::GP_NOTHING) {
                if (pingu->request_wall_action()) {
                    log_debug("Pingu: We are in front of a wall, setting persistant action");
                    return;
                }
                pingu->direction.change();
            } else {
                pingu->set_x(pingu->get_x() + static_cast<float>(pingu->direction));
                pingu->set_action(ActionName::FALLER);
                return;
            }
        }
    }
    if (head_collision_on_walk(0, 0)) {
        log_debug("Pingu: Head collision");
        pingu->direction.change();
        pingu->set_pos(last_pos);
        return;
    }
}
void Walker::draw (SceneContext& gc) {
    gc.color().draw(walker[pingu->direction], pingu->get_pos());
    if (pingu->get_fall_action() && pingu->get_fall_action()->get_type() == ActionName::FLOATER) {
        gc.color().draw(floaterlayer[pingu->direction], pingu->get_pos());
    }
}
PinguAction::PinguAction (Pingu* p)
    : pingu (p)
{ }
Vector3f PinguAction::get_center_pos() const {
    return pingu->get_pos() + Vector3f(0, -16);
}
bool PinguAction::head_collision_on_walk (int x, int y) {
    int pixel = rel_getpixel(x, y + pingu_height);
    if (pixel != Groundtype::GP_NOTHING && !(pixel & Groundtype::GP_BRIDGE)) {
        return true;
    }
    return false;
}
bool PinguAction::collision_on_walk (int x, int y) {
    bool collision = false;
    int pixel = Groundtype::GP_NOTHING;
    for (int pingu_y = 0; pingu_y <= pingu_height; ++pingu_y) {
        pixel = rel_getpixel(x, y + pingu_y);
        if (pixel != Groundtype::GP_NOTHING && pixel != Groundtype::GP_BRIDGE) {
            collision = true;
            break;
        }
    }
    return collision;
}
Pingu::Pingu (int arg_id, const Vector3f& arg_pos, int owner) :
    previous_action(ActionName::FALLER),
    id(arg_id),
    owner_id(owner),
    status(PS_ALIVE),
    pos_x(arg_pos.x),
    pos_y(arg_pos.y),
    velocity(0, 0, 0),
{
    direction.left ();
    action = create_action(ActionName::FALLER);
}
void Pingu::set_velocity (const Vector3f& velocity_) {
    velocity = velocity_;
    velocity.x = Math::clamp(-terminal_velocity, velocity.x, terminal_velocity);
    velocity.y = Math::clamp(-terminal_velocity, velocity.y, terminal_velocity);
}
bool Pingu::request_set_action(ActionName::Enum action_name) {
    bool ret_val = false;
    if (status == PS_DEAD) {
        log_debug("Setting action to a dead pingu");
        ret_val =  false;
    } else {
        switch (PinguAction::get_activation_mode(action_name)) {
        case INSTANT:
            if (action_name == action->get_type()) {
                log_debug("Pingu: Already have action");
                ret_val = false;
            } else if (action->change_allowed(action_name)) {
                log_debug("setting instant action");
                set_action(action_name);
                ret_val = true;
            } else {
                log_debug("change from action %1% not allowed", action->get_name());
                ret_val = false;
            }
            break;
        case WALL_TRIGGERED:
            if (wall_action && wall_action->get_type() == action_name) {
                log_debug("Not using wall action, we have already");
                ret_val = false;
            } else {
                log_debug("Setting wall action");
                wall_action = create_action(action_name);
                ret_val = true;
            }
            break;
        case FALL_TRIGGERED:
            if (fall_action && fall_action->get_type() == action_name) {
                log_debug("Not using fall action, we have already");
                ret_val = false;
            } else {
                log_debug("Setting fall action");
                fall_action = create_action(action_name);
                ret_val = true;
            }
            break;
        default:
            log_debug("unknown action activation_mode");
            ret_val = false;
            assert(0);
            break;
        }
    }
    return ret_val;
}
void Pingu::set_action (ActionName::Enum action_name) {
    set_action(create_action(action_name));
}
void Pingu::set_action(std::shared_ptr<PinguAction> act) {
    assert(act);
    previous_action = action->get_type();
    action = act;
}
bool Pingu::request_fall_action () {
    if (fall_action) {
        set_action(fall_action);
        return true;
    }
    return false;
}
bool Pingu::request_wall_action () {
    if (wall_action) {
        set_action(wall_action);
        return true;
    }
    return false;
}
bool Pingu::is_over (int x, int y) {
    Vector3f center = get_center_pos ();
    return (center.x + 16 > x && center.x - 16 < x &&
            center.y + 16 > y && center.y - 16 < y);
}
bool Pingu::is_inside (int x1, int y1, int x2, int y2) {
    assert (x1 < x2);
    assert (y1 < y2);
    return (pos_x > x1 && pos_x < x2
            &&
            pos_y > y1 && pos_y < y2);
}
float Pingu::dist(int x, int y) {
    Vector3f p = get_center_pos ();
    return Math::sqrt(((p.x - static_cast<float>(x)) * (p.x - static_cast<float>(x)) +
                       (p.y - static_cast<float>(y)) * (p.y - static_cast<float>(y))));
}
void Pingu::update() {
    if (status == PS_DEAD) {
        return;
    }
    if (rel_getpixel(0, -1) == Groundtype::GP_OUTOFSCREEN) {
        status = PS_DEAD;
        return;
    }
    action->update();
}
void Pingu::draw(SceneContext& gc) {
    char str[16];
    action->draw(gc);
}
int Pingu::rel_getpixel(int x, int y) {
    return WorldObj::get_world()->get_colmap()->getpixel(static_cast<int>(pos_x + static_cast<float>(x * direction)),
            static_cast<int>(pos_y - static_cast<float>(y)));
}
void Pingu::catch_pingu (Pingu* pingu) {
    action->catch_pingu(pingu);
}
bool Pingu::is_alive (void) {
    return (status != PS_DEAD && status != PS_EXITED);
}
std::shared_ptr<PinguAction> Pingu::create_action(ActionName::Enum action_) {
    switch(action_) {
    case ActionName::ANGEL:
        return std::make_shared<Angel>(this);
    case ActionName::BASHER:
        return std::make_shared<Basher>(this);
    case ActionName::BLOCKER:
        return std::make_shared<Blocker>(this);
    case ActionName::BOARDER:
        return std::make_shared<Boarder>(this);
    case ActionName::BOMBER:
        return std::make_shared<Bomber>(this);
    case ActionName::BRIDGER:
        return std::make_shared<Bridger>(this);
    case ActionName::CLIMBER:
        return std::make_shared<Climber>(this);
    case ActionName::DIGGER:
        return std::make_shared<Digger>(this);
    case ActionName::DROWN:
        return std::make_shared<Drown>(this);
    case ActionName::EXITER:
        return std::make_shared<Exiter>(this);
    case ActionName::FALLER:
        return std::make_shared<Faller>(this);
    case ActionName::FLOATER:
        return std::make_shared<Floater>(this);
    case ActionName::JUMPER:
        return std::make_shared<Jumper>(this);
    case ActionName::LASERKILL:
        return std::make_shared<LaserKill>(this);
    case ActionName::MINER:
        return std::make_shared<Miner>(this);
    case ActionName::SLIDER:
        return std::make_shared<Slider>(this);
    case ActionName::SMASHED:
        return std::make_shared<Smashed>(this);
    case ActionName::SPLASHED:
        return std::make_shared<Splashed>(this);
    case ActionName::SUPERMAN:
        return std::make_shared<Superman>(this);
    case ActionName::WAITER:
        return std::make_shared<Waiter>(this);
    case ActionName::WALKER:
        return std::make_shared<Walker>(this);
    default:
        assert(!"Invalid action name provied");
    }
}
