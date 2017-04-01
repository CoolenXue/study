

#ifndef _OSA_H_
#define _OSA_H_

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define OSA_DEBUGMODE_ENABLE // enable OSA_printf, OSA_assert
#define OSA_DEBUGFILE_ENABLE // enable printf's during OSA_fileXxxx
#define OSA_PROFILE_ENABLE // enable profiling APIs

#define OSA_OK      0  ///< Status : OK
#define OSA_ERROR   -1  ///< Status : Generic error

#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES

#ifndef TRUE

typedef int Bool;                 ///< Boolean type

#define TRUE		((Bool) 1)        ///< Boolean value : TRUE
#define FALSE		((Bool) 0)        ///< Boolean value : FALSE

#endif

/* unsigned quantities */
typedef unsigned long long Uint64;      ///< Unsigned 64-bit integer
typedef unsigned int Uint32;            ///< Unsigned 32-bit integer
typedef unsigned short Uint16;          ///< Unsigned 16-bit integer
typedef unsigned char Uint8;            ///< Unsigned  8-bit integer

/* signed quantities */
typedef long long Int64;               ///< Signed 64-bit integer
typedef int Int32;                     ///< Signed 32-bit integer
typedef short Int16;                   ///< Signed 16-bit integer
typedef char Int8;                     ///< Signed  8-bit integer

#endif /* _TI_STD_TYPES */

#ifndef KB
#define KB ((Uint32)1024)
#endif

#ifndef MB
#define MB (KB*KB)
#endif

#define OSA_NOWAIT        ((Uint32) 0)  // no wait
#define OSA_WAIT     ((Uint32)-1)  // wait forever


//used but not very useful.
#define OSA_memAlloc(size)      (void*)malloc((size))
#define OSA_memFree(ptr)        free(ptr)

// you may never use them.
#define OSA_align(value, align)   ((( (value) + ( (align) - 1 ) ) / (align) ) * (align) )
#define OSA_floor(value, align)   (( (value) / (align) ) * (align) )
#define OSA_ceil(value, align)    OSA_align(value, align) 

#include <osa_debug.h>
#include <osa_prf.h>

Uint32 OSA_getCurTimeInMsec();
void   OSA_waitMsecs(Uint32 msecs);
int    OSA_attachSignalHandler(int sigId, void (*handler)(int ) );
int    OSA_xstrToInt(char *hex);

#endif /* _OSA_H_ */



