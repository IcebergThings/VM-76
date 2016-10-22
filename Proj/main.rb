#!/usr/bin/env ruby

require '../VMDE/VMDE.so'

class Tile

	attr_accessor :x0, :y0

	def initialize(tid, x0, y0)
		x = tid / 16
		y = tid % 16
		tile_size = 1 / 16.0
		@tile_vbo = [
			0.0,0.0,0.0,    1.0, 1.0, 1.0, 1.0,  x * tile_size, y * tile_size,
			0.0,24.0,0.0,   1.0, 1.0, 1.0, 1.0,  x * tile_size, (y + 1.0) * tile_size,
			24.0,24.0,0.0,  1.0, 1.0, 1.0, 1.0,  (x + 1.0) * tile_size, (y + 1.0) * tile_size,
			24.0,0.0,0.0,   1.0, 1.0, 1.0, 1.0,  (x + 1.0) * tile_size, y * tile_size
		]

		@draw_item = VMDE::GDrawable.new
		@draw_item_b = @draw_item.bind(@tile_vbo, [0,1,3, 1,2,3])
		@draw_item.set_visible(@draw_item_b, true)
		move_to(x0.to_f, y0.to_f)
	end

	def move_to(x0, y0)
		self.x0 = x0
		self.y0 = y0
		@draw_item.model_translate(@draw_item_b, x0, y0, 0.0)
	end

	def move(x1, y1)
		self.x0 = self.x0 + x1
		self.y0 = self.y0 + y1
		@draw_item.model_translate(@draw_item_b, self.x0, self.y0, 0.0)
	end

	def render
		@draw_item.render(@draw_item_b)
	end
end

class Craft

	def initialize
		VMDE.init(880, 540)
		puts "Craft initialized"
		# Load tex0 as terrian
		VMDE.load_tex(0, "../Media/terrain.png")

		# Status monitoring
		Thread.new {
			loop do
				sleep 1
				puts "FPS : #{VMDE.fps}, Average Frame Time : #{VMDE.frame_time}ms"
			end
		}

		@tiles = []
		for x in 0..15
			for y in 0..15
				@tiles.push Tile.new(x * 16 + y, x * 24.0, y * 24.0);
			end
		end

	end

	# 这玩意每60分之一秒跑一次
	def game_loop
		loop do
			VMDE.matrix2D
			VMDE.prepare

			for a in @tiles
				a.move(0.01,0.01)
				a.render
			end

			VMDE.update
		end
	end

rescue
	p $!
	pause
	exit
end

main = Craft.new
main.game_loop
