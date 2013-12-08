/*  Copyright 1997 Artur Skawina <skawina@usa.net>
 *  All Rights Reserved.
 */
        
        
#include <unistd.h>
#include <stdio.h>


#ifndef __NORETURN
#define __NORETURN
#endif

#ifndef __NORETURN2
# if __GNUC__>2 || __GNUC__==2 && __GNUC_MINOR__>=7
#  define __NORETURN2 __attribute__ ((noreturn))
# else
#  define __NORETURN2
# endif
#endif


void __NORETURN fatalerr( const char *s ) __NORETURN2;

int Connect( const char *hostname, const int port );

inline extern void Disconnect( const int sock );
inline extern void Disconnect( const int sock )
{
   close( sock );
}
