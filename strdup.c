/*****************************************************************************

      STRDUP.C - Routines to duplicate a string & compare strings. 
                 Needed to make up for a deficincy in Dec's libs 

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

/* Will eventually not need this for MS DOS */
#if defined(mips) || defined(vms) || defined(MSDOS) || defined(__PARAGON__) || defined(linux)

#include <string.h>
#include <ctype.h>
#ifndef vms
#include <malloc.h>
#endif
#include "argtool.h"

#if 0
#ifndef linux
char *strdup(string)

   char   *string;

{
   int   i,l;
   char    *dup;

   l=strlen(string) + 1;

   /* Need one extra char bx of bug elsewhere - need to find out where ! */
   if( ( dup = (char *) malloc(l+1) ) == 0 )
      fatal_error(1,"strdup: Unable to allocate memory\n");

   for(i=0; i<l; i++) dup[i] = string[i];

   return( dup );
}
#endif
#endif


#ifndef MSDOS 
/* Routine to compare two strings */
int strnicmp(s1,s2,n)

   char *s1,*s2;
   int n;

{
   char c1,c2;
   int i;

   for(i=0; i<n; i++) {
      if( *s1==0 && *s2==0 ) return(0);
      if( *s1==0 ) return(-1);
      if( *s2==0 ) return(1);
      if( toupper(*s1) < toupper(*s2) ) return(-1);
      if( toupper(*s1) > toupper(*s2) ) return(1);
      s1++;
      s2++;
   }
   return(0);
}
#endif 

#endif

#ifdef vms
/* Routine to compare two strings */
int strcmpi(s1,s2)

   char *s1,*s2;

{
   while( *s1 != 0  &&  *s2 != 0 ) {
      if( toupper(*s1) < toupper(*s2) ) return(-1);
      if( toupper(*s1) > toupper(*s2) ) return(1);
      s1++;
      s2++;
   }
   if( *s1==0 && *s2==0 ) return(0);
   if( *s1==0 ) return(-1);
   if( *s2==0 ) return(1);
}

#endif

