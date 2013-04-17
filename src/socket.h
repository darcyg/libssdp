
#if !defined(libssdp_socket_h)
#define libssdp_socket_h

#include <string>

namespace libssdp {

	class socket_priv;

	class socket {
	public:
		enum type {
			type_stream,
			type_dgram,
		};

		socket (enum type type);
		~socket (void);

		bool set_multicastttl (int ttl);
		bool set_reuseaddr (bool on);
		bool add_membership (std::string address);
		bool drop_membership (std::string address);
		bool bind (std::string address, unsigned int port);

	private:
		socket_priv *_priv;
	};

};

#endif
