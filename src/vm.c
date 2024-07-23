#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

#include "include/vm.h"
#include "unicorn/unicorn.h"

//I used https://gitlab.com/critchancestudios/ProjectHalberd/-/blob/main/emulator/src/vm.c?ref_type=heads as a reference

struct MeStation {
	void* buf;
	int bufsize;
	void *mem;
	uc_engine *uc;
	int entryPoint;
	int heapStart;
	int heapLength;
};

static void handle_intr(uc_engine *uc, uint32_t intno, void *userdata){

	if (intno == 2) {
		int pc;
		uc_reg_read(uc, UC_ARM_REG_PC, &pc);
		
		//get immediate (-4 bytes from pc)
		uint32_t svc_cmd;
		uc_mem_read(uc, pc-4, &svc_cmd, 4);
		
		int svc_num = svc_cmd & 0xffffff;
		
		printf("%i", svc_num);
	
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
	uc_hook trace1;
	
	err = uc_open(UC_ARCH_ARM, UC_MODE_ARM, &me->uc);
	
	if(err){
		printf("failed to initialize unicorn: %u (%s)\n", err, uc_strerror(err));
		free(me->mem);
		free(me);
		return NULL;
	
	}
	
	uc_ctl(me->uc, UC_CTL_CPU_MODEL, UC_CPU_ARM_926);
	
	//enable floating point
	uint32_t fpexc;
	uc_reg_read(me->uc, UC_ARM_REG_FPEXC, &fpexc);
	fpexc |= (1<<30);
	uc_reg_write(me->uc, UC_ARM_REG_FPEXC, &fpexc);
	
	uc_hook_add(me->uc, &trace1, UC_HOOK_INTR, handle_intr, me, 1, 0);
	
	printf("Created VM\n");
	
	return me;
}

void destroy_vm(MeStation *me){
	uc_close(me->uc);
	free(me->mem);
	free(me);
	
	printf("destroyed vm");
}
