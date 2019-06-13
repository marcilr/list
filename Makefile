##
# Aug-07-2002
# genmake 1.0 by ma_muquit@fccc.edu
##
CC= cc
DEFS=
AR= ar cq
RANLIB= ranlib
LIBNAME= list

INCLUDES=  -I. -I..

# replace -O with -g in order to debug

DEFINES= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= -O $(DEFINES)

SRCS = list.c

OBJS = list.o

.c.o:
	rm -f $@
	$(CC) $(CFLAGS) -c $*.c

all: $(LIBNAME)

$(LIBNAME) : $(OBJS)
	rm -f $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@


clean:
	rm -f $(OBJS) $(LIBNAME) core

cycle: clean all
