# Makefile for ~CLU/driver (Linux version)

# Builds the PCLU driver (an alternative to pclu/plink)

# Options for compiling the driver

CC     = gcc
CFLAGS = -g

# Options to compile into the driver

PCLU_CLUHOME         = $(CLUHOME)
PCLU_CC              = $(CC)
PCLU_CFLAGS          = -w
PCLU_CFLAGS_DEBUG    = -w
PCLU_CFLAGS_OPTIMIZE = -w -O2

# Pass options for compiling driver.o

DRIVER_OPTIONS = \
    -DCLUHOME="\"$(PCLU_CLUHOME)\"" \
    -DC_COMPILER="\"$(PCLU_CC)\"" \
    -DCFLAGS_NORMAL="\"$(PCLU_CFLAGS)\"" \
    -DCFLAGS_DEBUG="\"$(PCLU_CFLAGS_DEBUG)\"" \
    -DCFLAGS_OPTIMIZE="\"$(PCLU_CFLAGS_OPTIMIZE)\""

OBJS = driver.o buffer.o error.o wordlist.o
SRCS = driver.c buffer.c error.c wordlist.c

RM    = /bin/rm -f
FLUFF = *~ *.old *.bak .*~ .*.old .*.bak

PCLU: $(OBJS)
	$(CC) -o PCLU $(OBJS)

driver.o:
	$(CC) ${CFLAGS} $(DRIVER_OPTIONS) -c driver.c

clean:
	${RM} PCLU $(OBJS) ${FLUFF}

veryclean: clean

driver.o: bool.h buffer.h proto.h error.h wordlist.h driver.c
buffer.o: buffer.h proto.h wordlist.h bool.h buffer.c
error.o: error.h proto.h error.c
wordlist.o: wordlist.h bool.h proto.h wordlist.c
