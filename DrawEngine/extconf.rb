# extconf.rb
require 'mkmf'

extension_name = "VMDE"

dirs = ["/usr/local/lib"]
$libs += "-lGLEW -lGLU -lglfw -lrt -lXrandr -lXinerama -lXi -lXcursor -lXrender -lGL -lm -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp"

dir_config(extension_name)
create_makefile(extension_name)
