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
         - [ ] ⓜ stop
         - [ ] ⓜ play_wave
             - [ ] triangle
             - [ ] sine
             - [ ] white_noise
         - [ ] ⓜ play_sound: load before playing
         - [ ] ⓜ play_music: load | play

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
	# This will "upload" the info to VMDE, if you don't use update, VMDE will not update this object but still render it (as what it is before)
	# ...
	VMDE.update
	# After calling VMDE.update, it will render all the things in the render list and refresh the screen.
end
```

\* 这已经过期了，请参考VMDE/test.rb的用法。被坑了吧
