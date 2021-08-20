/*****************************************************************************

      MARKERF.C - Routine to return the number of times a flags or var 
                  was specified - FTN version.

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

#include <string.h>
#include <stdio.h>
#include "argtool.h"

int arg_entries;
struct table_entry_ftn *arg_table ;

#if defined(sun) || defined(mips) || defined(__PARAGON__)
long get_marker_ftn_
#else
#ifdef linux
//long get_marker_ftn__
long get_marker_ftn_
#else
#define fortran 
long fortran get_marker_ftn
#endif
#endif
     (flag,arg_entries_p,arg_table_p )

   char  *flag;
   long *arg_entries_p;
   struct table_entry_ftn *arg_table_p ;

{
   int   found,n;

   /* Fortran interface to args */
   arg_entries = *arg_entries_p;
   arg_table = arg_table_p;

   /* Look for the flag */
   found = -1;
   for(n=0; n<arg_entries; n++){
      if( !strcmp(flag,arg_table[n].uname) ) found = n;
   }

   if( found < 0 ) {
      fprintf(stderr,"\nMARKER: Warning - Unrecongized flag/variable %s \n\n",flag);
      return(0);
   } else {
      return( arg_table[found].occurances );
   }

}

