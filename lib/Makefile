# Mafefile for ~CLU/lib

# Builds clu libraries

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

NOT_LOW = \
	_change_mode		_change_owner		\
	_dir_ent_size		_environs		\
	_exit			_file_access 		\
	_file_exists 		_file_facts 		\
	_file_owner 		_get_pname 		\
	_get_runtime 		_get_version 		\
	_get_xjname 		_host_name 		\
	_local_time 		_real_time 		\
	_sleep 			_stop 			\
	_system_root 		create_directory	\
	connected_dir		delete_directory	\
	exp 			file_date 		\
	file_exists 		gc			\
	get_argv 		get_jcl			\
	host_address		istream2chan 		\
	link_file 		log			\
	now 			quit_ 			\
	set_connected_dir 	sin			\
	sleep 			sqrt			\
	symlink_file		user_name		\
	unix_cmd

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

NOT_USE = \
	_anyize _change_owner _change_signal _cvt _dtable_size _environ   \
	_environs _erstr _ernum _free_space gcb gco gcr _get_gid _get_pid \
	_get_priority _get_termcap _get_uid _groups _heap_size _home_dir  \
	_host_id _ioctl _last_indexc _local_time _lstat _page_size _pause \
	_read_link _round_real _rusage _select _set_alarm _set_limit 	  \
	_set_priority _split_name_part _stat _termcap_entry _umask 	  \
	_unparse_real istream2chan now stream2chan user_name

MISC_SPECS = ${LIB_DIR}/random.spc

libs: ${LIBS}

misc.lib: ${MISC_SOURCES} ${MISC_SPECS}
	$(CLU) \#ext false \#newlib \
	    \#spec ${MISC_SOURCES} ${MISC_SPECS} \
	    \#dump misc 

lowlev.lib: ${LOW_SPECS} ${LOW_EQU} ${LOWLIB_SOURCES} ${LOWSYS_SOURCES}
	$(CLU) \#ext false \#newlib \
            \#spec ${LOW_SPECS} \#ce ${LOW_EQU} \
	    \#spec ${LOWLIB_SOURCES} ${LOW_SPECS} ${LOWLIB_SOURCES} \
	    \#spec ${LOWSYS_SOURCES} \
	    \#forget ${NOT_LOW}	\
	    \#dump lowlev 

useful.lib: lowlev.lib ${LOW_EQU} ${USE_SPECS} ${USELIB_SOURCES} \
	    ${USESYS_SOURCES}
	$(CLU) \#ext false \#newlib \
            \#me lowlev.lib \#ce ${LOW_EQU} \
            \#spec ${USE_SPECS} ${USELIB_SOURCES} ${USESYS_SOURCES} \
	    \#forget ${NOT_USE} \#unmerge lowlev.lib \
	    \#dump useful 

clean:
	${RM} ${FLUFF}
	cd clu; make clean

veryclean:
	${RM} ${FLUFF} *.lib
	cd clu; make veryclean
