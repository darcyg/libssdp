
#include <stdio.h>
#include <unistd.h>

#include "ssdp.h"

int main (int argc, char *argv[])
{
	int rc;
	class libssdp::ssdp *ssdp;
	ssdp = new libssdp::ssdp();
	if (ssdp == NULL) {
		printf("ssdp create failed\n");
		goto bail;
	}
	rc = ssdp->start();
	if (rc == false) {
		printf("ssdp start failed\n");
		goto bail;
	}
	while (1) {
		sleep(1);
	}
	delete ssdp;
	return 0;
bail:
	if (ssdp != NULL) {
		ssdp->stop();
		delete ssdp;
	}
	return -1;
}
