
#include <unistd.h>
#include <string.h>
#include <poll.h>

#include "poll.h"

namespace libssdp {

	static inline int event_to_bsd (enum poll::event event)
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

}

int libssdp::poll::poll (size_t nrequests, struct poll::request *requests)
{
	return libssdp::poll::poll(nrequests, requests, -1);
}

int libssdp::poll::poll (size_t nrequests, struct poll::request *requests, int timeout)
{
	int rc;
	size_t r;
	size_t npfds;
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
	for (npfds = 0, r = 0; r < nrequests; r++) {
		pfds[npfds].fd = requests[r].object->fd();
		pfds[npfds].events = event_to_bsd(requests[r].events);
		pfds[npfds].revents = 0;
	}
	rc = poll(pfds, npfds, timeout);
	delete [] pfds;
	return rc;
}
