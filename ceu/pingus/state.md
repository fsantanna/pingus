- data vs control variables
- examples of data-only variables
- examples of control-only vars
- examples of dual (sprites frame/finish)

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

# DIGGER
sprite          U/D                     [1], anonymous
delay_count     dig every 4 frames      [2], i%4
set_action()    walker

# DROWN
sprite          U/D, fin                [1], do-anonymous
set_status()    dead                    escape
