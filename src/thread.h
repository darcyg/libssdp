
#if !defined(libssdp_thread_h)
#define libssdp_thread_h

namespace libssdp {

	class mutex_priv;

	class mutex {
	public:
		mutex (void);
		~mutex (void);
		bool lock (void);
		bool unlock (void);

	private:
		friend class cond;
		mutex_priv *_priv;
	};

	class cond_priv;

	class cond {
	public:
		cond (void);
		~cond (void);
		bool signal (void);
		bool broadcast (void);
		bool wait (mutex &mutex);
		bool wait (mutex *mutex);
		bool timedwait (mutex &mutex);
		bool timedwait (mutex *mutex);

	private:
		cond_priv *_priv;
	};

	class thread_priv;

	class thread {
	public:
		thread (void * (*worker) (void *), void *arg);
		~thread (void);
		bool join (void);
		static bool yield (void);
	private:
		thread_priv *_priv;
	};

};

#endif
