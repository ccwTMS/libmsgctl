#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include "libmsgctl.h"

#define DBGprintf(str, args...)		MSGprintf(TARGET_APP1|L_DEBUG, str, ##args)
#define INFOprintf(str, args...)	MSGprintf(TARGET_APP1|L_INFO, str, ##args)
#define WARNprintf(str, args...)	MSGprintf(TARGET_APP1|L_WARNING, str, ##args)
#define ERRprintf(str, args...)		MSGprintf(TARGET_APP1|L_ERROR, str, ##args)
#define EMERprintf(str, args...)	MSGprintf(TARGET_APP1|L_EMERGENCY, str, ##args)

int main(void)
{
	while(1){
		MSGprintf(TARGET_APP1, "1111:TEST L_NO\n");
		DBGprintf("1111:TEST L_DEBUG\n");
		INFOprintf("1111:TEST L_INFO\n");
		WARNprintf("1111:TEST L_WARNING\n");
		ERRprintf("1111:TEST L_ERROR\n");
		EMERprintf("1111:TEST L_EMERGENCY\n");
		sleep(5);
	}
	return 0;
}
