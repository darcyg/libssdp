
#include <unistd.h>
#include <string.h>
#include <poll.h>

#include "../poll.h"

namespace libssdp {
	static inline int event_to_bsd (int event)
	{
		int pevent;
		pevent = 0;
		if (event & poll::event_in) {
			pevent |= POLLIN;
		}
		if (event & poll::event_out) {
			pevent |= POLLOUT;
		}
		if (event & poll::event_pri) {
			pevent |= POLLPRI;
		}
		if (event & poll::event_err) {
			pevent |= POLLERR;
		}
		if (event & poll::event_hup) {
			pevent |= POLLHUP;
		}
		if (event & poll::event_nval) {
			pevent |= POLLNVAL;
		}
		return pevent;
	}

	static inline int bsd_to_event (int pevent)
	{
		int event;
		event = 0;
		if (pevent & POLLIN) {
			event |= poll::event_in;
		}
		if (pevent & POLLOUT) {
			event |= poll::event_out;
		}
		if (pevent & POLLPRI) {
			event |= poll::event_pri;
		}
		if (pevent & POLLERR) {
			event |= poll::event_err;
		}
		if (pevent & POLLHUP) {
			event |= poll::event_hup;
		}
		if (pevent & POLLNVAL) {
			event |= poll::event_nval;
		}
		return pevent;
	}
}

int libssdp::poll::poll (size_t nrequests, struct poll::request *requests)
{
	return libssdp::poll::poll(nrequests, requests, -1);
}

int libssdp::poll::poll (size_t nrequests, struct poll::request *requests, int timeout)
{
	int rc;
	size_t r;
	struct pollfd *pfds;
	if (requests == NULL) {
		return false;
	}
	if (nrequests == 0) {
		return false;
	}
	pfds = new struct pollfd [nrequests];
	if (pfds == NULL) {
		return false;
	}
	for (r = 0; r < nrequests; r++) {
		pfds[r].fd = requests[r].object->fd();
		pfds[r].events = event_to_bsd(requests[r].events);
		pfds[r].revents = 0;
		requests[r].revents = (enum poll::event) 0;
	}
	rc = ::poll(pfds, nrequests, timeout);
	if (rc > 0) {
		for (r = 0; r < nrequests; r++) {
			requests[r].revents = bsd_to_event(pfds[r].revents);
		}
	}
	delete [] pfds;
	return rc;
}
