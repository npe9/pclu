# Makfile for ~CLU/code

# Creates libraries of PCLU system code for the C linker

# To create a pclu library for debugging ,you need to compile stream with
# compiler debugging on and cc debugging off.  Without compiler debugging on,
# dbg_info stuff will be lost and you can't eval system operations.  With cc
# debugging on, system operations enter the debugger on signals, etc., and
# confuse the debugger no end.

CLIBDIRS = sysasm libasm sysclu libclu

OPT    = libclu/Opt/*.o sysclu/Opt/*.o libasm/Opt/*.o sysasm/Opt/*.o
DEBUG1 = libclu/Debugger/*.o sysclu/Debugger/*.o libasm/Debugger/*.o \
         sysasm/Debugger/*.o
DEBUG2 = ../debug/*.o
G      = libclu/Debug/*.o sysclu/Debug/*.o libasm/Debug/*.o sysasm/Debug/*.o

RM     = /bin/rm -f
FLUFF  = *~ *.old *.bak .*~ .*.old .*.bak

optlib: optfiles libpclu_opt.a

athenalib: debugfiles libpclu_athena.a

debuglib: debugfiles libpclu_debug.a

cdebuglib: cdebugfiles libpclu_g.a

optfiles: 
	for i in ${CLIBDIRS}; do (cd $$i; make opt); done

libpclu_opt.a:
	${RM} libpclu_opt.a
	ar cruv libpclu_opt.a ${OPT}
	ranlib libpclu_opt.a

libpclu_debugG4.a: ${OPT}
	${RM} libpclu_debugG4.a
	ar cruv libpclu_debugG4.a ${DEBUG1}
	ranlib libpclu_debugG4.a

libpclu_athena.a: ${DEBUG1}
	${RM} libpclu_athena.a
	ar cruv libpclu_athena.a ${DEBUG1}
	ar d libpclu_athena.a util.o _signal.o new_parm_util.o new_selector.o
	ar q libpclu_athena.a ${DEBUG2}
	ranlib libpclu_athena.a

debugfiles:
	for i in ${CLIBDIRS}; do (cd $$i; make debugger); done
	cd ../debug; make

#libpclu_debug.a: ${DEBUG1} ${DEBUG2}
libpclu_debug.a:
	${RM} libpclu_debug.a
	ar cruv libpclu_debug.a ${DEBUG1}
	ar d libpclu_debug.a util.o _signal.o new_parm_util.o new_selector.o
	ar q libpclu_debug.a ${DEBUG2}
	ranlib libpclu_debug.a

cdebugfiles:
	for i in ${CLIBDIRS}; do (cd $$i; make debug); done

libpclu_g.a: ${G}
	${RM} libpclu_g.a
	ar cruv libpclu_g.a ${G}
	ranlib libpclu_g.a

clean:
	for i in ${CLIBDIRS} gc cmp; do (cd $$i; make clean); done
	${RM} ${FLUFF}

veryclean: clean
	for i in ${CLIBDIRS} gc cmp; do (cd $$i; make veryclean); done
	${RM} libpclu_opt.a libpclu_debug.a libpclu_g.a ${FLUFF}

