# Makefile for ~CLU/lib/clu

# Builds test_gcd without having to build ${LIBS}

CLU       = $(CLUHOME)/exe/pclu
LIB_DIR   = ${CLUHOME}/lib
LIB_CLU   = ${CLUHOME}/lib/clu
SYS_CLU   = ${CLUHOME}/sys/clu

LIBS 	  = misc.lib lowlev.lib useful.lib

RM        = /bin/rm -f
FLUFF     = *~ *.old *.bak .*~ .*.old .*.bak

MISC_SOURCES = \
	${LIB_CLU}/_int_range.clu 	${LIB_CLU}/bigint.clu 		\
	${LIB_CLU}/dparse.clu 		${LIB_CLU}/heap.clu		\
	${LIB_CLU}/lex_scan.clu 	${LIB_CLU}/os.clu 		\
	${LIB_CLU}/parse_line.clu	${LIB_CLU}/quicksort.clu 	\
	${LIB_CLU}/quick_sort.clu 	${LIB_CLU}/scase.clu 		\
	${LIB_CLU}/split_line.clu 	${LIB_CLU}/string_lindexs.clu	\
	${LIB_CLU}/string_ops.clu 	${LIB_CLU}/strim.clu 		\
	${LIB_CLU}/xref.clu 

LOWLIB_SOURCES = \
	${LIB_CLU}/_resolve.clu		${LIB_CLU}/_signal_type.clu	\
	${LIB_CLU}/gcd_tab.clu		${LIB_CLU}/inet_address.clu

LOWSYS_SOURCES = \
	${SYS_CLU}/_cleanup_.clu 	${SYS_CLU}/_fixup_dir_part.clu	\
	${SYS_CLU}/_fixup_file_name.clu	${SYS_CLU}/_skip_fill.clu	\
	${SYS_CLU}/_termcap.clu		${SYS_CLU}/_working_dir.clu

LOW_EQU = \
	${LIB_DIR}/gcd.equ

LOW_SPECS = \
	${LIB_DIR}/gcd_tab.spc		${LIB_DIR}/_adv.spc		\
	${LIB_DIR}/_byteptr.spc		${LIB_DIR}/_byteptr8.spc	\
	${LIB_DIR}/_bytevec.spc 	${LIB_DIR}/_chan.spc 		\
	${LIB_DIR}/_event.spc 		${LIB_DIR}/_gc.spc		\
	${LIB_DIR}/_job.spc		${LIB_DIR}/_obj.spc		\
	${LIB_DIR}/_tagcell.spc		${LIB_DIR}/_vec.spc		\
	${LIB_DIR}/_wordvec.spc		${LIB_DIR}/etc.spc

USELIB_SOURCES = \
	${LIB_CLU}/_all_file_names_.clu ${LIB_CLU}/_longsub.clu 	\
	${LIB_CLU}/_qindexc.clu 	${LIB_CLU}/_time_format.clu 	\
	${LIB_CLU}/_unquote.clu 	${LIB_CLU}/all_file_names.clu 	\
	${LIB_CLU}/array_replace.clu 	${LIB_CLU}/cos.clu		\
	${LIB_CLU}/delete_expunge.clu 	${LIB_CLU}/gc_copy.clu 		\
	${LIB_CLU}/gc_dump.clu 		${LIB_CLU}/gc_read.clu 		\
	${LIB_CLU}/hex.clu		${LIB_CLU}/isqrt.clu 		\
	${LIB_CLU}/octal.clu 		${LIB_CLU}/open_append.clu 	\
	${LIB_CLU}/open_read.clu 	${LIB_CLU}/open_write.clu 	\
	${LIB_CLU}/run_time.clu 	${LIB_CLU}/tan.clu 		\
	${LIB_CLU}/time.clu 		${LIB_CLU}/unify_spacing.clu

USESYS_SOURCES = \
	${SYS_CLU}/file_name_fill.clu

USE_SPECS = \
	${LIB_DIR}/_signal.spc 		${LIB_DIR}/etc.spc 		\
	${LIB_DIR}/maybe.spc 		${LIB_DIR}/mint.spc 		\
	${LIB_DIR}/table.spc

all:
	$(CLU) \#opt \#cfiles TRUE \
	    \#spec ${MISC_SOURCES} \
            \#ce ${LOW_EQU} \
	    \#spec ${LOWLIB_SOURCES} ${LOW_SPECS} \
	    \#spec ${LOWSYS_SOURCES} \
            \#spec ${USE_SPECS} ${USELIB_SOURCES} ${USESYS_SOURCES} \
	    \#clu gc_read, gcd_tab, gc_dump, test_gcd
	cp gcd_tab.c gc_read.c gc_dump.c ${CLUHOME}/code/libclu/Opt
	cd ${CLUHOME}/code; rm libpclu_opt.a;  make
	${CLUHOME}/exe/plink test_gcd.o -o test_gcd
	./test_gcd

clean:
	${RM} *.c *.o pclu.xref ${FLUFF}

veryclean: clean
	${RM} test_gcd
