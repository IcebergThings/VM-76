design.md
=========

- [ ] VMDE: draw engine
	- [ ] init_engine
	- [ ] update
	- [ ] get_frame_count
	- [ ] get_fps
	- [ ] GRect: rectangle
		- [ ] Shape: vertices
		- [ ] Texture
		- [ ] Color: per vertex, RGBA
		- [ ] draw
	- [ ] GResPic: texture resources
		- [ ] load_pic
	- [ ] Audio: audio *drawing* support
		- [x] stop: stop_wave in fact
		- [x] play_wave
			- [x] triangle
			- [x] sine
			- [x] white_noise
		- [x] play_sound
			- [x] play_sound
			- [x] play_loop
- [ ] VMGS: game system
	- main 主程序
	- gui 图形界面
	- map 地图
	- chunk 区块
	- player 玩家组件
	- physics 物理组件
		- 里面的结构没想好
		- 很有可能是一个巨大的physics.cpp和physics.hpp
	- tiles 方块（模版？）
		- tile 普通的方块
		- xxx 特殊的方块，继承tile

---

- global.hpp: project-wide header
- global.cpp: project-wide global variable table
