/*****************************************************************************

      PARSECMD.C - Routine to parse up the command line args.

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

extern char *USAGE,*HELP_MSG,*USAGE1;
extern int arg_entries;
extern struct table_entry arg_table[] ;

Parse_Cmd_Line(argc,argv)

   int   argc;
   /*   char  *argv[]; */
   char  **argv;

{
   int   i;
   int   flagged;
   int   n,found,arg_number;
   char** cc;
   char** pntrs;
   char*  array;

   int      *ip;
   unsigned *up;
   long     *lp;
   short    *sp;
   float    *fp;
   double   *dp;
   char     *cp;

   /* Init */
   flagged = 0;
   arg_number = 0;

   /* Fix-up usage message */
   USAGE=malloc(strlen(USAGE1)+strlen(argv[0])+10);
   sprintf(USAGE,USAGE1,argv[0]);

   /* Spin through all the args & assciate flags, etc. */
   for(i=1; i<argc; i++) {

      /* Check if this arg is a flag */
      if( argv[i][0] == '-' && isalpha(argv[i][1]) ){

         /* Its a flag - look for the corresponding flag in the table */
         found = -1;
         for(n=0; n<arg_entries && found<0 ; n++){
            if( arg_table[n].flag && !strcmp(argv[i],arg_table[n].uname) ) found = n;
         }
         if( found < 0 ) {
            fprintf(stderr,"\nUnrecongized flag %s \n\n",argv[i]);
            fatal_error(1,USAGE);
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
            fatal_error(1,USAGE);

         } else {

            /* Convert command line value */
            if(arg_table[arg_number].pointer == 1 ) {

               /* 1-d arrays */
               if(arg_table[arg_number].format[1] == 's' ) {

                  /* Strings are special */
                  cc = (char **) arg_table[arg_number].address;
                  *cc = argv[i];  

               } else {

                  /* Other 1-D arryas */
                  if( !arg_table[arg_number].occurances ) 
                     array = (char *) calloc( 1 , arg_table[arg_number].size );
                  else 
                     array = (char *) realloc( array , (arg_table[arg_number].occurances+1)*arg_table[arg_number].size );
                  cc = (char **) arg_table[arg_number].address ;
                  sscanf(argv[i],arg_table[arg_number].format,array+arg_table[arg_number].size*arg_table[arg_number].occurances ); 
                  *cc = array;

               }

            } else if(arg_table[arg_number].pointer == 2 && arg_table[arg_number].format[1] == 's' ) {

               /* Arrays of strings */
               if( !arg_table[arg_number].occurances ) 
                  pntrs = (char **) calloc( 1 , sizeof(char **) );
               else 
                  pntrs = (char **) realloc( pntrs , (arg_table[arg_number].occurances+1)*sizeof(char **) );
               cc = (char **) arg_table[arg_number].address;
               pntrs[arg_table[arg_number].occurances] = strdup( argv[i] );    
               *cc = (char *) pntrs;

            } else if(arg_table[arg_number].pointer == 0 ) {

               /* Single variables */
/*               sscanf(argv[i],arg_table[arg_number].format,arg_table[arg_number].address); /* This doesn't work when using the mixed-mode fortran library ???? Need to do it the hard way */

               /* Determine propoer i/o format */
               if( !strncmp(arg_table[arg_number].vartype,"int",3) ) {
                  ip = (int *) arg_table[arg_number].address;
                  *ip = atoi( argv[i] );
               } else if( !strncmp(arg_table[arg_number].vartype,"unsigned",8) ) {
                  up = (unsigned *) arg_table[arg_number].address;
                  *up = atol( argv[i] );
               } else if( !strncmp(arg_table[arg_number].vartype,"long",4) ) {
                  lp = (long *) arg_table[arg_number].address;
                  *lp = atol( argv[i] );
               } else if( !strncmp(arg_table[arg_number].vartype,"short",5) ) {
                  sp = (short *) arg_table[arg_number].address;
                  *sp = atoi( argv[i] );
               } else if( !strncmp(arg_table[arg_number].vartype,"float",5) ) {
                  fp = (float *) arg_table[arg_number].address;
                  *fp = atof( argv[i] );
               } else if( !strncmp(arg_table[arg_number].vartype,"double",6) ) {
                  dp = (double *) arg_table[arg_number].address;
                  *dp = atof( argv[i] );
               } else if( !strncmp(arg_table[arg_number].vartype,"char*",5) ) {
                  cc = (char **) arg_table[arg_number].address;
                  *cc = strdup( argv[i] );  
               } else if( !strncmp(arg_table[arg_number].vartype,"char",4) ) {
                  cp = (char *) arg_table[arg_number].address;
                  *cp = argv[i][0];
               } else {
                  fatal_error(1,"Unrecognized variable type %s\n",arg_table[arg_number].vartype);
               }

            } else {

               fprintf(stderr,"ARG Error - Unable to convert command line args\n");
               fatal_error(1,USAGE);

            }

            arg_table[arg_number].occurances++;
            if(!arg_table[arg_number].repeated) arg_number++;
         }

      }

   }

   /* Put up help message if -help flag given */
   if( MARKER("-help") ) argtool_help_message(HELP_MSG);

   /* Has everything been specified correctly ? */
   for(n=0; n<arg_entries; n++){
/*      print_table_entry(arg_table+n);         */

      /* Was this arg repeated ? */
      if( !arg_table[n].repeated && arg_table[n].occurances>2 ) {

         /* Is it a non-repeatable flag ? */
         if(arg_table[n].flag) {
            fprintf(stderr,"\nFlag %s is not repeatable\n",arg_table[n].uname);
            fatal_error(1,USAGE);

         /* Is it an argument of a flag ? */
         } else if(arg_table[n].flagged) {
            flagged=arg_table[n].flagged;

            /* Was the flag repeatable ? */
            if( !arg_table[flagged].repeated ) {
               fprintf(stderr,"\nFlagged argument %s in not repeatable\n",arg_table[n].uname);
               fatal_error(1,USAGE);
            }

         /* Must have been a non-repeatable unflagged arg */
         } else {
            fprintf(stderr,"\nUnflagged argument %s in not repeatable\n",arg_table[n].uname);
            fatal_error(1,USAGE);
         }

      }

      /* Is this table entry required ? */
      if( arg_table[n].required && !arg_table[n].occurances ) {

         /* Is it a required flag ? */
         if(arg_table[n].flag) {
            fprintf(stderr,"\nFlag %s must be specified\n",arg_table[n].uname);
            fatal_error(1,USAGE);

         /* Is it a required flagged argument ? */
         } else if(arg_table[n].flagged) {
            flagged=arg_table[n].flagged;

            /* Was the flag specified ? */
            if( arg_table[flagged].occurances ) {
               fprintf(stderr,"\nToo few arguments for flag %s\n",arg_table[flagged].uname);
               fatal_error(1,USAGE);
            }

         /* Must have been a required unflagged arg */
         } else {
            fprintf(stderr,"\nToo few unflagged arguments\n");
            fatal_error(1,USAGE);
         }

      }

   }

}



/* Routine to print out help info */
argtool_help_message(HELP_MSG)

   char *HELP_MSG;

{
   int n;

   fprintf(stderr,"\n%s\n",HELP_MSG);
   fprintf(stderr,"\nArguments:\n");
   for(n=0; n<arg_entries; n++) {
      if( !arg_table[n].flag ) {
         fprintf(stderr,"%s",arg_table[n].vartype);
         if( strlen(arg_table[n].vartype) < 8 ) fprintf(stderr,"\t");
      } else {
         fprintf(stderr,"FLAG\t");
      }
      fprintf(stderr,"\t%s",arg_table[n].uname);
      if( strlen(arg_table[n].uname) < 8 ) fprintf(stderr,"\t");
      if( strlen(arg_table[n].comment) > 1 ) fprintf(stderr,"\t\t# %s",arg_table[n].comment);
      if( !arg_table[n].flag && strlen(arg_table[n].initvalue) ) fprintf(stderr,"\t[%s]",arg_table[n].initvalue);
      fprintf(stderr,"\n");
   }
   fprintf(stderr,"\n");
   fatal_error(1,USAGE);
}

