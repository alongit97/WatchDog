#include <stdio.h>
#include <unistd.h> /* getpid() */
#include <pthread.h> /* pthread_mutex_t, pthread_mutex_lock, pthread_mutex_unlock */
#include <stdatomic.h>
#include "uid.h"

const ilrd_uid_t bad_uid = {0, -1, -1};
pthread_mutex_t uid_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Mutex for protecting the UID generation process */

ilrd_uid_t UIDGenerate(void)
{
	ilrd_uid_t uid = {0};
	static __sig_atomic_t count = 1;

	if (0 != pthread_mutex_lock(&uid_mutex))
	{
		perror("Mutex lock failed");
		return bad_uid;
	}

	uid.counter = count++;

	if (0 != pthread_mutex_unlock(&uid_mutex))
	{
		perror("Mutex unlock failed");
		return bad_uid;
	}

	uid.time = time(NULL);
	if (uid.time == -1)
	{
		pthread_mutex_unlock(&uid_mutex);
		return bad_uid;
	}

	uid.pid = getpid();

	return uid;
}

int UIDIsEqual(ilrd_uid_t one, ilrd_uid_t other)
{
    return (one.counter == other.counter && one.pid == other.pid &&
                                                    one.time == other.time);
}