#
# MLDDC Makefile 1.5  Artur Skawina <skawina@usa.net>
#
# 1.0  Initial version
# 1.1  New entry for OS/2 (Samuel Audet <guardia@cam.org>)
# 1.2  Improved dist and sdist targets
# 1.3  Added zip format to (s)dist
# 1.4  Added missing LDFLAGS (os/2) (Samuel Audet <guardia@cam.org>)
# 1.5  Added 'sdist-os2' target
#

CC=gcc
CFLAGSOPT= -O2 -m486 -malign-loops=0 -malign-jumps=0 -malign-functions=0 -fomit-frame-pointer
CFLAGS= -pipe -Wall $(CFLAGSOPT)
SSL_INC= /usr/include/ssl
SSL_LIBS= -lssl -lcrypto
BINDIR= /usr/local/bin

OBJS = mlddc.o base64.o netio.o

ifdef SECURE
CFLAGS += -I$(SSL_INC) -DSECURE=1
LDFLAGS= $(SSL_LIBS)
OBJS += secure.o
endif 

.c.o:
	$(CC) $(CFLAGS) $(DEBUG) -c $<
.o:
	$(CC) $^ $(LDFLAGS) -o $@
        
all: mlddc

mlddc: $(OBJS)


# for OS/2:

mlddc.exe: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) -lsocket -Zcrtdll -s

# for Solaris:

mlddc.sol: $(OBJS)
	$(CC) -o mlddc $(OBJS) -lsocket -lnsl


install:
	install -m 755 mlddc $(BINDIR)

clean:
	rm -f mlddc mlddc.exe *.o *.s core .gdb_history
	
strip:	mlddc
	rm -f *.o
	strip mlddc

dist: 
	make clean strip "DEBUG=-DUSE_SYSLOG"
	(tar cf - mlddc `echo [^s]*`| gzip -9 >../mlddc.tgz)
	rm -f ../mlddc.zip
	zip -9r ../mlddc.zip * -x secure.? ssl/ ssl/COPYRIGHT

sdist: 
	make clean SECURE=1 strip "DEBUG=-DUSE_SYSLOG"
	(cd .. ; tar cf - mlddc | gzip -9 >mlddcs.tgz)
	rm -f ../mlddcs.zip
	zip -9r ../mlddcs.zip *
        
sdist-os2: 
	make clean
	cp ../mlddcs-os2/mlddc.exe .
	rm -f ../mlddcs-os2.zip
	zip -9r ../mlddcs-os2.zip *
	rm mlddc.exe
