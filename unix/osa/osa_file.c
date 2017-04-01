

#include <osa_file.h>


int OSA_fileRead(char *fileName, Uint8 *addr, Uint32 readSize, Uint32 *actualReadSize)
{
  int retVal = OSA_OK;
  Uint8  *curAddr;
  
  Uint32 readDataSize, fileSize, chunkSize=1024*100;
  Uint32 userReadSize;

  FILE *hndlFile;

  #ifdef OSA_DEBUGFILE_ENABLE
  OSA_printf(" [OSA_FILE ] Reading file [%s] ... ", fileName);
  #endif
  
  hndlFile = fopen(fileName, "rb");

  if(hndlFile == NULL) {
  	retVal = OSA_ERROR;
        goto exit;
  }

  curAddr = addr;
  fileSize = 0;

  userReadSize = readSize;

  while(1) {
		if(userReadSize != 0) {
			if(chunkSize > userReadSize)
				chunkSize = userReadSize;
			readDataSize = fread(curAddr, 1, chunkSize, hndlFile);
			fileSize += readDataSize;
			if(chunkSize != readDataSize)
				break;
			if(userReadSize==fileSize)
				break;
			curAddr += chunkSize;
		}
		else {
		readDataSize = fread(curAddr, 1, chunkSize, hndlFile);
     		fileSize+=readDataSize;
      		if(chunkSize!=readDataSize)
        		break;
      		curAddr+=chunkSize;
	    	}
  }
  #ifdef OSA_DEBUGFILE_ENABLE
  OSA_printf("Done. [%d bytes] \r\n", fileSize);
  #endif
  fclose(hndlFile);

exit:
  if(retVal!=OSA_OK) {
    #ifdef OSA_DEBUGFILE_ENABLE
    OSA_printf("ERROR \r\n");
    #endif
    fileSize=0;
  }
  if(actualReadSize != NULL)
  *actualReadSize = fileSize;

  return retVal;
}

int OSA_fileWrite(char *fileName, Uint8 *addr, Uint32 size)
{
  int retVal = OSA_OK;
  Uint32 writeDataSize;
  
  Bool errorInWriting = FALSE;

  FILE *hndlFile;

  if(size==0)
	return OSA_OK;

  #ifdef OSA_DEBUGFILE_ENABLE
  OSA_printf(" [FILE ] Writing to file [%s] (%d bytes) ... ", fileName, size);
  #endif
  hndlFile = fopen(fileName, "wb");

  if(hndlFile == NULL) {
  	retVal = OSA_ERROR;
	goto exit;
  }
	
  {
    // write in units of chunkSize
    Int32 fileSize, chunkSize = 96*1024;
    Int8  *curAddr;

    
    fileSize = size;
    curAddr  = (Int8*)addr;
    while(fileSize>0) {
      if(fileSize<chunkSize) {
        chunkSize = fileSize;
      }
      writeDataSize=0;
      writeDataSize = fwrite(curAddr, 1, chunkSize, hndlFile);
      if(writeDataSize!=chunkSize) {
        // error in writing, abort
        errorInWriting = TRUE;
        retVal = OSA_ERROR;
        break;
      }
      curAddr += chunkSize;
      fileSize -= chunkSize;
    }
    writeDataSize = size - fileSize;
  }

  #ifdef OSA_DEBUGFILE_ENABLE
  OSA_printf("Done. [%d bytes] \r\n", writeDataSize);
  #endif

  fclose(hndlFile);

exit:
  if(retVal!=OSA_OK) {
    #ifdef OSA_DEBUGFILE_ENABLE
    OSA_printf("ERROR \r\n");
    #endif
  }
  return retVal;

}

