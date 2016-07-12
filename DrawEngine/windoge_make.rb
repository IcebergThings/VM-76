# 如果加了这个 → !/usr/bin/cat
# 就不能用了。ruby: no Ruby script found in input (LoadError)
if ARGV.size != 4
	puts "= Example ="
	puts '> ruby windoge_make.rb "C:\\Program Files\\Ruby" 2.3.0 D:\\glfw-3.2.bin.WIN32 D:\\glew-1.13.0'
	exit
end
# 模拟Make的行动
def make(command)
	puts command
	exit unless system command
end
ruby_path, ruby_version, glfw_path, glew_path = ARGV
sources = Dir.glob("*.cpp")
objects = []
dll_name = "VMDE.dll" # .dll: required to build DLL (maybe?)
so_name = "VMDE.so" # .so: required for Ruby to work
# some ugly hacks for Windoge
# 如果不这么搞就会LoadError
sources.each do |source_name|
	name = File.basename(source_name, ".cpp")
	command = "g++ -c #{source_name} -o #{name}.o -O3 -Wall -Wextra"
	command << " -I#{ruby_path}\\include\\ruby-#{ruby_version}"
	command << " -I#{ruby_path}\\include\\ruby-#{ruby_version}\\i386-mingw32"
	command << " -D__MINGW_USE_VC2005_COMPAT -D_FILE_OFFSET_BITS=64"
	command << " -fno-omit-frame-pointer"
	# GLFW & GLEW
	command << " -DGLFW_DLL -DGLEW_STATIC"
	command << " -I#{glfw_path}\\include"
	command << " -I#{glew_path}\\include"
	make command
	objects << "#{name}.o"
end
command = "gcc #{objects.join(" ")} -s -shared -o #{dll_name} -lstdc++"
command << " -L#{ruby_path}\\lib"
command << " -Wl,--enable-auto-image-base,--enable-auto-import"
command << " -lmsvcrt-ruby230 -lshell32 -lws2_32 -liphlpapi -limagehlp -lshlwapi"
command << " -L#{glfw_path}\\lib-mingw-w64"
command << " -L#{glew_path}\\lib\\Release\\Win32"
command << " -Wl,-subsystem,windows -lglfw3dll -lglew32s -lopengl32"
make command
# For tools like exeScope, the file must have correct suffix.
# However for Ruby, the DLL must have corrupted suffix.
# 而这就是PY交易。
File.rename(dll_name, so_name)
