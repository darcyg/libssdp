
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "interface.h"

#define INADDR(x) (*(struct in_addr *) &ifr->x[sizeof sa.sin_port])
#define IFRSIZE   ((int) (size * sizeof (struct ifreq)))

std::string libssdp::interface::address (std::string interface)
{
	int sock;
	const char *rc;
	struct ifreq *ifr;
	struct ifreq ifrr;
	struct sockaddr_in sa;
	char address[INET_ADDRSTRLEN];

	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0) {
		return "";
	}

	ifr = &ifrr;
	ifrr.ifr_addr.sa_family = AF_INET;
	strncpy(ifrr.ifr_name, interface.c_str(), sizeof(ifrr.ifr_name));

	if (ioctl(sock, SIOCGIFADDR, ifr) < 0) {
		return "";
	}

	close(sock);

	rc = inet_ntop(AF_INET, &INADDR(ifr_addr.sa_data), address, INET_ADDRSTRLEN);
	if (rc == NULL) {
		return "";
	}
	return rc;
}

std::string libssdp::interface::netmask (std::string interface)
{
	int sock;
	const char *rc;
	struct ifreq *ifr;
	struct ifreq ifrr;
	struct sockaddr_in sa;
	char address[INET_ADDRSTRLEN];

	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0) {
		return "";
	}

	ifr = &ifrr;
	ifrr.ifr_addr.sa_family = AF_INET;
	strncpy(ifrr.ifr_name, interface.c_str(), sizeof(ifrr.ifr_name));

	if (ioctl(sock, SIOCGIFNETMASK, ifr) < 0) {
		return "";
	}

	close(sock);

	rc = inet_ntop(AF_INET, &INADDR(ifr_addr.sa_data), address, INET_ADDRSTRLEN);
	if (rc == NULL) {
		return "";
	}
	return rc;
}
