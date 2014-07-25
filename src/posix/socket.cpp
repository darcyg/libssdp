
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../socket.h"

namespace libssdp {
	struct socket_priv {
		int type;
		int fd;
	};

	static inline int type_to_bsd (enum socket::type type)
	{
		switch (type) {
			case socket::type_dgram:  return SOCK_DGRAM;
			case socket::type_stream: return SOCK_STREAM;
		}
		return -1;
	}
}


libssdp::socket::socket (enum libssdp::socket::type type)
	: _priv(new socket_priv())
{
	if (_priv == NULL) {
		return;
	}
	_priv->type = type_to_bsd(type);
	_priv->fd = ::socket(AF_INET, _priv->type, 0);
	if (_priv->fd >= 0) {
		return;
	}
	delete _priv;
	_priv = NULL;
}

libssdp::socket::~socket (void)
{
	if (_priv != NULL) {
		if (_priv->fd >= 0) {
			close(_priv->fd);
		}
		delete _priv;
	}
}

bool libssdp::socket::set_multicast_ttl (int ttl)
{
	int rc;
	if (_priv == NULL) {
		return false;
	}
	if (_priv->fd < 0) {
		return false;
	}
	rc = setsockopt(_priv->fd, IPPROTO_IP, IP_MULTICAST_TTL, (char *) &ttl, sizeof(ttl));
	return (rc == 0) ? true : false;
}

bool libssdp::socket::set_reuseaddr (bool on)
{
	int rc;
	int val;
	if (_priv == NULL) {
		return false;
	}
	if (_priv->fd < 0) {
		return false;
	}
	val = (on == true) ? 1 : 0;
	rc = setsockopt(_priv->fd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(val));
	return (rc == 0) ? true : false;
}

bool libssdp::socket::bind (std::string address, unsigned int port)
{
	int rc;
	socklen_t len;
	struct sockaddr_in soc;
	if (_priv == NULL) {
		return false;
	}
	if (_priv->fd < 0) {
		return false;
	}
	len = sizeof(soc);
	memset(&soc, 0, len);
	soc.sin_family = AF_INET;
	soc.sin_addr.s_addr = inet_addr(address.c_str());
	soc.sin_port = htons(port);
	rc = ::bind(_priv->fd, (struct sockaddr *) &soc, len);
	return (rc == 0) ? true : false;
}

ssize_t libssdp::socket::read (void *buffer, size_t count)
{
	int rc;
	if (_priv == NULL) {
		return false;
	}
	if (_priv->fd < 0) {
		return false;
	}
	rc = ::read(_priv->fd, buffer, count);
	return rc;
}

bool libssdp::socket::add_membership (std::string address)
{
	int rc;
	struct hostent *h;
	struct ip_mreq mreq;
	struct in_addr mcastip;
	if (_priv == NULL) {
		return false;
	}
	if (_priv->fd < 0) {
		return false;
	}
	h = gethostbyname(address.c_str());
	if (h == NULL) {
		return false;
	}
	memcpy(&mcastip, h->h_addr_list[0], h->h_length);
	mreq.imr_multiaddr.s_addr = mcastip.s_addr;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	rc = setsockopt(_priv->fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &mreq, sizeof(mreq));
	return (rc == 0) ? true : false;
}

bool libssdp::socket::drop_membership (std::string address)
{
	int rc;
	struct hostent *h;
	struct ip_mreq mreq;
	struct in_addr mcastip;
	if (_priv == NULL) {
		return false;
	}
	if (_priv->fd < 0) {
		return false;
	}
	h = gethostbyname(address.c_str());
	if (h == NULL) {
		return false;
	}
	memcpy(&mcastip, h->h_addr_list[0], h->h_length);
	mreq.imr_multiaddr.s_addr = mcastip.s_addr;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	rc = setsockopt(_priv->fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void *) &mreq, sizeof(mreq));
	return (rc == 0) ? true : false;
}

int libssdp::socket::fd (void)
{
	if (_priv == NULL) {
		return -1;
	}
	return _priv->fd;
}
