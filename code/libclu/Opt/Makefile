# Makefile for ~CLU/code/libclu/Opt

CLU	= ../../../exe/pclu
LIB_DIR	= ../../../lib
LIBS 	= misc lowlev useful
LIB_CLU = ~CLU/lib/clu

RM      = /bin/rm -f
FLUFF   = *~ *.old *.bak .*~ .*.old .*.bak

o_files: ${OBJS}

c_files:
	cd ${LIB_DIR}; make libs
	$(CLU)  \#bindir `pwd` \#ext false \#dir ${LIB_DIR} \#me ${LIBS} \
		\#ce ${LIB_DIR}/gcd.equ \
		\#spec _host_address.spc _all_dir_entries_.spc \
		\#dir ${LIB_CLU} 		\
		\#spec gc_read \
		\#opt time \#cf \#co \*

clean:
	${RM} *.o ${FLUFF}

veryclean: clean

