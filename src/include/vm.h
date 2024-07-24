#include "unicorn/unicorn.h"
#pragma once
//typedef struct MeStation MeStation;

typedef struct MeStation {
	void* buf;
	int bufsize;
	void *mem;
	uc_engine *uc;
	int entryPoint;
	int heapStart;
	int heapLength;
} MeStation;

#define PRG_SIZE 2 * 1024 * 1024
#define MEM_SIZE 2 * 1024 * 1024

MeStation *create_vm();
int run_vm(MeStation *me);
void destroy_vm(MeStation *me);
