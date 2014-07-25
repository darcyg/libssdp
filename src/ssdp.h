
#if !defined(libssdp_ssdp_h)
#define libssdp_ssdp_h

#include <string>

namespace libssdp {
	class socket;
	class thread;

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
		class cond *_cond;
		class mutex *_mutex;
		class socket *_socket;
		class socket *_announce;
		class thread *_thread;
		static void * worker (void *arg);
		bool _started;
		bool _running;
		bool _stopped;
	};

};

#endif
