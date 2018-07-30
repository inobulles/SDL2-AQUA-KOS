
#!/bin/bash
set -e

if [ ! -d "asm/" ]; then
	git clone https://github.com/inobulles/AQUA-CW
	mv AQUA-CW asm
fi

gcc glue.c -lSDL2 -lGL -lGLU
./a.out

exit 0
