rm -Rf /opt/pingus/ceu/cmp/cpp/
rm -Rf /opt/pingus/ceu/cmp/ceu/
mkdir -p /opt/pingus/ceu/cmp/cpp/actions/
mkdir -p /opt/pingus/ceu/cmp/ceu/actions/

# CPP

cd /opt/pingus.official/src/pingus/
cp pingu.* pingu_action.* /opt/pingus/ceu/cmp/cpp/
cd actions/
cp basher.* blocker.* bomber.* bridger.* digger.* /opt/pingus/ceu/cmp/cpp/actions
cp drown.* exiter.* faller.* floater.* jumper.* /opt/pingus/ceu/cmp/cpp/actions
cp miner.* splashed.* waiter.* walker.* /opt/pingus/ceu/cmp/cpp/actions

cd /opt/pingus/ceu/cmp/cpp/
ls -R *.cpp actions/*.cpp | xargs cat > ../all.cpp
ls -R *.hpp actions/*.hpp | xargs cat > ../all.hpp

cd /opt/pingus/ceu/cmp/cpp/
sloccount .         # 2087
cd /opt/pingus/ceu/cmp/
sloccount all.cpp   # 1611
wc all.cpp          # 2370

# copy from all.hpp > all.cpp
    #const float& get_x () const { return pos_x; }
    #const float& get_y () const { return pos_y; }
    #int get_xi () const { return static_cast<int>(pos_x); }
    #int get_yi () const { return static_cast<int>(pos_y); }
    #void set_pos (int x, int y) { set_pos(static_cast<float>(x), static_cast<float>(y)); }
    #Vector3f get_velocity () const { return velocity; }
    #PinguAction* get_wall_action () { return wall_action.get(); }
    #PinguAction* get_fall_action () { return fall_action.get(); }

cd /opt/pingus/ceu/cmp/
astyle -A2 all.cpp

    #:g/^$/d
    #:g/^ *\/\//d
    #:g/#include/d
    #:g/#define/d
    # Manual
        # remove /* COMMENTS */
        # remove namespace { }
        # remove #if 0 ... #endif
        # remove move_with_forces()
        # remove ^L
        # join return type/function dcl
        # remove empty initializations
        # join if () {}
        # break line between field init and "{"
        # join strings in one line
        # one-line getter/setter

sloccount all.cpp   # 1031
wc all.cpp          # 1031

    # Removed
        # Pingu/PinguAction::need_catch (always returns false)
        # PinguAction::get_persistent_char (refactored in Ceu)
        # PinguAction::~ (nothing there)
        # PinguAction::get_action (why?)
        # PinguAction::rel_getpixel (same as Pingu::)
        # PinguAction::get_activation_mode (refactored in Ceu)
        # Pingu::apply_force (never used)
        # Pingu::change_allowed (refactored in Ceu)
        # Pingu::catchable (refactored in Ceu)
        # Pingu::set_* (using Vectors)
        # action_time/COUNTDOWN/countdown .*
        # one-line getter/setter

wc all.cpp          # 877

# CEU

cd /opt/pingus/ceu/pingus/
cp pingu.ceu /opt/pingus/ceu/cmp/ceu/
cp actions/* /opt/pingus/ceu/cmp/ceu/actions/

cd /opt/pingus/ceu/cmp/ceu/
ls -R *.ceu actions/*.ceu | xargs cat > ../all.ceu

cd /opt/pingus/ceu/cmp/
wc all.ceu          # 1476

    #:g/^$/d
    #:g/^ *\/\//d
    #:g/#include/d
    #:g/#define/d
    # Manual
        # remove /* COMMENTS */
        # remove #if 0 ... #endif
        # remove ^L
        # remove native blocks
        # remove #ifndef/#endif pairs
        # remove non-init interfaces
        # remove function prototypes
        # join class/with/do
        # join strings in one line
        # join C++ blocks {}
        # adapt CollisionMasks
        # one-line getter/setter

wc all.ceu          # 754

# remove all functions from Ceu (and the same ones from C++)

wc min.cpp          # 709
wc min.ceu          # 558
