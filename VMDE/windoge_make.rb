# 如果在第一行加了这个 → #!/usr/bin/cat ← 就不能用了。
# ruby: no Ruby script found in input (LoadError)
#==============================================================================
# ■ windoge_make.rb
#------------------------------------------------------------------------------
#   君住Linux中，我住Windows里。日日思君不见君，共通编译器。
#   警告几时休，错误何时已？但愿我心藏君境，定不负移植技。
#   ——Frog Chen《伪·卜算子·编译》
#==============================================================================

require 'io/console'

class WindogeMake
	#--------------------------------------------------------------------------
	# ● 常量
	#--------------------------------------------------------------------------
	BATCH_FILENAME = "windoge_make.bat"
	#--------------------------------------------------------------------------
	# ● 初始化对象
	#--------------------------------------------------------------------------
	def initialize(argv = ARGV)
		@argv = argv
	end
	#--------------------------------------------------------------------------
	# ● 输出帮助
	#--------------------------------------------------------------------------
	def display_help
		puts <<~EOF
			= Example =
			> ruby windoge_make.rb C:\\Ruby23 2.3.0 D:\\glfw-3.2.bin.WIN32 D:\\glew-1.13.0 D:\\glm-0.9.7.5
			> rem 作为开发者，你不应该在此类目录的名称中包含空格。
		EOF
	end
	#--------------------------------------------------------------------------
	# ● 请按任意键继续. . .
	#--------------------------------------------------------------------------
	def pause
		print "请按任意键继续……"
		$stdin.getch
	end
	#--------------------------------------------------------------------------
	# ● 主程序
	#--------------------------------------------------------------------------
	def main
		# display help when used incorrectly
		if @argv.size != 5
			display_help
			return
		end
		# 获取参数
		ruby_path, ruby_version, glfw_path, glew_path, glm_path = @argv
		sources = Dir.glob("*.cpp")
		objects = []
		# some ugly hacks for Windoge
		dll_name = "VMDE.dll" # .dll: required to build DLL (maybe?)
		so_name = "VMDE.so" # .so: required for Ruby to work
		# 如果不这么搞就会无法编译
		sources.each do |source_name|
			name = File.basename(source_name, ".cpp")
			object_name = "#{name}.o"
			objects << object_name
			# Look, I'm Ruby Make now!
			if File.exist?(object_name)
				next if File.mtime(object_name) > File.mtime(source_name)
			end
			command = "g++ -c #{source_name} -o #{name}.o -O3 -Wall -Wextra"
			# Ruby
			command << " -I#{ruby_path}\\include\\ruby-#{ruby_version}"
			command << " -I#{ruby_path}\\include\\ruby-#{ruby_version}\\i386-mingw32"
			# GLFW, GLEW & GLM headers and macros
			command << " -DGLFW_DLL -DGLEW_STATIC"
			command << " -I#{glfw_path}\\include"
			command << " -I#{glew_path}\\include"
			command << " -I#{glm_path}"
			make command
		end
		command = "gcc #{objects.join(" ")} -s -shared -o #{dll_name}"
		# libstdc艹 for exceptions and other random stuff
		command << " -llibstdc++"
		# Ruby
		command << " -L#{ruby_path}\\lib"
		command << " -Wl,--enable-auto-image-base,-subsystem,windows"
		command << " -lmsvcrt-ruby230 -lshell32 -lws2_32 -liphlpapi -limagehlp -lshlwapi"
		# GLFW & GLEW ~~footers~~ libraries
		command << " -L#{glfw_path}\\lib-mingw-w64"
		command << " -L#{glew_path}\\lib\\Release\\Win32"
		command << " -lglfw3dll -lglew32s -lopengl32"
		make command
		# For tools like exeScope, the file must have correct suffix.
		# However for Ruby, the DLL must have corrupted suffix.
		# 而这就是PY交易。
		File.rename(dll_name, so_name)
		# 如果没有错误，输出脚本以便下次运行
		output_batch
	end
	#--------------------------------------------------------------------------
	# ● 模拟Make的一步行动
	#--------------------------------------------------------------------------
	def make(command)
		puts command
		success = system command
		if not success
			system "title !! ERROR !!"
			pause
			exit
		end
	end
	#--------------------------------------------------------------------------
	# ● 输出能够快捷地执行本脚本的批处理文件
	#   如果批处理存在且比本脚本更新，则不做事。
	#--------------------------------------------------------------------------
	def output_batch
		if FileTest.exist?(BATCH_FILENAME)
			return if File.mtime(BATCH_FILENAME) > File.mtime(__FILE__)
		end
		File.write(BATCH_FILENAME, "ruby windoge_make.rb #{@argv.join(" ")}")
	end
#------------------------------------------------------------------------------
# ◇ 为了避免不好的事发生而添加的类级错误处理
#------------------------------------------------------------------------------
rescue
	p $!
	pause
	exit
end

#------------------------------------------------------------------------------
# ◇ “各种定义结束后，从这里开始实际运行。”
#------------------------------------------------------------------------------
WindogeMake.new.main if __FILE__ == $0
