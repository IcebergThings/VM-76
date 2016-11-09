#!/usr/bin/env ruby
# 如果在第一行加了这个 → #!/usr/bin/cat ← 就不能用了。
# ruby: no Ruby script found in input (LoadError)
#==============================================================================
# ■ make.rb
#------------------------------------------------------------------------------
#   往日生成按至鄙，而今欲有所依赖，奈何扳手不开怀。
#   工程图日新月异，Makefile有百弊，是适时一返梦里。
#   ——Frog Chen《伪·浣溪沙·构建》
#==============================================================================

require 'json'

module Kernel
	#--------------------------------------------------------------------------
	# ● Windoge or *nix, this is a problem.
	#--------------------------------------------------------------------------
	@@windows = (ENV["OS"] == "Windows_NT")
	#--------------------------------------------------------------------------
	# ● 请按任意键继续. . .
	#--------------------------------------------------------------------------
	def pause
		system "pause" if @@windows
		# We think *nix users always have an open terminal.
		# They're forced to check their windows frequently.
	end
end

class WindogixMake
	#--------------------------------------------------------------------------
	# ● 常量与类变量
	#--------------------------------------------------------------------------
	SHORTCUT_NAME = (@@windows ? "make.bat" : "make")
	DEFAULT_OPTIONS = {
		"output" => "b.out",
		"link the target" => true,
		"link together with" => [],
		"target type" => "executable",
	}
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
				All ‘-Idir’ options will be translated to ‘-isystem dir’ and no way back.
				-Wall, -Wextra and some necessary switches are built into this script.
				I recommend you to do ‘make.rb clean’ when build options are changed.
				No spaces or special characters in filenames. You know that.
			EOF
		else
			puts "= #{dir} ="
			File.open("#{dir}/flags.txt", "r", :external_encoding => "utf-8") do |f|
				options = JSON.parse(f.readline)
				unless options.empty?
					puts "Additional options configured for this project: "
					if options["target type"]
						puts "\tTarget type = #{options["target type"]}"
					end
					if options["output"]
						puts "\tTarget filename = #{options["output"]}"
					end
					if options["link the target"] == false
						puts "\tNo linking"
					end
					if options["link together with"]
						puts "\tLink together with:"
						options["link together with"].each do |v|
							puts "\t\t#{v}"
						end
					end
				end
				f.each_line { |l| puts l }
			end
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
			if FileTest.exist?("#{@argv[0]}/flags.txt")
				Dir.chdir(@argv.first)
				args = nil
				File.open(SHORTCUT_NAME, "r") do |f|
					f.readline
					args = f.readline[4..-1].split(" ")
				end
				WindogixMake.new(args).main
				Dir.chdir("..")
			elsif @argv[0] == "clean"
				clean
			else
				puts "not a project: #{@argv[0]}"
			end
			return
		end
		# make clean; make c l e a n
		if [@argv.join, @argv.first].include?("clean")
			clean
			return
		end
		# 调查选项
		options = nil
		File.open("flags.txt", "r", :external_encoding => "utf-8") do |f|
			options = f.readline
		end
		options = DEFAULT_OPTIONS.merge(JSON.parse(options))
		# 获取参数
		compiling_args = []
		linking_args = []
		@argv.each do |arg|
			case arg
			when /^-[mgDUO]|^-Wp,/
				compiling_args << arg
			when /^-[lL]|^-Wl,|\.(?:[ao]|lib)$/
				linking_args << arg
			when /^-I/
				compiling_args << "-isystem"
				compiling_args << arg[2, arg.size]
			when "-pthread"
				compiling_args << arg
				linking_args << arg
			else
				puts "option not recognized: #{arg}"
				return
			end
		end
		sources = Dir.glob("**/*.cpp")
		objects = []
		# 如果不这么搞就会无法编译
		sources.each do |source_name|
			name = File.basename(source_name, ".cpp")
			object_name = "#{name}.o"
			objects << object_name
			# Look, I'm Ruby 'make' now!
			if File.exist?(object_name)
				next if File.mtime(object_name) > File.mtime(source_name)
			end
			command = %w(g++ -c -Wall -Wextra -Wno-unused-parameter -std=c++14 -o)
			command.push(object_name, source_name)
			command.concat(compiling_args)
			make command
		end
		# 如果不这么搞也会无法编译
		if options["link the target"]
			case options["target type"]
			when "executable", "exe"
				command = %w(gcc -o)
			when "archive", "a"
				command = %w(ar -r)
				linking_args.clear
			when "so", "shared", "shared object", "dll"
				command = %w(gcc -shared -o)
			end
			command.push(options["output"])
			command.concat(objects)
			options["link together with"].each do |project|
				together = Dir["../#{project}/*.a"]
				together = Dir["../#{project}/*.o"] if together.empty?
				command.concat(together)
			end
			command.concat(linking_args)
			make command
		end
		# 如果没有错误，输出脚本以便下次运行
		output_shortcut
	end
	#--------------------------------------------------------------------------
	# ● 模拟Make的一步行动
	#    command : 参数数组
	#--------------------------------------------------------------------------
	def make(command)
		print "▎"
		puts command.reject { |a| /^[A-Za-z]:|^(-|\.?\.?[\/\\]\w)/ === a }.join(" ")
		success = system(*command)
		unless success
			if @@windows
				system "title !! ERROR !!"
			else
				# I kindly alert the user.
				print "\a"
			end
			puts "△ when executing this command:\n#{command.join(" ")}"
			pause
			exit
		end
	end
	#--------------------------------------------------------------------------
	# ● make clean
	#--------------------------------------------------------------------------
	def clean
		Dir["**/*.{o,a,dll}"].each do |filename|
			puts "removing #{filename}"
			File.delete(filename)
		end
	end
	#--------------------------------------------------------------------------
	# ● 输出能够快捷地执行本脚本的批处理文件
	#   如果批处理存在且比本脚本更新，则不做事。
	#--------------------------------------------------------------------------
	def output_shortcut
		if FileTest.exist?(SHORTCUT_NAME)
			return if File.mtime(SHORTCUT_NAME) > File.mtime(ABSOLUTE_FILE)
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
	ABSOLUTE_FILE = File.expand_path(__FILE__)
	WindogixMake.new.main if __FILE__ == $0
rescue
	p $!
	pause
end
