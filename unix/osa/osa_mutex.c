

#include <osa_mutex.h>

int OSA_mutexCreate(OSA_MutexHndl *hndl)
{
  pthread_mutexattr_t mutex_attr;
  int status=OSA_OK;
 
  status |= pthread_mutexattr_init(&mutex_attr);
  status |= pthread_mutex_init(&hndl->lock, &mutex_attr);
  
  if(status!=OSA_OK)
    OSA_error("OSA_mutexCreate() = %d \r\n", status);

  pthread_mutexattr_destroy(&mutex_attr);
    
  return status;
}

int OSA_mutexDelete(OSA_MutexHndl *hndl)
{
  pthread_mutex_destroy(&hndl->lock);  

  return OSA_OK;
}

int OSA_mutexLock(OSA_MutexHndl *hndl)
{
  return pthread_mutex_lock(&hndl->lock);
}

int OSA_mutexUnlock(OSA_MutexHndl *hndl)
{
  return pthread_mutex_unlock(&hndl->lock);
}


