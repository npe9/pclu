# Makefile for ~CLU/example/debug

# Sample makefile for building debugging executables from CLU sources.

PCLU  = ${CLUHOME}/exe/pclu
PLINK = ${CLUHOME}/exe/plink

OBJS  = factorial.o 

RM    = /bin/rm -f
FLUFF = *~ *.old *.bak .*~ .*.old .*.bak

factorial: ${OBJS}
	${PLINK} -debug $@ ${OBJS}

factorial.o: factorial.clu
	${PCLU} -cfiles -ext false -spec factorial -compile factorial

clean:
	${RM} ${OBJS} factorial factorial.lib factorial.sym pclu.xref *.c ${FLUFF}

veryclean: clean
