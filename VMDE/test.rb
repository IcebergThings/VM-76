require "./VMDE.so"

rc = VMDE::init_engine(640, 480)

if (rc==0)
	puts 'Ruby here init'
	while true
		VMDE::update
	end

end
