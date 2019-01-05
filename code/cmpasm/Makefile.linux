# Makefile for ~CLU/code/cmpasm (Linux version)

#  Builds files for the compiler.

OPT_FLAGS   = -O2 
DEBUG_FLAGS = -g 

RM    = /bin/rm -f
FLUFF = *~ *.old *.bak .*~ .*.old .*.bak

CFLAGS = -c -I../include -I../gc/include/private -w $(OPT_FLAGS)

SOURCES = _bin_id.c _save_state.c _restore_state.c version.c
OBJECTS = _bin_id.o _save_state.o _restore_state.o version.o

../cmp/npclu: $(OBJECTS)

clean:
	${RM}  *.o ${FLUFF}

veryclean: clean
