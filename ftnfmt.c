/*****************************************************************************

      FTNFMT.C - Routine to output a converted source line so its between 
                 cols 7 & 72.

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
#include <stdarg.h>

void ftn_format_line(FILE *fp,char *fmt,...) {

  va_list pargs;
  static char  line[1024];
  int   i,j;

  /* Initialise pargs to point to the first optional argument */
  va_start(pargs, fmt);

  /* Create formatted line using variable arg version of sprintf */
  vsprintf(line,fmt,pargs);

  // Write out formatted line, possible over multiple lines if needed
  fprintf(fp,"      ");                        /* Start in col 7 */
  j=7;
  for(i=0; i<strlen(line); i++ ) {
    if( j>72 ) {
      j=7;
      fprintf(fp,"\n     &");                /* Continue on next line */
    }
    fprintf(fp,"%c",line[i]);
    j++;
  }
  fprintf(fp,"\n");

  // Clean-up & exit
  va_end(pargs);  
}

