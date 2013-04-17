
#include <stdio.h>
#include <stdlib.h>

#include "socket.h"

int main (int argc, char *argv[])
{
	libssdp::socket *socket;
	(void) argc;
	(void) argv;
	socket = new libssdp::socket(libssdp::socket::type_dgram);
	if (socket == NULL) {
		printf("socket dgram failed\n");
		exit(-1);
	}
	delete socket;
	socket = new libssdp::socket(libssdp::socket::type_stream);
	if (socket == NULL) {
		printf("socket stream failed\n");
		exit(-1);
	}
	delete socket;
	return 0;
}
