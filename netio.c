/*  Copyright 1997 Artur Skawina <skawina@usa.net>
 *  All Rights Reserved.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#include "netio.h"

int Connect( const char *hostname, const int port )
{
   struct sockaddr_in sin = {0};

   struct hostent   *phe;
   int              sock;

   sin.sin_family = AF_INET;
   sin.sin_port   = htons( port );

   if ( !(phe = gethostbyname( hostname )) )
      fatalerr( "DNS error (gethostbyname)" );

   memcpy( (char *)&sin.sin_addr, phe->h_addr, phe->h_length );

   if ( (sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP )) == -1 )
      fatalerr( "socket" );

   if ( connect( sock, (struct sockaddr *)&sin, sizeof(sin)) == -1 )
      fatalerr( "connect");

   return sock;
}
