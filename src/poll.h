
#if !defined(libssdp_poll_h)
#define libssdp_poll_h

#include <string>

namespace libssdp {
	namespace poll {
		class object {
		public:
			virtual int fd (void) = 0;
		};

		enum event {
			event_in   = 0x00000001,
			event_out  = 0x00000002,
			event_pri  = 0x00000004,
			event_err  = 0x00000008,
			event_hup  = 0x00000010,
			event_nval = 0x00000020,
		};

		struct request {
			class poll::object *object;
			int events;
			int revents;
		};

		int poll (size_t nrequests, struct poll::request *requests);
		int poll (size_t nrequests, struct poll::request *requests, int timeout);
	};
};

#endif
