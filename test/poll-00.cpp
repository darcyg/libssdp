
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
	libssdp::socket *socket;
	size_t nrequests;
	struct libssdp::poll::request requests;
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
	requests.events = libssdp::poll::event_in;
	requests.object = socket;
	nrequests = 1;
	rc = libssdp::poll::poll(nrequests, &requests, 1000);
	if (rc < 0) {
		printf("poll request failed\n");
		goto bail;
	}
	if (rc == 0) {
		printf("poll timeout\n");
	} else {
		printf("poll data\n");
	}
	if (socket != NULL) {
		delete socket;
	}
	return 0;
bail:
	if (socket != NULL) {
		delete socket;
	}
	return -1;
}
