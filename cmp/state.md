- data vs control variables
- examples of data-only variables
- examples of control-only vars
- examples of dual (sprites frame/finish)

- hierarchies vs scope (draw,update)

- no pointers, less identifiers

# BASHER
sprite          U/D,frame               [1], frame
basher_c        update                  loop-i
                check every 3 frames    await UPD * 3   (+)
                bash every 6 frames     [2], i%2 == 0   (+)
first_bash      update                  [1]
draw()          call sprite             ---
set_action()    drown,walker,faller     escape          (+)

# BLOCKER
sprite          U/D                     [1], anonymous
draw()          call sprite             ---
set_action()    faller                  escape

# BOMBER
sprite          U/Ð,frame,fin           [1], frame,fin
explo_surf      drw                     [3], anonymous
particle_thrown check a single frame    await UPD           (+)
sound_played    check a single frame    await UPD           (+)
gfx_exploded    check a single frame    await UPD           (+)
colmap_exploded check a single frame    await UPD           (+)
set_action()    drown,splashed          escape
set_status()    dead                    escape

# BRIDGER
walk_sprite     U/D,fin,rst             [3], do-anonymous   (+)
build_sprite    U/D,frame,fin,rst       [3], frame,fin
block_build     check single frame      await UPD
set_action()    walker,waiter           escape

# CLIMBER (- continuous)
sprite          U/D                     [1], anonymous
set_action()    walker                  escape

# DIGGER
sprite          U/D                     [1], anonymous
delay_count     dig every 4 frames      [2], i%4
set_action()    walker                  escape

# DROWN
sprite          U/D, fin                [1], do-anonymous
set_status()    dead                    escape

# EXITER
sprite          U/D, fin                [1], do-anonymous
sound_played    check single frame      on start
set_status()    exiter                  escape

# FALLER
faller          U/D,is_tumbling()       [3], anonymous
tumbler         U/D,is_tumbling()       [3], anonymous
mover           update                  [2],mover.ok_collided
set_action()    drown,splashed,walker   escape

# FLOATER
sprite          U/D                     [1], anonymous
set_action()    walker                  escape

# JUMPER
sprite          D                       [1], anonymous
set_action()    faller                  escape

# LASERKILL
sprite          U/D, fin                [1], do-anonymous
set_status()    dead                    escape

# MINER
sprite          U/D                     [1], anonymous
delay_count     mine every 4 frames     [2], i%4
set_action()    walker                  escape

# SLIDER
sprite          U/D                     [1], anonymous
set_action()    faller,walker           escape

# SPLASHED
sprite          U/D, fin                [1], do-anonymous
particle_thrown check single frame      on start
set_status()    dead                    escape

# WAITER
sprite          U/D                     [1], anonymous
countdown       until 0                 [1] (similar)

# WALKER
walker          U/D                     [1], anonymous
floaterlayer    U/D                     [3], anonymous (only if fall_action)

================

# SPIKE
surface         U/D                     [4], await sfc
killing         control collision       ---                 (+)
draw()          call sprite             ---

# GUILLOTINE
sprite_kill_r/l U/D,killing,fin,frame   [4], await
sprite_idle     U/D                     [4], anonymous
direction       update sprite           ---
killing         control collision       ---

# ENTRANCE
last_release                            [2]
last_direction                          [2]

# EXIT
sprite          U/D                     [1], anon
flag            U/D                     [1], anon

# LASEREXIT
surface         U/D,killing,fin         [4], await sfc
killing         control collision       ---

# SMASHER (+)
smashing                                sequence
downwards                               sequence
count           up,down                 local

================

# PINGU_PARTICLE_HOLDER
surface         U/D                     [1], anon
livetime        control lifetime, if    loop
particles       manual add/rem          pool

# SMOKE_PARTICLE_HOLDER
surface         U/D                     [1], anon
livetime        control lifetime, if    loop
particles       manual add/rem          pool

================

# SMALLMAP
scroll_mode                             ---

# SMALLMAP_IMAGE (+)
surface         U/D externally          [2], anon
update_count                            [2]


================

# PLAYFIELD

(+)
mouse_scrolling                         ---
scroll_center                           [3]
old_state_pos                           [3]
