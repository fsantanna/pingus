code/await Story (void) -> bool do
    spawn Wood();
    var& RRect rect = spawn RRect(IRRect(Rect(0,0, _,_),
                                      AnchorXY(AnchorX.Center(), AnchorY.Center()),
                                      &outer.main.rect,
                                      AnchorXY(AnchorX.Center(), AnchorY.Center()),
                                      _, _));
    spawn Sprite_from_name(&rect.pub, "core/menu/blackboard", &outer.main.dt);
    var& RRect r1 = spawn RRect(IRRect(Rect(-50,-60, _,_),
                                       AnchorXY(AnchorX.Right(), AnchorY.Bottom()),
                                       &rect.pub,
                                       AnchorXY(AnchorX.Right(), AnchorY.Bottom()),
                                       _, _));
    var& SurfaceButton next = spawn SurfaceButton(&r1.pub, "core/misc/next", "core/misc/next", "core/misc/next_hover");
    var IRRect r = val IRRect(Rect(-30,-30,_,_),
                              AnchorXY(AnchorX.Left(), AnchorY.Top()),
                              &outer.main.rect,
                              AnchorXY(AnchorX.Right(), AnchorY.Bottom()),
                              _, _);
    var&? LabelButton skip = spawn LabelButton(&r, "skip");
    if not ({globals::developer_mode} as bool) then
        //kill skip;
    end
    {
        static WorldmapNS::WorldmapStory story = WorldmapNS::WorldmapStory(reader);
        static std::vector<StoryPage> pages;
        story = WorldmapNS::WorldmapStory(reader);
        pages = story.get_pages();
        static std::string str1;
        str1 = story.get_title().c_str();
        Sound::PingusSound::play_music(story.get_music(), 0.7);
    }
    event void next_text;
    vector[] byte title = [] .. ({str1.c_str()} as _char&&);
    par do
        var int i;
        loop i in [0 <- {pages.size()}[ do
            {
                static StoryPage page;
                page = pages.at(@i);
            };
            vector[] byte text = [].."TODO";    // printing "" crashes
            {
                static std::string str2;
                str2 = page.image.res_name;
            }
            var& RRect r2 = spawn RRect(IRRect(Rect(0,10, _,_), AnchorXY(AnchorX.Center(), AnchorY.Bottom()), &rect.pub, AnchorXY(AnchorX.Center(), AnchorY.Center()), _, _));
            spawn Sprite_from_name(&r2.pub, {str2.c_str()} as _char&&, &outer.main.dt);
            par/or do
                var float time = 0;
                watching next_text do
                    loop do
                        var int dt = await outer.main.dt;
                        time = time + dt;
                        var int cur = (time*0.02) as int;
                        var int max = {UTF8::length(page.text)};
                        {
                            static std::string str3;
                            str3 = UTF8::substr(page.text, 0, Math::min(@max,@cur));
                        }
                        text = [] .. ({str3.c_str()} as _char&&);
                        if cur >= max then
                            break;
                        end
                    end
                end
                {
                    static std::string str4;
                    str4 = page.text;
                }
                text = [] .. ({str4.c_str()} as _char&&);
                await next_text;
            with
                every outer.main.redraw do
                    call ({Fonts::chalk_normal}.render as /nohold)( {origin_top_left}, outer.main.rect.abs.w/2 - 280, outer.main.rect.abs.h/2 + 35, &&text[0] as _char&&, *{Display::s_framebuffer}
                    );
                end
            end
        end
        escape true;
    with
        every next.component.on_click do
            emit next_text;
        end
    with
        every next.component.on_pointer_enter do
            call {Sound::PingusSound::play_sound}("tick");
        end
    with
        await skip!.component.on_click;
        escape true;
    with
        every skip!.component.on_pointer_enter do
            call {Sound::PingusSound::play_sound}("tick");
        end
    with
        loop do
            var _Input__ButtonEvent&& but = await ON_BUTTON_PRESSED;
            if but:name == {Input::ESCAPE_BUTTON} then
                escape false;       // is_click = false
            else/if but:name == {Input::FAST_FORWARD_BUTTON} then
                emit next_text;
            end
        end
    with
        every outer.main.redraw do
            call ({Fonts::chalk_large}.render as /nohold)( {origin_top_center}, outer.main.rect.abs.w/2, outer.main.rect.abs.h/2 - 200, &&title[0] as _char&&, *{Display::s_framebuffer}
            );
        end
    end
end
