Pingus keeps track of a stack of screens (e.g., Menu, World Map, Game Play, 
etc.) and only dispatches events to the one at the top.
The game loop &#91;[X][pingus-gameloop]&#93; of Pingus resides in the 
`ScreenManager` class, method `display` &#91;[X][cpp-screenmanager]&#93;:

```
// screen_manager.hpp/cpp

class ScreenManager
{
    <...>
    std::vector<std::shared_ptr<Screen> > screens;
    void display();
    void update(float delta, <...>);
}

void ScreenManager::display() {
    Uint32 last_ticks = SDL_GetTicks();
    float previous_frame_time;

    while (!screens.empty()) {                  // X1
        Uint32 ticks = SDL_GetTicks();                      // X3
        previous_frame_time  = (ticks - last_ticks)/1000;
        last_ticks = ticks;                                 // X4
        <...>
        update(previous_frame_time, <...>);
    }                                           // X2
}

void ScreenManager::update(float delta, <...>) {
    ScreenPtr last_screen = get_current_screen();
    <...>
    last_screen->update(delta);                 // X5
    last_screen->draw(<...>);                   // X6
    <...>
}
```

Every iteration of the loop (ln. X1-X2) calculates the time elapsed between two 
frames (ln. X3-X4) and dispatches `update` and `draw` to the active screen (ln.  
X5-X6).

The `Bomber` action occurs during game play, in the `GameSession` screen class 
&#91;[X][cpp-gamesession]&#93;.
All game screens extends `GUIScreen` &#91;[X][cpp-guiscreen]&#93;, which 
contains a `gui_manager` holding all sub-components to `update` and `redraw`:

```
// gui_screen.hpp/cpp

class GUIScreen : public Screen
{
    <...>
    std::unique_ptr<GUI::GUIManager> gui_manager;
    virtual void draw(DrawingContext& gc);
    virtual void update (float);
}

void GUIScreen::draw(DrawingContext& gc)
{
    <...>
    gui_manager->draw(gc);
}

void GUIScreen::update(float delta) {
    gui_manager->update(delta);
}
```

The `GUIManager` class &#91;[X][cpp-guimanager]&#93; extends `GroupComponent` 
&#91;[X][cpp-groupcomponent]&#93;, which does the relevant work, holding all 
sub-components and dispatching the `update` and `draw` methods to them:

```
// group_component.hpp/cpp

class GroupComponent : public RectComponent
{
    std::vector<Component*> children;
    <...>
    void add(Component* comp);
    void draw(DrawingContext& gc);
    void update(float delta);
}

GroupComponent::GroupComponent(const Rect& rect_, bool clip_) :
    <...>
    children()
{
}

GroupComponent::~GroupComponent() {
    for(Components::iterator i = children.begin(); i != children.end(); ++i) {
        delete *i;
    }
}

void GroupComponent::add(Component* comp) {
    <...>
    children.push_back(comp);
}

void GroupComponent::draw (DrawingContext& parent_gc)
{
    for(Components::iterator i = children.begin(); i != children.end(); ++i) {
        if ((*i)->is_visible())
            (*i)->draw(drawing_context);
    }
    <...>
}

void GroupComponent::update (float delta) {
    for(Components::iterator i = children.begin(); i != children.end(); ++i) {
        if ((*i)->is_visible())
            (*i)->update(delta);
    }
}
```

Among all `GameSession` sub-components &#91;[X][cpp-gamesession-subs]&#93;, the 
`World` class is yet another container holding the actual concrete and 
recognizable game entities, such as all pingus, terrains, and traps 
&#91;[X][cpp-world-hpp]&#93;:

```
// world.hpp

class World
{
    <...>
    std::vector<WorldObj*> world_obj;       // X1
    PinguHolder* pingus;                    // X2
    void draw (SceneContext& gc);
    void update ();
}

// world_obj.hpp

class WorldObj
{
    <...>
    virtual void draw(SceneContext& gc) = 0;
    virtual void update ();
}

// pingu_holder.hpp

class PinguHolder : public WorldObj
{
    <...>
    std::list<Pingu*> pingus;
    Pingu* create_pingu(const Vector3f& pos, int owner_id);
    void draw (SceneContext& gc);
    void update();
}
```

The `World` holds all `WorldObj` instances into another container `world_obj` 
(ln.  X1).
In particular, `world_obj` holds all `pingus` (ln. X2) in a `PinguHolder` class 
extending `WorldObj` as follows &#91;[X][cpp-world-cpp]&#93;:

