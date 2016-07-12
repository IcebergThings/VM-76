require "./VMDE.so"

VMDE::init_engine(640, 480)
while true
	VMDE::update
end
