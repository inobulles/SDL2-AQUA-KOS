
#!/bin/bash

if [ "$2" = "" ]; then
    has_curl_args=""
    has_discord_args=""
    
    curl_link="-lcurl"
    discord_link="-L. -l:dynamic/libdiscord-rpc.so"
    
    ld $curl_link    && has_curl_args="-D__HAS_CURL $curl_link";
    ld $discord_link && has_discord_args="-D__HAS_DISCORD $discord_link"
fi

if [ -d "assembler/c/.hidden/lib" ]; then
	cd assembler/c/.hidden/lib
	git pull origin master
	cd ../../../../
fi

if [ ! -d "asm/" ]; then
	set -e
	git clone https://github.com/inobulles/AQUA-CW
	mv AQUA-CW asm
else
	cd asm/
	git pull origin master
	cd ../
	set -e
fi

if [ ! -f "ROM.zed" ]; then
	echo "WARNING ROM file was not found (ROM.zed), so attempting to install an extension (or run it if it already exists) so you can write C programs and compile them into ROMs ..."
	
	if [ "$1" = "" ]; then
		echo "NOTE    You will have to create assembler/c/code/main.c to compile, obviously (all your headers will go in the same folder). Press enter to continue ..."
		read a
	fi
	
	if [ ! -d "assembler/" ]; then
		git clone https://github.com/inobulles/AQUA-assembler
		mv AQUA-assembler assembler
	fi
	
	cd assembler/
	sh compile_c.sh
	cd ../
	mv assembler/ROM.zed ROM.zed
fi

if [ "$2" = "" ]; then
	gcc glue.c -std=gnu99 -Wall \
		-Wno-unused-variable -Wno-unused-but-set-variable -Wno-main \
		-lSDL2 -lGL -lGLU -lSDL2_ttf -lm \
		$has_curl_args $has_discord_args
	
	./a.out
fi

exit 0
