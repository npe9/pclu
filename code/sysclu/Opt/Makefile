# Makefile for ~CLU/code/sys/Opt

LIB_DIR	= ${CLUHOME}/lib
LIBS 	= misc lowlev useful
SYS_CLU = ~CLU/sys/clu

RM      = /bin/rm -f
FLUFF   = *~ *.old *.bak .*~ .*.old .*.bak

o_files: ${OBJS}

c_files:
	cd ${LIB_DIR}; make libs
	$(CLU)  \#ext false \#dir ${LIB_DIR} \#me ${LIBS} 	\
		\#spec _gprof.spc \#dir ${SYS_CLU} 		\
		\#bindir `pwd` \#ccopt 0			\
		\#opt time \#co \*

clean:
	${RM} *.o ${FLUFF}

veryclean: clean
