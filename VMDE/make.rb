# 如果在第一行加了这个 → #!/usr/bin/cat ← 就不能用了。
# ruby: no Ruby script found in input (LoadError)
#==============================================================================
# ■ make.rb
#------------------------------------------------------------------------------
#   往日生成按至鄙，而今欲有所依赖，奈何扳手不开怀。
#   工程图日新月异，Makefile有百弊，是适时一返梦里。
#   ——Frog Chen《伪·浣溪沙·构建》
#==============================================================================

class WindogixMake
	#--------------------------------------------------------------------------
	# ● 常量与类变量
	#--------------------------------------------------------------------------
	@@windows = (ENV["OS"] == "Windows_NT")
	SHORTCUT_NAME = (@@windows ? "make.bat" : "make")
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
			> ruby windoge_make.rb -D... -I... *.a *.lib -L... -l...
			> rem 作为开发者，你不应该在库目录的名称中包含空格。
			Typical arguments for Windoge:
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
				-O3 -Wl,--enable-auto-image-base,-subsystem,windows
				D:\\SOIL\\lib\\libSOIL.a
				-LD:\\bin -llibstdc++
				-LD:\\glfw-3.2.bin.WIN32\\lib-mingw-w64
				-LD:\\glew-1.13.0\\lib\\Release\\Win32
				-lglfw3dll -lglew32s -lopengl32 -lportaudio_x86 -lvorbisfile
			And for *nix:
				-I~/SOIL/lib/include -I. -I..
				$(pkg-config --cflags --libs glfw3 gl glm glew portaudio-2.0 ogg)
				-lstdc++ -lSOIL
			All ‘-Idir’ options will be translated to ‘-isystem dir’ and no way back.
			libstdc艹 is for exceptions and other random stuff; omit it if not used.
			-Wall, -Wextra and some necessary switches are built into this script.
			‘D:\\bin’ above is where you put all libraries (.lib files) and DLLs.
		EOF
	end
	#--------------------------------------------------------------------------
	# ● 请按任意键继续. . .
	#--------------------------------------------------------------------------
	def pause
		system @@windows ? "pause" : "echo -n 'PRESS BUTTON'; read -n 1"
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
			case arg
			when /^-[gDUO]|^-Wp,/
				compiling_args << arg
			when /^-[lL]|^-Wl,|\.(?:a|lib)$/
				linking_args << arg
			when /^-I/
				compiling_args << "-isystem"
				compiling_args << arg[2, arg.size]
			else
				puts "option not recognized: #{arg}"
				exit
			end
		end
		sources = Dir.glob("*.cpp")
		objects = []
		dll_name = "VMDE.dll"
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
		# 如果没有错误，输出脚本以便下次运行
		output_shortcut
	end
	#--------------------------------------------------------------------------
	# ● 模拟Make的一步行动
	#    command : 参数数组
	#--------------------------------------------------------------------------
	def make(command)
		print "▎"
		puts command.reject { |a| /^[A-Za-z]:|^\/|^-/ === a }.join(" ")
		success = system(*command)
		unless success
			system "title !! ERROR !!" if @@windows
			puts "△ when executing this command:\n#{command.join(" ")}"
			pause
			exit
		end
	end
	#--------------------------------------------------------------------------
	# ● 输出能够快捷地执行本脚本的批处理文件
	#   如果批处理存在且比本脚本更新，则不做事。
	#--------------------------------------------------------------------------
	def output_shortcut
		if FileTest.exist?(SHORTCUT_NAME)
			return if File.mtime(SHORTCUT_NAME) > File.mtime(__FILE__)
		end
		File.open(SHORTCUT_NAME, "w") do |f|
			f.write <<~EOF
				#{@@windows ? "@" : "#!/bin/sh\n"}ruby make.rb ^
				#{@argv.join(" ^\n")}
			EOF
			f.chmod(0755)
		end
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
WindogixMake.new.main if __FILE__ == $0
