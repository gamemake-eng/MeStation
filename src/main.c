#include "unicorn/unicorn.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/vm.h"

int main(int argc, char **argv, char **envp)
{
    MeStation *me = create_vm();
    destroy_vm(me);
	return 0;
}
