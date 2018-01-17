/*====== Debug Messages Control ======*/

/* ** How to support a program to use Debug Messages Control:
 *	1. add target definition into target_table in libmsgctl.h, and then re-compile libmsgctl library and msgctl.
 *	2. add two lines code that listed below in your source file, use MSGprintf to replace your printf function. 
 *		#include <libmsgctl.h>
 *		#define your_printf(str, args...)	MSGprintf(TARGET_APP1|L_DEBUG, str, ##args)	
 *					modify TARGET_APP1 to you new definition(target_table[]) to specify application for control; 
 *					message level(L_DEBUG) can refer to msg_ctl_level's definition in libmsgctl.h.
 *	3. add -lmsgctl into Makefile of your application and rebuild it.
 *
 * ** How to control:
 * 	1. During runtime: use control program "msgctl".
 * 		the usage is:
 * 			msgctl program_name command
 * 		where
 * 			program_name: specify a name that added into target_table[].
 * 			command: now has 3 commands: 
 * 					msg_disabled: no message dump.
 * 					msg_console: messages dump to console.
 * 					msg_syslog: messages dump to syslog (default setting)
 * 					msg_to=[filename or device name] :dump to file or other terminal device.
 *					level=[0-5] : message level, 0.Debug(default), 1.Info, 2.Warning, 3.Error, 4.Emergency, 5.Quiet.
 * 		ex:
 * 		    # msgctl app1 msg_console	<---let app1's message dump to console.
 *
 *	2. When initial time: use environment variable.
 *		ex:
 *		   # M_PATH1=msg_console ./app1   <---set message output to console.
 *		   # M_LVL=3 ./app1               <---set message level to Error (Error and Emergency messages shown only).
 *
 *									
 * */

#ifndef __LIBMSGCTL_H__
#define __LIBMSGCTL_H__
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>


#define MSG_IPCNAME "/msg_ipcname"
#define DBG_DATASIZE	32

typedef struct msgctl_msg_buf{
	unsigned long mtype;
	char mtext[DBG_DATASIZE];
} msgctl_msg_buf_t;

typedef struct target_ctl{
	unsigned long target_id; //for MSGprintf()
	char target[DBG_DATASIZE]; //for msgctl command line setting. 
	char target_dest[DBG_DATASIZE]; //environment variable for message output control.
	char level[DBG_DATASIZE]; //environment variable for message level control.
} target_ctl_t;

#define LEVEL_MASK	0x0000FFFF
#define GET_LEVEL(target)	(target & LEVEL_MASK)
enum msg_ctl_level{
	L_DEBUG=0,
	L_INFO,
	L_WARNING,
	L_ERROR,
	L_EMERGENCY
};

/* one application/thread one target, edit it. */
#define TARGET_ALL	0
#define TARGET_APP1	(1<<16)
#define TARGET_APP2	(2<<16)
#define TARGET_APP3	(3<<16)

#define GET_TARGETID(target)	(target >> 16)

/* one application/thread one definition, edit it. */
static target_ctl_t target_table[]={
		{TARGET_ALL, "all", "MSGCTL_ALL", "MSGCTL_LVL"}, /* preserved */
		{TARGET_APP1, "app1", "M_PATH1", "M_LVL1"},
		{TARGET_APP2, "app2", "M_PATH2", "M_LVL2"},
		{TARGET_APP3, "app3", "M_PATH3", "M_LVL3"},
		{0, "", "", ""} /* the end */
};

static int msgctl_ipc_id = -1;

#ifdef __cplusplus
extern "C" {
#endif
extern int msgctl_ipc_id;
extern int msgctl_msg_init( void );
extern int msgctl_clt_msg_process(int ipc_id, msgctl_msg_buf_t *msg, unsigned long target);
extern int msgctl_svr_msg_process(int ipc_id, msgctl_msg_buf_t *msg);
extern int cmd_process(msgctl_msg_buf_t *msg, char *target, char*cmd);
extern void MSGprintf(unsigned long target, const char *format, ...);
#ifdef __cplusplus
}
#endif

#endif /*  __LIBMSGCTL_H__ */
