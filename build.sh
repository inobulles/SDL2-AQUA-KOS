
#!/bin/sh

# Here is a list of possible arguments and what they do:
	# no-note:               Skips the note that waits for user input
	# no-compile:            Prevents the compilation of the KOS or the CW and prevents the running of the KOS at the end
	# no-update:             Prevents the updation of any of the components (CW, KOS and assembler)
	# no-run:                Prevents the running of the generated ROM file at the end, overrides execute flag
	# remote:                DO NOT USE Connects to an external server that can freely modify your AQUA installation
	# execute:               Forces the execution of the KOS, even if "no-compile" set and will ALWAYS compile the KOS if "a.out" is not found
	# app:                   Download and run a ROM file, the second argument being the identifier in the official AQUA Store ROM repository (sh build.sh app lasagna)
	# xephyr:                Launch KOS in Xephyr
	# xwm:                   Launch KOS with its own WM
	# debian:                Install all the packages for Debian
	# use-sdl-ttf:           Use deprecated SDL2 TTF library for rendering fonts
	# softpipe:              Use GALLIUM_DRIVER softpipe (fix for VMWare)
	# rom:                   Just execute the ROM. Nothing more
	# no-vertex-pixel-align: Compile surface structure without the vertex_pixel_align field (for ROMs compiled before this was added)

echo "INFO    Parsing arguments ..."

no_note=""
no_compile=""
no_update=""
no_run=""
remote=""
execute=""
xephyr=""
xwm=""
rom=""
use_sdl_ttf=""
no_vertex_pixel_align=""

while test $# -gt 0; do
	if [ "$1" = "no-note"               ]; then no_note="true";                 fi
	if [ "$1" = "no-compile"            ]; then no_compile="true";              fi
	if [ "$1" = "no-update"             ]; then no_update="true";               fi
	if [ "$1" = "no-run"                ]; then no_run="true";                  fi
	if [ "$1" = "remote"                ]; then remote="true";                  fi
	if [ "$1" = "execute"               ]; then execute="true";                 fi
	if [ "$1" = "xephyr"                ]; then xephyr="true";                  fi
	if [ "$1" = "xwm"                   ]; then xwm="true";                     fi
	if [ "$1" = "app"                   ]; then rom="$2";                       fi
	if [ "$1" = "use-sdl-ttf"           ]; then use_sdl_ttf="true";             fi
	if [ "$1" = "softpipe"              ]; then export GALLIUM_DRIVER=softpipe; fi
	if [ "$1" = "no-vertex-pixel-align" ]; then no_vertex_pixel_align="true";   fi
	
	if [ "$1" = "rom" ]; then
		no_update="true"
		no_compile="true"
		execute="true"
	fi
	
	if [ "$1" = "debian" ]; then
		sudo apt-get install -y libcurl4-openssl-dev
		sudo apt-get install -y libsdl2-2.0-0 libsdl2-dev
		
		if [ "$use_sdl_ttf" = "" ]; then
			sudo apt-get install -y libfreetype6-dev
		else
			sudo apt-get install -y libsdl2-ttf-2.0-0 libsdl2-ttf-dev
		fi
	fi
	
	shift
done

if [ "$remote" != "" ]; then
	set -e
	
	echo "WARNING This argument (remote) is extremely dangerous and should NEVER be used. That said, if you're high, feel free to press enter to continue ..."
	read a
	
	echo "INFO    Compiling client ..."
	cd remote/
	gcc main.c -std=gnu99 -Wall
	
	echo "WARNING Ok now this is your last chance to bail out ... No? Press enter to continue then ..."
	read a
	
	echo "INFO    Executing client ..."
	./a.out
	rm a.out
