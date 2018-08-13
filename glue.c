
#include "src/kos.h"
#include "asm/asm.h"

static kos_t kos;
static program_t de_program;

#define ROM_PATH "ROM (temp)"
#define CODE_ROM 0

#if CODE_ROM
	#define CODE 75, 1, 64, 65, 0, 83, 104, 105, 110, 106, 105, 32, 105, 115, 32, 110, 111, 116, 32, 97, 32, 112, 117, 115, 115, 121, 10, 0, 256, 257, 0, -1, 5, 3, 65, 255, 1, 4, 2, 25, 255, 1, 0, 2, 25, 2, 24, 255, 1, 0, 2, 27, 9, 64, 255, 1, 0, 2, 0, 4, 0, 255, 1, 3, 9, 0, 255, 1, 5, 2, 25, 255, 1, 1, 255, 
#endif

void main(void) {
	click_proxy = 1;
	
	printf("\nControl passed to the CW\n");
	printf("Initializing the KOS ...\n");
	
	if (kos_init(&kos)) {
		printf("WARNING Failed to initialize KOS. Exitting ...\n");
		exit(1);
		
	}
	
	printf("Entering the DE ...\n");
	
	#if !CODE_ROM
		FILE* file = fopen(ROM_PATH, "rb");
		
		if (!file) {
			printf("WARNING Could not open ROM file (`"ROM_PATH"`)\n");
			kos_quit(&kos);
			exit(1);
			
		}
		
		fseek(file, 0L, SEEK_END);
		unsigned long long buffer_bytes = ftell(file);
		rewind(file);
		
		char* code_buffer = (char*) malloc(buffer_bytes * sizeof(char));
		fgets(code_buffer, buffer_bytes, file);
		
		printf("Raw ROM file size is %llu bytes\n", buffer_bytes);
		
		char* original_code_buffer = code_buffer;
		sscanf(code_buffer, "%llu", &de_program.code_length);
		program_alloc(&de_program);
		
		int digits;
		
		unsigned long long i;
		for (i = 0; i < strlen(original_code_buffer); i++) {
			if (sscanf(code_buffer, "%llu, %n", &de_program.code[i], &digits) != 1) {
				break;
				
			}
			
			code_buffer += digits;
			
		}
		
		if (*code_buffer) {
			printf("WARNING ROM parsing did not end cleanly (%d)\n", *code_buffer);
			
		}
		
		free(original_code_buffer);
		fclose(file);	
	#else
		unsigned long long code[] = {CODE};
		
		de_program.code_length = code[0];
		program_alloc(&de_program);
		
		unsigned long long i;
		for (i = 0; i < de_program.code_length; i++) {
			de_program.code[i] = code[i];
			
		}
	#endif
	
	printf("Starting run setup phase ...\n");
	program_run_setup_phase(&de_program);
	
	while (!program_run_loop_phase(&de_program)) { // kos -> cw communication goes here
		if (kos_has_clicked) { /// REMME
			kos_has_clicked = 0;
			click_proxy = 1;
			
		}
		
	}
	
	printf("DE return code is %d\n", de_program.error_code);
	
	program_free(&de_program);
	
	printf("Quitting KOS ...\n");
	kos_quit(&kos);
	
}
