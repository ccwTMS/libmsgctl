include ./Compile.mak

LIBA=libmsgctl

LIB_CFLAGS=-I. -I./include

LIB_FOLDER=./lib
BIN_FOLDER=./bin
INCLUDE_FOLDER=./include
INSTALL_INCLUDE=/usr/local/include
INSTALL_BIN=/usr/local/bin
INSTALL_LIBRARY=/usr/local/lib

TARGETS=util

all: $(LIBA)
	mkdir -p $(BIN_FOLDER)
	for i in $(TARGETS) ; do make -C $$i || exit $? ; done	


$(LIBA).o:
	$(CC) $(LIB_CFLAGS) -fPIC -c $(LIBA).c 

$(LIBA): $(LIBA).o
	$(CC) $(LIB_CFLAGS) -shared -Wl,-soname,$(LIBA).so $(LIBA).o -o $(LIBA).so 	
	$(STRIP) -s $(LIBA).so 
	mkdir -p $(LIB_FOLDER)
	install -m 755 $(LIBA).so $(LIB_FOLDER)

install: install-lib install-app
	install -m 755 $(INCLUDE_FOLDER)/libmsgctl.h $(INSTALL_INCLUDE) 

install-lib:
	cd $(LIB_FOLDER) ; ln -s $(LIBA).so $(LIBA).so.1 ; cp -d $(LIBA).so $(LIBA).so.1 $(INSTALL_LIBRARY)
	cd $(INSTALL_LIBRARY) ; ldconfig
	
install-app: 
	for i in $(TARGETS) ; do make -C $$i install || exit $? ; done	
	install -m 755 $(BIN_FOLDER)/msgctl $(INSTALL_BIN) 
	

clean:
	for i in $(TARGETS) ; do make -C $$i clean || exit $? ; done	
	rm -f *.o  *~ *.so* 
	rm -f $(LIB_FOLDER)/lib*.so*

