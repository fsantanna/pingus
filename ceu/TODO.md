- warnings de ceu
- bridger offset
- rename SpritePingus => Sprite
- testar climber, wall-mode-activation
- previous_action as argument to action constr.
- drown,exiter,smashed
- public Pingu::current_action
- passar update de actions=>sprites + evento is_finished
    - checar se precisa mesmo de restart
    - provavelmente vai precisar de um pause ou is_active
      por causa de left/right e outros sprites que sao mutuamente excludentes
- SDL_DT p/ pingus/actions/sprites
- remove pointers, use parent orgs for "me"

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

# BUGs

## multiple `delete`
    ` _sprite_impl_ok`
## copy constructor
