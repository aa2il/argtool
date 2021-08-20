/*****************************************************************************

      PF.C - Routine to parse up the command line args - FTN version.

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

#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef vms
#include <malloc.h>
#endif
#include "argtool.h"

char *USAGE,*HELP_MSG,*PROGRAM_NAME;
int arg_entries;
struct table_entry_ftn *arg_table ;

struct table_entry_ftn the_table[100];


#if defined(sun) || defined(mips) || defined(__PARAGON__) || defined(linux)
void parse_cmd_line_ftn_
#else
#define fortran 
void fortran parse_cmd_line_ftn
#endif
   (arg_entries_p,arg_table_p,argc_p,argv,
          USAGE_TXT,HELP_MSG_TXT,PROGRAM_NAME_TXT)

   long *arg_entries_p;
   struct table_entry_ftn *arg_table_p ;
   long  *argc_p;
   char  argv[][80];
   char *USAGE_TXT;
   char *HELP_MSG_TXT;
   char *PROGRAM_NAME_TXT;

{
   int arg_entries;
   int  argc,lang;

   int   i,kk,j;
   int   flagged;
   int   n,found,arg_number;
   char** cc;
   char** pntrs;
   char*  array;

   short    *ip;
   unsigned *up;
   long     *lp;
   short    *sp;
   float    *fp;
   double   *dp;
   char     *cp;

   /* Fortran interface to args */
   arg_entries = *arg_entries_p;
   arg_table = arg_table_p;
   argc = *argc_p;
   USAGE = USAGE_TXT;
   HELP_MSG = HELP_MSG_TXT;
   PROGRAM_NAME = PROGRAM_NAME_TXT;

   /* Init */
   flagged = 0;
   arg_number = 0;
   lang = FTN_LANG;

   /* Spin through all the args & assciate flags, etc. */
   for(i=1; i<argc; i++) {

      /* Put up help message if -help flag given */
      if( !strcmp(argv[i],"-help") ) argtool_help_message_ftn(HELP_MSG,arg_entries,arg_table);

      /* Check if this arg is a flag */
      if( argv[i][0] == '-' && isalpha(argv[i][1]) ){

         /* Its a flag - look for the corresponding flag in the table */
         found = -1;
         for(n=0; n<arg_entries && found<0 ; n++){
#ifdef vms
            if( arg_table[n].flag && !strcmpi(argv[i],arg_table[n].uname) ) found = n;
#else
            if( arg_table[n].flag && !strcmp(argv[i],arg_table[n].uname) ) found = n;
#endif
         }
         if( found < 0 ) {
            fprintf(stderr,"\nUnrecongized flag %s \n\n",argv[i]);
            fatal_error(1,USAGE,PROGRAM_NAME);
         } else {
            flagged = found;
            arg_table[found].occurances++;
            arg_number = found + 1;
         }

      } else {

         /* Its a value - assicate it with the approriate variable */

         /* Ckeck if too many args specified */
         if( arg_table[arg_number].flag ){

            /* Too many args specified */
            if(flagged) {
               fprintf(stderr,"\nToo many arguments for flag %s\n",arg_table[flagged].uname);
            } else {
               fprintf(stderr,"\nToo many unflagged arguments\n");
            }
            fatal_error(1,USAGE,PROGRAM_NAME);

         } else {

            /* Convert command line value */
            if( arg_table[arg_number].pointer == 0 || arg_table[arg_number].pointer == 1 ||
              ( arg_table[arg_number].pointer == 2 && arg_table[arg_number].format[1] == 's') ){

               /* Determine propoer i/o format */
               kk = (int) arg_table[arg_number].size;
               kk *= (int) arg_table[arg_number].occurances; 
               if( (lang==C_LANG && !strncmp(arg_table[arg_number].vartype,"int",3)) || !strcmpi(arg_table[arg_number].vartype,"integer*2") ) {
                  ip = (short *) (arg_table[arg_number].address + kk);
                  *ip = atoi( argv[i] );
               } else if( lang==C_LANG && !strncmp(arg_table[arg_number].vartype,"unsigned",8) ) {
                  up = (unsigned *) (arg_table[arg_number].address + kk);
                  *up = atol( argv[i] );
               } else if( (lang==C_LANG && !strncmp(arg_table[arg_number].vartype,"long",4)) || !strnicmp(arg_table[arg_number].vartype,"integer",7) ) {
                  lp = (long *) (arg_table[arg_number].address + kk);
                  *lp = atol( argv[i] );
               } else if( lang==C_LANG && !strncmp(arg_table[arg_number].vartype,"short",5) ) {
                  sp = (short *) (arg_table[arg_number].address + kk);
                  *sp = atoi( argv[i] );
               } else if( !strncmp(arg_table[arg_number].vartype,"float",5) || !strcmpi(arg_table[arg_number].vartype,"real") || !strcmpi(arg_table[arg_number].vartype,"real*4") ) {
                  fp = (float *) (arg_table[arg_number].address + kk);
                  *fp = atof( argv[i] );
               } else if( !strncmp(arg_table[arg_number].vartype,"double",6) || !strcmpi(arg_table[arg_number].vartype,"real*8") ) {
                  dp = (double *) (arg_table[arg_number].address + kk);
                  *dp = atof( argv[i] );
               } else if( lang==C_LANG && !strncmp(arg_table[arg_number].vartype,"char*",5) ){
                  cp = (char *) (arg_table[arg_number].address + kk);
                  strcpy( cp , argv[i] );
               } else if( lang==C_LANG && !strncmp(arg_table[arg_number].vartype,"char",4) ){
                  cp = (char *) (arg_table[arg_number].address + kk);
                  *cp = argv[i][0];
               } else if( lang==FTN_LANG && !strcmpi(arg_table[arg_number].vartype,"character*1") ) {
                  cp = (char *) (arg_table[arg_number].address + kk);
                  *cp = argv[i][0];
               } else if( lang==FTN_LANG && !strnicmp(arg_table[arg_number].vartype,"character",9) ) {
                  cp = (char *) (arg_table[arg_number].address + kk);
                  for(j=0; j< (int) arg_table[arg_number].size; j++ ) cp[j]=' ';     /* Clear it out */
                  strcpy( cp , argv[i] );
               } else {
                  fatal_error(1,"Unrecognized variable type #%s#\n",arg_table[arg_number].vartype);
               }  

            } else {

               fprintf(stderr,"ARGtool Error - Unable to convert command line args\n");
               fatal_error(1,USAGE,PROGRAM_NAME);

            }

            arg_table[arg_number].occurances++;
            if(!arg_table[arg_number].repeated) arg_number++;
         }

      }

   }

   /* Has everything been specified correctly ? */
   for(n=0; n<arg_entries; n++){

      /* Was this arg repeated ? */
      if( !arg_table[n].repeated && arg_table[n].occurances>2 ) {

         /* Is it a non-repeatable flag ? */
         if(arg_table[n].flag) {
            fprintf(stderr,"\nFlag %s is not repeatable\n",arg_table[n].uname);
            fatal_error(1,USAGE,PROGRAM_NAME);

         /* Is it an argument of a flag ? */
         } else if(arg_table[n].flagged) {
            flagged=arg_table[n].flagged;

            /* Was the flag repeatable ? */
            if( !arg_table[flagged].repeated ) {
               fprintf(stderr,"\nFlagged argument %s in not repeatable\n",arg_table[n].uname);
               fatal_error(1,USAGE,PROGRAM_NAME);
            }

         /* Must have been a non-repeatable unflagged arg */
         } else {
            fprintf(stderr,"\nUnflagged argument %s in not repeatable\n",arg_table[n].uname);
            fatal_error(1,USAGE,PROGRAM_NAME);
         }

      }

      /* Is this table entry required ? */
      if( arg_table[n].required && !arg_table[n].occurances ) {

         /* Is it a required flag ? */
         if(arg_table[n].flag) {
            fprintf(stderr,"\nFlag %s must be specified\n",arg_table[n].uname);
            fatal_error(1,USAGE,PROGRAM_NAME);

         /* Is it a required flagged argument ? */
         } else if(arg_table[n].flagged) {
            flagged=arg_table[n].flagged;

            /* Was the flag specified ? */
            if( arg_table[flagged].occurances ) {
               fprintf(stderr,"\nToo few arguments for flag %s\n",arg_table[flagged].uname);
               fatal_error(1,USAGE,PROGRAM_NAME);
            }

         /* Must have been a required unflagged arg */
         } else {
            fprintf(stderr,"\nToo few unflagged arguments\n");
            fatal_error(1,USAGE,PROGRAM_NAME);
         }

      }

   }

}



