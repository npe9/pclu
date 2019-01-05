# Makefile for CLU indenter (Linux version)

INCLUDEDIR = $(CLUHOME)/include
LIBDIR	   = $(CLUHOME)/code
LIBS       = $(CLUHOME)/lib/lowlev.lib $(CLUHOME)/lib/useful.lib $(CLUHOME)/lib/misc.lib
CFLAGS     = -I$(INCLUDEDIR) -c

CLU	   = ${CLUHOME}/exe/pclu
LINK	   = ${CLUHOME}/exe/plink

CLUOBJS = _buffered_read.o _io_buf.o buf.o case.o ienv.o spiffy.o start_up.o
CFILES  = _buffered_read.c _io_buf.c buf.c case.c ienv.c spiffy.c start_up.c
COBJS   = _calc_hpos.o

RM      = /bin/rm -f
FLUFF   = *~ *.old *.bak .*~ .*.old .*.bak

cludent: $(CLUOBJS) $(COBJS)
	${LINK} -opt -o cludent ${CLUOBJS} ${COBJS}

$(CLUOBJS): _buffered_read.clu _io_buf.clu buf.clu case.clu \
		ienv.clu spiffy.clu start_up.clu ind.equ
	$(CLU) -ce *.equ -merge ${LIBS} -spec *.spc *.clu -dump cludent.lib
	${CLU} -ce *.equ -merge $(LIBS) cludent.lib -opt -compile *.clu

clean: 
	${RM} ${CFILES} cludent.lib *.o pclu.xref ${FLUFF}

veryclean: clean
	${RM} cludent
