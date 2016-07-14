#==============================================================================
# ■ extconf.rb
#------------------------------------------------------------------------------
#   标准的Ruby C/C++扩展配置程序。
#   事实证明这仅能在Linux环境下无障碍使用，Windows用户请使用windoge_make.rb。
#==============================================================================

require 'mkmf'

extension_name = "VMDE"

dirs = ["/usr/local/lib"]
$libs += %w(
	-lGLEW -lGLU -lglfw -lrt -lXrandr -lXinerama -lXi -lXcursor -lXrender
	-lGL -lm -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2
	-lXxf86vm -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp
).join(" ")

dir_config(extension_name)
create_makefile(extension_name)
