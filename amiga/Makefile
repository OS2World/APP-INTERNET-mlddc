#
# MLDDC Amiga Makefile 1.0  P-O Yliniemi <peoyli@algonet.se>
#

CC=gcc
CFLAGSOPT= -O2 -m68000 -msoft-float -fomit-frame-pointer
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

.cc.o:
	$(CC) $(CFLAGS) $(DEBUG) -c $<
.o:
	$(CC) $^ $(LDFLAGS) -o $@
        
all: mlddc

mlddc: $(OBJS)


# for OS/2:

mlddc.exe: $(OBJS)
	$(CC) -o $@ $(OBJS) -lsocket -Zcrtdll -s

# for Solaris:

mlddc.sol: $(OBJS)
	$(CC) -o mlddc $(OBJS) -lsocket -lnsl


install:
	install -m 755 mlddc $(BINDIR)

clean:
	rm -f mlddc *.o *.s core .gdb_history
	
strip:	mlddc
	rm -f *.o
	strip mlddc

dist: 
	make clean strip "DEBUG=-DUSE_SYSLOG"
	(tar cf - mlddc `echo [^s]*`| gzip -9 >../mlddc.tgz)

sdist: 
	make clean SECURE=1 strip "DEBUG=-DUSE_SYSLOG"
	(cd .. ; tar cf - mlddc | gzip -9 >mlddcs.tgz)
