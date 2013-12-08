#include <ssl.h>
   
void ssl_init( void );
SSL *ssl_connect( const char *hostname, const int port );

/* MLDDC -> */

#define read(s,b,l)  SSL_read(s,b,l)
#define write(s,b,l) SSL_write(s,b,l)

/* <- MLDDC */
