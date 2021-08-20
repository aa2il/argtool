/*****************************************************************************

      DETAB.C - Routine to expand tabs in a string.

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
#include <stdlib.h>
#ifndef vms
#include <malloc.h>
#endif
#include "argtool.h"

#define TAB_SPACING  8

char* detab(string)

   char *string;

{
   int   i,j,k,j9,k9,ipass;
   char  *s;

   /* Its a 2-pass process - on the 1st pass, we determine length of expanded
             string, on the 2nd, we expand the tabs */
   for(ipass=0; ipass<2; ipass++ ) {

      if(ipass) s = malloc(k+1);             /* Reserve space for expanded string */

      /* Look for tabs */
      j=0;
      k=0;
      for(i=0; i<strlen(string); i++ ) {

         if( string[i] == '\t' ) {

            /* Here's a tab - expand it */
            j9 = TAB_SPACING*(j / TAB_SPACING + 1) ;
            for(k9=0; k9<j9-j; k9++) {
               if(ipass) s[k] = ' ';
               k++;
            }
            j=j9;

         } else {

            /* Just copy non-tab chars */
            if(ipass) s[k] = string[i];
            k++;

            /* Keep track of char returns */
            if( string[i] == '\n' ) 
               j=0;
            else if( string[i] == '\\' && string[i+1] == 'n') {
               j=0;
            } else
               j++;
         }

      }

   }

   s[k]=0;

   return(s);
}



/* This routine converts everything to ascii instead of the C encoded formatting */
char* detab2(string)

   char *string;

{
   int   i,j,k,j9,k9,ipass;
   char  *s;

   /* Its a 2-pass process - on the 1st pass, we determine length of expanded
             string, on the 2nd, we expand the tabs */
   for(ipass=0; ipass<2; ipass++ ) {

      if(ipass) s = malloc(k+1);             /* Reserve space for expanded string */

      /* Look for tabs */
      j=0;
      k=0;
      for(i=0; i<strlen(string); i++ ) {

         if( string[i] == '\t' ) {

            /* Here's a tab - expand it */
            j9 = TAB_SPACING*(j / TAB_SPACING + 1) ;
            for(k9=0; k9<j9-j; k9++) {
               if(ipass) s[k] = ' ';
               k++;
            }
            j=j9;

         } else if( string[i] == '\\' && string[i+1] == 't') {

            /* Here's a tab - expand it */
            j9 = TAB_SPACING*(j / TAB_SPACING + 1) ;
            for(k9=0; k9<j9-j; k9++) {
               if(ipass) s[k] = ' ';
               k++;
            }
            j=j9;
            i++;

         } else {

            /* Just copy non-tab chars */
            if(ipass) s[k] = string[i];
            k++;

            /* Keep track of char returns */
            if( string[i] == '\n' ) 
               j=0;
            else if( string[i] == '\\' && string[i+1] == 'n') {
               j=0;
               string[i] = 13;
               string[i+1] = 10;
            } else
               j++;
         }

      }

   }

   s[k]=0;

   return(s);
}

