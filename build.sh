
#!/bin/bash
set -e

if [ ! -d "asm/" ]; then
	git clone https://github.com/inobulles/AQUA-CW
	mv AQUA-CW asm
fi

if [ ! -f "ROM.zed" ]; then
	echo "WARNING ROM file was not found (ROM.zed), so attempting to install an extension (or run it if it already exists) so you can write C programs and compile them into ROMs ..."
	echo "NOTE    You will have to create assembler/c/code/main.c to compile, obviously (all your headers will go in the same folder). Press enter to continue ..."
	read a
	
	if [ ! -d "assembler/" ]; then
		git clone https://github.com/inobulles/AQUA-assembler
		mv AQUA-assembler assembler
	fi
	
	cd assembler/
	sh compile_c.sh
	cd ../
	mv assembler/ROM.zed ROM.zed
fi

gcc glue.c -Werror -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-main -lSDL2 -lGL -lGLU -lSDL2_ttf
./a.out

exit 0
