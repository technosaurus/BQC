#include "bqc.h"
int _main(){
	u8 child_stack[64];
	unsigned long flags = CLONE_FILES|CLONE_IO|CLONE_FS;
	pid_t ptid, ctid, cid,status,options = __WALL;
	const struct timespec req = {10,0}, rem = {0};
	cid = clone(flags, child_stack, NULL, NULL, NULL);
	if (cid == 0){
		struct timespec rem, req={5,0};
		nanosleep(&req,&rem);
		exit(cid);
	}else if (cid < 0) exit(-cid);
	wait4(cid,&status,options,NULL);
	exit(0);
}
