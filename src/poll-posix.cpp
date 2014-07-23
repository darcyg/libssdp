
#include <unistd.h>
#include <string.h>

#include "poll.h"

libssdp::poll::poll (void)
{
}

libssdp::poll::~poll (void)
{
}

bool libssdp::poll::add_object (class libssdp::poll::object *object, enum poll::event event)
{
	if (object == NULL) {
		return false;
	}
	if ((event & (event_in | event_out | event_pri)) == 0) {
		return false;
	}
	return true;
}

bool libssdp::poll::del_object (class libssdp::poll::object *object)
{
	if (object == NULL) {
		return false;
	}
	return true;
}
