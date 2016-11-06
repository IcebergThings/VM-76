# 如果在第一行加了这个 → #!/usr/bin/cat ← 就不能用了。
# ruby: no Ruby script found in input (LoadError)
#==============================================================================
# ■ make.rb
#------------------------------------------------------------------------------
#   往日生成按至鄙，而今欲有所依赖，奈何扳手不开怀。
#   工程图日新月异，Makefile有百弊，是适时一返梦里。
#   ——Frog Chen《伪·浣溪沙·构建》
#==============================================================================

module Kernel
	#--------------------------------------------------------------------------
	# ● 历史遗留问题
	#--------------------------------------------------------------------------
	@@windows = (ENV["OS"] == "Windows_NT")
	#--------------------------------------------------------------------------
	# ● 请按任意键继续. . .
	#--------------------------------------------------------------------------
	def pause
		system @@windows ? "pause" : "echo -n 'PRESS BUTTON'; read -n 1"
	end
end

class WindogixMake
	#--------------------------------------------------------------------------
	# ● 常量与类变量
	#--------------------------------------------------------------------------
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
	def self.display_help(dir = nil)
		if dir.nil?
			puts <<~EOF
				= Usage =
				> ruby make.rb <directory> [clean]
				> ruby make.rb --help [directory]
				> ruby ..\\make.rb -D... -I... *.a *.lib -L... -l...
				> ruby ..\\make.rb clean <garbage>
				> rem 作为开发者，你不应该在库目录的名称中包含空格。
				All ‘-Idir’ options will be translated to ‘-isystem dir’ and no way back.
				-Wall, -Wextra and some necessary switches are built into this script.
				I recommend you to do ‘make.rb clean’ when build options are changed.
				No spaces or special characters in filenames. You know that.
			EOF
		else
			puts "= #{dir} ="
			puts File.read("#{dir}/flags.txt", :external_encoding => "utf-8")
		end
	end
	#--------------------------------------------------------------------------
	# ● 主程序
	#--------------------------------------------------------------------------
	def main
		# display help when used incorrectly
		if @argv.empty? or ["--help", "-help", "-h"].include?(@argv.first)
			WindogixMake.display_help(@argv[1])
			return
		end
		# make <project>
		if !FileTest.exist?("flags.txt")
			Dir.chdir(@argv.first)
			args = nil
			File.open(SHORTCUT_NAME, "r") do |f|
				f.readline
				args = f.readline[4..-1].split(" ")
			end
			WindogixMake.new(args).main
			Dir.chdir("..")
			return
		end
		# make clean; make c l e a n
		if [@argv.join, @argv.first].include?("clean")
			Dir["*.o"].each { |filename| File.delete(filename) }
			return
		end
		# 获取参数
		compiling_args = []
		linking_args = []
		@argv.each do |arg|
			case arg
			when /^-[gDUO]|^-Wp,/
				compiling_args << arg
			when /^-[lL]|^-Wl,|\.(?:[ao]|lib)$/
				linking_args << arg
			when /^-I/
				compiling_args << "-isystem"
				compiling_args << arg[2, arg.size]
			else
				puts "option not recognized: #{arg}"
				return
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
			f.write @@windows ? <<~BATCH : <<~SHELL
				@echo off
				rem #{@argv.join(" ")}
				ruby ../make.rb %1 ^
				#{@argv.join(" ^\n")}
			BATCH
				#!/bin/sh
				### #{@argv.join(" ")}
				ruby ../make.rb $1 \\
				#{@argv.join(" \\\n")}
			SHELL
			f.chmod(0755)
		end
	end
end

#------------------------------------------------------------------------------
# ◇ “各种定义结束后，从这里开始实际运行。”
#------------------------------------------------------------------------------
begin
	WindogixMake.new.main if __FILE__ == $0
rescue
	p $!
	pause
end
