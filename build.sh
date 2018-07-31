
#!/bin/bash
set -e

if [ ! -d "asm/" ]; then
	git clone https://github.com/inobulles/AQUA-CW
	mv AQUA-CW asm
fi

gcc glue.c -Werror -Wall -Wno-unused-variable -Wno-main -lSDL2 -lGL -lGLU -lSDL2_ttf
./a.out

exit 0
