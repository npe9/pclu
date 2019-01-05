# Makefile for ~CLU/code/sysclu/Debugger

# Assumes that ~CLU/lib/*.lib have been made.

LIB_DIR = ${CLUHOME}/lib
LIBS    = misc lowlev useful
SYS_CLU = ~CLU/sys/clu
CLU     = ${CLUHOME}/exe/pclu

RM      = /bin/rm -f
FLUFF   = *~ *.old *.bak .*~ .*.old .*.bak


# sparc stattions seem to need the following definition
.c.o:
	cc -c ${CFLAGS} $<

all: c_files ${OBJS}

c_files:
	$(CLU)  \#ext false \#dir ${LIB_DIR} \#me ${LIBS}       \
		\#spec _gprof.spc \#dir ${SYS_CLU}              \
		\#bindir `pwd` \#ccdbg 0                        \
		\#co \*

clean:
	${RM} *.o *.c ${FLUFF}

veryclean: clean


