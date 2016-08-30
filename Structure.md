结构点标记下
============

Structure
---------

Ⓜ = module / namespace
Ⓒ = class
ⓜ = method / function

- [ ] ○ VMDE: draw engine
    - [ ] ⓜ init_engine
    - [ ] ⓜ update
    - [ ] ⓜ get_frame_count
    - [ ] ⓜ get_fps
    - [ ] ⓜ GRect: rectangle
        - [ ] Shape: vertices
        - [ ] Texture
        - [ ] Color: per vertex, RGBA
        - [ ] ⓜ draw
    - [ ] Ⓒ GResPic: texture resources
        - [ ] ⓜ load_pic
    - [ ] Ⓜ Audio: audio *drawing* support
        - [x] ⓜ stop: stop_wave in fact
        - [x] ⓜ play_wave
            - [x] triangle
            - [x] sine
            - [x] white_noise
        - [x] ⓜ play_sound
            - [x] play_sound
            - [x] play_loop

它是怎么绘图的
--------------

```ruby
draw_me = GDrawable.new
hide_me = GDrawable.new
thing_I_want_to_draw3 = GDrawable.new
# ...
draw_me.add_to_render_list
hide_me.remove_from_render_list
# ...
def all_the_render_in_the_game
	# ...
	draw_me.update
	# This will “upload” the info to VMDE.
	# If you don't use update, VMDE will not update this object,
	# but still render it as what it was before.
	# ...
	VMDE.update
	# After calling VMDE.update, it will render everything
	# in the render list and refresh the screen.
end
```

\* 这已经过期了，请参考VMDE/test.rb的用法。被坑了吧
