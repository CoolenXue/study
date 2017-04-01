

#include <osa_thr.h>

int OSA_thrCreate(OSA_ThrHndl *hndl, OSA_ThrEntryFunc entryFunc, Uint32 pri, Uint32 stackSize, void *prm)
{
  int status=OSA_OK;
  pthread_attr_t thread_attr;
  struct sched_param schedprm;

  // initialize thread attributes structure
  status = pthread_attr_init(&thread_attr);
  
  if(status!=OSA_OK) {
    OSA_error("OSA_thrCreate() - Could not initialize thread attributes\n");
    return status;
  }
  
  if(stackSize!=OSA_THR_STACK_SIZE_DEFAULT)
    pthread_attr_setstacksize(&thread_attr, stackSize);

  status |= pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
  status |= pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO);
    
  if(pri>OSA_THR_PRI_MAX)   
    pri=OSA_THR_PRI_MAX;
  else 
  if(pri<OSA_THR_PRI_MIN)   
    pri=OSA_THR_PRI_MIN;
    
  schedprm.sched_priority = pri;
  status |= pthread_attr_setschedparam(&thread_attr, &schedprm);

  if(status!=OSA_OK) {
    OSA_error("OSA_thrCreate() - Could not initialize thread attributes\n");
    goto error_exit;
  }

  status = pthread_create(&hndl->hndl, &thread_attr, entryFunc, prm);

  if(status!=OSA_OK) {
    OSA_error("OSA_thrCreate() - Could not create thread [%d]\n", status);
  }

error_exit:  
  pthread_attr_destroy(&thread_attr);

  return status;
}

// if you need returnVal.you should change it.
int OSA_thrJoin(OSA_ThrHndl *hndl)
{
  int status=OSA_OK;
  void *returnVal;
  status = pthread_join(hndl->hndl, &returnVal); 
  return status;    
}

int OSA_thrDelete(OSA_ThrHndl *hndl)
{
  int status=OSA_OK;
  
  status |= pthread_cancel(hndl->hndl); 
  status |= OSA_thrJoin(hndl);
  
  return status;    
}

int OSA_thrChangePri(OSA_ThrHndl *hndl, Uint32 pri)
{
  int status = OSA_OK;
  struct sched_param schedprm;  

  if(pri>OSA_THR_PRI_MAX)   
    pri=OSA_THR_PRI_MAX;
  else 
  if(pri<OSA_THR_PRI_MIN)   
    pri=OSA_THR_PRI_MIN;
  
  schedprm.sched_priority = pri;  
  status |= pthread_setschedparam(hndl->hndl, SCHED_FIFO, &schedprm);
  
  return status;
}

int OSA_thrExit(void *returnVal)
{
  pthread_exit(returnVal);
  return OSA_OK;
}

