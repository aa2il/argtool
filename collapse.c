/*****************************************************************************

      COLLAPSE.C

      Copyright (C) 1993 - 2021 by Joseph B. Attili, aa2il AT arrl DOT net

   Routine to eliminate white space at start & end of string and any 
   duplicate spaces within the string 

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

collapse_white_space(line)

   char  *line;      

{
   int   i,j,n,quote;
   char  ch,clast;

   if( !line[0] ) return(0);
   n=strlen(line);                  /* Get string length */
   j=0;                             /* Point to next available spot */
   clast=' ';                       /* Start out by eliminating leading space */
   quote=0;                         /* Start with all quted strings closed */
   for(i=0; i<n; i++){
      ch = line[i];
      if(ch == '\"') quote = ! quote;       /* Don't touch anything inside a quoted string */
      if( ch < 32 ) ch = 32;                /* Tabs, CRs, etc. are equivalent to spaces for this operation */
      if( ch != 32 || clast != 32 || quote ) line[j++] = ch ;
      clast = ch;
   }
   j--;
   while( line[j]<=32 && j>=0 ) j--;   /* Remove trailing spaces */
   line[j+1] = 0;                      /* Add terminating null */

   return( strlen(line) );             /* Return compresed string length */
} 

