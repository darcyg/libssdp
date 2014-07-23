
#if !defined(libssdp_socket_h)
#define libssdp_socket_h

#include <string>
#include "poll.h"

namespace libssdp {

	class socket_priv;

	class socket :
		public poll::object {
	public:
		enum type {
			type_stream,
			type_dgram,
		};

		socket (enum libssdp::socket::type type);
		virtual ~socket (void);

		bool set_multicast_ttl (int ttl);
		bool set_reuseaddr (bool on);
		bool add_membership (std::string address);
		bool drop_membership (std::string address);
		bool bind (std::string address, unsigned int port);

		virtual int fd (void);

	private:
		socket_priv *_priv;
	};

};

#endif
