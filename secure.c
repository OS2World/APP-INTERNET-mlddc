/*  Copyright 1997 Artur Skawina <skawina@usa.net>
 *  All Rights Reserved.
 */
        
#include "netio.h"

#include "secure.h"
   
   
static   SSL_CTX  *ctx;
   
   
void ssl_init( void )
{
   SSL_load_error_strings();
   SSLeay_add_ssl_algorithms();
     
   ctx=SSL_CTX_new( SSLv23_client_method() );      /* setup a context */
}

SSL *ssl_connect( const char *hostname, const int port )
{
   int      fd;
   SSL      *ssl;
    
      /* create a new SSL structure */
      
   ssl = SSL_new( ctx );

   fd = Connect( hostname, port );
   
   SSL_set_fd( ssl, fd );
   
   SSL_connect( ssl );
   
   return ssl;
}
