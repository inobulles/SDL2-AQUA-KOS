
#!/bin/bash
set -e

if [ ! -d "asm/" ]; then
	git clone https://github.com/inobulles/AQUA-CW
	mv AQUA-CW asm
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
	gcc glue.c -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-main -lSDL2 -lGL -lGLU -lSDL2_ttf -lm
	./a.out
fi

exit 0
