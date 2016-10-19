#!/usr/bin/env ruby
#==============================================================================
# ■ extconf.rb
#------------------------------------------------------------------------------
#   标准的Ruby C/C++扩展配置程序。
#   事实证明这仅能在Linux环境下无障碍使用，Windows用户请使用windoge_make.rb。
#==============================================================================

require 'mkmf'
require 'rbconfig'

extension_name = "VMDE"

dirs = ["/usr/local/lib"]

if RbConfig::CONFIG['host_os'].index("linux")
	puts "Linux system detected"
	$CFLAGS += " -Wall -O4 -O -pthread -malign-double "
else
	$CFLAGS += " -Wall -O3 "
end

$LOCAL_LIBS+="../lib/SOIL/lib/libSOIL.a"
$INCLUDE += " -I../lib/SOIL/src "

$libs += %w(
	-lGLEW -lGLU -lglfw -lrt -lportaudio -lasound -lXrandr -lXinerama -lXi -lXcursor -lXrender
	-lGL -lm -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2
	-lXxf86vm -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp -lvorbisfile
).join(" ")
$libs = append_library($libs, "supc++")

dir_config(extension_name)
create_makefile(extension_name)
