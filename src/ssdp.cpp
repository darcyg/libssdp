
#include "interface.h"

#include "ssdp.h"

libssdp::event::event (void)
{
}

libssdp::event::~event (void)
{
}

libssdp::ssdp::ssdp (void)
{
	_address = "";
	_netmask = "";
}

libssdp::ssdp::ssdp (std::string interface)
{
	_address = libssdp::interface::address(interface);
	_netmask = libssdp::interface::netmask(interface);
}

libssdp::ssdp::~ssdp (void)
{
}
