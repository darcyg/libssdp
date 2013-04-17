
#if !defined(libssdp_ssdp_h)
#define libssdp_ssdp_h

#include <string>

#include "socket.h"
#include "thread.h"

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

		bool set_ttl (void);
		bool start (void);
		bool stop (void);

	private:
		int _ttl;
		std::string _address;
		std::string _netmask;
		cond _cond;
		mutex _mutex;
		socket *_socket;
		socket *_announce;
		thread *_thread;
		static void * worker (void *arg);
		bool _started;
		bool _running;
		bool _stopped;
	};

};

#endif
