
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "poll.h"
#include "socket.h"

static int usage (const char *pname)
{
	printf("%s -i address -p port\n", pname);
	printf("  -i: source address\n");
	printf("  -p: source port\n");
	return 0;
}

int main (int argc, char *argv[])
{
	int c;
	int rc;
	int port;
	const char *address;
	libssdp::poll *poll;
	libssdp::socket *socket;
	poll = NULL;
	socket = NULL;
	port = 0;
	address = NULL;
	while ((c = getopt(argc, argv, "hi:p:")) != -1) {
		switch (c) {
			case 'i':
				address = optarg;
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'h':
			default:
				usage(argv[0]);
				exit(0);
		}
	}
	if (address == NULL) {
		usage(argv[0]);
		goto bail;
	}
	if (port == 0) {
		usage(argv[0]);
		goto bail;
	}
	socket = new libssdp::socket(libssdp::socket::type_dgram);
	if (socket == NULL) {
		printf("socket dgram failed\n");
		goto bail;
	}
	rc = socket->set_multicast_ttl(1);
	if (rc == false) {
		printf("can not set multicast ttl\n");
		goto bail;
	}
	rc = socket->set_reuseaddr(true);
	if (rc == false) {
		printf("can not set reuseaddr\n");
		goto bail;
	}
	rc = socket->bind(address, port);
	if (rc == false) {
		printf("can not bind\n");
		goto bail;
	}
	rc = socket->add_membership(address);
	if (rc == false) {
		printf("can not add membership\n");
		goto bail;
	}
	poll = new libssdp::poll();
	if (poll == NULL) {
		printf("can not create poll\n");
		goto bail;
	}
	rc = poll->add_object(socket, libssdp::poll::event_in);
	if (rc == false) {
		printf("can not add poll object\n");
		goto bail;
	}
	if (socket != NULL) {
		if (poll != NULL) {
			poll->del_object(socket);
		}
		delete socket;
	}
	if (poll != NULL) {
		delete poll;
	}
	return 0;
bail:
	if (socket != NULL) {
		if (poll != NULL) {
			poll->del_object(socket);
		}
		delete socket;
	}
	if (poll != NULL) {
		delete poll;
	}
	return -1;
}
