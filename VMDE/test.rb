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

obj=VMDE::GDrawable.new
obj_b=obj.bind

i = 0
loop do
	i+=1
	if (i > 255)
		i = 0
	end
	puts i
	
	VMDE.set_brightness(0.5 + i.to_f / 255)
	VMDE.matrix2D
	VMDE.update
end
