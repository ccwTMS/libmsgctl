/*====== Debug Messages Control ======*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdarg.h>
#include "libmsgctl.h"


int msgctl_msg_init( void )
{
	key_t key;
	int ipc_id;

	key =  ftok(MSG_IPCNAME, 'a');
	
	ipc_id = msgget(key, IPC_CREAT|IPC_EXCL|00666);
	if((ipc_id < 0)&&(errno != EEXIST)){
		return -1;
	}else{
		if(ipc_id = -1){
			ipc_id = msgget(key, 00666);
		}
	}
	msgctl_ipc_id = ipc_id;
	return ipc_id;
}

/* cilent */
int msgctl_clt_msg_process(int ipc_id, msgctl_msg_buf_t *msg, unsigned long target)
{
	memset(msg, 0, sizeof(msgctl_msg_buf_t));
	return msgrcv( ipc_id, msg, DBG_DATASIZE, target, IPC_NOWAIT);
}

/* server */
int msgctl_svr_msg_process(int ipc_id, msgctl_msg_buf_t *msg)
{
	return msgsnd( ipc_id, msg, DBG_DATASIZE, 0);
}

int cmd_process(msgctl_msg_buf_t *msg, char *target, char*cmd)
{
	int i=0;
	do{
		if(!strcmp(target, target_table[i].target)){
			msg->mtype = GET_TARGETID(target_table[i].target_id);
			strcpy(msg->mtext, cmd);
			return 0;
		}
		i++;
	}while(target_table[i].target_id!=0);

	return -1;
}

/* application */
//TMS: debug message control.
void MSGprintf(unsigned long target, const char *format, ...)
{
	msgctl_msg_buf_t msg;
	int ipc_id = msgctl_ipc_id;
	char *msg_dest;
	char *msg_level;
	va_list args;
	unsigned long r_target = GET_TARGETID(target);
	unsigned long r_level = GET_LEVEL(target);

	if(msgctl_ipc_id < 0)
		ipc_id = msgctl_msg_init();

	if(ipc_id >= 0){	
		if(msgctl_clt_msg_process(ipc_id, &msg, r_target)!=0){
			if(msg.mtext[0]=='m'){
				setenv(target_table[r_target].target_dest,msg.mtext,1);
			}else if(msg.mtext[0]=='l'){
				setenv(target_table[r_target].level,&msg.mtext[6],1);//skip 'level='
			}
		}
	}
	
	msg_level = getenv(target_table[r_target].level);
	if(msg_level != NULL && (int)(r_level - (msg_level[0]-'0')) < 0 ){//default is L_DEBUG.
		return;
	}

	msg_dest = getenv(target_table[r_target].target_dest);

	va_start(args, format);
	if((msg_dest!=NULL)&&(!strcmp(msg_dest, "msg_disabled"))){
	}else if((msg_dest!=NULL)&&(!strcmp(msg_dest, "msg_console"))){
		vprintf(format, args);
	}else if((msg_dest!=NULL)&&(!strncmp(msg_dest, "msg_to=", 7))){
		char *filename = msg_dest+7;
		FILE *fp = fopen(filename,"a");
		if(fp != NULL){
			vfprintf(fp, format, args);
			fclose(fp);
		}
	}else{//default log to syslog
		vsyslog(LOG_NOTICE, format, args);
	}
	va_end(args);
}

