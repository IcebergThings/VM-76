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

obj1 = VMDE::GDrawable.new
obj1_b = obj1.bind([50.0,50.0,0.0,50.0,200.0,0.0,200.0,200.0,0.0,50.0,50.0,0.0,200.0,50.0,0.0,200.0,200.0,0.0])
obj2 = VMDE::GDrawable.new
obj2_b = obj2.bind([600.0,600.0,0.0,450.0,450.0,0.0,600.0,475.0,0.0])
# VMDE::Audio.play_triangle(440)

obj2.set_visible(obj2_b,true)

i = 0
loop do
	i += 1
	i = 0 if i > 255
	p obj1.get_visible(obj1_b).to_s + " " + i.to_s

	if i==128
		obj1.set_visible(obj1_b, true)
	end
	if i==255
		obj1.set_visible(obj1_b, false)
	end

	VMDE.set_brightness(0.5 + i / 255.0)
	VMDE.matrix2D
	VMDE.update
end
