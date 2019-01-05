# Makefile for ~CLU/code/libclu/Debugger

# Assumes that ~CLU/lib/*.lib files have been made.

LIB_DIR = ${CLUHOME}/lib
LIBS    = misc lowlev useful
LIB_CLU = ~CLU/lib/clu
CLU     = ${CLUHOME}/exe/pclu

RM      = /bin/rm -f
FLUFF   = *~ *.old *.bak .*~ .*.old .*.bak

# sparc stattions seem to need the following definition
.c.o:
	cc -c ${CFLAGS} $<

all: ${OBJS}

#all: c_files ${OBJS}

c_files:
	$(CLU)  \#ext false \#dir ${LIB_DIR} \#me ${LIBS} \#ce gcd.equ  \
		\#spec _host_address _all_dir_entries_                  \
		\#dir $(LIB_CLU) \#spec gcd_tab, gc_read                \
		\#bindir `pwd` \#ccdbg 0                                \
		\#co \*

clean:
	${RM} *.o *.c ${FLUFF}

veryclean: clean

