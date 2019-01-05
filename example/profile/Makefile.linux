# Makefile for ~CLU/example/profile

# Sample makefile for profiling executables from CLU sources.

CLU     = ${CLUHOME}/exe/pclu                    # the compiler
LINK    = ${CLUHOME}/exe/plink                   # the linker

OBJS    = factorial.o 

RM      = /bin/rm -f
FLUFF   = *~ *.old *.bak .*~ .*.old .*.bak

factorial: ${OBJS}
	${LINK} -prof -opt -o $@ ${OBJS}
	@echo "Run the program 'factorial' (it terminates normally)."
	@echo "Raw profiling information will be in gmon.out."
	@echo "Use 'gprof factorial gmon.out' to process the raw output."
	@echo "The information from this example is not very interesting;"
	@echo "usually it shows most of the time being spent doing i/o."
	@echo ""
	@echo "Alternatively (on alpha workstations) use pixie."
	@echo "     pixie factorial -o factorial.pixie"
	@echo "Run 'factorial.pixie' (it terminate normally)."
	@echo "Use 'prof -pixie -quit 50 factorial factorial.Addrs factorial.Counts > factorial.prof."
	@echo "The profiling information will be in factorial.prof."

factorial.o: factorial.clu
	${RM} factorial.c
	${CLU} -ext false -spec factorial -opt -co factorial

clean:
	${RM} ${OBJS} factorial gmon.out factorial.pixie \
		factorial.Addrs factorial.Counts pclu.xref *.c ${FLUFF}

veryclean: clean

