require "./VMDE.so"

VMDE::init_engine(640, 480)
puts 'Ruby here init'
while true
	VMDE::update
end
