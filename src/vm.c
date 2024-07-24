#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

#include "include/vm.h"
#include "unicorn/unicorn.h"

//I used https://gitlab.com/critchancestudios/ProjectHalberd/-/blob/main/emulator/src/vm.c?ref_type=heads as a reference

/* *
 * *
 20150: 04 00 02 24  	addiu	$2, $zero, 0x4
 20154: 09 00 04 24  	addiu	$4, $zero, 0x9
 20158: 0c 00 00 00  	syscall
 * *
 * */

#define TEST_CODE "\x04\x00\x02\x24\x09\x00\x04\x24\x0c\x00\x00\x00"

#define START_PRG 0x1E8480

struct MeStation {
	void* buf;
	int bufsize;
	void *mem;
	uc_engine *uc;
	int entryPoint;
	int heapStart;
	int heapLength;
};

static void code_hook(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
	int val = 0;
	uc_mem_read(uc, address, &val, 4);
	printf(">>> Tracing instruction at 0x%" PRIx64
           ", instruction size = 0x%x, inst= %x\n",
           address, size, val);
}
static void handle_intr(uc_engine *uc, uint32_t intno, void *userdata){
	printf("Intr %i called\n", intno);

	if(intno == 17){
		int call = 0;

		int a0, a1, a2, a3;

		//The syscall number is stored in v0
		uc_reg_read(uc, UC_MIPS_REG_2, &call);

		//Store a0-a3

		uc_reg_read(uc, UC_MIPS_REG_A0, &a0);
		uc_reg_read(uc, UC_MIPS_REG_A1, &a1);
		uc_reg_read(uc, UC_MIPS_REG_A2, &a2);
		uc_reg_read(uc, UC_MIPS_REG_A3, &a3);



		printf("System call %i\nA0: %i\nA1: %i\nA2: %i\nA3: %i\n",call, a0, a1, a2, a3);

		switch(call){
			case 4:
				//This call will be replaced
				//with something more useful
				printf("Hello World! #%i\n", a0);
				break;
			default:
				printf("Unknown Call\n");
				break;
		}
	}
}

MeStation *create_vm(){
	MeStation *me = malloc(sizeof(MeStation));
	if(me == NULL){
		printf("Failed to allocate mem");
		return NULL;
	}
	
	me->buf = NULL;
	me->bufsize = 0;
	
	me->mem = malloc(MEM_SIZE+PRG_SIZE);
	if(me->mem == NULL){
		printf("Failed to allocate mem");
		free(me);
		return NULL;
	}
	
	uc_err err;
	uc_hook trace1, trace2;
	
	err = uc_open(UC_ARCH_MIPS, UC_MODE_MIPS32 + UC_MODE_LITTLE_ENDIAN, &me->uc);
	
	if(err){
		printf("failed to initialize unicorn: %u (%s)\n", err, uc_strerror(err));
		free(me->mem);
		free(me);
		return NULL;
	
	}

	//Tempoary
	uc_mem_map(me->uc,0x1000, 4*1024*1024, UC_PROT_ALL);
	uc_mem_write(me->uc, START_PRG, TEST_CODE, sizeof(TEST_CODE)-1);	
	
	//Hook for handling syscalls	
	uc_hook_add(me->uc, &trace1, UC_HOOK_INTR, handle_intr, me, 1, 0);
	
	//debugging stuff
	uc_hook_add(me->uc, &trace2, UC_HOOK_CODE, code_hook, me, 0x1000, 0x1000);


	printf("Created VM\n");
	
	return me;
}
int run_vm(MeStation *me){
	return uc_emu_start(me->uc, START_PRG,START_PRG + sizeof(TEST_CODE) - 1,0,0);
}
void destroy_vm(MeStation *me){
	uc_close(me->uc);
	free(me->mem);
	free(me);
	
	printf("destroyed vm");
}
