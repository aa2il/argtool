# *****************************************************************************
#
#     Makefile for Unix for ArgTool
#
#      Copyright (C) 1993 - 2021 by Joseph B. Attili, aa2il AT arrl DOT net
#
# *****************************************************************************
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
# *****************************************************************************

#CC	      = icc

#FC            = ifc

#FFLAGS	      = -g -w -cm

#CFLAGS	      = -g -w

CC	      = gcc

FC            = gfortran

#FFLAGS	      = -g -w -m32 -Dlinux -cpp

#CFLAGS	      = -g -w -m32 -Dlinux

FFLAGS	      = -g -w -Dlinux -cpp -fdec-structure

CFLAGS	      = -g -w -Dlinux

DEST	      = .

EXTHDRS	      = 

HDRS	      = argtool.h

#LDFLAGS	      = -L/usr/local/lib -Bstatic

#LDFLAGS	      = -Vaxlib

#LDFLAGS	      = -m32

LDFLAGS	      =

SLIBS	      = 

ULIBS         = 

LIBRARY	      = libargtool.a

MAKEFILE      = Makefile

PROGRAM       = argtool

OBJS	      = argtool.o \
                collapse.o \
		detab.o \
		error.o \
		findargs.o \
		ftnfmt.o \
		machine.o \
		marker.o \
		markerf.o \
		parsecmd.o \
		parselin.o \
		parsetab.o \
		pb.o \
		pf.o \
		saveinfo.o \
		spltpath.o \
		strdup.o

PRINT	      = pr

#LINKER	      = ifc 

LINKER	      = gfortran

SRCS	      = argtool.arg \
                collapse.c \
		detab.c \
		error.c \
		findargs.f \
		ftnfmt.c \
		machine.c \
		marker.c \
		markerf.c \
		parsecmd.c \
		parselin.c \
		parsetab.c \
		pb.c \
		pf.c \
		saveinfo.c \
		spltpath.c \
		strdup.c

DEMOS         = demo \
                demo2 

#                demo5 


DEMOSRCS      = demo.arg \
                demo2.arf

DEMOOBJS      = demo.o \
		demo.c \
                demo2.f \
                demo2.o

SUFFIX        = .arg:sC .arf:sF .for:sF

LIBS          = $(LIBRARY)

INCS          = argtool.inc

.SUFFIXES: .arg .arf .for

.arf.f:
	argtool $*.arf -ftn

.arg.c:
	argtool $*.arg -c

.arg.o:
	argtool $*.arg -c
	$(CC) $(CFLAGS) -c $*.c

.arf.o:
	argtool $*.arf -ftn
	$(FC) $(FFLAGS) -c $*.for

.for.o:
	$(FC) $(FFLAGS) -c $*.for

all:		$(LIBRARY) $(PROGRAM) $(DEMOS)

$(DEMOS):       $(PROGRAM) $(LIBRARY) $(DEMOOBJS)
		@echo -n "Building C demo ... "
		@$(LINKER) $(LDFLAGS) demo.o \
			$(ULIBS) $(LIBS) $(SLIBS) $(EXTLIBS) 	\
			-o demo 
		@echo -n "Building FORTRAN demo ... "
		@echo -n "$(LINKER) $(LDFLAGS) demo2.o 	$(ULIBS) $(LIBS) $(SLIBS) $(EXTLIBS) -o demo2 "
		@$(LINKER) $(LDFLAGS) demo2.o 	\
			$(ULIBS) $(LIBS) $(SLIBS) $(EXTLIBS) 	\
			-o demo2 
		@echo "done"

$(DEMOOBJS):    $(DEMOSRCS)

$(PROGRAM):     $(LIBS) $(ULIBS) $(GLIBS)
		@echo -n "Loading prog $(PROGRAM) ... "
		@echo -n $(LINKER) $(LDFLAGS) $(PROGRAM).o 	\
			$(ULIBS) $(LIBS) $(SLIBS) $(EXTLIBS) 	\
			-o $(PROGRAM) 
		@$(LINKER) $(LDFLAGS) $(PROGRAM).o 	\
			$(ULIBS) $(LIBS) $(SLIBS) $(EXTLIBS) 	\
			-o $(PROGRAM) 
		@echo "done"

$(LIBRARY):	$(OBJS)
		@echo -n "Loading $(LIBRARY) ... "
		@ar cr $(LIBRARY) $(OBJS)
		@ar d $(LIBRARY) argtool.o
		@ranlib $(LIBRARY)
		@echo "done"

clean:;		@rm -f $(OBJS) $(DEMOS) $(DEMOOBJS) $(PROGRAM) $(LIBRARY)

depend:;	@mkmf -f $(MAKEFILE) LIBRARY=$(LIBRARY) DEST=$(DEST)

extract:;	@ar xo $(DEST)/$(LIBRARY)
		@rm -f __.SYMDEF

index:;		@ctags -wx $(HDRS) $(SRCS)

install:	$(PROGRAM) $(LIBRARY) clean
		@echo Installing $(LIBRARY) in $(DEST)
		@install $(LIBRARY) $(DEST)
		@ranlib $(DEST)/$(LIBRARY)
		@echo Installing $(PROGRAM) in $(DEST)
		@install -s $(PROGRAM) $(DEST)

library:        $(LIBRARY)

print:;		@$(PRINT) $(HDRS) $(SRCS)

lprint:;	fold -w 132 $(HDRS) $(SRCS) | rsh adiron /usr/local/bin/imprint -J -Pip -L -h$(PROGRAM)

tags:           $(HDRS) $(SRCS); @ctags $(HDRS) $(SRCS)

update:         $(DEST)/$(LIBRARY) $(DEST)/$(PROGRAM)

#$(DEST)/$(PROGRAM): $(SRCS) $(LIBS) $(HDRS) $(EXTHDRS)
#	$(LINKER) $(OBJS)

#$(DEST)/$(LIBRARY): $(SRCS) $(HDRS) $(EXTHDRS)
#		@-ar xo $(DEST)/$(LIBRARY)
#		@make -f $(MAKEFILE) DEST=$(DEST) install clean

demo2.f : demo2.arf argtool.inc
