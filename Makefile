# Makefile for Portable CLU

# To use this Makefile:
#   (a) Set the CLUHOME environment variable to the name of this directory.
#       Using the bash shell: export CLUHOME=`pwd`
#       Using the csh shell:  setenv CLUHOME `pwd`
#   (b) If you are using "make install", edit the definition of INSTALL_DIR.
#   (c) Type "make" 
# If this Makefile does not work on your platform, follow the instructions
# in howto.install, which you can also read if you want to understand how
# this Makefile works.

# The following options have not been tested for this distribution.
#   (a) Using "make configure_G0" to get -G 0 libraries for debugging
#       (for building large programs on the mips or alpha platforms).
#   (b) Adding #define athena to CLUHOME/include/pclu_sys.h (to get
#       smaller executables at the cost of some speed),

PLATFORM = linux

EXEDIR = exe

CONFIGDIRS = cludent code/cmp code/cmpasm code/libasm code/libclu \
             code/sysasm code/sysclu debug driver util

CLEANDIRS  = cludent cmpclu code debug doc driver example lib sys util

G0_DIRS    = code/sysasm code/libasm code/libclu code/sysclu debug

INSTALL_DIR 	= /usr/local/pclu
INSTALL_FILES	= ./include ./lib/*.lib ./lib/*.spc ./lib/short.help \
		  ./code/include ./code/libpclu.a ./code/libpclu_opt.a \
		  ./cmp ./code/cmp/basic.types \
		  ./code/cmp/ccdbg.cmd* ./code/cmp/ccopt.cmd* \
		  ./clu.order ./doc/pclu.pdf ./doc/mini-pclu.pdf \
		  ${EXEDIR}/pclu ${EXEDIR}/plink ${EXEDIR}/cludent \
		  ./example ./elisp 
		# ./code/libpclu_debug.a ./driver

INSTALL_TAR_FILE = ../pclu-install.tar.Z
INSTALL_TARGZ_FILE = ../pclu-install.tar.gz

LN = /bin/ln -s
MV = /bin/mv -f
RM = /bin/rm -f
CP = /bin/cp

FLUFF = *~ *.old *.bak .*~ .*.old .*.bak

all: configure gc clu

configure:
	echo Configuring links to platform-dependent Makefiles, command-line scripts
	for i in ${CONFIGDIRS};				  		     \
	   do (cd $$i; ${RM} Makefile; ${LN} Makefile.${PLATFORM} Makefile); \
	   done
	cd code/cmp; ${RM} ccopt.cmd ccdbg.cmd; ${LN} ccdbg.cmd.${PLATFORM} ccdbg.cmd; \
                     ${LN} ccopt.cmd.${PLATFORM} ccopt.cmd

gc:
	echo Installing and testing the garbage collector
	cd code/gc; ./configure make; make gctest; ./gctest

clu: libpclu compiler clulibs cludent # PCLU debugger

configure_G0:
	for i in ${G0_DIRS}; \
	   do (cd $$i; ${RM} Makefile.${PLATFORM}.G0; \
		sed -e '/DEBUG/s/-G 4/-G 0/' Makefile.${PLATFORM} > Makefile.${PLATFORM}.G0; \
		${MV} Makefile.${PLATFORM}.G0 Makefile.${PLATFORM}) \
	   done; \
	cd cmp ; sed -e 's/-G 4/-G 0/' ccdbg.cmd.${PLATFORM} > ccdbg.cmd.${PLATFORM}.G0; \
		${MV} ccdbg.cmd.${PLATFORM}.G0 ccdbg.cmd.${PLATFORM}

configure_G4:
	for i in ${G0_DIRS}; \
	   do (cd $$i; ${RM} Makefile.${PLATFORM}.G4; \
		sed -e '/DEBUG/s/-G 0/-G 4/' Makefile.${PLATFORM} > Makefile.${PLATFORM}.G4; \
		${MV} Makefile.${PLATFORM}.G4 Makefile.${PLATFORM}) \
	   done; \
	cd cmp ; sed -e 's/-G 0/-G 4/' ccdbg.cmd.${PLATFORM} > ccdbg.cmd.${PLATFORM}.G4; \
		${MV} ccdbg.cmd.${PLATFORM}.G4 ccdbg.cmd.${PLATFORM}

libpclu:
	echo Creating library of optimized CLU system code for the C linker.
	cd code; make optlib

compiler:
	echo Building and installing the pclu compiler
	echo Assumes that CLUHOME is set properly
	cd code/cmp; make
	mv code/cmp/pclu $(EXEDIR)/pclu

clulibs:
	echo Building CLU runtime libraries
	cd lib; make

debugger:
	echo Creating library of debuggable CLU system code for the C linker.
	cd code; make debuglib

PCLU:
	echo 'Building and installing the PCLU driver (an alternative to pclu/plink)'
	cd driver; make PCLU
	mv driver/PCLU $(EXEDIR)/PCLU

install:
	echo Installing PCLU in ${INSTALL_DIR}
	tar cf - ${INSTALL_FILES} | (cd ${INSTALL_DIR}; tar xf -)

install-tar-z:
	tar cfv - ${INSTALL_FILES} | compress > ${INSTALL_TAR_FILE}

install-tar-gz:
	tar cfv - ${INSTALL_FILES} | gzip -c > ${INSTALL_TARGZ_FILE}

example: fictitious_file
	cd example; make
	cd example/debug; make
	cd example/profile; make

cludent: fictitious_file
	cd cludent; make
	mv cludent/cludent $(EXEDIR)/cludent

newcompiler:
	cd cmpclu; make
	make compiler

cdebugger:
	cd code; make cdebuglib

fictitious_file:

clean:
	for i in ${CLEANDIRS}; do (cd $$i; make clean); done
	${RM} ${FLUFF}
	cd ${EXEDOR}; ${RM} ${FLUFF}

veryclean:
	${RM} ${FLUFF}
	for i in ${CLEANDIRS}; do (cd $$i; make veryclean); done
	cd ${EXEDIR}; ${RM} pclu cludent liblst PCLU ${FLUFF}
