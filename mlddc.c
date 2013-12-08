/*
 * mlddc
 *
 * Copyright 1997 Artur Skawina <skawina@usa.net>
 * All Rights Reserved.
 * Freely Distributable.
 *
 */
 
#define MLDDC_VERSION   "3.2.0"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "config.h"

#include "netio.h"

#ifdef USE_SYSLOG
#include <syslog.h>
#define SYSLOG(x) x
#else
#define SYSLOG(x)
#endif

#ifdef SECURE

#include "secure.h"

#define SECSTAT "SECURE"
#define URLPORT 443
typedef SSL    *netsock;

#define Connect(h,p) ( ssl_init(), ssl_connect((h),(p)) )
#define Disconnect(s) {}      /* yet? */

#else

#define SECSTAT "STD"
#define URLPORT 80
typedef int    netsock;

#endif /*SECURE*/



void encbasic( char *d, const char *u, const char *p );

static const char MYDOMAIN[]  = "dyn.ml.org";

#ifndef DEBUG
static const char URLHOST[]  = "members.ml.org";
#else
static const char URLHOST[]  = "localhost";
#endif

static const char URLPATHF[] = "/mis-bin/ms3/nic/dyndns?"
                  "command=Update+Host&"
                  "domain=%s&"               /* (char *) myhost   */
                  "act=%s&"                  /* "act" or "dec"    */
                  "wildcard=on&"
                  "do=mod&"
                  "agree=agree";
                 
static const char OKSTRING[] = "MS3V STATUS:OK";


static char             URLPATH[1024*4];


void fatalerr( const char *s )
{
   /*if (s)*/
      perror( s );
      
   SYSLOG( syslog( LOG_AUTHPRIV|LOG_NOTICE, "FAILED: %s (%m)\n", s ) );

   exit( 10 );
}

int main( int argc, char *argv[] )
{
   static char    myhost[128],
                  mymid[128],
                  mypass[128];
   char           *action = "act";
                  
   char           *result;        /*from http-query*/
   char           *c;
   
   int len;
                  

   printf( "MLDDC " MLDDC_VERSION " " SECSTAT
            " Copyright 1997 Artur Skawina <skawina@usa.net>\n" );

   SYSLOG( openlog( "mlddc", 0, LOG_AUTHPRIV|LOG_NOTICE ) );

   if ( argc>1 )
   {
      if ( (argv[1][0] == 'd') || (argv[1][0] == 'D') )
      {
         action = "dec";
         printf( "Deactivating\n" );
      }
   }
   
            /* Get user data */
            /* and */
            /* Check the user data */
   if(
      !gets( myhost )      ||
      strlen(myhost)  <1   ||
      !gets( mymid )       ||
      strlen(mymid)   <3   ||
      !gets( mypass )      ||
      strlen(mypass)  <3
     ) 
     fatalerr( "Invalid user data" );
     
     encbasic( mypass, mymid, mypass );
     
#ifdef DEBUG                                 
   printf( "%s\n", URLPATH );
#endif
    
{
    static char   buf[1024*64];
    int           i;
    netsock       sock;

    sock = Connect( URLHOST, URLPORT );
        
            /* Prepare the URL */
            
   sprintf( URLPATH, URLPATHF, myhost, action );

   sprintf( buf, "GET %s HTTP/1.0\r\n"
                              "Host: %s\r\n"
                              "Pragma: no-cache\r\n"
                              "User-Agent: mlddc/" MLDDC_VERSION "/" SECSTAT "\r\n"
                              "Authorization: Basic %s\r\n"
                              "\r\n",
                                        URLPATH, URLHOST, mypass );
    
   if ( write( sock, buf, strlen( buf ) ) != (ssize_t)strlen( buf ) )
      fatalerr( "write( buf )" );
   
   if ( (i=read(sock, buf, sizeof(buf)-1)) <=0 )
      fatalerr( "read" );
        
    buf[i] = '\0';
    
    if ( (c=strstr( buf, "\r\n\r\n" )) )
        c+=4;
    else if ( (c=strstr( buf, "\n\n" )) )
        c+=2;

    if ( c==0 )
       fatalerr("Error: Unable to find HTTP header");

//P     puts( c );
    
           /* Check if OKSTRING is present */
   
   len=i;

    if ( !(result=strstr( buf, OKSTRING )) )
       while( (i=read(sock, buf+len, sizeof(buf)-1-len))>0 )
       {
         len += i;
         buf[len] = '\0';
          
//P         puts( buf );
    
          result=strstr( buf, OKSTRING );
          
          if ( result )
             break;
       }

    Disconnect( sock );
}

   if ( result )
   {
      printf( "Host %s.%s updated.\n", myhost, MYDOMAIN );
      
      SYSLOG( syslog( LOG_AUTHPRIV|LOG_NOTICE, "Host %s.%s updated.\n", myhost, MYDOMAIN ) );

      result += 5;
      while ( *result != '-' )
         putchar( *result++ );
      putchar( '\n' );
      fflush( stdout );
   }
   else
   {
      puts( c );
      printf( "Host %s.%s NOT updated.\n", myhost, MYDOMAIN );
      fflush( stdout );
      
      SYSLOG( syslog( LOG_AUTHPRIV|LOG_NOTICE, "Host %s.%s NOT updated.\n", myhost, MYDOMAIN ) );

      return 1;
   }

   return 0;
}
/*EOF*/
