/*****************************************************************************

      SPLTPATH.C - Routine to split a filname into its path, name & extension 

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
#include "argtool.h"

void split_path(fname,path,name,ext)

char *fname;            /* Complete file spec */
char *path;             /* Path name */
char *name;             /* File name */
char *ext;              /* Extension */

{
   int      i,iext,iname,j;
   char     ch;

   /* Find extension */
   iext = 0;
   for(i=strlen(fname)-1 ; i>=0 ; i-- ){
      ch = fname[i];
      if( (ch==']') || (ch==':') || (ch=='[') || (ch=='/') || (ch=='\\') ) {
         break;
      } else if( ch=='.') {
         iext=i ;
         break;
      }
   }

   /* Copy the extension */
   if( iext > 0) {
      for(i=0 ; i<4 ; i++ ){
         ext[i] = fname[iext+i];
         if( !ext[i] ) break;
      }
   } else {
      for(i=0 ; i<4 ; i++ ) ext[i] = ' ';
      iext = strlen(fname);
   }
   ext[4] = 0;

   /* Find name */
   iname = 0;
   for(i=iext-1 ; i>=0 ; i--){
      ch = fname[i];
      if( (ch==':') || (ch=='\\') || (ch==']') || (ch=='/') ){
         iname = i+1;
         break;
      }
   }

   /* Copy name */
   j=0;
   for(i=iname ; i<iext ; i++ ) name[j++] = fname[i];
   name[j] = 0;

   /* Copy path */
   for(i=0 ; i<iname ; i++ ) path[i] = fname[i];
   path[i] = 0;

}

