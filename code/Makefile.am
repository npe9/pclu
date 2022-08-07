SUBDIRS = gc sysasm libclu libasm sysclu cmp     cmpasm   


noinst_LIBRARIES = libpclu_opt.a
libasm_opt_a_CFLAGS= -I../../include/ -DLINUX
LDADD = ./code/libclu/Opt/libclu_opt.a ./code/libasm/Opt/libasm_opt.a ./code/libgc.a  ./code/libpclu.a ./code/sysasm/Opt/libsysasm_opt.a