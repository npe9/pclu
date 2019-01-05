# Makefile for ~CLU/example

# Sample makefile for building production executables from CLU sources

PCLU  = ${CLUHOME}/exe/pclu
PLINK = ${CLUHOME}/exe/plink

OBJS  = factorial.o

RM    = /bin/rm -f
FLUFF = *~ *.old *.bak .*~ .*.old .*.bak

factorial: ${OBJS}
	${PLINK} -opt -o $@ ${OBJS}

factorial.o: factorial.clu
	${PCLU} -ext false -spec factorial -opt -cfiles -compile factorial

clean:
	${RM} ${OBJS} factorial factorial.lib  pclu.xref *.c ${FLUFF}
	cd debug; make clean
	cd profile; make clean

veryclean: clean

