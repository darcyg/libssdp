
#if !defined(LIBINTERFACE_INTERFACE_H)
#define LIBINTERFACE_INTERFACE_H

#include <string>

namespace libssdp {

	class interface {
	public:
		static std::string address (std::string interface);
		static std::string netmask (std::string interface);
	};

};

#endif
