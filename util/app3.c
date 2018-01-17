#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include "libmsgctl.h"

int main(void)
{
	while(1){
		MSGprintf(TARGET_APP3, "3333:THOMAS test\n");
		sleep(5);
	}
	return 0;
}
