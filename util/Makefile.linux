# Makefile for ~CLU/util (Linux version)

# Builds liblst

CLU     = ../exe/pclu
LIBS    = ~CLU/lib/lowlev ~CLU/lib/useful ~CLU/lib/misc
PJCL    = ~CLU/cmpclu/pjcl

CFLAGS  = -I${CLUHOME}/code/include -w -O
L1FLAGS = -L$(CLUHOME)/code -O
L2FLAGS = -lpclu -lgc -lc -lm

RM      = /bin/rm -f
FLUFF   = *~ *.old *.bak .*~ .*.old .*.bak

liblst: liblst
	$(CLU) -opt \#ext false -spec ../lib/*.spc ../lib/clu/*.clu ../sys/clu/* \#spec $(PJCL) \#spec liblst \
		\#co liblst
	cc ${CFLAGS} -c liblst.c
	cc ${L1FLAGS} -o liblst liblst.o ../code/cmp/pjcl.o ${L2FLAGS}

gcdprt: gcdprt
	$(CLU) \#ext false \#me $(LIBS) \#spec $(PJCL) \#spec gcdprt \
		\#co gcdprt
	cc ${CFLAGS} -c gcdprt.c
	cc ${L1FLAGS} -o gcdprt gcdprt.o ../code/cmp/pjcl.o \
		/u/dcurtis/may_bug/gc_read.o ${L2FLAGS}

clean:
	${RM} liblst liblst.o liblst.c ${FLUFF}

veryclean: clean
	${RM} liblst gcdprt
