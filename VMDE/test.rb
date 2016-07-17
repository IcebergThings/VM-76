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

obj = VMDE::GDrawable.new
obj_b = obj.bind
VMDE::Audio.play_triangle(440.0)

i = 0
loop do
	i += 1
	i = 0 if i > 255
	sleep 1
	p [obj.get_visible(obj_b), i]
	obj.set_visible(obj_b, true) if i == 128
	obj.set_visible(obj_b, false) if i == 255
	VMDE.brightness = 0.5 + i / 510.0
	VMDE.matrix2D
	VMDE.update
end
