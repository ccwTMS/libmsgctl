#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include "libmsgctl.h"

#define RET_OK		0
#define RET_USAGE	1
#define RET_EXCEED	2
#define RET_FAILED	3


int usage(void)
{
	int table_size = sizeof(target_table);
	int ctl_size = sizeof(target_ctl_t);
	int total_apps = table_size/ctl_size - 2;
	int i;
	
	printf("<<MSGprintf Message Controller v1.0>>\n");
	printf("Usage: msgctl [target] [command]\n");
	printf("\tsupported target name: ");
	for(i=1; i<=total_apps; i++){
		printf("%s ", target_table[i].target);
	}
	printf("\n");
	printf("	supported command:\n \
			msg_disabled: no message dump.\n \
			msg_console: messages dump to console.\n \
			msg_syslog: messages dump to syslog (default setting)\n \
			msg_to=[filename or device name] :dump to file or other terminal device.\n \
			level=[0-5] : message level, 0.Debug(default), 1.Info, 2.Warning, 3.Error, 4.Emergency, 5.Quiet.\n");
}



int main( int argc, char** argv )
{
	int ipc_id=0;
	msgctl_msg_buf_t msg;

	if(argc != 3){
		usage();
		return -RET_USAGE;
	}

	if( strlen(argv[2]) > DBG_DATASIZE){
		printf("NOTICE: buffer size of msg_* series commands is %d bytes.\n", DBG_DATASIZE);
		return -RET_EXCEED;
	}

	memset(&msg, 0, sizeof(msgctl_msg_buf_t));
	if(cmd_process(&msg, argv[1], argv[2])==0){
		ipc_id = msgctl_msg_init();
		if(ipc_id >= 0){
			msgctl_svr_msg_process(ipc_id, &msg);
			return RET_OK;
		}
	}

	return RET_FAILED;
}