else
	if [ ! -d "kos" ]; then
		git clone https://github.com/inobulles/AQUA-KOS --depth 1 -b master
		mv AQUA-KOS kos
	fi
	
	if [ ! -d "kos/src/librequests" ]; then
		cd kos/src
		git clone https://github.com/inobulles/librequests --depth 1 -b master
		cd ../..
	fi
	
	if [ ! -d "root" ]; then
		git clone https://github.com/inobulles/AQUA-root --depth 1 -b master
		mv AQUA-root root
	fi
	
	if [ "$no_update" = "" ]; then
		cd kos
		git pull origin master
		cd ..
		
		cd root
		git pull origin master
		cd ..
	fi
	
	if [ "$no_compile" = "" ]; then
		echo "INFO    Testing for libraries ..."
		
		has_curl_args=""
		has_discord_args=""
		
		x11_link="-lX11"
		curl_link="-lcurl"
		discord_link="-L. -l:dynamic/libdiscord-rpc.so"
		
		if [ "$xwm" != "" ] || [ "$xephyr" != "" ]; then
			ld $x11_link && has_x11_args="-D__HAS_X11 $x11_link"
		fi
		
		ld $curl_link    && has_curl_args="-D__HAS_CURL $curl_link";
		ld $discord_link && has_discord_args="-D__HAS_DISCORD $discord_link"
	fi

	if [ -d "assembler/c/.hidden/lib" ] && [ "$no_update" = "" ]; then
		echo "INFO    Updating the standard library ..."
		
		cd assembler/c/.hidden/lib
		git pull origin master
		cd ../../../../
	fi

	if [ ! -d "kos/asm/" ]; then
		echo "INFO    Installing the CW ..."
		set -e
		
		git clone https://github.com/inobulles/AQUA-CW --depth 1 -b master
		mv AQUA-CW kos/asm
	elif [ "$no_update" = "" ]; then
		echo "INFO    Updating the CW ..."
		
		cd kos/asm/
		git pull origin master
		
		cd ../../
		set -e
	fi
	
	if [ "$rom" != "" ]; then
		echo "INFO    Downloading ROM file \"$rom\" ..."
		wget "https://raw.githubusercontent.com/inobulles/AQUA-Store/master/roms/$rom/rom.zed"
		mv rom.zed ROM.zed
		
		no_compile=""
		execute="true"
	fi
	
	if [ ! -f "ROM.zed" ]; then
		if [ "$no_compile" = "" ]; then
			echo "WARNING ROM file was not found (ROM.zed), so attempting to install an extension (or run it if it already exists) so you can write C programs and compile them into ROMs ..."
			
			if [ "$no_note" = "" ]; then
				echo "NOTE    You will have to create assembler/c/code/main.c to compile, obviously (all your headers will go in the same folder). Press enter to continue ..."
				read a
			fi
		fi
		
		if [ ! -d "assembler/" ]; then
			echo "INFO    Installing the assembler ..."
			
			git clone https://github.com/inobulles/AQUA-assembler --depth 1 -b master
			mv AQUA-assembler assembler
		elif [ "$no_update" = "" ]; then
			echo "INFO    Updating the assembler ..."
			
			cd assembler/
			git pull origin master
			cd ../
		fi
		
		if [ "$no_compile" = "" ]; then
			echo "INFO    Compiling code ..."
			
			cd assembler/
			sh compile_c.sh
			cd ../
			mv assembler/ROM.zed ROM.zed
		fi
	fi
	
	if [ ! -f "a.out" ] && [ "$execute" != "" ] || [ "$no_compile" = "" ]; then
		echo "INFO    Compiling KOS ..."
		
		if [ "$use_sdl_ttf" = "" ]; then
			font_library="-lfreetype -Ikos/src/functions/freetype2" # "-I/usr/include/freetype2"
		else
			font_library="-lSDL2_ttf -D__USE_SDL_TTF"
		fi
		
		original_width=800
		original_height=600
		
		vertex_pixel_align=-DSURFACE_VERTEX_PIXEL_ALIGN=1
		
		if [ "$no_vertex_pixel_align" != "" ]; then
			vertex_pixel_align=-DSURFACE_VERTEX_PIXEL_ALIGN=0
		fi
		
		gcc kos/glue.c -o a.out -std=gnu99 -Wall \
			-DKOS_ORIGINAL_WIDTH=$original_width -DKOS_ORIGINAL_HEIGHT=$original_height \
			-DKOS_CURRENT=KOS_DESKTOP \
			-Wno-unused-variable -Wno-unused-but-set-variable -Wno-main \
			-lSDL2 -lGL -lGLU -lm \
			$has_curl_args $has_discord_args $has_x11_args \
			$font_library $vertex_pixel_align
		
		execute="true"
	fi
	
	xephyr_args=""
	
	if [ "$xephyr" != "" ]; then
		xephyr_bin=$(command -v Xephyr)
		if [ "$xephyr_bin" != "" ]; then
			echo "INFO    Opening Xephyr ..."
			execute=""
			xwm="true"
			xephyr_args="-- $xephyr_bin :1024 -ac -screen 640x480 -host-cursor"
			
		else
			echo "WARNING Xephyr was not found"
		fi
	fi
	
	if [ "$xwm" != "" ]; then
		echo "INFO    Opening xinit ..."
		execute=""
		
		cp kos/src/xwm/env/xinitrc xinitrc
		sudo xinit       ./xinitrc $xephyr_args
		rm                 xinitrc
	fi
	
	if [ "$execute" != "" ] && [ "$no_run" = "" ]; then
		echo "INFO    Executing KOS ..."
		./a.out
	fi
fi

echo "INFO    build.sh finished"
exit 0
