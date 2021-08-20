/*****************************************************************************

      PARSETAB.C - Routine to parse an entry from the table.

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
#include "argtool.h"

parse_table_entry(iline,line,entry,num_entry,lang)

   int                  iline;      /* Source file line number */
   char                 *line;      /* One line from the table */
   struct table_entry   *entry;     /* Parsed table entry */
   int                  num_entry;  /* The entry number */
   int                  lang;       /* Lanugage code */

{
   int         i,n;
   static int  flagged=0;
   static int  count=0;
   static int  repeat_flag=0;
   char        *cmt;
   char        *lp,*rp;

   /* Strip off comment */
   cmt = strchr(line ,'#');
   if( cmt ) {
      cmt[0] = 0  ;
      cmt[strlen(cmt+1)] = 0;
   }

   /* Remove excess white space */
   collapse_white_space(line);

   /* Ignore blank & comment lines */
   if( !strlen(line) ) return(0);

   /* Get USAGE name */
   entry->uname = parse_line(line,1,lang);
   n=strlen(entry->uname)-1; 

   /* Is this arg repeated ? */
   if( !strncmp(entry->uname+n-2,"...",3) ) {
     if( lang==DCL_LANG || lang==BAT_LANG ) {
       fatal_error(1,"argtool(parsetab): Repeated arguments not supported for DOS BATCH or VMS DCL processing.\n");
     } else {
       entry->repeated = 1;
       n -= 3;
     }
   } else {
      entry->repeated = 0;
   }

   /* Is this arg optioanl ? */
   if( entry->uname[0] == '<' && entry->uname[n] == '>' ) {
      entry->required = 1;
   } else if( entry->uname[0] == '[' && entry->uname[n] == ']' ) {
      entry->required = 0;
   } else {
      fatal_error(1,"argtool(parsetab): Syntax error on line %d in table\n",iline);
   }
   entry->uname[n] = 0;              /* Remove Bracketing */  
   strcpy( entry->uname ,entry->uname+1 );

   /* Is this a flag or a variable ? */
   if( entry->uname[0] != '-' ) {

      /* Its a variable - Get its type */
      entry->flag = 0;
      entry->vartype = parse_line(line,2,lang);
      if( repeat_flag ) entry->repeated = 1;
      if( entry->repeated && lang==C_LANG ) strcat( entry->vartype ,"*");  

      /* Get its name */
      entry->varname = parse_line(line,3,lang);

      /* Get its initial value */
      entry->initvalue = parse_line(line,4,lang);
      if( strlen(entry->initvalue) && entry->repeated )
         fatal_error(1,"Repeated variable %s on line %d in table cannot have initial value\n",entry->varname,iline);

      /* Get comment */
      if( cmt ) {
         entry->comment = strdup(cmt+1);
      } else {
         entry->comment = strdup(" ");
      }

      /* Keep track of attributes */
      entry->flagged = flagged;
      entry->param_number = count++;

      /* Don't put point *'s with variable names */
      i=0;
      while( entry->varname[i] == '*' ) {
         entry->varname[i++] = ' ' ;
         strcat(entry->vartype ,"*");
      }

      /* Determine if this is a pointer */
      entry->pointer = 0;
      if( lang==C_LANG ) {
         for(i=0; i<strlen(entry->vartype); i++)
            if( entry->vartype[i] == '*' ) entry->pointer++ ;
         if( entry->pointer > 3 )
            fatal_error(1,"Unrecognized variable type %s on line %d in table\n",entry->vartype,iline);

      } else if( lang==FTN_LANG ) {
         lp = strchr( entry->varname , '(' );
         if( lp && !entry->repeated )
            fatal_error(1,"Array variable %s on line %d in table must be repeated\n",entry->varname,iline);
         else if( !lp && entry->repeated )
            fatal_error(1,"Repeated variable %s on line %d in table must be array\n",entry->varname,iline);
         else if( lp ) {
            entry->pointer++ ;
            rp = strchr( lp+1, ')' );
            if( !rp ) fatal_error(1,"Syntax error on line %d in table - array size needed\n",iline);
            *lp = 0;
            *rp = 0;
            entry->repeated = atoi( lp+1 );
            if( entry->repeated <=0 ) fatal_error(1,"Syntax error on line %d in table - non-zero array size needed\n",iline);
         }

      }

      /* Determine propoer i/o format & element size */
      if( lang==BAT_LANG || lang == DCL_LANG || lang == CSH_LANG || lang == TCL_LANG ){

         /* We treat all batch stuff as strings */
        if( !strncmp(entry->vartype,"string",4) ) {
          entry->format = strdup("%s");
          entry->size   = sizeof(char);
        } else if( ( lang==CSH_LANG || lang==TCL_LANG ) && 
                   ( !strncmp(entry->vartype,"int",3) ||
                     !strncmp(entry->vartype,"float",3) )) {
          free(entry->vartype);
          entry->vartype = strdup("string");
          entry->format = strdup("%s");
          entry->size   = sizeof(char);
        } else {
          fatal_error(1,"argtool(parsetab): Unrecognized variable type %s on line %d in table\n",entry->vartype,iline);
         } 

      } else if( lang==C_LANG && !strncmp(entry->vartype,"int",3) ) {
         entry->format = strdup("%d");
         entry->size   = sizeof(int);
      } else if( !strcmpi(entry->vartype,"integer*2") ) {
         entry->format = strdup("%d");
         entry->size   = sizeof(short);
      } else if( lang==C_LANG && !strncmp(entry->vartype,"unsigned",8) ) {
         entry->format = strdup("%u");
         entry->size   = sizeof(unsigned);
      } else if( (lang==C_LANG && !strncmp(entry->vartype,"long",4)) || !strnicmp(entry->vartype,"integer",7) ) {
         entry->format = strdup("%D");
         entry->size   = sizeof(long);
      } else if( lang==C_LANG && !strncmp(entry->vartype,"short",5) ) {
         entry->format = strdup("%d");
         entry->size   = sizeof(short);
      } else if( !strncmp(entry->vartype,"float",5) || !strcmpi(entry->vartype,"real") || !strcmpi(entry->vartype,"real*4") ) {
         entry->format = strdup("%f");
         entry->size   = sizeof(float);
      } else if( !strncmp(entry->vartype,"double",6) || !strcmpi(entry->vartype,"real*8") ) {
         entry->format = strdup("%f");
         entry->size   = sizeof(double);
      } else if( lang==C_LANG && !strncmp(entry->vartype,"char*",5) ) {
         entry->format = strdup("%s");
         entry->size   = sizeof(char);
      } else if( lang==C_LANG && !strncmp(entry->vartype,"char",4) ) {
         entry->format = strdup("%c");
         entry->size   = sizeof(char);
      } else if( lang==FTN_LANG && !strcmpi(entry->vartype,"character*1") ) {
         entry->format = strdup("%c");
         entry->size   = sizeof(char);
      } else if( lang==FTN_LANG && !strnicmp(entry->vartype,"character*",10) ) {
         entry->format = strdup("%s");
         entry->size   = sizeof(char)*atoi(entry->vartype+10) ;
      } else {
         fatal_error(1,"argtool(parsetab); Unrecognized variable type %s on line %d in table\n",entry->vartype,iline);
      }

      if( entry->pointer > 2 && entry->format[1] != 's' )
         fatal_error(1,"argtool(parsetab): Unrecognized variable type %s on line %d in table\n",entry->vartype,iline);

   } else {

      /* Its a flag */
      entry->flag = 1;
      entry->vartype = entry->varname = entry->format = entry->initvalue = entry->comment = strdup(" ");
      if( cmt ) entry->comment = strdup(cmt+1);

      entry->flagged = flagged = num_entry;
      entry->param_number = count = 0;
      entry->pointer = 0;
      entry->size   = 0;
      repeat_flag = entry->repeated ;

   }

   /* Init portions of table used in proc'ed program */
   entry->address = 0;
   entry->occurances = 0;
/*   print_table_entry(entry);      */

   /* Indicate to caller that a good line was found */
   return(1);
}




/* Routine to print an entry from the table */
print_table_entry(entry)

   struct table_entry   *entry;      /* Parsed table entry */

{
   printf("\nUsage name    = *%s*\n",entry->uname);
   printf("Var type      = *%s*\n",entry->vartype);
   printf("Var name      = *%s*\n",entry->varname);
   printf("Format        = *%s*\n",entry->format);
   printf("Initial value = *%s*\n",entry->initvalue);
   printf("Comment       = *%s*\n",entry->comment);

   printf("Flag          = %d\n",entry->flag);
   printf("Required      = %d\n",entry->required);
   printf("Repeated      = %d\n",entry->repeated);

   printf("Flagged       = %d\n",entry->flagged);
   printf("Count         = %d\n",entry->param_number);
   printf("Address       = %p\n",entry->address);
   printf("Occurances    = %d\n",entry->occurances);
   printf("Pointer       = %d\n",entry->pointer);
   printf("Size          = %d\n",entry->size);
}

