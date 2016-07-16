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

i = 0
loop do
	i += 1
	i = 0 if i > 255
	p obj.get_visible(obj_b).to_s + " " + i.to_s

	if i==128
		obj.set_visible(obj_b, true)
	end
	if i==255
		obj.set_visible(obj_b, false)
	end

	VMDE.set_brightness(0.5 + i / 255.0)
	VMDE.matrix2D
	VMDE.update
end
