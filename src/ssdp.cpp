
#include <stdio.h>

#include "interface.h"
#include "socket.h"
#include "thread.h"
#include "ssdp.h"

static const char *default_ssdp_address     = "239.255.255.250";
static const unsigned int default_ssdp_port = 1900;

libssdp::event::event (void)
{
}

libssdp::event::~event (void)
{
}

libssdp::ssdp::ssdp (void)
{
	_ttl = 1;
	_address = "";
	_netmask = "";
	_socket = NULL;
	_announce = NULL;
	_thread = NULL;
	_started = false;
	_running = false;
	_stopped = false;
	_cond = new libssdp::cond();
	_mutex = new libssdp::mutex();
}

libssdp::ssdp::ssdp (std::string interface)
{
	_ttl = 1;
	_address = libssdp::interface::address(interface);
	_netmask = libssdp::interface::netmask(interface);
	_socket = NULL;
	_announce = NULL;
	_thread = NULL;
	_started = false;
	_running = false;
	_stopped = false;
	delete _cond;
	delete _mutex;
}

libssdp::ssdp::~ssdp (void)
{
	stop();
}

void * libssdp::ssdp::worker (void *arg)
{
	int rc;
	size_t nrequests;
	class libssdp::ssdp *ssdp;
	struct libssdp::poll::request requests;
	char *buffer;
	size_t count;
	ssdp = (class ssdp *) arg;
	ssdp->_mutex->lock();
	ssdp->_started = true;
	ssdp->_running = true;
	ssdp->_stopped = false;
	ssdp->_cond->signal();
	ssdp->_mutex->unlock();
	requests.events = libssdp::poll::event_in;
	requests.object = ssdp->_socket;
	nrequests = 1;
	count = 65536;
	buffer = new char[count];
	if (buffer == NULL) {
		goto out;
	}
	while (1) {
		thread::yield();
		ssdp->_mutex->lock();
		if (ssdp->_running == 0) {
			ssdp->_mutex->unlock();
			break;
		}
		ssdp->_mutex->unlock();
		rc = libssdp::poll::poll(nrequests, &requests, 100);
		if (rc < 0) {
			break;
		}
		if (rc == 0) {
			continue;
		}
		if ((requests.revents & libssdp::poll::event_in) == 0) {
			break;
		}
		rc = ssdp->_socket->read(buffer, count);
		if (rc < 0) {
			break;
		}
		if (rc == 0) {
			break;
		}
		printf("in data rc: %d\n", rc);
	}
out:	if (buffer != NULL) {
		delete [] buffer;
	}
	ssdp->_mutex->lock();
	ssdp->_started = true;
	ssdp->_running = false;
	ssdp->_stopped = true;
	ssdp->_cond->signal();
	ssdp->_mutex->unlock();
	return NULL;
}

bool libssdp::ssdp::start (void)
{
	bool rc;
	_mutex->lock();
	if (_started == true) {
		printf("already started\n");
		_mutex->unlock();
		return false;
	}
	_mutex->unlock();
	_socket = new socket(socket::type_dgram);
	if (_socket == NULL) {
		printf("can not create listen socket\n");
		goto bail;
	}
	_announce = new socket(socket::type_dgram);
	if (_announce == NULL) {
		printf("can not create announce socket\n");
		goto bail;
	}
	rc = _announce->set_multicast_ttl(_ttl);
	if (rc == false) {
		printf("can not set multicast ttl for announce socket\n");
		goto bail;
	}
	rc = _socket->set_reuseaddr(true);
	if (rc == false) {
		printf("can not set reuse for listen socket\n");
		goto bail;
	}
	rc = _socket->bind(default_ssdp_address, default_ssdp_port);
	if (rc == false) {
		printf("can not bind to ssdp ip/port\n");
		goto bail;
	}
	rc = _socket->add_membership(default_ssdp_address);
	if (rc == false) {
		printf("can not join to multicast address\n");
		goto bail;
	}
	_thread = new thread(libssdp::ssdp::worker, this);
	if (_thread == NULL) {
		printf("can not create ssdp listen worker\n");
		goto bail;
	}
	_mutex->lock();
	while (_started == 0) {
		_cond->wait(_mutex);
	}
	_mutex->unlock();
	return true;
bail:
	if (_thread != NULL) {
		delete _thread;
	}
	if (_socket != NULL) {
		delete _socket;
	}
	if (_announce != NULL) {
		delete _announce;
	}
	_thread = NULL;
	_socket = NULL;
	_announce = NULL;
	_started = false;
	_running = false;
	_stopped = false;
	return false;
}

bool libssdp::ssdp::stop (void)
{
	_mutex->lock();
	if (_started == false) {
		_mutex->unlock();
		return false;
	}
	_running = false;
	_cond->signal();
	while (_stopped == false) {
		_cond->wait(_mutex);
	}
	_mutex->unlock();
	_thread->join();
	delete _thread;
	delete _socket;
	delete _announce;
	_thread = NULL;
	_socket = NULL;
	_announce = NULL;
	_started = false;
	_running = false;
	_stopped = false;
	return true;
}
