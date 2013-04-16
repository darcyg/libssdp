
#if !defined(LIBSSDP_SSDP_H)
#define LIBSSDP_SSDP_H

#include <string>

namespace libssdp {

	class event {
	public:
		event (void);
		~event (void);
	};

	class ssdp {
	public:
		ssdp (void);
		ssdp (std::string interface);
		~ssdp (void);

	private:
		std::string _address;
		std::string _netmask;
	};

};

#endif
