
#include "src/kos.h"

static kos_t kos;

void main(void) {
	if (kos_init(&kos)) {
		printf("WARNING Failed to initialize KOS. Exitting ...\n");
		exit(1);
		
	}
	
	kos_quit(&kos);
	
}
