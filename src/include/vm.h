#pragma once
typedef struct MeStation MeStation;

#define PRG_SIZE 2 * 1024 * 1024
#define MEM_SIZE 2 * 1024 * 1024

MeStation *create_vm();
void destroy_vm(MeStation *me);
