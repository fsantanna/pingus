<head>
    <title>Francisco Sant'Anna</title>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
</head>

# What?

This report documents the process of porting the video game
Pingus [1](http://pingus.seul.org/)
       [2](https://github.com/Pingus/pingus)
from C++ to the programming language
Céu [1](http://ceu-lang.org/)
    [2](https://github.com/fsantanna/ceu).

<img src="pingus-1.png" width="300"/>
<img src="pingus-2.png" width="300"/>

# Why?

In order of importance, these are the motivations to port Pingus to Céu:

1. Advocate the programming model of Céu in the context of video games.
    Céu provides concurrent and deterministic abstractions for applications 
    with a high degree of synchronization such as video games.
2. Expose Céu to a real (somewhat big) code base that was neither specified nor 
    implemented by the designers of language.
    Céu is an academic "toy" language, in the sense that its main motivation is 
    to propose new ideas to the community.
    Even if video games match the purpose of Céu, a real-world project consits 
    of a range of (sometimes conflicting) requirements, forcing us to transpose
    the "academic fences" of papers which only explore and discuss idiomatic
    code.
3. Exercise the interface between Céu and C/C++.
    Céu was designed to integrate seamlessly with C.
    This allowed us to perform a *live porting*, i.e., we incrementally ported 
    code from C++ to Céu without breaking the game for long.
4. Stress-test the implementation of Céu.
    Academic artifacts typically do not go beyond a working prototype.
    We also want it to be a robust and practical language for everyday use.
5. Evaluate the performance of Céu.
    Having C++ as a reference, how does Céu compare in terms of memory usage, 
    code size, and execution time (e.g., FPS rate)?

<!--
- GOALS: stress-test the language
    - robustness
        - silly bugs (untested stuff)
    - programability
        - everyday stuff
            - maths, vectors
    - performance
        - FPS, RAM, ROM
    - false positives (too much?)
        - tight loops
    - C tricks: {}, _XXX, etc
-->

# How?

## Warming Up!

<img src="double-click-opt.gif" align="right" valign="top"/>

Let's consider the case of handling double clicks.

In Pingus, double clicking the *Armageddon* button in under 1 second literally 
all pingus, as illustrated in the figure in the right.

The code in C++ uses the class `ArmageddonButton` which extends `RectComponent` 
with custom rendering and event-handling methods.
Below, we show only the parts related to detect a double click on the button:

```C++
ArmageddonButton::ArmageddonButton(Server* s, int x, int y) :
    RectComponent(Rect(Vector2i(x, y), Size(38, 60))),
    pressed(false);
    <...>
{
    <...>
}

void ArmageddonButton::draw (DrawingContext& gc) {
    <...>
}

void ArmageddonButton::update (float delta) {
    <...>
    if (pressed) {
        press_time += delta;
        if (press_time > 1.0f) {
            press_time = 0;
            pressed = false;
        }
    } else {
        pressed = false;
        press_time = 0;
    }
}

void ArmageddonButton::on_primary_button_click (int x, int y) {
    if (pressed) {
        server->send_armageddon_event();
    } else {
        pressed = true;
    }
}
```

The class uses the variable `pressed` (ln. X) to remember the first click (ln. 
X).
If another click doesn't occur within 1 second, the variable is reset (ln. X).
Otherwise, the class signals the double click to the application (ln. X).
Even though the variable `pressed` is private, unrelated methods, such as 
`draw`, can potentially access it.
Also, the accesses are spread across the class definition in multiple methods.
For instance, the distance between the declaration (ln. X) and the last access 
(ln. X) is over X lines, making the understanding and maintenance of the 
pattern more difficult.

The code in Céu defines the class `ArmageddonButton` as follows:

    class ArmageddonButton with
        var Rect& rect;
    do
        this.rect.left = 38;
        this.rect.top  = 60;
        var RectComponent component = RectComponent.build(&this.rect);
        <...>
        loop do
            await component.on_primary_button_click;
            watching 1s do
                await component.on_primary_button_click;
                break;
            end
        end
        <...>
        emit global:go_armageddon;
    end

Besides methods (not used above), classes in Céu can have an execution body 
that reacts events.
The double click detection is a `loop` (ln. X-Y) that awaits the first click 
(ln. X) and then, watching 1 second (ln. X-Y), awaits the second click.
If the second click occurs within 1 second, we `break` the (ln. X) and signal 
the double click to the application.
Otherwise, the `watching` block as a whole aborts and restarts the loop, 
falling back to the first click await.
The double click detection in Céu doesn't require an extra state variable and 
is self-contained in the `loop` (ln. X-Y), describing the pattern with 
appropriate control-flow mechanisms (e.g., `await` and `watching`).

We believe that some difficulties in implementing control patterns in games is 
not inherent to the domain, but are the result of accidental complexity due to 
the use of bad (or the lack of) programming models to handle events 
concurrently.

In this report, we describe 6 recurrent patterns found in Pingus and discuss 
the differences in their implementations in C++ and Céu:

1. State Machines
    State machines is a form of expressing XXX through data
    - state machines vs await

2. Dispatching Hierarchies
    - class hierarchies/dispatching vs await
    - lexical scope

3. Continuation Passing
    - CPS vs return continue
        - screen trasnitions
        - story screen advancing
        - story screen -> credits screen
        - worse w/o closures

4. Signaling

    - signaling/f pointer vs events
        - option save on click
        - global events double direction
            - key/mouse-but events
                - sendo que but tem que ficar highlight ou nao
        - verificar caso do option_menu
            - configuracao pode ser alterada por fora?
        - worse w/o closures

5. Timers
    - wall-clock time
        - story chars
        - credits up
        - fps

6. Pausing
    - pause
        - alternative is again hierarchies which enable/disable forwarding

## The Synchronous Concurrency Model

## Céu

- control
    = Accidental complexity
- not pure functions

## Idioms

### State Machines

    - state machines vs await

### Hierarchies

    - class hierarchies/dispatching vs await

### Continuations

    - CPS vs return continue
        - screen trasnitions
        - story screen advancing
        - story screen -> credits screen
        - worse w/o closures

### Signaling

    - signaling/f pointer vs events
        - option save on click
        - global events double direction
            - key/mouse-but events
                - sendo que but tem que ficar highlight ou nao
        - verificar caso do option_menu
            - configuracao pode ser alterada por fora?
        - worse w/o closures

### Wall-Clock Time
    - wall-clock time
        - story chars
        - credits up
        - fps

### Pausing
    - pause
        - alternative is again hierarchies which enable/disable forwarding

# Evaluation

- DOC: two main points:
    = WHY
        - state machines
        - C libraries
        - static memory
        - control safety
    = WHY NOT
        - functional, immutability
        - type safety
        - generic code
        - tooling
    = BOUNS: Lua

## The Code Base

Pingus has a code base around 40k lines of code (LoC):

    > cd pingus/src/
    > sloccount .
    ...
    SLOC	Directory	SLOC-by-Language (Sorted)
    18173   pingus          cpp=18173
    10073   engine          cpp=10073
    6532    editor          cpp=6532
    2771    util            cpp=2771
    1138    math            cpp=1138
    679     lisp            cpp=679
    365     win32           ansic=365
    248     macosx          objc=248
    7       top_dir         cpp=7

The code base includes a game engine, utilities, level editor, platform stubs, 
and the game logic.
Most of the porting process centered around the `engine` and `pingus` 
directory, as we did not port the level editor:

SLOC	Directory	SLOC-by-Language (Sorted)
6844    top_dir         cpp=6844
3045    screens         cpp=3045
2354    worldmap        cpp=2354
2118    worldobjs       cpp=2118
1838    actions         cpp=1838
1338    components      cpp=1338
531     particles       cpp=531
64      colliders       cpp=64
41      movers          cpp=41

SLOC	Directory	SLOC-by-Language (Sorted)
4666    display         cpp=4666
3617    input           cpp=3617
621     gui             cpp=621
603     screen          cpp=603
410     sound           cpp=410
109     resource        cpp=109
47      system          cpp=47

Most of

 `engine/`, a level `editor/`



Total Physical Source Lines of Code (SLOC)                = 39,986

The next Mainstream Programming Language, slides for his invited talk at POPL 2006
Video game programming has a
entities have a high degree of interdependence
current abstractions impose a high degree of accidental complexity

More concretely

- BUG: a cada reinicio, o dobro de pingus sao criados
- BUG: is_world=false in sprite.ceu
- BUG: cliques nos pingus
- BUG: bg do pingu counter fixo
- BUG: small map esta atras da agua embaixo do level snow20
- BUG: queda do pingu saindo da entrada esta levemente desalinhado
- BUG: glitch periodico no smallmap relacionado ao RENDER
- BUG: pequeno glitch no smallmap no inicio da fase
- BUG: procurar /// TODO: w/o me.ceu this function becomes @rec
- z-order front entrance snow-tutorial
- TODO: pos_offset em todos os objs em factory
- front entrance atras do pingu
- color.a em surface_background.ceu
- idle laser_exit
- EXIT/LASER_EXIT pega somente 1 de cada vez
    - verificar se o loop nao deveria pegar varios
- cache de sprites ou performance inviavel
- EXITER/LASERKILL in pingu.ceu
- BUG: climber center
- global:world.pingus vai simplificar min.ceu
- bug do WORLD_UPDATE necessario
- pingu! = <...>
- world! = <...>
- countdown action disable from the original??
- walker behind all
- bridger offset
- rename SpritePingus => Sprite
- testar climber, wall-mode-activation
- testar previous action: Climber->Jumper->direction-change/Blocker->Faller->Blocker
- SDL_DT p/ pingus/actions/sprites

# Who?

# When?

# PORTING

```
native/pre do
    ##include "../src/engine/display/sprite.hpp"
end
native @plain _SpriteImpl;

input _SpriteImpl&& SPRITE_IMPL_NEW;
input _SpriteImpl&& SPRITE_IMPL_DELETE;

class SpriteImpl with
    var _SpriteImpl& me;
do
    par/or do
        var _SpriteImpl&& me_ = await SPRITE_IMPL_DELETE
                                until me_ == &&this.me;
    with
        <...>
    end
end

class SpriteImplFactory with
do
    every me_ in SPRITE_IMPL_NEW do
        spawn SpriteImpl with
            this.me = &_XXX_PTR2REF(me_);
        end;
    end
end

var SpriteImplFactory _;
```

# RESULTS

```
## CPP

> cd /tmp/cmp/cpp/
> sloccount .
    2087
> cd /tmp/cmp/
> sloccount all.cpp
    1611
> cd /tmp/cmp/
> wc all.cpp
    2370

## CEU

> wc all.ceu
    1476

## ALL_CLEAR.CPP

- namespaces
- comments
- blank lines
- includes
- one-line empty methods

-
void
Basher::update ()
{
    ...
}

void Basher::update () {
    ...
}
-
if (x)
{
    ...
}
else
{
    ...
}
if (x)
    ...
else
    ...

if (x) {
    ...
} else {
    ...
}

======

- native
- #ifdef FILE
- defines
- interfaces/class with ... do
-
{
    tp C;
}
==>
{ tp C; }
