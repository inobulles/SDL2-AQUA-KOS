
#!/bin/sh

# Here is a list of possible arguments and what they do:
	# no-note:    Skips the note that waits for user input
	# no-compile: Prevents the compilation of the KOS or the CW and prevents the running of the KOS at the end
	# no-update:  Prevents the updation of any of the components (CW, KOS and assembler)
	# remote:     DO NOT USE Connects to an external server that can freely modify your AQUA installation
	# execute:    Forces the execution of the KOS, even if "no-compile" set and will ALWAYS compile the KOS if "a.out" is not found
	# xephyr:     Launch KOS in Xephyr
	# xwm:        Launch KOS with its own WM

echo "INFO    Parsing arguments ..."

no_note=""
no_compile=""
no_update=""
remote=""
execute=""
xephyr=""
xwm=""

while test $# -gt 0; do
	if [ "$1" = "no-note"    ]; then no_note="true";    fi
	if [ "$1" = "no-compile" ]; then no_compile="true"; fi
	if [ "$1" = "no-update"  ]; then no_update="true";  fi
	if [ "$1" = "remote"     ]; then remote="true";     fi
	if [ "$1" = "execute"    ]; then execute="true";    fi
	if [ "$1" = "xephyr"     ]; then xephyr="true";     fi
	if [ "$1" = "xwm"        ]; then xwm="true";        fi
	
	shift
done

if [ "$remote" != "" ]; then
	set -e
	
	echo "WARNING This argument (remote) is extremely dangerous and should NEVER be used. That said, if your high, feel free to press enter to continue ..."
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

	if [ ! -d "asm/" ]; then
		echo "INFO    Installing the CW ..."
		set -e
		
		git clone https://github.com/inobulles/AQUA-CW
		mv AQUA-CW asm
	elif [ "$no_update" = "" ]; then
		echo "INFO    Updating the CW ..."
		
		cd asm/
		git pull origin master
		cd ../
		set -e
	fi

	if [ ! -f "ROM.zed" ]; then
		echo "WARNING ROM file was not found (ROM.zed), so attempting to install an extension (or run it if it already exists) so you can write C programs and compile them into ROMs ..."
		
		if [ "$no_note" = "" ]; then
			echo "NOTE    You will have to create assembler/c/code/main.c to compile, obviously (all your headers will go in the same folder). Press enter to continue ..."
			read a
		fi
		
		if [ ! -d "assembler/" ]; then
			echo "INFO    Installing the assembler ..."
			
			git clone https://github.com/inobulles/AQUA-assembler
			mv AQUA-assembler assembler
		elif [ "$no_update" = "" ]; then
			echo "INFO    Updating the assembler ..."
			
			cd assembler/
			git pull origin master
			cd ../
		fi
		
		echo "INFO    Compiling code ..."
		
		cd assembler/
		sh compile_c.sh
		cd ../
		mv assembler/ROM.zed ROM.zed
	fi
	
	if [ ! -f "a.out" ] && [ "$execute" != "" ] || [ "$no_compile" = "" ]; then
		echo "INFO    Compiling KOS ..."
		
		gcc glue.c -std=gnu99 -Wall \
			-Wno-unused-variable -Wno-unused-but-set-variable -Wno-main \
			-lSDL2 -lGL -lGLU -lSDL2_ttf -lm \
			$has_curl_args $has_discord_args $has_x11_args
		
		execute="true"
	fi
	
	xephyr_args=""
	
	if [ "$xephyr" != "" ]; then
		xephyr_bin=$(command -v Xephyr)
		if [ "$xephyr_bin" != "" ]; then
			echo "INFO    Opening Xephyr ..."
			execute=""
			xwm="true"
			xephyr_args="-- $xephyr_bin :1024 -ac -screen 800x600 -host-cursor"
			
		else
			echo "WARNING Xephyr was not found"
		fi
	fi
	
	if [ "$xwm" != "" ]; then
		echo "INFO    Opening xinit ..."
		execute=""
		
		cp src/xwm/env/xinitrc xinitrc
		xinit        ./xinitrc $xephyr_args
		rm             xinitrc
	fi
	
	if [ "$execute" != "" ]; then
		echo "INFO    Executing KOS ..."
		./a.out
	fi
fi

echo "INFO    build.sh finished"
exit 0
