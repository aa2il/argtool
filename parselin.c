/*****************************************************************************

      PARSELIN.C - Routine to find a given word number in a line

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

char  word[1050];

char *parse_line(line,iword,lang)

   char  *line;      /* A line of text with the white space removed */
   int   iword;      /* Word number */
   int   lang;       /* Lanugage code */

{
   int   i,j,k,n,quote;
   char  ch,*p,qch;

   /* What is the quote char ? */
   if( lang==C_LANG ) {
      qch = '\"';
   } else if( lang==FTN_LANG ) {
      qch = '\'';
   } else if( lang==BAT_LANG || lang == DCL_LANG || lang == CSH_LANG || lang == TCL_LANG) {
      qch = '\"';
   } else {
      fatal_error(1,"parse_line: Unrecognized language code %d\n",lang);
   }

   /* Locate the word */
   quote=0;                                  /* Start with all quted strings closed */
   n=strlen(line);                           /* Get string length */
   k=0;                                      /* Position at end of this word */
   for(i=0; i<iword; i++){
      j=k;                                   /* Position at start of this word */
      if(j==n) {
         word[0] = 0;
         p = strdup(word) ;
         return( p );
      }

      ch='X';
      while( k<n && (ch != ' ' || quote) ) {
         ch = line[k];
         if(ch == qch) quote = ! quote;      /* Don't touch anything inside a quoted string */
         k++;                                /* Point to start of next word */
      }

   }
   if(k==n) k++;

   /* Copy the string */
   for(i=0; i<k-j-1; i++) word[i] = line[i+j];
   word[k-j-1]=0;

   return( strdup(word) );
} 

