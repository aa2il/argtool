/*****************************************************************************

      MACHINE.C - Module with stuff that varies from machine to machine.

      Copyright (C) 1993 - 2021 by Joseph B. Attili, aa2il AT arrl DOT net

******************************************************************************

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

*****************************************************************************/

#include <stdio.h>
#include "argtool.h"

/* ************************* DEFS for MS DOS ************************* */

#ifdef MSDOS
#define fortran 
extern short fortran nargs();
extern void fortran getarg();

long fortran machine_type()
{
   return(1);
}

long fortran number_of_args()
{
   return(( long) nargs() );
}

void fortran lib$get_foreign(s)
   char *s;
{
   fatal_error(1,"lib$get_foreign: Unexpected subr call for DOS\n");
}
#endif

/* ************************* DEFS for Unix ************************* */

#if defined(sun) || defined(mips) || defined(__PARAGON__) || defined(linux)
   extern long iargc_();
   extern void getarg_();

//#ifdef linux
//long machine_type__()
//#else
long machine_type_()
     //#endif

{
#ifdef sun
   return(2);
#else
   return(3);
#endif
}

#ifdef linux
//long number_of_args__()
#else
long number_of_args_()
{
  return( iargc() );
}
#endif

#ifndef linux
void lib$get_foreign_(s)
   char *s;
{
   fatal_error(1,"lib$get_foreign: Unexpected subr call for Unix\n");
}
#else
#if 0
void getarg_(i,argv)
   long *i;
   char *argv;
{
   fatal_error(1,"getarg: Unexpected call for VMS 1\n");
}
#endif
#endif

#endif

/* ************************* DEFS for VMS ************************* */

#ifdef vms
long machine_type()
{
   return(4);
}

long number_of_args()
{
   fatal_error(1,"number_of_args: Unexpected subr call for VMS\n");
}

void getarg(i,argv)
   long *i;
   char *argv;
{
   fatal_error(1,"getarg: Unexpected call for VMS 2\n");
}
#endif


