# Makefile for ~CLU/code/sysclu (Linux version)

# Compilation flags for subdirectories of this directory

CFLAGS         = -I../../include -c

OPT_FLAGS      = -O2 
DEBUG_FLAGS    = -g 
DEBUGGER_FLAGS = -O2

LN             = /bin/ln -s
RM             = /bin/rm -f
FLUFF          = *~ *.old *.bak .*~ .*.old .*.bak

OBJS  = _cause_bounds.o _cause_illegal_char.o _cause_overflow.o \
        _cause_wrong_tag.o _cause_wrong_type.o _cleanup_.o _dir_ent_size.o \
        _erstr.o _fixup_dir_part.o _fixup_file_name.o _get_termcap.o \
        _home_dir.o _last_indexc.o _skip_fill.o _split_name_part.o \
        _termcap.o _termcap_entry.o _working_dir.o date.o e_form.o f_form.o \
        file_name.o file_name_fill.o file_spec.o g_form.o set_working_dir.o \
        stream.o user_name.o working_dir.o

opt: fictitious_file
	cd Opt; make CFLAGS="${CFLAGS} ${OPT_FLAGS}" OBJS="${OBJS}"

debugger: fictitious_file
	cd Debugger; make CFLAGS="${CFLAGS} ${DEBUGGER_FLAGS}" OBJS="${OBJS}" c_files
	cd Debugger; make CFLAGS="${CFLAGS} ${DEBUGGER_FLAGS}" OBJS="${OBJS}" all

debug: fictitious_file
	cd Debug; ${RM} *.c; ${LN} ../Opt/*.c .
	cd Debug; make CFLAGS="${CFLAGS} ${DEBUG_FLAGS}" OBJS="${OBJS}"

fictitious_file:

clean:
	${RM} */*.o Debugger/*.c Debug/*.c ${FLUFF}
	cd Debug; make clean
	cd Debugger; make clean
	cd Opt; make clean

veryclean: clean

clean: clean


