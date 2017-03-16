var& RRect rect = spawn RRect(IRRect(Rect(-105+40+40,-5,38,60),
                                     AnchorXY(AnchorX.Center(), AnchorY.Bottom()),
                                     &outer.main.rect,
                                     AnchorXY(AnchorX.Right(), AnchorY.Bottom()),
                                     _, _));
var& RectComponent component = spawn RectComponent(&rect.pub);
spawn Sprite_from_name(&rect.pub, "core/buttons/hbuttonbgb", &outer.main.dt);
do
    var&? Sprite_from_name s = spawn Sprite_from_name(&rect.pub, "core/buttons/armageddon_anim", &outer.main.dt);
    s!.sprite.frame_delay = 0;
    s!.sprite.frame = 7;
    loop do
        await component.component.on_primary_button_pressed;
        watching 1s do
            await component.component.on_primary_button_pressed;
            break;
        end
    end
end
do
    spawn Sprite_from_name(&rect.pub, "core/buttons/armageddon_anim", &outer.main.dt);
    emit outer.game.go_armageddon;
    await FOREVER;
end
