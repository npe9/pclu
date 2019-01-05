# Makefile for ~CLU/code/cmp (Linux version)

#  Builds the pclu compiler.
#  Assumes that .c files exist and that ${CLUHOME} is defined

LN = /bin/ln -s
RM = /bin/rm -f
CC = gcc

FLUFF = *~ *.old *.bak .*~ .*.old .*.bak

CFLAGS = -c -O -w -I../include
#CFLAGS = -c -g -I../include 
LFLAGS = -L${CLUHOME}/code -lpclu_opt -lgc -lm -lc -lpthread -g

O_FILES = build.o cconst.o cdchan.o cdefn1.o cdefn2.o ce.o cenv.o \
	cexpr1.o cexpr2.o cmpvar.o coper.o cspecs.o cstmt1.o cstmt2.o \
	dtchan.o du.o eval1.o eval2.o gconst.o gdbg.o genv.o giter.o gparm.o \
	gdefn.o gstmt.o gtbl.o gexpr.o gopt1.o gtypes.o gutil.o gvars.o \
	idn.o init.o lineedit.o pconst.o pdefn1.o pdefn2.o penv.o \
	pexpr.o pjcl.o plist.o pstmt1.o pstmt2.o pttkn.o \
	stable.o syntax.o tkntab.o top1.o top2.o \
	vcmp.o xlib.o \
	../cmpasm/_bin_id.o ../cmpasm/_save_state.o \
	../cmpasm/_restore_state.o ../cmpasm/version.o

C_FILES = build.c cconst.c cdchan.c cdefn1.c cdefn2.c ce.c \
	cenv.c cexpr1.c cexpr2.c cmpvar.c coper.c cspecs.c \
	cstmt1.c cstmt2.c dtchan.c du.c eval1.c eval2.c \
	gconst.c gdbg.c genv.c giter.c gparm.c gdefn.c gstmt.c \
	gtbl.c gexpr.c gopt1.c gtypes.c gutil.c gvars.c \
	idn.c init.c lineedit.c pconst.c pdefn1.c pdefn2.c \
	penv.c pexpr.c pjcl.c plist.c pstmt1.c pstmt2.c \
	pttkn.c stable.c syntax.c tkntab.c \
	top1.c top2.c vcmp.c xlib.c ../cmpasm/_bin_id.c \
	../cmpasm/_save_state.c ../cmpasm/_restore_state.c \
	../cmpasm/version.c

pclu: $(O_FILES)
	cd ../cmpasm; make
	${CC} -o pclu $(O_FILES) $(LFLAGS) 

clean:
	cd ../cmpasm; make clean
	${RM} *.o ${FLUFF}

veryclean:
	cd ../cmpasm; make veryclean
	${RM} pclu npclu basic.junk ${FLUFF}

