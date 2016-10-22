# 如果在第一行加了这个 → #!/usr/bin/cat ← 就不能用了。
# ruby: no Ruby script found in input (LoadError)
#==============================================================================
# ■ windoge_make.rb
#------------------------------------------------------------------------------
#   君住Linux中，我住Windows里。日日思君不见君，共通编译器。
#   警告几时休，错误何时已？但愿我心藏君境，定不负移植技。
#   ——Frog Chen《伪·卜算子·编译》
#==============================================================================

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
			= Usage =
			> ruby windoge_make.rb -D<...> -I<...> <...>.a <...>.lib -L<...> -l<...>
			> rem 作为开发者，你不应该在库目录的名称中包含空格。
			Typical arguments:
				-IC:\\Ruby23\\include\\ruby-2.3.0
				-IC:\\Ruby23\\include\\ruby-2.3.0\\i386-mingw32
				-DGLFW_DLL -DGLEW_STATIC
				-ID:\\glfw-3.2.bin.WIN32\\include
				-ID:\\glew-1.13.0\\include
				-ID:\\glm-0.9.7.5
				-ID:\\portaudio\\include
				-ID:\\libogg-1.3.2\\include
				-ID:\\libvorbis-1.3.5\\include
				-ID:\\SOIL\\src
				-O3 -LD:\\bin -llibstdc++
				-LC:\\Ruby23\\lib
				-Wl,--enable-auto-image-base,-subsystem,windows
				-lmsvcrt-ruby230 -lshell32 -lws2_32 -liphlpapi -limagehlp -lshlwapi
				D:\\SOIL\\lib\\libSOIL.a
				-LD:\\glfw-3.2.bin.WIN32\\lib-mingw-w64
				-LD:\\glew-1.13.0\\lib\\Release\\Win32
				-lglfw3dll -lglew32s -lopengl32 -lportaudio_x86 -lvorbisfile
			All ‘-Idir’ options will be translated to ‘-isystem dir’ and no way back.
			libstdc艹 is for exceptions and other random stuff; omit it if not used.
			-Wall, -Wextra and some required arguments are built into this script.
			‘D:\\bin’ above is where you put all libraries (.lib files) and DLLs.
		EOF
	end
	#--------------------------------------------------------------------------
	# ● 请按任意键继续. . .
	#--------------------------------------------------------------------------
	def pause
		system "pause"
	end
	#--------------------------------------------------------------------------
	# ● 主程序
	#--------------------------------------------------------------------------
	def main
		# display help when used incorrectly
		if @argv.empty? or @argv.include?("--help") or @argv.include?("-h")
			display_help
			return
		end
		# 获取参数
		compiling_args = []
		linking_args = []
		@argv.each do |arg|
			(
				case arg
				when /^-[gDIO]/
					compiling_args
				when /^-[lL]/
					linking_args
				when /\.(?:a|lib)$/
					linking_args
				else
					linking_args
				end
			) << arg
		end
		compiling_args.map! { |a| a[1, 1] == "I" ? ["-isystem", a[2, a.size]] : a }
		compiling_args.flatten!
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
			# Look, I'm Ruby 'make' now!
			if File.exist?(object_name)
				next if File.mtime(object_name) > File.mtime(source_name)
			end
			command = %w(g++ -c -Wall -Wextra -std=c++11 -o)
			command.push(object_name, source_name)
			command.concat(compiling_args)
			make command
		end
		command = %w(gcc -shared -o)
		command.push(dll_name)
		command.concat(objects)
		command.concat(linking_args)
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
	#    command : 参数数组
	#--------------------------------------------------------------------------
	def make(command)
		puts command.join(" ")
		success = system(*command)
		unless success
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
