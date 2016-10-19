#!/usr/bin/env ruby
#==============================================================================
# ■ test.rb
#------------------------------------------------------------------------------
#   VMDE的测试代码，但并不是一般意义上的单元测试。
#   因此可以直接通过ruby test.rb的方式暴力运行，而测试也是暴力测试的。
#   printf & puts大法好！
#==============================================================================

require "./VMDE.so"

def main

VMDE.init(880, 540)
puts "Ruby initializes here"

obj1_arr = [
	50.0,50.0,0.0,   1.0, 1.0, 0.0,
	50.0,300.0,0.0,  1.0, 0.0, 1.0,
	300.0,300.0,0.0, 0.0, 1.0, 1.0,
	300.0,50.0,0.0,  1.0, 1.0, 1.0
]
obj2_arr = [
	600.0,600.0,0.0, 1.0, 0.0, 0.0,
	450.0,450.0,0.0, 0.0, 1.0, 0.0,
	600.0,475.0,0.0, 0.0, 0.0, 1.0
]

obj1 = VMDE::GDrawable.new
obj1_b = obj1.bind(obj1_arr, [0,1,3, 1,2,3])
obj2 = VMDE::GDrawable.new
obj2_b = obj2.bind(obj2_arr, [0,1,2])

Thread.new {
	freq = 261.626
	freq_factor = 2.0 ** (1.0 / 12)
	10.times do |i|
		puts "##{i} - #{freq}Hz"
		VMDE::Audio.play_wave(:triangle, freq)
		freq *= freq_factor
		sleep 0.25
	end
	VMDE::Audio.stop
	VMDE::Audio.play_loop("../Media/loop-test.ogg")	
}
Thread.new {
	loop do
		sleep 1
		puts "FPS : #{VMDE.fps}"
	end
}

obj2.set_visible(obj2_b,true)

i = 0
loop do
	i += 1
	i = 0 if i > 255

	if i == 128
		obj1.set_visible(obj1_b, true)
	end
	if i == 255
		obj1.set_visible(obj1_b, false)
	end

	obj2_arr_t = obj2_arr.map { |v| (i.to_f / 128.0) * v }
	obj2.update_vertices(obj2_b, obj2_arr_t, [0,1,2])

	VMDE.brightness = 0.5 + i / 510.0

	VMDE.matrix2D
	VMDE.update
end

#------------------------------------------------------------------------------
# 为了避免不好的事发生而从windoge_make搬运来的类级错误处理
#------------------------------------------------------------------------------
rescue
        p $!
        pause
        exit
end

main
