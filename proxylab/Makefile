#
# Makefile for Proxy Lab 
#
# You may modify this file any way you like (except for the handin
# rule). Autolab will execute the command "make" on your specific 
# Makefile to build your proxy from sources.
#
CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread

all: proxy

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

sbuf.o: sbuf.c sbuf.h csapp.h
	$(CC) $(CFLAGS) -c sbuf.c 

proxy.o: proxy.c sbuf.h csapp.h 
	$(CC) $(CFLAGS) -c proxy.c

cache.o: cache.c cache.h csapp.h 
	$(CC) $(CFLAGS) -c cache.c

cache_.o: cache_.c cache_.h csapp.h 
	$(CC) $(CFLAGS) -c cache_.c

proxy: sbuf.o cache_.o proxy.o csapp.o 
	$(CC) $(CFLAGS) sbuf.o cache_.o proxy.o csapp.o -o proxy $(LDFLAGS)

# Creates a tarball in ../proxylab-handin.tar that you should then
# hand in to Autolab. DO NOT MODIFY THIS!
handin:
	(make clean; cd ..; tar czvf proxylab-handin.tar.gz proxylab-handout)

clean:
	rm -f *~ *.o proxy core *.tar *.zip *.gzip *.bzip *.gz


