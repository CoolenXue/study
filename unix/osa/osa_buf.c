

#include <osa_buf.h>
#include <string.h>

int OSA_bufDelete(OSA_BufHndl *hndl)
{
  int status=OSA_OK;

  if(hndl==NULL)
    return OSA_ERROR;

  status = OSA_queDelete(&hndl->emptyQue);
  status |= OSA_queDelete(&hndl->fullQue);
  status |= OSA_mutexDelete(&hndl->readerLock);  
  
  return status;
}

int OSA_bufCreate(OSA_BufHndl *hndl, OSA_BufCreate *bufInit)
{
  int status = OSA_OK;
  int i;

  if(hndl==NULL || bufInit==NULL)
    return OSA_ERROR;
    
  if(  bufInit->numBuf >  OSA_BUF_NUM_MAX )
    return OSA_ERROR;

  memset(hndl, 0, sizeof(OSA_BufHndl));
  
  hndl->numReaders = bufInit->numReaders;
  if( hndl->numReaders <= 0)
    hndl->numReaders = 1;
 
  status = OSA_queCreate(&hndl->emptyQue, bufInit->numBuf);

  if(status!=OSA_OK) {
    OSA_error("OSA_bufCreate() = %d \r\n", status);
    return status;
  }

  status = OSA_queCreate(&hndl->fullQue, bufInit->numBuf);

  if(status!=OSA_OK) {
    OSA_queDelete(&hndl->emptyQue);
    OSA_error("OSA_bufCreate() = %d \r\n", status);
    return status;
  }
  
  status = OSA_mutexCreate(&hndl->readerLock);
  if(status!=OSA_OK) {
    OSA_queDelete(&hndl->fullQue);  
    OSA_queDelete(&hndl->emptyQue);
    OSA_error("OSA_mutexCreate() = %d \r\n", status);
    return status;
  }

  hndl->numBuf   = bufInit->numBuf;

  for(i=0; i<hndl->numBuf; i++) {
    hndl->bufInfo[i].size = 0;
    hndl->bufInfo[i].flags = 0;    
    hndl->bufInfo[i].timestamp = 0;    
    hndl->bufInfo[i].physAddr = bufInit->bufPhyAddr[i];
    hndl->bufInfo[i].virtAddr = bufInit->bufVirtAddr[i];    
    hndl->curReaders[i] = 0;
    OSA_quePut(&hndl->emptyQue, i, OSA_WAIT);
  }

  return status;
}

int OSA_bufGetEmpty(OSA_BufHndl *hndl, int *bufId, Uint32 timeout)
{
  int status;

  if(hndl==NULL || bufId==NULL)
    return OSA_ERROR;
    
  status = OSA_queGet(&hndl->emptyQue, bufId, timeout);
  
  if(status!=OSA_OK) {
    *bufId = OSA_BUF_ID_INVALID;  
  } 

  return OSA_OK;
}

int OSA_bufPutFull (OSA_BufHndl *hndl, int bufId)
{
  int status;
  
  if(bufId >= hndl->numBuf || bufId < 0 || hndl==NULL)
    return OSA_ERROR;
    
  hndl->curReaders[bufId] = 0;   
    
  status = OSA_quePut(&hndl->fullQue, bufId, OSA_WAIT);
  
  return OSA_OK;
}

int OSA_bufGetFull(OSA_BufHndl *hndl, int *bufId, Uint32 timeout)
{
  int status;

  if(hndl==NULL || bufId==NULL)
    return OSA_ERROR;

  status = OSA_queGet(&hndl->fullQue, bufId, timeout);
  
  if(status!=OSA_OK) {
    *bufId = OSA_BUF_ID_INVALID;  
  } 

  return OSA_OK;
}

int OSA_bufPutEmpty(OSA_BufHndl *hndl, int bufId)
{
  int status;
  Bool putBuf;
  
  if(bufId >= hndl->numBuf || bufId < 0 || hndl==NULL)
    return OSA_ERROR;

  putBuf = FALSE;
  
  OSA_mutexLock(&hndl->readerLock);  
  
  hndl->curReaders[bufId]++;    
  if(hndl->curReaders[bufId]==hndl->numReaders)
    putBuf = TRUE;
  
  OSA_mutexUnlock(&hndl->readerLock);    
  
  if(putBuf)
    status = OSA_quePut(&hndl->emptyQue, bufId, OSA_WAIT);
  
  return OSA_OK;
}


int OSA_bufSwitchFull(OSA_BufHndl *hndl, int *bufId)
{
  int status;
  int newBufId;

  status = OSA_bufGetEmpty(hndl, &newBufId, OSA_NOWAIT);
  
  if(status==OSA_OK) {

    if(*bufId!=OSA_BUF_ID_INVALID) {

      OSA_bufPutFull(hndl, *bufId);
    }
    
    *bufId = newBufId;
  }

  return status;
}

int OSA_bufSwitchEmpty(OSA_BufHndl *hndl, int *bufId)
{
  int status;
  int newBufId;

  status = OSA_bufGetFull(hndl, &newBufId, OSA_NOWAIT);
  
  if(status==OSA_OK) {

    if(*bufId!=OSA_BUF_ID_INVALID) {

      OSA_bufPutEmpty(hndl, *bufId);
    }
    
    *bufId = newBufId;
  }

  return status;
}

OSA_BufInfo *OSA_bufGetBufInfo(OSA_BufHndl *hndl, int bufId)
{
  if(hndl==NULL)
    return NULL;
    
  if(bufId>=hndl->numBuf)
    return NULL;
    
  return &hndl->bufInfo[bufId];
}