/* Routine to print an entry from the table */
void print_table_entry_ftn(entry)

   struct table_entry_ftn   *entry;      /* Parsed table entry */

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



/* Routine to print out help info */
void argtool_help_message_ftn(HELP_MSG,arg_entries,arg_table)

   char *HELP_MSG;
   int arg_entries;
   struct table_entry_ftn *arg_table ;


{
   int n;

   fprintf(stdout,"\n%s\n",HELP_MSG);
   fprintf(stdout,"\nArguments:\n");
   for(n=0; n<arg_entries; n++) {
      if( !arg_table[n].flag ) {
         fprintf(stdout,"%s",arg_table[n].vartype);
         if( strlen(arg_table[n].vartype) < 8 ) fprintf(stdout,"\t");
      } else {
         fprintf(stdout,"FLAG\t");
      }
      fprintf(stdout,"\t%s",arg_table[n].uname);
      if( strlen(arg_table[n].uname) < 8 ) fprintf(stdout,"\t");
      if( strlen(arg_table[n].comment) > 1 ) fprintf(stdout,"\t\t# %s",arg_table[n].comment);
      if( !arg_table[n].flag && strlen(arg_table[n].initvalue) ) fprintf(stdout,"\t[%s]",arg_table[n].initvalue);
      fprintf(stdout,"\n");
   }
   fprintf(stdout,"\n");
   fatal_error(1,USAGE,PROGRAM_NAME);
}



