/*****************************************************************************

      ERROR.C - Fatal error processor.

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

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef MSDOS
/*#include <io.h> */
#endif
#include "argtool.h"

extern char *PROGRAM_NAME;
extern char *OUTFILE;
extern int ARGTOOL;

fatal_error(int code, char *fmt,...){
  
  va_list pargs;
  
  /* Initialise pargs to point to the first optional argument */
  va_start(pargs, fmt);

  //printf("ERROR: fmt=%s\n",fmt);

  // Use variable arg version of fprintf
  vfprintf(stderr,fmt,pargs);
#ifdef VMS
  exit (7474);
#else
  exit (code);
#endif

  // Clean-up & exit
  va_end(pargs);  
}

