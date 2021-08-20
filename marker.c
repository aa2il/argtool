/*****************************************************************************

      MARKER.C - Routine to return the number of times a flags or var was 
                 specified - C version.

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

extern int arg_entries;
extern struct table_entry arg_table[] ;

MARKER(flag)

   char  *flag;

{
   int   found,n;

   /* Look for the flag */
   found = -1;
   for(n=0; n<arg_entries; n++){
      if( !strcmp(flag,arg_table[n].uname) ) found = n;
   }

   if( found < 0 ) {
      if( !strcmp(flag,"-help") )
        fprintf(stderr,"\nMARKER: Warning - Unrecongized flag/variable %s \n\n",flag);
      return(0);
   } else {
      return( arg_table[found].occurances );
   }

}

