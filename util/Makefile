include ../Compile.mak

EXEC=msgctl
TEST_APP=app1 app2 app3
TEST2_APP=app

CFLAGS=-I../include -L../lib -lmsgctl

all:	apps test

apps:	
	for i in $(EXEC) ; do \
		$(CC) $(CFLAGS) -o $$i $$i.c ;\
	done

test:
	for i in $(TEST_APP) ; do \
		$(CC) $(CFLAGS) -o $$i $$i.c ;\
	done
	for i in $(TEST2_APP) ; do \
		$(CC) $(CFLAGS) -lpthread -o $$i $$i.c ;\
	done

install:
	for i in $(EXEC) ; do \
		$(STRIP) -s $$i ;\
		cp -f $$i ../bin ;\
	done	 

clean:
	rm -f *.o  *~ $(EXEC) $(TEST_APP) $(TEST2_APP)
	rm -f ../bin/*
