# Makefile for ~CLU/debug

# Generates .o files for debugging.

CFLAGS = -I${CLUHOME}/code/include -w -O2
#CFLAGS = -I${CLUHOME/code/include -w -g

RM    = /bin/rm -f
FLUFF = *~ *.old *.bak .*~ .*.old .*.bak

PCLU = ../exe/pclu

OBJECTS = debug.o debug2.o parse.o debug_c.o util.o stable.o \
	  lineedit.o libinfo.o _signal.o call_clu.o new_selector.o \
	  new_parm_util.o

CFILES = debug.c debug2.c parse.c debug_c.c util.c \
	 libinfo.c _signal.c call_clu.c new_selector.c \
	 new_parm_util.c

CLU_SOURCES = parse.clu debug.clu debug2.clu

.SUFFIXES:
.SUFFIXES: .o .c .clu

.clu.c: ; $(PCLU) -cfiles -ccdbg 0 -xf debug.xfile -spec *.clu -opt time -co $*.clu

it: $(OBJECTS) libinfo.h $(CFILES)

all:
	$(PCLU) -xf debug.xfile -spec *.clu -opt time -co *.clu -dump debug

clean:
	${RM} debug_c.o new_selector.o new_parm_util.o libinfo.o \
		_signal.o util.o \
		call_clu.o parse.o debug.o debug2.o parse.c debug.c debug2.c ${FLUFF}

veryclean: clean
	${RM} debug.lib

libinfo.o: libinfo.h libinfo.c
new_selector.o: selector.h new_selector.c
debug.o: debug.clu
debug2.o: debug2.clu
parse.o: parse.clu
