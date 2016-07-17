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
obj1_b = obj1.bind([])
obj2 = VMDE::GDrawable.new
obj2_b = obj2.bind([])
VMDE::Audio.play_triangle(440)
VMDE.wtftestingcode([
	1.1, 2.2, 3.3,
	4.4, 5.5, 6.6,
	7.7, 8.8, 9.9,
	100.0, 200.0, 300.0,
])
exit
i = 0
loop do
	i += 1
	i = 0 if i > 255
	p obj1.get_visible(obj1_b).to_s + " " + i.to_s

	if i==128
		obj1.set_visible(obj1_b, true)
		obj2.set_visible(obj2_b, true)
	end
	if i==255
		obj1.set_visible(obj1_b, false)
		obj2.set_visible(obj2_b, false)
	end

	VMDE.set_brightness(0.5 + i / 255.0)
	VMDE.matrix2D
	VMDE.update
end
