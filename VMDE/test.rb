#!/usr/bin/env ruby
#==============================================================================
# ■ test.rb
#------------------------------------------------------------------------------
#   VMDE的测试代码，但并不是一般意义上的单元测试。
#   因此可以直接通过ruby test.rb的方式暴力运行，而测试也是暴力测试的。
#   printf & puts大法好！
#==============================================================================

require "./VMDE.so"

VMDE.init(880, 540)
puts "Ruby initializes here"

obj2_arr = [600.0,600.0,0.0,450.0,450.0,0.0,600.0,475.0,0.0]

obj1 = VMDE::GDrawable.new
obj1_b = obj1.bind([50.0,50.0,0.0,50.0,200.0,0.0,200.0,200.0,0.0,50.0,50.0,0.0,200.0,50.0,0.0,200.0,200.0,0.0])
obj2 = VMDE::GDrawable.new
obj2_b = obj2.bind(obj2_arr)
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
	obj2.update_vertices(obj2_b, obj2_arr_t)

	VMDE.brightness = 0.5 + i / 510.0

	VMDE.matrix2D
	VMDE.update
end
