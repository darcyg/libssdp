
#if !defined(libssdp_poll_h)
#define libssdp_poll_h

#include <string>

namespace libssdp {

	class poll {
	public:
		class object {
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

		poll (void);
		~poll (void);

		bool add_object (class libssdp::poll::object *object, enum poll::event event);
		bool del_object (class libssdp::poll::object *object);

	private:
	};

};

#endif
