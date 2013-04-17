
#if !defined(libssdp_interface_h)
#define libssdp_interface_h

#include <string>

namespace libssdp {

	class interface {
	public:
		static std::string address (std::string interface);
		static std::string netmask (std::string interface);
	};

};

#endif
