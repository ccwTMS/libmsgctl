#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "libmsgctl.h"


#define P1_INFO(str, args...)		MSGprintf(TARGET_APP1|L_INFO, str, ##args)
#define P2_INFO(str, args...)		MSGprintf(TARGET_APP2|L_INFO, str, ##args)
#define P3_INFO(str, args...)		MSGprintf(TARGET_APP3|L_INFO, str, ##args)

void thread1(void)
{
	while(1){
		P1_INFO("1111:TEST test(th)\n");
		sleep(5);
	}
	
}

void thread2(void)
{
	while(1){
		P2_INFO("2222:TEST test(th)\n");
		sleep(5);
	}
}

int main(void)
{
	pthread_t id1,id2;
	int ret;
	ret = pthread_create(&id1, NULL, (void *) thread1, NULL);
	if(ret != 0){
		printf("create thread1 failed.\n");
		exit(-1);
	}
	ret = pthread_create(&id2, NULL, (void *) thread2, NULL);
	if(ret != 0){
		printf("create thread2 failed.\n");
		exit(-1);
	}

	while(1){
		P3_INFO("3333:TEST test(th)\n");
		sleep(5);
	}

	//pthread_join(id1, NULL);
	//pthread_join(id2, NULL);
	return 0;
}
