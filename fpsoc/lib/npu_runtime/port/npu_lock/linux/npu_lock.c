#include <pthread.h>

#include "npu_lock.h"


static pthread_mutex_t m_mutex;

AL_S8 npu_lock_init() { 
    pthread_mutex_init(&m_mutex, NULL);
    return 0;
}

AL_S8 npu_lock_destroy() { 
    pthread_mutex_destroy(&m_mutex);
    return 0;
}

AL_S8 npu_lock_lock() { 
    pthread_mutex_lock(&m_mutex);
    return 0;
}

AL_S8 npu_lock_unlock() {
    pthread_mutex_unlock(&m_mutex);
    return 0;
}