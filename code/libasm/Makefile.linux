# Makefile for ~CLU/code/libasm (Linux version)

# Compilation flags for subdirectories of this directory

CFLAGS         = -c -I../../include -DLINUX
OPT_FLAGS      = -O2 
DEBUG_FLAGS    = -g 
DEBUGGER_FLAGS = -O2

# _job.c not currently supported

OBJS  = _all_dir_entries_.o _byteptr.o _byteptr8.o _change_mode.o \
        _change_owner.o _change_signal.o _dtable_size.o _exit.o _file_access.o \
        _file_exists.o _file_facts.o _file_owner.o _get_gid.o \
        _get_pname.o _get_priority.o _get_runtime.o _get_uid.o _get_version.o \
        _get_xjname.o _groups.o _heap_size.o _host_address.o _host_id.o \
        _host_name.o _ioctl.o  _lstat.o _micro_time.o _nice.o _obj.o \
        _page_size.o _pause.o _read_link.o _real_time.o _rusage.o _select.o \
        _set_alarm.o _set_limit.o _set_priority.o _signal.o _sleep.o _stat.o \
        _stop.o _type_base.o _umask.o _wordvec.o create_directory.o \
        delete_directory.o file_date.o file_exists.o gc.o get_argv.o get_env.o \
        get_jcl.o hash.o hash_int.o hash_obj.o istream2chan.o link_file.o \
        mint.o pause.o quit_.o random.o set_connected_dir.o sleep.o \
        stream2chan.o symlink_file.o version.o _free_space.o \
	_get_active_heap.o _get_max_heap.o _set_max_heap.o _get_min_gc.o \
	_set_min_gc.o _cluref_size.o

LN = /bin/ln -s
RM = /bin/rm -f

FLUFF = *~ *.old *.bak .*~ .*.old .*.bak

opt: fictitious_file
	cd Opt; make CFLAGS="${CFLAGS} ${OPT_FLAGS}" OBJS="${OBJS}"

debugger: fictitious_file
	cd Debugger; ${RM} *.c; ${LN} ../Opt/*.c .
	cd Debugger; make CFLAGS="${CFLAGS} ${DEBUGGER_FLAGS}" OBJS="${OBJS}"

debug: fictitious_file
	cd Debug; ${RM} *.c; ${LN} ../Opt/*.c .
	cd Debug; make CFLAGS="${CFLAGS} ${DEBUG_FLAGS}" OBJS="${OBJS}"

fictitious_file:

clean:
	${RM} ${FLUFF}
	cd Opt; make clean
	cd Debug; make clean
	cd Debugger; make clean

veryclean: clean