/* Routine to set an entry in the table */

#if defined(sun) || defined(mips) || defined(__PARAGON__) || defined(linux)
void set_table_ftn_
#else
void fortran set_table_ftn
#endif 
   (arg_table_p,n,
   uname,vartype,varname,format,initvalue,comment,
   flag,required,repeated,param_number,flagged,address,occurances,pointer,size)

   struct table_entry_ftn *arg_table_p ;
   int *n;
   char *uname,*vartype,*varname,*format,*initvalue,*comment;
   long *flag,*required,*repeated,*param_number,*flagged,*address,*occurances,*pointer,*size;

{
   arg_table = arg_table_p;
  
  strcpy( arg_table[*n].uname     , uname );
  strcpy( arg_table[*n].vartype   , vartype );
  strcpy( arg_table[*n].varname   , varname );
  strcpy( arg_table[*n].format    , format );
  strcpy( arg_table[*n].initvalue , initvalue );
  strcpy( arg_table[*n].comment   , comment );

  arg_table[*n].flag=*flag;
  arg_table[*n].required=*required;
  arg_table[*n].repeated=*repeated;
  arg_table[*n].param_number=*param_number;
  arg_table[*n].flagged=*flagged;
  arg_table[*n].address=*address;
  arg_table[*n].occurances=*occurances;
  arg_table[*n].pointer=*pointer;
  arg_table[*n].size=*size;

}


#ifdef linux
long *loc_(x)

     long *x;

{
  return(x);
}
#endif