```
// world.cpp

World::World(const PingusLevel& plf) :
    <...>
    pingus(new PinguHolder(plf)),           // X3
{
    <...>
    world_obj.push_back(pingus);            // X4
}

World::~World() {
    for (WorldObjIter it = world_obj.begin(); it != world_obj.end(); ++it) {
        delete *it;
    }
}

void World::draw (SceneContext& gc)
{
    <...>
    for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj) {
        (*obj)->draw(gc);
    }
}

void World::update()
{
    <...>
    for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj) {
        (*obj)->update();
    }
}
```

The `PinguHolder` class receives the `update` and `draw` calls from the `World` 
and redirects them to each individual pingu &#91;[X][cpp-pinguholder]&#93;:

```
PinguHolder::PinguHolder(const PingusLevel& plf) :
  <...>
  pingus()
{
}

PinguHolder::~PinguHolder() {
    for(std::vector<Pingu*>::iterator i = pingus.begin(); i != pingus.end(); ++i) {
        delete *i;
    }
}

Pingu* PinguHolder::create_pingu (<...>) {
    <...>
    pingu = <...>;
    pingus.push_back(pingu);
    return pingu;
}

void PinguHolder::draw (SceneContext& gc) {
    <...>
    for(std::list<Pingu*>::iterator pingu=pingus.begin(); pingu!=pingus.end(); ++pingu) {
        <...>
        (*pingu)->draw(gc);
    }
}

void PinguHolder::update() {
    PinguIter pingu = pingus.begin();
    while(pingu != pingus.end()) {
        (*pingu)->update();
        <...>
        if ((*pingu)->get_status() == Pingu::PS_DEAD) {
            pingu = pingus.erase(pingu);
        }
        <...>
        ++pingu;
    }
}
```

Finally, the `Pingu` class &#91;[X][cpp-pingu]&#93; holds the current 
`PinguAction` &#91;[X][cpp-pinguaction]&#93;, which `Bomber` extends, and 
redirects `update` and `draw` invocations to it:

```
// pingu.hpp/cpp

class Pingu
{
    <...>
    std::shared_ptr<PinguAction> action;            // current action
    void draw (SceneContext& gc);
    void update();
}

Pingu::Pingu (int arg_id, const Vector3f& arg_pos, int owner) :
    <...>
    action()
{
    <...>
    action = create_action(ActionName::FALLER);     // initial action
}

void Pingu::update() {
    <...>
    action->update();
}

void Pingu::draw(SceneContext& gc)
{
    <...>
    action->draw(gc);
}

// pingu_action.hpp

class PinguAction
{
    <...>
    virtual void update () = 0;
    virtual void draw (SceneContext& gc) =0;
}
```

[cpp-sprite]: 
https://github.com/Pingus/pingus/blob/v0.7.6/src/engine/display/sprite_impl.cpp
[cpp-sprite-update]: https://github.com/Pingus/pingus/blob/v0.7.6/src/engine/display/sprite_impl.cpp#L111
[cpp-sprite-render]: https://github.com/Pingus/pingus/blob/v0.7.6/src/engine/display/sprite_impl.cpp#L139

[cpp-screenmanager]: https://github.com/Pingus/pingus/blob/v0.7.6/src/engine/screen/screen_manager.cpp#L163
[cpp-gamesession]: https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/screens/game_session.hpp
[cpp-gamesession-subs]: https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/screens/game_session.cpp#L80
[cpp-guiscreen]: https://github.com/Pingus/pingus/blob/v0.7.6/src/engine/screen/gui_screen.hpp

[cpp-guimanager]: https://github.com/Pingus/pingus/blob/v0.7.6/src/engine/gui/gui_manager.hpp
[cpp-groupcomponent]: https://github.com/Pingus/pingus/blob/v0.7.6/src/engine/gui/group_component.cpp
<!--
[cpp-rectcomponent]: https://github.com/Pingus/pingus/blob/v0.7.6/src/engine/gui/rect_component.hpp
[cpp-component-hpp]: https://github.com/Pingus/pingus/blob/v0.7.6/src/engine/gui/component.hpp
-->

[cpp-world-hpp]: https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/world.hpp
[cpp-world-cpp]: https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/world.cpp
[cpp-pinguholder]: https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/pingu_holder.cpp

[cpp-pingu]: https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/pingu.cpp
[cpp-pinguaction]: https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/pingu_action.hpp

<!--
new Server
https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/screens/game_session.cpp#L55

new World
https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/server.cpp#L77

world->draw()
https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/components/playfield.cpp#L66

server->update()
https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/screens/game_session.cpp#L159

world->update()
https://github.com/Pingus/pingus/blob/v0.7.6/src/pingus/server.cpp#L103
-->




