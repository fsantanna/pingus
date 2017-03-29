<head>
<title>Structured Synchronous Reactive Programming for Game Development ---
       Case Study: On Rewriting Pingus from C++ to Céu</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<style>
    body {
        width:950px;
        margin:0 auto;
        text-align: justify;
        text-justify: inter-word;
    }
    pre {
        padding: 10px 10px 10px 10px;
        background-color: #E5E4E2;
    }
    code {
        background-color: #E5E4E2;
    }
    div.images {
        float: right;
        clear: right;
        background-color: #ffffff;
        border: 1px solid black;
        padding: 10px;
        margin: 0 0 5px 10px;
/*
        width: 120px;
*/
        text-align: center;
    }
    div.box {
        float: right;
        background-color: #ffffff;
        border: 1px solid black;
        padding: 10px;
        margin: 0 0 5px 10px;
        display: inline-block;
        width: 400px;
    }
    div.summary {
        background-color: #FFCCCC;
        border: 1px solid black;
        padding: 5px;
    }
</style>
</head>
<body>

@[[
FIG_COUNT = 0

function FIG_NEW (file, title, width)
    assert(file and _G[file] == nil, 'repeated Figure: "'..tostring(file)..'"')
    width = (width and ('width="'..width..'"')) or ''
    FIG_COUNT = FIG_COUNT + 1
    _G[file] = FIG_COUNT
    return [=[
<div class="images">
<img src="images/]=]..file..[=[" ]=]..width..[=[/>
<br>
<a href="images/]=]..file..[=[">
Figure ]=]..FIG_COUNT..[=[</a>: ]=]..title..[=[
</div>
]=]
end

function FIG_REF (file)
    return 'Figure '..assert(_G[file], 'figure not found: '..file)
end

CODE2N = nil

local KEYS = {
    CPP = {
        'class', 'else', 'return', 'if', 'while', 'case',
        'bool', 'this', 'false', 'true', 'void', 'int',
        'new', 'switch', 'typedef', 'for', 'break', 'float',
    },
    CEU = {
        'code', 'else', 'return', 'if', 'data', 'as',
        'bool', 'outer', 'false', 'true', 'void', 'int',
        'do', 'end', 'loop', 'escape', 'then', 'with', 'watching',
        'await', 'emit',
        'par', 'par/or', 'par/and', 'call',
        'var', 'event', 'in', 'or',
        'and', 'until', 'pool',
        'every', 'spawn', 'break', 'not',
    },
}

function CODE_LINES (code)
    local sep = lpeg.P('\n')
    local elem = lpeg.C((1 - sep)^0)
    local p = lpeg.Ct(elem * (sep * elem)^0)
    local t = lpeg.match(p, code)

    local OPTS = {}
    for opt,v in string.gmatch(t[1], '(%a+)=(%a+),?') do
        OPTS[opt] = v
    end

    assert(t[#t] == '')
    table.remove(t, 1)
    table.remove(t, #t)

    if OPTS.reset ~= 'false' then
        CODE2N = {}
    end

    for i, line in ipairs(t) do
        local id = string.match(line, '@(.*)$')
        if id then
            assert(not CODE2N[id], 'repeated @id: "'..id..'"')
            CODE2N[id] = i
            line = string.gsub(line, ' +@.*$', '')
            t[i] = line
        end
        if OPTS.lines ~= 'false' then
            t[i] = string.format('<b><font size="-1" color="#666666">%2s',i)..
                                 ':</font></b>  '..line
        end
    end
    local ret = table.concat(t,'\n')

    if OPTS.language then
        for _, key in ipairs(assert(KEYS[OPTS.language])) do
            ret = string.gsub(ret, '([%s%p]+)('..key..')([%s%p])', '%1<b>%2</b>%3')
            ret = string.gsub(ret, '([%s%p]+)('..key..')$',    '%1<b>%2</b>')
            --ret = string.gsub(ret, '([%s%p]+)('..key..'%p+)',  '%1<b>%2</b>')
        end
    end

    return '<pre><code>'..ret..'</code></pre>'
end

function N (id)
    return assert(CODE2N[id], 'not found: @'..id)
end

function NN (id1, sep, id2)
    sep = (sep=='' and ',') or sep
    if sep then
        return '(ln. '..N(id1)..sep..N(id2)..')'
    else
        return '(ln. '..N(id1)..')'
    end
end


SECS = {}

function SEC (str)
    local id, ln, hs, tit = string.match(str, '([^\n]*),(\n*)(#*) (.*)')
    local n = string.len(hs) - 1
    if n > #SECS then
        SECS[#SECS+1] = 0
    elseif n == #SECS then
    else
        while n < #SECS do
            SECS[#SECS] = nil
        end
    end
    SECS[#SECS] = SECS[#SECS] + 1
    local num = table.concat(SECS,'.')
    SECS[id] = num
    return '<a name="'..id..'"/>\n\n'..
            ln..hs..' '..num..') '..tit
end

function SEC_REF (id)
    return '<a href="#'..id..'"> Section '..assert(SECS[id], 'section not found: '..id)..'</a>'
end

]]

<!--
silentcast, transparent window interior, dont go under the default size
convert credits-anim.gif -fuzz 10% -layers Optimize optimised.gif
convert -delay 200 -loop 0 *.png state-anim.gif
:%s/\(\[X]\[[^]]*\]\)/\&#91;\1\&#93;/g
chico@note:/opt/pingus/ceu$ lua md-macros.lua README.md > /tmp/README.md
chico@note:/opt/pingus/ceu$ pandoc /tmp/README.md >README.html 
key-mon --noshift --noalt

* TODO
    * events before signalling
    - 2 casos p/ cada pattern
        - pause: pause/if e o nao gerar evento
        - hier: adicionar resize

TODO:
    - TARGET AUDIENCE
        - game developers that have faced these problems before

TODO:
    - ver os patterns do GPP
        - nao cubro algum que ocorre no pingus?
        - cubro algum com nome diferente?
        - cubro algum que o GPP nao cobre?
    - hierarchy
        - exemplo de z-order lexico em vez de dinamico

TODO:
    - only control flow advances
        - no type inference
        - no GC
        - no functional compositions
        - no data abstraction
        - no rearrange of overall arch

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

- TODO: threads, lua

- compare to actors
    - parallel, synchronous-run-to-completion
    - not a loop+queue
-->

[X]: images/link_12.png

# Structured Synchronous Reactive Programming for Game Development --- Case Study: On Rewriting Pingus from C++ to Céu

<!--
* [What](#what-is-this-all-about),
  [Why](#why-rewrite-pingus-to-céu),
  [How](#how-to-rewrite),
  [Who](#who)?
* Analysis: [Qualitative](#qualitative-analysis),
            [Quantitative](#quantitative-analysis)

* [TLDR!](#tldr!)
    - did you do a complete port?
    - why is this cool?
    - ok, but with which gains in productivity?
    - what about CPU,ROM,RAM?
-->

## Introduction

This report documents the process of rewriting the video game
Pingus [[![X]][pingus-1]] from C++ to Céu [[![X]][ceu_1],[![X]][ceu_2]].

[pingus-1]: http://pingus.seul.org/
[ceu_1]:    http://ceu-lang.org/
[ceu_2]:    https://github.com/fsantanna/ceu/

<!--
<img src="images/pingus-1.png" align="right" width="400"/>
<img src="images/pingus-2.png" align="right" width="400"/>
-->

@FIG_NEW(pingus-2.png,
         Pingus gameplay,
         350)

Pingus is an open-source [[![X]][pingus-2]] clone of
Lemmings [[![X]][lemmings]], a puzzle-platformer video game.      
The objective of the game is to guide a group of penguins through a number of
obstacles towards a designated exit [[![X]][pingus-3]].

[pingus-2]: https://github.com/Pingus/pingus/
[pingus-3]: https://www.youtube.com/watch?v=MKrJgIFtJX0
[lemmings]: https://en.wikipedia.org/wiki/Lemmings_(video_game)  

Pingus is developed in object-oriented C++, the *lingua franca* of game
development [[![X]][cpp_1]].
The codebase is about 40.000 lines of code [[![X]][git-1]], divided into the
engine, level editor, auxiliary libraries, and the game logic itself.

[cpp_1]: http://gameprogrammingpatterns.com/introduction.html#about-the-sample-code
<!--
    I chose C++ for a couple of reasons. First, it’s the most popular language
    for commercially shipped games.
    It is the lingua franca of the industry.
    Moreso, the C syntax that C++ is based on is also the basis for Java, C#,
    JavaScript, and many other languages.
    Even if you don’t know C++, the odds are good you can understand the code
    samples here with a little bit of effort.
-->
[git-1]: https://github.com/Pingus/pingus/commit/7b255840c201d028fd6b19a2185ccf7df3a2cd6e

<!-- TODO: codebases for other open source games -->

<!--
 (the `pingus/` directory):
```
$ sloccount pingus/
SLOC	Directory	SLOC-by-Language (Sorted)
18173   pingus/         cpp=18173
10062   engine/         cpp=10062
6532    editor/         cpp=6532
2771    util/           cpp=2771
1138    math/           cpp=1138
679     lisp/           cpp=679
365     win32/          ansic=365
248     macosx/         objc=248
7       ./              cpp=7

Totals grouped by language (dominant language first):
cpp:          39362 (98.47%)
ansic:          365 (0.91%)
objc:           248 (0.62%)

Total Physical Source Lines of Code (SLOC) = 39,975
```
-->

Céu is a programming language that aims to offer a higher-level and safer
alternative to C/C++ with the characteristics that follow:

- *Reactive:* code only executes in reactions to events.
- *Structured:* programs use structured control mechanisms, such as `await` (to
  suspend a line of execution), and `par` (to combine multiple lines of
  execution).
- *Synchronous:* reactions run atomically and to completion on each line of
  execution, i.e., there's no implicit preemption or real parallelism.

Structured programming eliminates the *callback hell* [[![X]][callback-hell]],
letting programmers write code in direct/sequential style in multiple lines of
execution.
In addition, when a line of execution is aborted, all allocated resources
inside it are safely released.

[callback-hell]: http://callbackhell.com/

@FIG_NEW(sweeney.png,
         Three "kinds" of code,
         350)

According to Tim Sweeney (of Unreal Engine fame) [[![X]][sweeney]], about half
the complexity in game development resides in *simulation*
(aka *game logic*), but which accounts for only 10% of the CPU budget.
<!-- the way entities interact in real time -->
The high development costs contrasting with the low impact on performance
appeals for alternatives with productivity in mind, especially considering that
it is the game logic that varies the most between projects.

[sweeney]: https://www.cs.princeton.edu/~dpw/popl/06/Tim-POPL.ppt

<!--
When updating 10,000 objects at 60 FPS, everything is performance-sensitive
But:
Productivity is just as important
Will gladly sacrifice 10% of our performance
for 10% higher productivity
We never use assembly language

Gameplay Simulation
Gratuitous use of mutable state
10,000’s of objects must be updated
Typical object update touches 5-10 other objects

This is the hardest problem…
10,00’s of objects
Each one contains mutable state
Each one updated 30 times per second
Each update touches 5-10 other objects
 
Manual synchronization (shared state concurrency) is 
hopelessly intractible here.
 
Solutions?
Rewrite as referentially-transparent functions?
Message-passing concurrency?
Continue using the sequential, single-threaded approach?

Update all objects concurrently in arbitrary order, with each update wrapped in 
an atomic {...} block.
With 10,000’s of updates, and 5-10 objects touched per update, collisions will 
be low.
~2-4X STM performance overhead is acceptable:
if it enables our state-intensive code to scale to many threads, it’s still a win

Claim: Transactions are the only plausible solution to concurrent mutable state
-->

The main motivation for rewriting Pingus to Céu is to suggest structured
synchronous reactive programming as an expressive and productive alternative
for the game logic development.
In Pingus, the game logic also accounts for almost half the size of the
codebase (18173 from 39362 LoC, or 46%).

`TODO: total rewritten, gains in %`

The rewriting process consisted of identifying a set of callbacks implementing
a *control-flow behavior* in the game and translating them to Céu using
appropriate structured constructs.
As an example, a double mouse click consists of the first click, followed by a
maximum amount of time, followed by a second click.
This behavior depends on different event (clicks and timers) that have to occur
in a particular order.
In C++, the implementation involves callbacks crossing successive reactions to
events manipulating state variables explicitly
More concretely, we could identify control-flow behaviors in C++ by looking for
class members with identifiers resembling verbs, statuses, or counters
(e.g.,
[`pressed`][state-pressed],
[`particle_thrown`][state-particle-thrown],
[`mode`][state-mode], or
[`delay_count`][state-delay-count]).
Good chances are that variables with "suspicious names" encode some form of
control-flow progression that cross multiple callback invocations.

[state-pressed]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/components/action_button.hpp#L36
[state-particle-thrown]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/actions/bomber.hpp#L31
[state-mode]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/actions/bridger.hpp#L30
[state-delay-count]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/actions/digger.hpp#L32

<!--
, and redirect the instantiation and event dispatching to the new 
class.
The remaining classes in C++ should interoperate with the new classes in Céu 
until we complete the rewriting process.
-->

Note that we only touch code that deals with events, as Céu is actually less 
expressive than C++ for pure data manipulation (e.g., game preferences, graph
algorithms, etc).
Therefore, we also rely on the seamless integration between Céu and C/C++ to
take advantage of the existing code base and libraries.

We selected `TODO` game behaviors and describe their implementations in C++ and
Céu.
As an afterthought, we categorized these examples in more abstract control-flow
patterns that likely apply to other games:

<a name="finite-state-machines"/>

1. [**Finite State Machines**](#finite-state-machines):
    State machines describe the behavior of entities by mapping event 
    occurrences to transitions between states that trigger appropriate actions.
    * [ [case 1](#finite-state-machines-1) |
        [case 2](#finite-state-machines-2) |
        [summary](#finite-state-machines-summary) ]

2. [**Continuation Passing**](#continuation-passing):
    The completion of a long-lasting activity may carry a continuation, i.e.,
    some action to execute next.
    * [ [case 1](#continuation-passing-1) |
        [case 2](#continuation-passing-2) |
        [summary](#continuation-passing-summary) ]

3. [**Dispatching Hierarchies**](#dispatching-hierarchies):
    Entities typically form a dispatching hierarchy in which a container entity
    that receives a stimulus automatically forwards it to its managed children.
    * [ [case 1](#dispatching-hierarchies-1) |
        [summary](#dispatching-hierarchies-summary) ]

<!-- TODO: resize -->

4. [**Lifespan Hierarchies**](#lifespan-hierarchies):
    Entities typically form a lifespan hierarchy in which a terminating
    container entity automatically destroys its managed children.
    * [ [case 1](#lifespan-hierarchies-1) |
        [case 2](#lifespan-hierarchies-2) |
        [summary](#lifespan-hierarchies-summary) ]

5. [**Signaling**](#signaling):
    Entities often need to communicate explicitly through a signaling 
    mechanism, especially if there is no hierarchy relationship between them.
    * [ [case 1](#signaling_1) |
        [case 2](#signaling_2) |
        [summary](#signaling-summary) ]

<!-- TODO: pause -->

<!--
6. [**Wall-Clock Timers**](#wall-clock-timers):
    Wall-clock timers measure the passage of time from the real world
    (e.g., *10 seconds*) such as for periodic sampling and timeout watchdogs.
    * [ [summary](#wall-clock-timers-summary) ]
-->

6. [**Pausing**](#pausing):
    Pausing allows parts of the game to temporarily stop reacting to incoming
    events.
    <!-- * [ [summary](#pausing-summary) ] -->
    * `TODO`

7. [**Resource Acquisition and Release**](#resource-acquisition-and-release):
    External resources, such as configuration files and saved games,
    must be acquired and safely released.
    <!-- * [ [summary](#resource-acquisition-and-release-summary) ] -->
    * `TODO`

Other games manifesting these patterns also use some form of explicit state
which are likely subject to the same rewriting process.
<!-- TODO: The patterns are not entirely orthogonal -->

Overall, we believe that most difficulties in implementing control behavior in 
game logic is not inherent to this domain, but a result of accidental
complexity due to the lack of structured abstractions and an appropriate
concurrency model to handle event-based applications.

<!--
## Why rewriting Pingus to Céu?

Besides promoting the concurrency model of Céu, we have additional motivations 
to write this report as follows:

* Expose Céu to a real code base that was neither specified nor implemented by 
  the designers of the language.
  Even though video games match the domain of Céu, a real-world project 
  consists of a range of requirements, forcing us to transpose the "academic
  fences" of papers (which usually only explore idiomatic code).
* Exercise the interface between Céu and C/C++.
  Céu is designed to integrate seamlessly with C.
  This allowed us to perform a *live rewriting*, i.e., we incrementally rewrote 
  code from C++ to Céu without breaking the game for long.
* Serve as a comprehensive guide for developers interested in trying Céu.
  We provide an in-depth comparison between the original code in C++ and the 
  equivalent code rewritten to Céu for a number of behaviors in the game.
* Stress-test the implementation of Céu.
  Academic artifacts typically do not go beyond working prototypes.
  We also want Céu to be a robust and practical language for everyday use.
* Evaluate the performance of Céu.
  Having C++ as a benchmark, how does Céu compare in terms of memory usage, 
  code size, and execution time (e.g., FPS rate)?

## Control-Flow Patterns in Pingus
-->

## Author

Francisco Sant'Anna

* <http://www.ceu-lang.org/chico/>
* <https://github.com/fsantanna/>
* [&#64;_fsantanna](https://twitter.com/_fsantanna/)

### Acknowledgments

Leonardo Kaplan

* <https://github.com/leokaplan/>

Alexander Tkachov

* <https://github.com/Tkachov/>

-------------------------------------------------------------------------------

<!--
# Qualitative Analysis

- why not quantitative?
    - focus on how
    - not all changes delete code
    - more intereseted in changes that remove global rearrange
        - expressiveness

TODO: Selected Code Snippets
TODO: state vars, code reduction para cada case
-->

@SEC[[finite-state-machines,
## Finite State Machines
]]

State machines describe the behavior of entities by mapping event 
occurrences to transitions between states that trigger appropriate actions.

<!--
The double click behavior for the *Armageddon button* is an example of a simple 
state machine.
TODO: Case 3: Sprite Animations
-->

@SEC[[finite-state-machines-1,
### The *Armageddon* Button Double Click
]]

<!--
Let's consider the case of handling double clicks in the game.
-->

@FIG_NEW(double-click-opt.gif,
         Double click detection,
         350)

A double click in the *Armageddon* button at the bottom right of the screen
literally explodes all pingus (@FIG_REF[[double-click-opt.gif]]).

<!-- CPP-ARMAGEDDON -->

#### C++

In C++, the class `ArmageddonButton` [[![X]][cpp_armageddon]] implements
methods for rendering the button and handling mouse and timer events.
Here, we focus on the double click detection, hiding unrelated parts with 
`<...>`:

@CODE_LINES[[language=CPP,
ArmageddonButton::ArmageddonButton(<...>):
    RectComponent(<...>),                                   @base_class
    pressed(false); // button state: initially not pressed  @pressed_1
    press_time(0);  // how long since the 1st click?        @press_time_1
    <...>
{
    <...>
}

void ArmageddonButton::draw (<...>) {                       @draw_1
    <...>
}                                                           @draw_2

void ArmageddonButton::update (float delta) {               @update_1
    <...>
    if (pressed) {                                          @pressed_4
        press_time += delta;                                @press_time_2
        if (press_time > 1.0f) {                            @check
            pressed = false;    // giving up, 1st click was @reset_1
            press_time = 0;     //            too long ago  @reset_2
        }
    } else {
        <...>
        press_time = 0;
    }
}                                                           @update_2

void ArmageddonButton::on_click (<...>) {                   @on_click_1
    if (pressed) {                                          @pressed_3
        server->send_armageddon_event();                    @armageddon
    } else {
        pressed = true;                                     @pressed_2
    }
}                                                           @on_click_2
]]

The methods `update` @NN(update_1,-,update_2) and `on_click` 
@NN(on_click_1,-,on_click_2) are examples of *short-lived callbacks*, which are
pieces of code that execute atomically in reaction to external input events.
The callback `on_click` reacts to mouse clicks detected by the button base
class `RectComponent` @NN(base_class), while the callback `update` continuously
reacts to the passage of time.
Callbacks are short lived because they must react to input as fast as possible
to let other callbacks execute, keeping the game with real-time responsiveness.

@FIG_NEW(double-click.png,
         State machine for the *Armageddon* double click,
         550)

The class first initializes the variable `pressed` to track the first click 
@NN(pressed_1,,pressed_2).
It also initializes the variable `press_time` to count the time since the first 
click @NN(press_time_1,,press_time_2).
If another click occurs within 1 second @NN(check), the class signals the
double click to the application @NN(armageddon).
Otherwise, the `pressed` and `press_time` state variables are reset 
@NN(reset_1,-,reset_2).

@FIG_REF[[double-click.png]] illustrates how we can model the double-click 
behavior as a state machine.
The circles represent the state of the variables in the class, while the arrows 
represent the callbacks manipulating state.

Note in the code how the accesses to the state variables are spread
across the entire class.
For instance, the distance between the initialization of `pressed` 
@NN(pressed_1) and the last access to it @NN(pressed_2) is over 40 lines in the 
original file [[![X]][cpp_armageddon_2]].
Arguably, this dispersion of code across methods makes the understanding and 
maintenance of the double-click behavior more difficult.
Also, even though the state variables are private, unrelated methods such as 
`draw`, which is defined in middle of the class @NN(draw_1,-,draw_2), can
potentially access them.

<!--
Because callbacks are short lived, the only way they can affect each other is 
by manipulating persisting member variables in the object.
These *state variables* retain their values across multiple invocations and 
serve as a control mechanism across reactions to external events.
For instance, the callbacks `on_click` and `update` react independently but must 
agree on a common protocol to detect the double click:

* `on_click` writes to `pressed` in the first click @NN(pressed_3), and checks
  its state in further clicks @NN(pressed_2).
* `update`, in the meantime, also checks for `pressed` and may reset its state
  @NN(pressed_4,,reset_1).
-->

<!-- CEU-ARMAGEDDON -->

#### Céu

Céu provides structured constructs to deal with events, aiming to eradicate
explicit manipulation of state variables for control-flow purposes.
The equivalent code in Céu for the double-click
detection is as follows [[![X]][ceu_armageddon]]:

@CODE_LINES[[language=CEU,
do                                      @do
    var& RectComponent c = <...>;       // (the symbol `&` denotes an alias declaration)
    <...>
    loop do                             @loop_do
        await c.component.on_click;     @await_1
        watching 1s do                  @watching_do
            await c.component.on_click; @await_2
            break;                      @break
        end                             @watching_end
    end                                 @loop_end
    <...>
    emit outer.game.go_armageddon;      @emit
end                                     @end
]]

<!--
Instead of *objects*, classes in Céu instantiate *organisms* with a body 
declaration @NN(do,-,end) that starts to execute automatically.
Unlike objects, an organism is a reactive entity that executes concurrently 
with other organisms.
Organisms react to external events sequentially, one after the other, resulting 
in deterministic programs.
Unlike callbacks, organism bodies keep the execution context across event 
occurrences alive (if they don't terminate).
-->

The loop detection @NN(loop_do,-,loop_end) awaits the first click @NN(await_1)
and then, while watching 1 second @NN(watching_do,-,watching_end), awaits the
second click @NN(await_2).
If the second click occurs within 1 second, the `break` terminates the loop
@NN(break) and the `emit` signals the double click to the application @NN(emit).
Otherwise, the `watching` block as a whole aborts and restarts the loop, 
falling back to the first click `await` @NN(await_1).

Double click detection in Céu doesn't require any state variables and is
entirely self-contained in the `loop` body  @NN(loop_do,-,loop_end).
Furthermore, these 7 lines of code **only** detect the double click, leaving
the actual effect to happen outside the loop @NN(emit).

<!--
The complete implementations for the *Armageddon* button in C++ and Céu are 47
and 24 lines of code, respectively [[![X]][diff_armageddon]].

As we argue throughout this document, appropriate control-flow mechanisms for 
reactive applications (e.g., the `await` and `watching` statements) help on the
structure and composition of code, resulting in considerable gains in
productivity.
-->

[cpp_armageddon]:   https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/components/action_button.cpp#L24 
[cpp_armageddon_2]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/components/action_button.cpp#L33-#L90
[ceu_armageddon]:   https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/game/input.ceu#L107
[diff_armageddon]:  https://github.com/fsantanna/pingus/commit/d0afe53648862643857811d0af8a7a9f60119f6c

@SEC[[finite-state-machines-2,
### The *Bomber* Action
]]

@FIG_NEW(bomber-opt.gif,
         The *Bomber* action,
         350)

The *Bomber* action explodes the clicked pingu, throwing particles around and 
also destroying the terrain under its radius (@FIG_REF[[bomber-opt.gif]]).

@FIG_NEW(state-anim/state-anim.gif,
         State machine for the *Bomber* animation,
         550)

We can model the explosion animation with a sequential state machine
(@FIG_REF[[state-anim/state-anim.gif]]) with actions associated to specific
frames as follows:

1. 0th frame:  plays a "Oh no!" sound.
2. 10th frame: plays a "Bomb!" sound.
3. 13th frame: throws particles, destroys the terrain, and shows an explosion
               sprite.
4. Game tick:  hides the explosion sprite.
5. Last frame: kills the pingu.

*([This video][youtube_bomber] presents the sound effects.)*

[youtube_bomber]: https://youtu.be/QLXIT59il6o?t=306

#### C++

In C++, the class `Bomber` [[![X]][cpp_bomber]] defines the callbacks `draw`
and `update` to manage the state machine described above:

@CODE_LINES[[language=CPP,
Bomber::Bomber (Pingu* p) :
    <...>
    sprite(<...>),              // bomber sprite
    sound_played(false),        // tracks state 2               @def_1
    particle_thrown(false),     // tracks state 3
    colmap_exploded(false),     // tracks state 3
    gfx_exploded(false)         // tracks state 4               @def_2
{
    <...>
    // 1. 0th frame: plays a "Oh no!" sound.
    get_world()->play_sound("ohno", pingu->get_pos());          @sound_ohno
}

void Bomber::update ()
{
    sprite.update();                                            @update_1
    <...>   // pingu movement                                   @update_2

    // 2. 10th frame: plays a "Bomb!" sound.
    if (sprite.get_current_frame()==10 && !sound_played) {      @sound_bomb_1
        sound_played = true;
        get_world()->play_sound("plop", pingu->get_pos());
    }                                                           @sound_bomb_2

    // 3. 13th frame: throws particles, destroys the terrain, shows an explosion sprite
    if (sprite.get_current_frame()==13 && !particle_thrown) {   @state_3_1
        particle_thrown = true;
        get_world()->get_pingu_particle_holder()->add_particle(...);
    }
    if (sprite.get_current_frame()==13 && !colmap_exploded) {
        colmap_exploded = true;
        get_world()->remove(bomber_radius, <...>);
    }                                                           @state_3_2

    // 5. Last frame: kills the Pingu
    if (sprite.is_finished ()) {                                @die_1
        pingu->set_status(Pingu::PS_DEAD);
    }                                                           @die_2
}

void Bomber::draw (SceneContext& gc) {
    // 3. 13th frame: throws particles, destroys the terrain, shows an explosion sprite
    // 4. Game tick: hides the explosion sprite
    if (sprite.get_current_frame()==13 && !gfx_exploded) {      @state_3_3
        gfx_exploded = true;                                    @state_3_4
        gc.color().draw(explo_surf, <...>);                     @state_4
    }
    gc.color().draw(sprite, pingu->get_pos());
}
]]

<!--*-->

The class defines one state variable for each action to perform 
@NN(def_1,-,def_2).
The "Oh no!" sound plays as soon as the object starts in *state-1* 
@NN(sound_ohno).
The `update` callback updates the pingu animation and movement every frame 
regardless of its current state @NN(update_1,-,update_2).
When the animation reaches the 10th frame, it plays the "Bomb!" sound and 
switches to *state-2* @NN(sound_bomb_1,-,sound_bomb_2).
The state variable `sound_played` is required because the sprite frame doesn't 
necessarily advance on every `update` invocation.
The same reasoning and technique applies to the *state-3*
(ln. @N(state_3_1)-@N(state_3_2) and @N(state_3_3)-@N(state_3_4)).
The explosion sprite appears in a single frame in *state-4* @NN(state_4).
Finally, the pingu dies after the animation frames terminate 
@NN(die_1,-,die_2).

Even though a single numeric state variable suffices to track the states, the
original authors probably chose to encode each state in an independent boolean 
variable to rearrange and experiment with them during development.
Still, due to the short-lived nature of callbacks, state variables are 
unavoidable and are actually the essence of object-oriented programming
(i.e., *methods + mutable state*).

<a name="bomber"/>

#### Céu

The equivalent code for the *Bomber* action in Céu doesn't require any state
variables and reflects the sequential state machine implicitly, using `await`
statements to separate the actions in direct style [[![X]][ceu_bomber]]:

@CODE_LINES[[language=CEU,
code/await Bomber (void) -> _ActionName__Enum
do
    <...>
    spawn Mover();                          // spawn the pingu movement to execute in the "background"      @move

    var&? Sprite s = spawn Sprite(<...>);   // spawn the bomber animation to execute in the "background"    @sprite
    watching s do                                                   @watch-1
        // 1. 0th frame: plays a "Oh no!" sound.                    @anim_1
        {Sound::PingusSound::play_sound("ohno", 0.5, 0.0)};

        // 2. 10th frame: plays a "Bomb!" sound.
        await outer.game.dt until s!.sprite.frame == 10;            @frame_1
        {Sound::PingusSound::play_sound("plop", 0.5, 0.0)};

        // 3. 13th frame: throws particles, destroys the terrain, shows an explosion sprite
        await outer.game.dt until s!.sprite.frame == 13;            @frame_2
        spawn PinguParticles(<...>) in outer.pingu_particles;       @particles
        call Game_Remove({&bomber_radius}, <...>);
        do                                                          @do
            <...>
            spawn Sprite(<...>);            // explosion            @explo

            // 4. Game tick: hides the explosion sprite
            await outer.game.dt;                                    @frame_3
        end                                                         @end
        await FOREVER;
    end                                                             @watch-2

    // 5. Last frame: kills the pingu
    escape {ActionName::DEAD};                                      @anim_2
end
]]

The `Bomber` is a `code/await` abstraction of Céu,
which is similar to a co-routine: a function that retains state, such as local
variables and the program counter, across reactions to events (i.e., across
`await` statements).
The pingu movement and sprite animation are isolated in two other abstractions
and execute in separate through the `spawn` primitive @NN(move,,sprite).

The code tracks the animation abstraction @NN(watch-1,-,watch-2), performing
the last action on termination @NN(anim_2).
As soon as the animation starts, the code performs the first action
@NN(anim_1).
The intermediate actions are performed when the corresponding conditions occur
(ln. @N(frame_1),@N(frame_2),@N(frame_3)).

<a name="bomber_explo"/>

The `do-end` block @NN(do,-,end), restricts the lifespan of the single-frame
explosion sprite @NN(explo): after the next game tick @NN(frame_3), the block
terminates and automatically destroys the spawned abstraction (removing it from
the screen).

<!--
The complete implementations for the *Bomber* action in C++ and Céu are 50 and
19 lines of code, respectively [[![X]][diff_bomber]].
-->

<a name="finite-state-machines-summary"/>
<br/>

<div class="summary">
**Summary**:

Implicit state machines in Céu provide some advantages in comparison to 
explicit state machines:

* They encode all states with direct sequential code, not requiring state
  variables.
* They handle all states (and only them) in the same contiguous block,
  improving code encapsulation.
</div>

[cpp_bomber]:  https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/actions/bomber.cpp
[ceu_bomber]:  https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/game/pingu/actions/bomber.ceu
[diff_bomber]: https://github.com/fsantanna/pingus/commit/8e8cf7ed81427d575975fc7a4da579c7b76219a0

@SEC[[continuation-passing,
## Continuation Passing
]]

The completion of a long-lasting activity may carry a continuation, i.e.,
some action to execute next.

<!--
If the execution flow is dynamic, the program has to tell the activity where to 
go when it completes.
In Pingus, when the player terminates a level, the game may terminate or return 
to the main menu, depending on how it was invoked from the command line.

Continuation passing is a special case of a state machine in which the previous 
state passes to the current state what will be the next state.

- tudo porque eu consigo voltar/retornar das "chamadas"
    - programacao estruturada

more natural structured code with sequences, conditionals, and loops
-->

@SEC[[continuation-passing-1,
### Advancing Pages in the *Story* Screen
]]

@FIG_NEW(story-anim.gif,
         The *Story* screen,
         350)

The clickable *blue dots* in the world map of Pingus transit to ambience story 
screens (@FIG_REF[[story-anim.gif]]).
A story is composed of multiple pages and inside each page, the words of the
story appear incrementally over time.
A first click in the button `>>>` fast forwards the words to show the full 
page.
A second click advances to the next page, until the story terminates.
If the page completes before a click (due to the time elapsing), a first click 
advances to the next page.

<!-- CPP-STORY-PAGES -->

#### C++

In C++, the class `StoryScreenComponent` [[![X]][cpp_story_screen_component]]
implements the method `next_text`, which is a callback for clicks in `>>>`:

@CODE_LINES[[language=CPP,
StoryScreenComponent::StoryScreenComponent (<...>) :
    <...>
{
    pages        = <...>;           // vector with loaded pages     @pages_1
    current_page = pages.back();    // first loaded page            @pages_2
    displayed    = false;           // if current page is complete  @dsp_1
    <...>
}

<...>   // draw page over time

void StoryScreenComponent::update (<...>) {
    <...>
    if (<all-words-appearing>) {
        displayed = true;                       @dsp_11
    }
}

void StoryScreenComponent::next_text() {
    if (!displayed) {                           @dsp_2
        displayed = true;                       @dsp_3
        <...>                       // remove current page
    } else {                                    @adv_1
        pages.pop_back();                       @pages_3
        if (!pages.empty()) {       // next page
            current_page = pages.back();        @pages_4
            displayed    = false;               @dsp_4
            <...>
        } else {
            <...>                   // terminates the story screen
        }
    }                                           @adv_2
}
]]

@FIG_NEW(story.png,
         State machine for the *Story* screen,
         550)

The variable `pages` (ln. @N(pages_1)-@N(pages_2), @N(pages_3)-@N(pages_4)) is 
a vector holding each page, but which also encodes *continuations* for the 
story progress:
each call to `next_text` that advances the story @NN(adv_1,-,adv_2) removes the 
current page @NN(pages_3) and sets the next action to perform (i.e., "display a 
new page") in the variable `current_page` @NN(pages_4).
@FIG_REF[[story.png]] illustrates a state machine for fast-forwarding words 
(inside the dashed rectangle) and also the continuation mechanism to advance 
pages.
The state variable `displayed`
(ln. @N(dsp_1),@N(dsp_11),@N(dsp_2),@N(dsp_3),@N(dsp_4))
switches between the behaviors "advancing text" and "advancing pages", which 
are both handled inside the method `next_text`.

<!-- CEU-STORY-PAGES -->

#### Céu

The code in Céu [[![X]][ceu_story]] uses the event `next_text`, which is 
emitted from clicks in `>>>`:

@CODE_LINES[[language=CEU,
code/await Story (void) -> bool do
    <...>
    event void next_text;   // emitted from clicks in `>>>`

    { pages = <...>; }      // same as in C++
    loop i in [0 <- {pages.size()}[ do                      @loop_do
        par/or do
            watching next_text do
                <...>       // loop to advance text over time   @advance
            end
            await next_text;                                @await
        with
            <...>           // loop to redraw current _pages[i]
        end
    end                                                     @loop_end
end
]]

The sequential navigation from page to page uses a direct loop 
@NN(loop_do,-,loop_end) instead of an explicit continuation state variable.
While the text advances in an inner loop (hidden in ln. @N(advance)), we watch 
the `next_text` event that fast forwards it.
The inner loop may also eventually terminate with the time elapsing normally.
This way, we don't need a variable (such as `displayed` in C++) to switch 
between the states "advancing text" and "advancing pages".
<!-- which are not mixed in the source code. -->
The `await next_text` in sequence @NN(await) is the condition to advance to the
next page.

<!--
The complete implementations for the *Story* screen in C++ and Céu are 125
and 111 lines of code, respectively [[![X]][diff_story]].
-->

[diff_story]: https://github.com/fsantanna/pingus/commit/1e17519467c8b0c3d616f0452966b6f5357ccd34

@SEC[[continuation-passing-2,
### Transition to the *Credits* Screen from the *Story* Screen
]]

@FIG_NEW(credits-anim.gif,
         Transition from *Story* to *Credits* screen,
         350)

The world map has clickable blue dots for both introductory and ending 
stories.
For introductory stories, the game returns to the world map after displaying 
the pages.
For ending stories, the game also displays a *Credits* screen before returning 
to the world map (@FIG_REF[[credits-anim.gif]]).

<!-- CPP-STORY-CREDITS -->

#### C++

In C++, the class `StoryDot` [[![X]][cpp_story_dot]] reads the level file to
check whether the story should, after termination, display the *Credits* screen
or not:

@CODE_LINES[[language=CPP,
StoryDot::StoryDot(const FileReader& reader) :
    m_credits(false),                           // by default, don't display
{
    <...>
    reader.read_bool("credits", m_credits);     // read from the file
}

void StoryDot::on_click() {
    <...>
    ScreenManager::instance()->push_screen(std::make_shared<StoryScreen>(<...>, m_credits)); @call
    <...>
}
]]

The boolean variable `m_credits` is passed to the `StoryScreen` @NN(call)
[[![X]][cpp_story_screen]] and represents the screen continuation, i.e., what
to do after displaying the story.
The `StoryScreen` then forwards the continuation [[![X]][cpp_story_screen_forward]] 
to the `StoryComponent` [[![X]][cpp_story_screen_component]] discussed in
@SEC_REF[[continuation-passing-1]]:

@CODE_LINES[[language=CPP,
StoryScreenComponent::StoryScreenComponent (<...>) :
    m_credits(credits),
    <...>
{
    <...>
}

<...>   // draw and update page

void StoryScreenComponent::next_text() {
    if (!displayed) {
        <...>
    } else {
        <...>
        if (!pages.empty()) {
            <...>
        } else {                @adv_1
            if (m_credits) {    @m_credits
                ScreenManager::instance()->replace_screen(std::make_shared<Credits>(<...>));
            } else {
                ScreenManager::instance()->pop_screen();
            }
        }                       @adv_2
    }
}
]]

When the method `next_text` has no pages left to display @NN(adv_1,-,adv_2), it 
decides where to go next, depending on the continuation flag `m_credits` 
@NN(m_credits).

<!-- CEU-STORY-CREDITS -->

#### Céu

In Céu, the flow between the screens to display is a direct sequence of 
statements [[![X]][ceu_credits]]:

@CODE_LINES[[language=CEU,
loop do
    var int ret = await Worldmap();                                 @call_world
    if ret=={WORLDMAP_RETURN_STORY_MAP} or ret=={WORLDMAP_RETURN_STORY_CREDITS} then
        <...>                                                       @story_1
        var bool is_click = await Story();                          @call_story
        if is_click and ret=={WORLDMAP_RETURN_STORY_CREDITS} then   @check
            <...>
            await Credits();                                        @call_credits
        end                                                         @story_2
    else
        <...>
    end
end
]]

<!--
<div class="box">
**The `do` notation**:

The `do` notation is a syntactic sugar for creating and awaiting an organism, 
e.g.:

@CODE_LINES[[language=CEU, lines=false, reset=false,
var int ret = do WorldmapScreen;
<...>

// is equivalent to

var int ret;
do
    var WorldmapScreen w;
    ret = await w;
end
<...>
]]

A `do` is analogous to a procedure call, holding the current state while the 
started organism executes.
The code in sequence (marked as `<...>`) only executes after the organism 
terminates.
</div>
-->

We first invoke the `Worldmap`, which exhibits the map and let
the player interact with it until a dot is clicked @NN(call_world).
If the player selects a story dot @NN(story_1,-,story_2), we invoke the `Story`
and await its termination @NN(call_story).
Finally, we check the returned values @NN(check) to display the `Credits` 
@NN(call_credits).
The enclosing loop restores the `Worldmap` and repeats the process.

#### Discussion

@FIG_NEW(continuation.png,
         Continuation [C++] vs Direct [Céu] Styles,
         500)

@FIG_REF[[continuation.png]] illustrates the *continuation-passing style* of
C++ and the *direct style* of Céu for screen transitions:

1. `Main Loop` => `Worldmap`:
    C++ uses an explicit stack to push the `Worldmap` screen;
    Céu invokes the `WorldMap` screen expecting a return value;
2. `Worldmap` (*blue dot click*) => `Story`:
    C++ pushes the `Story` screen passing the continuation flag;
    Céu stores the `Worldmap` return value and invokes the `Story` screen.
3. `Story` => `Credits`:
    C++ replaces the curren `Story` screen with the `Credits` screen.
    Céu invokes the `Credits` screen after the `await Story` returns.
4. `Credits` => `Worldmap`:
    C++ pops the `Credits` screen, going back to the `Worldmap` screen.
    Céu uses an enclosing `loop` to restart the process.

In contrast with C++, the screens in Céu are decoupled and only the `Main Loop` 
touches them:
the `Worldmap` has no references to `Story`,
which has no references to `Credits`.

<!--
TODO:
- always forward, no returns
- nao existe retorno, sempre continuacao apos continuacao
"nowhere to return"
-->

<a name="continuation-passing-summary"/>
<br/>

<div class="summary">
**Summary**:

The direct style of Céu has some advantages in comparison to the 
continuation-passing style:

* It uses structured control flow (i.e., sequences and loops) instead of 
  explicit data structures (e.g., stacks) or continuation variables.
* The activities are decoupled and do not hold references to one another.
* A single parent class describes the flow between the activities in a 
  self-contained block of code.
    <!-- (instead of being spread among the activity classes). -->
</div>

[cpp_story_screen]:https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/screens/story_screen.cpp#L136
[cpp_story_screen_component]:https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/screens/story_screen.cpp#L158
[cpp_story_screen_forward]:https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/screens/story_screen.cpp#L143
[cpp_story_dot]:https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/worldmap/story_dot.cpp#L31
[cpp_story_pages]:https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/screens/story_screen.cpp#L159

[ceu_story]:https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/story.ceu#L60-L112
[ceu_credits]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/worldmap/worldmap.ceu#L99-L122

[wiki_style_direct]:       https://en.wikipedia.org/wiki/Direct_style
[wiki_style_continuation]: https://en.wikipedia.org/wiki/Continuation-passing_style

@SEC[[dispatching-hierarchies,
## Dispatching Hierarchies
]]

Entities typically form a dispatching hierarchy in which a container entity
that receives a stimulus automatically forwards it to its managed children.

<!--
Some entities in games manage other child entities, resulting in dispatching 
hierarchies for event forwarding.
-->

<!--
It is common to broadcast notifications so that active objects can react to 
them.
The .
Some notifications
Given that .
https://en.wikipedia.org/wiki/Observer_pattern

TODO: falar de broadcast (in Ceu: unless it is paused, all receive always)

    In Pingus, the *Main Menu* in the figure above is represented as a 
    container class with five buttons as children.
    When a button click occurs, it is first dispatched to the container class,
    which may take an action before deciding to forward the event (or not) to 
    the buttons.
-->

@SEC[[dispatching-hierarchies-1,
### Bomber `draw` and `update` callbacks
]]

<!-- CPP-BOMBER-SPRITE -->

#### C++

In C++, the class `Bomber` [[![X]][cpp_bomber]] declares a `sprite` member to
handle its animation frames:

@CODE_LINES[[language=CPP,
class Bomber : public PinguAction
{
    <...>
    Sprite sprite;
    void draw (SceneContext& gc);
    void update();
}

Bomber::Bomber (<...>) : <...>
{
    sprite.load(<...>);                     @load
    <...>
}

void Bomber::update () {                    @update_1
    sprite.update ();
}                                           @update_2

void Bomber::draw (SceneContext& gc) {      @draw_1
    <...>
    gc.color().draw(sprite, <...>);
}                                           @draw_2
]]

The `Sprite` class is part of the game engine and knows how to update
[[![X]][cpp_sprite_update]] and render [[![X]][cpp_sprite_render]] itself.
However, the `Bomber` still has to respond to `update` and `draw` request from
the game and forward them to the sprite
(ln. @N(update_1)-@N(update_2) and @N(draw_1)-@N(draw_2)).

[cpp_sprite]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/display/sprite_impl.cpp
[cpp_sprite_update]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/display/sprite_impl.cpp#L112
[cpp_sprite_render]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/display/sprite_impl.cpp#L140

@FIG_NEW(hierarchy.png,
         Dispatching chain for `update`,
         550)

To understand how the `update` callback flows from the original environment
stimulus from the game down to the sprite, we need to follow a long chain of 7
method dispatches (@FIG_REF[[hierarchy.png]]):

1. `ScreenManager::display` [[![X]][cpp_screenmanager_11]] in the main game
    loop
        calls
    `update` [[![X]][cpp_screenmanager_12]].
2. `ScreenManager::update` [[![X]][cpp_screenmanager_21]]
        calls
   `last_screen->update` [[![X]][cpp_screenmanager_22]] for the active game
    screen (a `GameSession` instance considering an active `Bomber`).
3. `GameSession::update` [[![X]][cpp_gamesession_1]]
        calls
    `world->update` [[![X]][cpp_gamesession_2]].
4. `World::update` [[![X]][cpp_world_1]]
        calls
    `obj->update` [[![X]][cpp_world_2]] for each object in the world.
5. `PinguHolder::update` [[![X]][cpp_pinguholder_1]]
        calls
    `pingu->update` [[![X]][cpp_pinguholder_2]] for each pingu alive.
6. `Pingu::update` [[![X]][cpp_pingu_1]]
        calls
    `action->update` [[![X]][cpp_pingu_2]] for the active pingu action.
7. `Bomber::update` [[![X]][cpp_bomber_1]]
        calls
    `sprite.update` [[![X]][cpp_bomber_2]].
8. `Sprite::update` [[![X]][cpp_sprite_1]]
    finally updates the animation frame.

Nonetheless, each dispatching step is necessary considering the game
architecture:

* With a single assignment to `last_screen`, we can easily deactivate the
  current screen and redirect all dispatches to a new screen.
* The `World` class manages and dispatches events to all game entities with a 
  common interface (i.e., `WorldObj` [[![X]][cpp_worldobj]]), such as all
  pingus and traps.
* Since it is common to iterate only over the pingus (vs. all world objects),
  it is convenient to manage all pingus in a `PinguHolder` container.
* Since a single pingu can change between actions during lifetime, it is also
  convenient to decouple it from actions with a level of indirection through
  the `action` member.
* Sprites are part of the game engine and are reusable everywhere (e.g., UI
  buttons, world objects, etc.), so it is also convenient to decouple them
  from actions.

The `draw` callback flows through the same dispatching hierarchy until reaching
the `Sprite` class.

<!-- CEU-BOMBER-SPRITE -->

#### Céu

In Céu, the `Bomber` action spawns a `Sprite` animation [[![X]][ceu_bomber]]:

@CODE_LINES[[language=CEU,
code/await Bomber (void) -> _ActionName__Enum do
    <...>
    var&? Sprite sprite = spawn Sprite(<...>);  @dcl
    <...>
end
]]

The `Sprite` instance @NN(dcl) can react directly to external `update`
[[![X]][ceu_sprite_update]] and `draw` [[![X]][ceu_sprite_redraw]] events,
bypassing the program hierarchy entirely.

The radical decoupling between the program hierarchy and external reactions
completely eliminates dispatching chains.
For instance, we removed from the game engine most of the boilerplate related
to event dispatching ([[![X]][TODO]]).

<!--
On the other hand, now that organisms themselves decide whether or not to react 
to external input, we support that lexical scopes should control their life 
cycles.

@FIG_NEW(explo.png,
         Explosion sprite for the `Bomber` animation,
         )

Just like standard local variables, we can delimit the scope of organisms 
through explicit blocks.
As an example, the explosion sprite for the `Bomber` animation above 
[[![X]](#bomber)] reacts and redraws exactly for one occurrence of 
`WORLD_UPDATE` (after the 13th animation frame):

@CODE_LINES[[language=CEU,
class Bomber with
    <...>
do
    var Sprite sprite = Sprite.build_name(<...>);
    <...>
        // 13th frame:
        await WORLD_UPDATE until sprite.get_current_frame() == 13;
        <...>
        do                                                      @do
            var Sprite _ = Sprite.build_name(<...>, &&explo);
            await WORLD_UPDATE;                                 @await
        end                                                     @end
        <...>
end
]]

We enclose the declaration with an explicit block @NN(do,-,end) that restricts 
its lifespan to a single occurrence of `WORLD_UPDATE` @NN(await).
When the block terminates, the organism goes out of scope and its execution 
body aborts automatically, effectively removing it from the screen.
Note here that we never manipulate references to the `Sprite`, which is 
declared anonymous with the placeholder `_`.
In constrast, the animation in C++ requires to explicitly check the state 
variable `gfx_exploded` and forward the `draw` method down to the child sprite 
`explo_surf` [[![X]][cpp_bomber_explo]].
-->

<!-- CEU-vs-CPP-BOMBER-SPRITE -->

<!--
`TODO: SDL_RESIZE`
-->

<a name="dispatching-hierarchies-summary"/>
<br/>

<div class="summary">
**Summary**:

Passive objects subjected to a hierarchy require a dispatching architecture
that makes the reasoning about the program harder:

* The full dispatching chain may go through dozen of files.
  <!--(note that we omitted class hierarchies from the discussion).-->
* The dispatching chain may interleave between classes specific to the game and 
  also classes from the game engine (possibly third-party classes).
<!--
* The actual objects in the hierarchy are often dynamically allocated,
  specially for entities held in class containers.
-->

Abstractions in Céu can react directly to the environment, not depending on
hierarchies spread across multiple files.

</div>

<!--* TODO: efficiency?-->

[cpp_screenmanager_11]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/screen/screen_manager.cpp#L164
[cpp_screenmanager_12]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/screen/screen_manager.cpp#L217
[cpp_screenmanager_21]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/screen/screen_manager.cpp#L235
[cpp_screenmanager_22]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/screen/screen_manager.cpp#L258
[cpp_gamesession_1]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/screens/game_session.cpp#L195
[cpp_gamesession_2]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/server.cpp#L103 
[cpp_groupcomponent_1]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/gui/group_component.cpp#L58
[cpp_groupcomponent_2]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/gui/group_component.cpp#L63
[cpp_world_1]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/world.cpp#L146
[cpp_world_2]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/world.cpp#L183
[cpp_pinguholder_1]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/pingu_holder.cpp#L89
[cpp_pinguholder_2]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/pingu_holder.cpp#L95
[cpp_pingu_1]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/pingu.cpp#L311
[cpp_pingu_2]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/pingu.cpp#L339
[cpp_bomber_1]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/actions/bomber.cpp#L58
[cpp_bomber_2]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/actions/bomber.cpp#L60
[cpp_sprite_1]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/display/sprite_impl.cpp#L112
[cpp_worldobj]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/worldobj.hpp#L34
[cpp_bomber_explo]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/actions/bomber.cpp#L50

@SEC[[lifespan-hierarchies,
## Lifespan Hierarchies
]]

Entities typically form a lifespan hierarchy in which a terminating
container entity automatically destroys its managed children.

<!--
Similarly to *dispatching hierarchies*, some entities control the lifespan of 
other child entities, resulting in dynamic and explicit allocation and 
deallocation of objects.

However, it is actually common to have children with a static lifespan which 
are known at compile time.
-->

@SEC[[lifespan-hierarchies-1,
### Game UI Widgets
]]

<!-- CPP-CONTAINER -->

#### C++

@FIG_NEW(game-session-arrows.png,
         UI children with static lifespan,
         300)

In C++, the class `GameSession` makes the UI widgets, such as the buttons,
pingus counter, and small map (@FIG_REF(game-session-arrows.png)) to coexist
with the game screen during its whole lifespan:

@CODE_LINES[[language=CPP,
GameSession::GameSession(<...>) :
    <...>
{
    <...>
    button_panel = new ButtonPanel(<...>);      // always active...
    pcounter     = new PingusCounter(<...>);
    small_map    = new SmallMap(<...>);
    <...>
    gui_manager->add(button_panel);             // ...but added dynamically
    gui_manager->add(pcounter);                 //    to the dispatching
    gui_manager->add(small_map);                //    hierarchy
    <...>
}
]]

The widgets are created in the constructor, added to a UI container, and are
never removed explicitly [[![X]][cpp_gamesession_containers]].
This implies that the widgets could be top-level automatic (non-dynamic)
members, which would match the coexisting lifespan intent better.

However, the class uses the container to automate `draw` and `update`
dispatching as discussed in @SEC_REF[[dispatching-hierarchies-1]].
The container `add` method expects only dynamically allocated children
because they are automatically deallocated inside the container destructor 
[[![X]][cpp_groupcomponent_delete]].

The dynamic nature of containers in C++ demand extra caution:

* When containers are part of a dispatching chain, it gets even harder to track 
  which objects are dispatched:
  one has to "simulate" the program execution and track calls to `add` and
  `remove`.
* For objects with dynamic lifespan, calls to `add` must always have matching 
  calls to `remove`:
  missing calls to `remove` lead to memory and CPU leaks (see the
  *lapsed listener* problem in SEC_REF[[lifespan-hierarchies-2]]).

[cpp_groupcomponent_delete]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/gui/group_component.cpp#L37

<!-- CPP-CONTAINER-STATIC -->

<!-- CEU-CONTAINER-STATIC -->

#### Céu

In Céu, entities that coexist just have to be created in the same lexical
block:

@CODE_LINES[[language=CEU,
code/await Game (void) do
    <...>                       // other coexisting functionality
    spawn ButtonPanel(<...>);
    spawn PingusCounter(<...>);
    spawn SmallMap(<...>);
    <...>                       // other coexisting functionality
end
]]

Lexical lifespan never requires containers, allocation and deallocation, or
manipulating references explicitly.
In addition, all required memory is known at compile time.

The actual code in the repository [[![X]][ceu_world_top]] is equivalent to the
code above with abstractions, but inlines all functionality in parallel:

@CODE_LINES[[language=CEU,
par do
    <...>                           // other coexisting funcionality
with
    #include "button_panel.ceu"     // inlined code for the pannel
with
    #include "pingus_counter.ceu"   // inlined code for the counter
with
    #include "small_map.ceu"        // inlined code for the map
with
    <...>                           // other coexisting funcionality
end
]]

The [*Bomber* state machine](#bomber_explo) also takes advantage of lexical
scope to delimit the lifespan of the explosion sprite to a single frame.

@SEC[[lifespan-hierarchies-2,
### The Pingus Container
]]

@FIG_NEW(pingus_create_die-anim.gif,
         Creation and death of pingus,
         400)

A pingu is a dynamic entity created periodically and destroyed under certain
conditions, such as falling from a high altitude [[![X]][cpp_pingu_dead]]
(@FIG_REF(pingus_create_die-anim.gif)).

<!-- CPP-CONTAINER-DYNAMIC -->

#### C++

In C++, the class `PinguHolder` is a container that holds all alive pingus:

@CODE_LINES[[language=CPP,
Pingu* PinguHolder::create_pingu (<...>) {              @create_1
    <...>
    Pingu* pingu = new Pingu (<...>);                   @push_1
    pingus.push_back(pingu);                            @push_2
    <...>
}                                                       @create_2

void PinguHolder::update() {                            @update_1
    <...>
    while(pingu != pingus.end()) {
        (*pingu)->update();                             @update
        if ((*pingu)->get_status() == Pingu::PS_DEAD) { @dead_1
            pingu = pingus.erase(pingu);                @erase
        }                                               @dead_2
        <...>
        ++pingu;
    }
}                                                       @update_2
]]

The method `PinguHolder::create_pingu` @NN(create_1,-,create_2) is called
periodically to create a new `Pingu` and add it to the `pingus` collection
@NN(push_1,-,push_2).
The method `PinguHolder::update` @NN(update_1,-,update_2) checks all pingus
every frame, removing those with the status `Pingu::PS_DEAD`
@NN(dead_1,-,dead_2).

Entities with dynamic lifespan in C++ require explicit `add` and `remove` calls
associated to a container.
Without the `erase` call above @NN(erase), a dead pingu would remain in the
collection being updated every frame @NN(update).
Since the `redraw` behavior for a dead pingu is innocuous, the death would go
unnoticed and the program would keep consuming memory and CPU time.
This problem is known as the *lapsed listener* [[![X]][gpp_lapsed_listener]] 
and also occurs in languages with garbage collection:
A container typically holds a strong reference to a child (sometimes the only 
reference to it), and a collector cannot magically detect it as garbage.

<!-- CEU-CONTAINER-DYNAMIC -->

#### Céu

Céu supports `pool` declarations to hold dynamic abstraction instances.
Additionally, the `spawn` statement can specify a pool identifier to associate
the new instance with a pool.

The game screen spawns a new `Pingu` on every invocation of `Pingu_Spawn`
[[![X]][ceu_pinguholder]]:

@CODE_LINES[[language=CEU,
code/await Game (void) do                   @do
    <...>
    pool[] Pingu pingus;                    @pool
    code/await Pingu_Spawn (<...>) do
        <...>
        spawn Pingu(<...>) in outer.pingus; @spawn
    end
end                                         @end
]]

The `spawn` statement @NN(spawn) specifies the pool declared at the top-level
block of the game screen @NN(pool).
Since pools are also subject to lexical scope, the lifespan of all dynamically
allocated pingus is constrained to the game screen.

<!--
Therefore, if the top-level block of `PinguHolder` goes out of scope 
@NN(do,-,end), the execution of all pingus is aborted and they are 
automatically reclaimed from memory.
The same happens if the block containing the instance of `PinguHolder` goes out 
of scope [[![X]][ceu_world_pinguholder]] (and so on, up to the outermost block 
of the program [[![X]][ceu_main_outermost]]).
-->

@FIG_NEW(pool.png,
         Lifespan of dynamic instances,
         400)

Lexical scopes handle memory and event dispatching automatically for static
instances and pools.
However, the lifespan of a dynamic instance does not necessarily match the 
lifespan of its associated pool (@FIG_REF[[pool.png]]).
In Céu, when the execution block of a dynamic instance terminates, which
characterizes its *natural termination*, the instance is automatically removed
from its pool.
Therefore, dynamic instances don't require any extra bookkeeping related to 
containers.

To remove a pingu from the game in Céu, we just need to terminate its execution
block according to the appropriate conditions [[![X]][ceu_pingu_dead]]:

@CODE_LINES[[language=CEU,
code/await Pingu (<...>) do
    <...>
    loop do
        await outer.game.dt;
        if call Pingu_Rel_Getpixel(0,-1) == {Groundtype::GP_OUTOFSCREEN} then
            <...>
            escape {PS_DEAD};       @escape
        end
    end
end
]]

The `escape` statement @NN(escape) aborts the execution block of the `Pingu`
instance, removing it from its associated pool automatically.
Hence, a dynamic instance that terminates naturally leaves no traces in the 
program.

<!--
The language ensures, at compile time, that there are no possible dangling 
pointers to organisms (TODO: not discussed here).
Céu distinguishes between *aliases* and *pointers*.
Aliases are similar to C++ references [[![X]][cpp_reference]], while pointers 
are the same as in C and C++.
Aliases respect static scoping rules and can only be bound to variables defined 
on enclosing (wider) scopes.
For this reason, aliases are more restricted but safer than pointers.
Given that the control-flow statements of Céu cross event occurrences, scopes 
tend to last long and aliases are used extensively in programs.

Note that we can safely pass the `pingus` and the anonymous `PinguHolder` 
because they are in the same scope.
[cpp_reference]: https://en.wikipedia.org/wiki/Reference_%28C%2B%2B%29

To complete the previous example, the class `World` also declares a 
`PinguHolder`:

```
class World with
    <...>
do                                  // X4
    <...>
    pool IPingu[] pingus;           // X1
    var PinguHolder _ with          // X2
        this.pingus = &pingus;
        <...>
    end;                            // X3
    <...>
end                                 // X5
class World with
    <...>
do
    <...>
    var CPingusCounter pcounter = <...>;        // always active and
    var ButtonPanel button_panel;               // reacting directly without
    var SmallMap smallmap with                  // a dispatching hierarchy
        <...>
    end;
    <...>
end
```
-->

<a name="lifespan-hierarchies-summary"/>
<br/>

<div class="summary">
**Summary**:

Lexical lifespan for static instances and natural termination for dynamic
instances provide some advantages in comparison to lifespan hierarchies through
containers:

- Lexical scope makes an abstraction lifespan explicit in the source code.
- The memory for static instances is known at compile time.
- Natural termination makes an instance innocuous and, hence, susceptible to
  immediate reclamation.
- Containers require explicit manipulation of pointers/references.

<!--
Overall, passive objects of C++ impose a dispatching architecture that makes 
the reasoning about the program harder:

# case-1
* When containers are part of a dispatching chain, it gets even harder to track 
  what objects are dispatched:
  one has to "simulate" the program execution and track calls to `add` and
  `remove`.
* For objects with dynamic lifespan, calls to `add` must always have matching 
  calls to `remove`:
  missing calls to `remove` lead to memory and CPU leaks (see the *lapsed listener* problem below).
In Céu, entities that coexist with an enclosing class just need to be declared 
at the top-level block [[![X]][ceu_world_top]]:
Again, here we never manipulate references to deal with containers, or 
allocation and deallocation.
Also, all memory required for static instances is known at compile time.

# case-2
-->
</div>

[cpp_gamesession_containers]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/screens/game_session.cpp#L76
[cpp_pingu_dead]:https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/actions/splashed.cpp#L48
[gpp_lapsed_listener]: http://gameprogrammingpatterns.com/observer.html#don't-worry,-i've-got-a-gc

[ceu_sprite_update]: https://github.com/fsantanna/pingus/blob/ceu/ceu/engine/display/sprite.ceu#L71
[ceu_sprite_redraw]: https://github.com/fsantanna/pingus/blob/ceu/ceu/engine/display/sprite.ceu#L94
[ceu_world_top]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/game/game.ceu#L198

[ceu_pinguholder]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/game/game.ceu#L163
[ceu_pingu]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/pingu.ceu#L54
[ceu_ipingu]: https://github.com/fsantanna/pingus/blob/ceu/ceu/main.ceu#L95
[ceu_world_pingus]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/world.ceu#L114
[ceu_world_pinguholder]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/world.ceu#L116
[ceu_pingu_dead]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/game/pingu/pingu.ceu#L98

[ceu_main_outermost]: https://github.com/fsantanna/pingus/blob/ceu/ceu/main.ceu#L249

@SEC[[signaling,
## Signaling Mechanisms
]]

Entities often need to communicate explicitly through a signaling 
mechanism, especially if there is no hierarchy relationship between them.

@SEC[[signaling_1,
### Pausing the World
]]

@FIG_NEW(pause-anim-opt.gif,
         Pausing the world.,
         350)

A click in the *Pause* button at the bottom right of the screen pauses all
world objects, such as the clouds and pingus, but not other elements, such as
the *Armageddon* button animation (@FIG_REF[[pause-anim-opt.gif]]).
The button indicates its state with different backgrounds and is also affected
when the player presses `P` on the keyboard.

#### C++

In C++, the class `PauseButton` [[![X]][cpp_pausebutton]] handles mouse clicks
to toggle the world pause state which is checked when redrawing the button:

@CODE_LINES[[language=CPP,
PauseButton::PauseButton(GameSession s, <...>):
    RectComponent(<...>),
    session(s),
    background("core/buttons/hbuttonbgb"),          @spr_1
    backgroundhl("core/buttons/hbuttonbg"),         @spr_2
    <...>
{
    <...>
}

void PauseButton::on_click (<...>) {                @click_1
    session->set_pause(!session->get_pause());
}                                                   @click_2

void PauseButton::draw (<...>) {                    @draw_1
    <...>
    if (session->get_pause()) {
        gc.draw(backgroundhl, <...>);
    } else {
        gc.draw(background, <...>);
    }
    <...>
}                                                   @draw_2
]]

The mouse callback `on_click` @NN(click_1,-,click_2) toggles the world pause
state.
Depending on the current state, the method `draw` @NN(draw_1,-,draw_2) chooses
the appropriate background sprite loaded in the class constructor
@NN(spr_1,-,spr_2).

The class `GameSession` [[![X]][cpp_gamesession]] handles keyboard presses and
applies the pause state to the world:

@CODE_LINES[[language=CPP,
void GameSession::on_pause_press () {
    set_pause(!get_pause());
}

void GameSession::update_server (<...>) {
    <...>
    if (!get_pause()) {     @pause
        <...>
        server->update();   @update
    }
    <...>
}
]]

The call to the world `update` @NN(update) only applies if the world is not
paused @NN(pause).
Since `update` methods propagate throughout the world hierarchy, skipping the
call to the root method effectively pauses the world.

#### Céu

As discussed in @SEC_REF[[dispatching-hierarchies-1]], entities in Céu
do not update through dispatching hierarchies, but directly in reaction to
events.
This way, the pausing technique applied to the implementation in C++ is not
effective.


In Céu, the button communicates with the rest of the application through the
event `go_pause_toggle` [[![X]][ceu_input_ui]]:

@CODE_LINES[[language=CEU,
<...>
var& RectComponent c = spawn RectComponent(<...>);
spawn do
    loop do
        watching go_pause_toggle do     @but_11
            spawn Sprite(<...>, "core/buttons/hbuttonbgb");
            await c.component.on_click; @clk_1
            emit go_pause_toggle;       @emt_1
        end                             @but_12
        watching go_pause_toggle do     @but_21
            spawn Sprite(<...>, "core/buttons/hbuttonbg");
            await c.component.on_click; @clk_2
            emit go_pause_toggle;       @emt_2
        end                             @but_22
    end
end
<...>
]]

The button toggles between showing the light @NN(but_11,-,but_12) and dark
@NN(but_21,-,but_22) backgrounds.
The background changes when the button is clicked @NN(clk_1,,clk_2) or
when `go_pause_toggle` is emitted from a keyboard press @NN(but_11,,but_21).
The button also broadcasts `go_pause_toggle` whenever it is clicked
@NN(emt_1,,emt_2).

The pause mechanism relies on two update events, `main.dt` and `game.dt`,
for the main application and game world, respectively [[![X]][ceu_input_evt]]:

@CODE_LINES[[language=CEU,
event void go_pause_toggle;
<...>
spawn do
    var bool is_paused = false;
    par do
        every go_pause_toggle do            @tog_1
            is_paused = not is_paused;
        end                                 @tog_2
    with
        every outer.main.dt do              @dt1
            <...>
            if not is_paused then           @chk_1
                emit outer.game.dt(<...>);  @dt
            end                             @chk_2
        end                                 @dt2
        <...>
    end
end
<...>
]]

Whenever `go_pause_toggle` occurs, the local state variable `is_paused`
is toggled @NN(tog_1,-,tog_2).
Also, whenever `main.dt` occurs @NN(dt1,-,dt2), `game.dt` is emitted only if
the world is not paused @NN(chk_1,-,chk_2).
World entities react to `game.dt`, while all other entities react to `main.dt`.

Since all world entities are `Sprite` instances, the abstraction interface
receives a reference to use as its update event [[![X]][ceu_sprite]].
On creation, world and non-world sprites pass distinct events, e.g.:

* The *Bomber* action uses `game.dt` [[![X]][ceu_bomber_sprite]], since it is a
   world entity.
* The *Armageddon* button uses `main.dt` [[![X]][ceu_armageddon_sprite]], since
  it should not pause with the world entities.

#### Discussion

Events decouple .

Using events helps on decoupling the button and pause as well as not using the
dispatching hierarchy.

@SEC[[signaling_2,
### Global Keys and the Options Menu
]]

@FIG_NEW(options-anim-opt.gif,
         The *Mouse Grab* configuration option.,
         350)

The *Mouse Grab* option restricts the mouse movement to the game window
boundaries (@FIG_REF[[options-anim-opt.gif]]).
The option can be set anywhere in the game by pressing *Ctrl-G*.
In addition, the *Options* menu has a check box to toggle the *Mouse Grab*
option with mouse clicks while still responding to *Ctrl-G* presses.

@FIG_NEW(events.png,
         Mutual dependecy between signals.,
         450)

The implementations in C++ and Céu use a signalling mechanism to connect the
key presses, the check box, and a configuration manager that applies the
appropriate side effects in the game (i.e., restrict the mouse movement).
@FIG_REF(events.png) illustrates how the mutual notifications create a 
dependency cycle between the configuration manager and the check box.

<!-- CPP-GRAB -->

#### C++

In C++, the class `GlobalEvent` [[![X]][cpp_global_event]] detects *Ctrl-G*
presses and invokes the callback `config_manager.set_mouse_grab`:

@CODE_LINES[[language=CPP,
void GlobalEvent::on_button_press (<...>) {
    <...>
    switch (event.keysym.sym) {
        case SDLK_g:
            if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL]) {
                config_manager.set_mouse_grab(          @ctrl_g_cpp
                    !config_manager.get_mouse_grab());
            }
            break;
        <...>
    }
}
]]

<a name="cpp_config-manager"/>

The class `ConfigManager` [[![X]][cpp_config_manager]] uses a `boost::signal`
[[![X]][boost_signal]] to notify the application when the new configuration is
applied:

@CODE_LINES[[language=CPP,
boost::signals2::signal<void(bool)> on_mouse_grab_change;   // definition in `config_manager.h` @signal_def

void ConfigManager::set_mouse_grab (bool v) {   @set_mouse_grab
    <...>
    if (v != get_mouse_grab()) {                @if_1
        <...>   // the actual "grab" effect
        on_mouse_grab_change(v);                @signal
    }                                           @if_2
}
]]

The `if` enclosing the signal emission @NN(if_1,-,if_2) breaks the dependency 
cycle of @FIG_REF(events.png) and prevents the infinite execution loop.

The class `CheckBox` [[![X]][cpp_check_box]] also uses a `boost::signal` to 
notify the application on changes:

<a name="cpp_check-box"/>

@CODE_LINES[[language=CPP,reset=false,
boost::signals2::signal<void (bool)> on_change;   // definition in `check_box.hpp`

void CheckBox::set_state (bool is_on, bool send_signal) {   @last_argument
    <...>   // switches the check box state
    if (send_signal) {      @if_cb_1
        on_change(is_on);
    }                       @if_cb_2
}
]]

Again, the `if` enclosing the signal emission @NN(if_cb_1,-,if_cb_2) breaks the 
dependency cycle of @FIG_REF(events.png).

The class `OptionMenu` [[![X]][cpp_option_menu]] creates the dependency loop by
connecting the two signals:

@CODE_LINES[[language=CPP,reset=false,
typedef std::vector<boost::signals2::connection> Connections;   // definition in `option_menu.hpp`
Connections connections;                                        // definition in `option_menu.hpp`

OptionMenu::OptionMenu() :
    connections(),
    mousegrab_box(),
    <...>
{
    mousegrab_box = new CheckBox(<...>);
    connections.push_back(                              @bind_11
        config_manager.on_mouse_grab_change.connect(
            std::bind(&CheckBox::set_state, mousegrab_box, <...>, false); @bind_false
        )
    );                                                  @bind_12
    connections.push_back(                              @bind_21
        mousegrab_box->on_change.connect(
            std::bind(&ConfigManager::set_mouse_grab, &config_manager, <...>);
        )
    );                                                  @bind_22
    <...>

}

OptionMenu::~OptionMenu() {     @destr_1
    for (Connections::iterator i=connections.begin(); i!=connections.end(); ++i) {
        (*i).disconnect();
    }
}                               @destr_2
]]

The constructor binds
the signal `config_manager.on_mouse_grab_change` to the callback method
           `mousegrab_box->set_state`
           @NN(bind_11,-,bind_12),
and also
the signal `mousegrab_box->on_change` to the callback method
           `config_manager.set_mouse_grab`
           @NN(bind_21,-,bind_22).
This way, every time the `ConfigManager` signals `on_mouse_grab_change`
(ln. @N(signal) [up](#cpp_config-manager)), `set_state` is implicitly called.
The same happens between the signal `on_change` in the `CheckBox` and the 
method `set_mouse_grab` in the `ConfigManager`
(ln. @N(set_mouse_grab) [up](#cpp_config-manager)).

Note that the signal binding to call `CheckBox::set_state` @NN(bind_false) 
receives a fixed value `false` as the last argument to prevent infinite
execution (ln. @N(last_argument) [up](#cpp_check-box)).

The destructor @NN(destr_1,-,destr_2) breaks the connections when the *Option*
screen terminates.

<!-- CEU-GRAB -->

#### Céu

Céu supports *internal events* as a signalling mechanism between lines of
execution.
A *Ctrl-G* key press broadcasts the internal event
`config_manager.go_mouse_grab` to the application [[![X]][ceu_global_event]]:

@CODE_LINES[[language=CEU,reset=false,
spawn do
    var _SDL_KeyboardEvent&& e;
    every e in SDL_KEYDOWN do
        var _u8&& keystate = _SDL_GetKeyState(null);
        <...>
        if e:keysym.sym == {SDLK_g} then
            if ((keystate[{SDLK_LCTRL}] as bool) or (keystate[{SDLK_RCTRL}] as bool)) then
                emit config_manager.go_mouse_grab(    @ctrl_g_ceu
                        not ({config_manager.get_mouse_grab()} as bool));
            end
        end
        <...>
    end
end
]]

The configuration manager [[![X]][ceu_config_manager]] just needs to react to
`go_mouse_grab` continuously to perform the *grab* effect:

@CODE_LINES[[language=CEU,reset=false,
data ConfigManager with
    event bool go_mouse_grab;
end
var ConfigManager config_manager = val ConfigManager(_);

spawn do
    var bool v;
    every v in config_manager.go_mouse_grab do
        <...>   // the actual "grab" effect
    end
end
]]

The `CheckBox` [[![X]][ceu_check_box]] exposes the event `go_click` for 
notifications in both directions, i.e., from the class to the application and 
*vice versa*:

@CODE_LINES[[language=CEU,reset=false,
data ICheckBox with
    var   bool is_on;
    event bool go_click;
end

code/await CheckBox (<...>) -> (var ICheckBox checkbox) -> FOREVER do
    checkbox = val ICheckBox(<...>);
    <...>
    par do
        every c.component.on_click do                   @every_1
            emit checkbox.go_click(not checkbox.is_on); @dir_class_app
        end                                             @every_2
    with
        loop do                                         @loop_1
            <...>   // switches the check box state
            checkbox.is_on = await checkbox.go_click;   @dir_app_class
        end                                             @loop_2
    end
end
]]

The class reacts to external clicks continuously @NN(every_1,-,every_2) to 
broadcast the event `go_click` @NN(dir_class_app).
It also react continuously to `go_click` in another line of execution
@NN(loop_1,-,loop_2), which awakes from notifications from the first line of
execution or from the application.

The `OptionMenu` [[![X]][ceu_option_menu]] connects the two events as 
follows:

@CODE_LINES[[language=CEU,reset=false,
code/await OptionMenu <...> do
    <...>
    var& CheckBox b2 = <...>;
    spawn do
        par do
            var bool v;
            every v in outer.config_manager.go_mouse_grab do    @loop_11
                emit b2.checkbox.go_click(v);
            end                                                 @loop_12
        with
            var bool v;
            every v in b2.checkbox.go_click do                  @loop_21
                emit outer.config_manager.go_mouse_grab(v);
            end                                                 @loop_22
        end
    end
    <...>
end
]]

The two loops in parallel handle the connections in opposite directions:
from the configuration manager to the check box @NN(loop_11,-,loop_12);
and
from the check box to the configuration manager @NN(loop_21,-,loop_22).

When the *Option* screen terminates, the connections break automatically.

Note that the implementation in Céu does not check event emits to break the
dependency cycle and prevent infinite execution.
Due to the [stack-based execution for internal events][ceu_stack] in Céu,
programs with mutually-dependent events cannot create infinite execution loops.

<a name="signaling-summary"/>
<br/>

<div class="summary">
**Summary**:

First-class internal events of Céu provide some advantages in comparison to 
Boost signals:

* They use the same convenient syntax of external events (e.g., `emit`,
    `await`, `every`, etc.).
* They never create infinite dependency loops.
* They do not require explicit unbinding.
</div>

[boost_signal]:http://www.boost.org/doc/libs/1_60_0/doc/html/signals2.html
[cpp_global_event]:https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/global_event.cpp#L34
[ceu_global_event]:https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/global_event.ceu
[cpp_config_manager]:https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/config_manager.cpp#L182
[ceu_config_manager]:https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/config_manager.ceu#L4
[cpp_option_menu]:https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/screens/option_menu.cpp#L79
[ceu_option_menu]:https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/options/options.ceu#L23
[cpp_check_box]:https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/components/choice_box.cpp#L54
[ceu_check_box]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/options/check_box.ceu
[ceu_stack]: http://fsantanna.github.io/ceu/out/manual/v0.20/#internal-reactions

[cpp_pausebutton]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/components/action_button.cpp#L135
[cpp_gamesession]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/pingus/screens/game_session.cpp#L159
[ceu_input_ui]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/game/input.ceu#L162
[ceu_input_evt]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/game/input.ceu#L17
[ceu_sprite]: https://github.com/fsantanna/pingus/blob/ceu/ceu/engine/display/sprite.ceu#L20
[ceu_bomber_sprite]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/game/pingu/actions/bomber.ceu#L22
[ceu_armageddon_sprite]: https://github.com/fsantanna/pingus/blob/ceu/ceu/pingus/screens/game/input.ceu#L117

<!--
<a name="wall-clock-timers"/>

SEC[[
## Wall-Clock Timers
]]

5. **Wall-Clock Timers**
    Wall-clock timers measure the passage of time from the real world
    (e.g., *10 seconds*) such as for periodic sampling and timeout watchdogs.

    The double click behavior above uses a timeout of 1 second to restart.

<a name="wall-clock-timers-summary"/>
<br/>

<div class="summary">
**Summary**:
</div>

<a name="pausing"/>

SEC[[
## Pausing
]]

6. **Pausing**
    Pausing allows parts of the game to temporarily suspend execution or
    reactions to incoming events.

    In Pingus, the player can press a button in the screen to toggle between 
    pause and resume.

<a name="pausing-summary"/>
<br/>

<div class="summary">
**Summary**:
</div>

<a name="resource-acquisition-and-release"/>

SEC[[
## Resource Acquisition and Release
]]

7. **Resource Acquisition and Release**
    External resources, such as configuration files and saved games,
    must be acquired and properly released.

    TODO

<a name="resource-acquisition-and-release-summary"/>
<br/>

<div class="summary">
**Summary**:
</div>

# Quantitative Analysis

## Code Size

## Memory

## CPU
-->

<!-- ************************************ -->

<!--

[[![X]][see pausing]]
[[![X]][cpp_engine]]: removed files

** remove = death

* tracking: follow the source code
    ** execution order, redraw, sort

composition over inheritance


doesn't need

- tradeoff here is clear
    - indirect reaction + dynamic scope
    vs
    - direct reaction + lexical scope

- class hier and dispatch hier

- no lapsed listener
- no dynamic allocation or GC
- static reasoning
- inheritance vs composition
- no hier b/c orgs can react directly to the env
    + lexical scope
- animation w/ the dispatch path

## The Game Loop

The *game loop* determines the general structure of virtually all games 
[[![X]][gpp_gameloop]] (Pingus is no different [[![X]][pingus-gameloop]]):

```
while (true)
{
    processInput();
    update();
    render();
}
```

> A game loop runs continuously during gameplay.
> Each turn of the loop, it processes user input without blocking, updates the 
> game state, and renders the game.
> It tracks the passage of time to control the rate of gameplay.

The `update` function does the hard work, dealing with the state of all game 
entities, and has to execute as fast as possible to keep real-time 
responsiveness to input events.
However, short-lived functions such as `update` do not retain local variables 
and control-flow state across consecutive invocations.
In this sense, they eliminate any vestige of structured programming, becoming 
*our generation's goto* [[![X]][goto]].

[gpp_gameloop]: http://gameprogrammingpatterns.com/game-loop.html
[pingus_gameloop]: https://github.com/Pingus/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/screen/screen_manager.cpp#L172
[goto]: http://tirania.org/blog/archive/2013/Aug-15.html


## The Synchronous Concurrency Model

```
    initialize state;
    while (true) do
        read inputs;
        update state;
        write outputs;
    end
```

The game loop:
`ScreenManager::display`
https://github.com/fsantanna/pingus/blob/7b255840c201d028fd6b19a2185ccf7df3a2cd6e/src/engine/screen/screen_manager.cpp#L164

```
void ScreenManager::display() {
    Uint32 old = SDL_GetTicks();

    while (!screens.empty()) {
        // READ INPUTS
        Uint32 now = SDL_GetTicks();
        Uint32 delta = now - old;
        old = now;
        <read-other-inputs>

        // UPDATE STATE
        screens.back()->update(delta);
        <update-other-inputs>

        // WRITE OUTPUTS
        screens.back()->draw(<...>);
        Display::flip_display();
    }
}
```

## Céu

<!-
- control
    = Accidental complexity
- not pure functions

- end of document
    - GC, why gcc doesn't solve: lapsed listeners
        - static mem, orgs, aliases, lexical scope
    - GC also doesn't solve resources:
Garbage collecting resources (file handles, etc):

This is a very different question, because resource freeing has observable 
consequences beyond performance and memory consumption - unlike garbage 
collection, which is justified by the realization that if you don't have any 
pointers to a value in memory, then it can simply dissapear (or not) without 
observable consequences.

For example, if you have a file handle open for writing, then other applications can't open that file. You want such resource usage to be clear and deterministic, so that files don't just remain open for a random duration depending on the garbage collector's internals.

In general, I would not advocate garbage collection of OS resources or any other thing requiring explicit cleanup. For example, I think that Java/C# finalizers are a misguided idea, because they have observable, nondeterministic consequences. That is exactly the sort of feature a high-level, secure language should avoid. The bizarre finalization state diagrams for those languages should be enough to indicate that something is wrong here!

For resource freeing, constructs which guarantee that every resource is freed look promising (think of wrapping a file handle in an abstraction like a Haskell State monad). Or just plain old handles with explicit closing (and thus the risk of not closing a handle, closing it when it's not open, etc).

Garbage collection without type information?

C++ garbage collection would greatly benefit if it was assisted by the compiler. During a scan, only pointers to objects need to be scanned. Unfortunately, it is not possible without assistance from the compiler. That is the reason third-party solutions are not good enough for performance-intensive applications.

Memory management and resource management is not the same. Resources other than memory are usually few in a program and not interconnected. A program may have files, sockets, windows and other types of handles, but usually these handles are 'dead ends', i.e. they do not contain references to other handles. Memory, on the other hand, is a totally different beast: a block of memory usually contains references to other blocks of memory. So the idea of finalizers is a bad one, since resources other than memory should be freed by RAII techniques (for deterministic resource management), but memory should be handled by garbage collector.

I will say it again, because no one seemed to notice it: could it be that there exists a "calculus" for resource management?
By Achilleas Margaritis at Fri, 2006-02-03 11:46 | login or register to post comment

->

## Idioms

<!-
All patterns relate to event handling and control flow in games, and we argue 
how Céu offers more appropriate abstractions than existing languages.

### State Machines

    At any time, the program can only be in a single state, which globally and 
    univocally represents the current XXX.
    This unique and global view of the program state as a single value has 
maintenance scalability problems (AKA the *state explosion phenomena*).
    - state machines vs await
    The more XXX, the more states to track, state explosion.
    The machine transits from state to state

, as we encode the fact

 It can change from one state to another when initiated by a triggering event 
or condition; this is called a transition.
    - state machines vs await

Map the whole behavior into a single number is a problem.
    - hierarchical machines can help, but still has this mapping property
        - locally unscallabe
        - still a data sultion to a control problem
        - explicit state machines vs implicit
        - incremental implementation requires global changes
            - in ceu, its just compositions

### Hierarchies

    - class hierarchies/dispatching vs await
    - difficult to track origin
        - has to traverse the whole hierarchy
    - used in pause
    - not class hier
    - flat dispatching (diagrama mostrando os dois)

 (e.g., a key press or expiring timer)
    - class hierarchies/dispatching vs await
    - lexical scope
    - visitor pattern

    - new /delete

### Continuations

    - CPS vs return continue
        - screen trasnitions
        - story screen advancing
        - story screen -> credits screen
        - WM pingu path traversal
        - worse w/o closures
    Typically, screen transitions are not static

### Signaling Between Entities

    - signaling/f pointer vs events
        - option save on click
        - global events double direction
            - key/mouse-but events
                - sendo que but tem que ficar highlight ou nao
        - verificar caso do option_menu
            - configuracao pode ser alterada por fora?
        - worse w/o closures

### Wall-Clock Timers
    - wall-clock time
        - story chars
        - credits up
        - fps
Activities that involve reactions to \emph{wall-clock time}%
\footnote{
By wall-clock time we mean the passage of time from the real world, measured in 
hours, minutes, etc.
}
appear in typical patterns of embedded development, such as timeout watchdogs 
and sensor samplings.
However, support for wall-clock time is somewhat low-level in existing 
languages, usually through timer callbacks or ``sleep'' blocking calls.

### Pausing
    - pause
        - alternative is again hierarchies which enable/disable forwarding
    - problem with timer callbacks

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
->

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

<!-
Most of

 `engine/`, a level `editor/`



Total Physical Source Lines of Code (SLOC) = 39,986

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
->


<!-
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

-->

-------------------------------------------------------------------------------

## Conclusion

`TODO`
