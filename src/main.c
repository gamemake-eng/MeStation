#include "unicorn/unicorn.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/vm.h"

int main(int argc, char **argv, char **envp)
{
	MeStation *me = create_vm();
	int err = run_vm(me);
	if(err){
		printf("Err: %s\n",uc_strerror(err));
		return err;
	}
	destroy_vm(me);
	return 0;
}
