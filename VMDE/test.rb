#==============================================================================
# ■ test.rb
#------------------------------------------------------------------------------
#   VMDE的测试代码，但并不是一般意义上的单元测试。
#   因此可以直接通过ruby test.rb的方式暴力运行，而测试也是暴力测试的。
#   printf & puts大法好！
#==============================================================================

require "./VMDE.so"

rc = VMDE.init(870,540)
if rc==0 

	puts "Ruby initializes here"
	loop do
		VMDE::matrix2D
		VMDE::update
	end

end
