
#ifndef _OSA_DEBUG_H_
#define _OSA_DEBUG_H_

#include <stdio.h>
           
// printf wrapper that can be turned on and off by defining/undefining
#ifdef OSA_DEBUGMODE_ENABLE
     
#define OSA_printf(...)  printf(__VA_ARGS__); 

#define OSA_error(...) \
			  do \
			  { \
			  fprintf(stderr, " ERROR  (%s|%s|%d): ", __FILE__, __func__, __LINE__); \
			  fprintf(stderr, __VA_ARGS__); \
			  } \
			  while(0);

#define OSA_assert(x)  \
				  if( (x) == 0) { \
					fprintf(stderr, " ASSERT (%s|%s|%d)\r\n", __FILE__, __func__, __LINE__); \
					while (getchar()!='q')	\
					  ; \
				  }

#define OSA_assertSuccess(ret)  OSA_assert(ret==OSA_OK)


//used not very much.

#define OSA_DEBUG \
  fprintf(stderr, " %s:%s:%d Press Any key to continue !!!", __FILE__, __func__, __LINE__); 


#define OSA_DEBUG_WAIT \
  OSA_DEBUG \
  getchar();
            
#else
  
  #define OSA_printf(...)
  #define OSA_error(...)
  #define OSA_assert(x)
  #define OSA_assertSuccess(ret)

  
  #define OSA_DEBUG    
  #define OSA_DEBUG_WAIT  
#endif



#endif  //_OSA_DEBUG_H_



