
#include <pthread.h>
#include "../thread.h"

namespace libssdp {

	struct mutex_priv {
		pthread_mutex_t mutex;
	};

	struct cond_priv {
		pthread_cond_t cond;
	};

	struct thread_priv {
		pthread_t thread;
	};

}

libssdp::mutex::mutex (void)
	: _priv(new mutex_priv())
{
	int rc;
	if (_priv == NULL) {
		return;
	}
	rc = pthread_mutex_init(&_priv->mutex, NULL);
	if (rc == 0) {
		return;
	}
	delete _priv;
	_priv = NULL;
}

libssdp::mutex::~mutex (void)
{
	if (_priv == NULL) {
		return;
	}
	delete _priv;
}

bool libssdp::mutex::lock (void)
{
	int rc;
	if (_priv == NULL) {
		return false;
	}
	rc = pthread_mutex_lock(&_priv->mutex);
	return (rc == 0) ? true : false;
}

bool libssdp::mutex::unlock (void)
{
	int rc;
	if (_priv == NULL) {
		return false;
	}
	rc = pthread_mutex_unlock(&_priv->mutex);
	return (rc == 0) ? true : false;
}

libssdp::cond::cond (void)
	: _priv(new cond_priv())
{
	int rc;
	if (_priv == NULL) {
		return;
	}
	rc = pthread_cond_init(&_priv->cond, NULL);
	if (rc == 0) {
		return;
	}
	delete _priv;
	_priv = NULL;
}

libssdp::cond::~cond (void)
{
	if (_priv == NULL) {
		return;
	}
	delete _priv;
}

bool libssdp::cond::signal (void)
{
	int rc;
	if (_priv == NULL) {
		return false;
	}
	rc = pthread_cond_signal(&_priv->cond);
	return (rc == 0) ? true : false;
}

bool libssdp::cond::broadcast (void)
{
	int rc;
	if (_priv == NULL) {
		return false;
	}
	rc = pthread_cond_broadcast(&_priv->cond);
	return (rc == 0) ? true : false;
}

bool libssdp::cond::wait (mutex &mutex)
{
	int rc;
	if (_priv == NULL) {
		return false;
	}
	if (mutex._priv == NULL) {
		return false;
	}
	rc = pthread_cond_wait(&_priv->cond, &mutex._priv->mutex);
	return (rc == 0) ? true : false;
}

bool libssdp::cond::wait (mutex *mutex)
{
	if (_priv == NULL) {
		return false;
	}
	if (mutex == NULL) {
		return false;
	}
	return wait(*mutex);
}

libssdp::thread::thread (void * (*worker) (void *), void *arg)
	: _priv(new thread_priv())
{
	int rc;
	if (_priv == NULL) {
		return;
	}
	rc = pthread_create(&_priv->thread, NULL, worker, arg);
	if (rc == 0) {
		return;
	}
	delete _priv;
	_priv = NULL;
}

libssdp::thread::~thread (void)
{
	join();
}

bool libssdp::thread::join (void)
{
	int rc;
	if (_priv == NULL) {
		return false;
	}
	rc = pthread_join(_priv->thread, NULL);
	if (rc != 0) {
		return false;
	}
	delete _priv;
	_priv = NULL;
	return true;
}

bool libssdp::thread::yield (void)
{
	int rc;
	rc = sched_yield();
	return (rc == 0) ? true : false;
}
