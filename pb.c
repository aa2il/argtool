/*****************************************************************************

      PB.C - Routine to parse up the command line args for batch processing.

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


parse_cmd_line_bat(fp2,fp3,arglist,arg_table,arg_entries,
                   PROGRAM_NAME,USAGE,HELP_MSG,lang)

   FILE  *fp2,*fp3;         /* Output file pointer */
   char  *arglist;          /* List of args */
   struct table_entry arg_table[] ;
   int arg_entries;
   char *PROGRAM_NAME,*USAGE,*HELP_MSG;
   int lang;

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

   char  *argv;

   /* Init */
   i=1;
   flagged = 0;
   arg_number = 0;

   /* Remove excess white space */
   collapse_white_space(arglist);

   /* Get first arg */
   argv = parse_line(arglist,i,lang);

   /* Spin through all the args & assciate flags, etc. */
   while( strlen(argv) > 0 ){

      /* Check if this arg is a flag */
      if( argv[0] == '-' && isalpha(argv[1]) ){

         /* Its a flag - look for the corresponding flag in the table */
         found = -1;
         for(n=0; n<arg_entries && found<0 ; n++){
            if( arg_table[n].flag && !strcmp(argv,arg_table[n].uname) ) found = n;
         }
         if( found < 0 ) {
            fprintf(stderr,"\nargtool(pb): Unrecongized flag %s \n\n",argv);
            fatal_error(1,USAGE,PROGRAM_NAME);
         } else {
            flagged = found;
            arg_table[found].occurances++;
            arg_number = found + 1;
            if( lang == BAT_LANG )
               fprintf(fp2,"set %s_m=1\n",argv+1);
            else if( lang == DCL_LANG )
               fprintf(fp2,"$ %s_m=1\n",argv+1);
            else if( lang == CSH_LANG )
               fprintf(fp2,"set %s_m=1\n",argv+1);
            else if( lang == TCL_LANG )
               fprintf(fp2,"set %s_m 1\n",argv+1);
            else
               fatal_error(1,"argtool(pb): Unknown language code %d\n",lang);

            /* Put up help message if -help flag given */
            if( !strcmp(argv,"-help") ) argtool_help_message_bat(HELP_MSG,arg_entries,arg_table,USAGE,PROGRAM_NAME);

         }

      } else {

         /* Its a value - assicate it with the approriate variable */

         /* Check if too many args specified */
         if( arg_table[arg_number].flag ){
/*         if( arg_number+1 >= arg_entries ){ */

            /* Too many args specified */
            if(flagged) {
               fprintf(stderr,"\nargtol(pb): Too many arguments for flag %s\n",arg_table[flagged].uname);
            } else {
               fprintf(stderr,"\nargtool(pb): Too many unflagged arguments\n");
            }
            fatal_error(1,USAGE,PROGRAM_NAME);

         } else {

            /* Convert command line value */
            if(arg_table[arg_number].pointer == 1 ) {

               /* 1-d arrays */
               if(arg_table[arg_number].format[1] == 's' ) {

                  /* Strings are special */
                  if( lang == BAT_LANG )
                    fprintf(fp2,"set %s=%s\n",arg_table[arg_number].varname,argv);
                  else if( lang == DCL_LANG )
                    fprintf(fp2,"$ %s=\"%s\"\n",arg_table[arg_number].varname,argv);
                  else if( lang == CSH_LANG ) 
                    fprintf(fp2,"set %s=\"%s\"\n",arg_table[arg_number].varname,argv);
                  else if( lang == TCL_LANG ) 
                    fprintf(fp2,"set %s \"%s\"\n",arg_table[arg_number].varname,argv);
                  else
                     fatal_error(1,"argtool(pb): Unknown language code %d\n",lang);

               } else {

                  /* Other 1-D arrays */
                  if( !arg_table[arg_number].occurances ) 
                     array = (char *) calloc( 1 , arg_table[arg_number].size );
                  else 
                     array = (char *) realloc( array , (arg_table[arg_number].occurances+1)*arg_table[arg_number].size );
                  cc = (char **) arg_table[arg_number].address ;

               }

            } else if(arg_table[arg_number].pointer == 2 && arg_table[arg_number].format[1] == 's' ) {

               /* Arrays of strings */
               if( !arg_table[arg_number].occurances ) 
                  pntrs = (char **) calloc( 1 , sizeof(char **) );
               else 
                  pntrs = (char **) realloc( pntrs , (arg_table[arg_number].occurances+1)*sizeof(char **) );
               cc = (char **) arg_table[arg_number].address;

            } else if(arg_table[arg_number].pointer == 0 ) {

               /* Single variables */
/*               sscanf(argv,arg_table[arg_number].format,arg_table[arg_number].address); /* This doesn't work when using the mixed-mode fortran library ???? Need to do it the hard way */

               /* Determine propoer i/o format */
               if( !strncmp(arg_table[arg_number].vartype,"int",3) ) {
                  ip = (int *) arg_table[arg_number].address;
               } else if( !strncmp(arg_table[arg_number].vartype,"unsigned",8) ) {
                  up = (unsigned *) arg_table[arg_number].address;
               } else if( !strncmp(arg_table[arg_number].vartype,"long",4) ) {
                  lp = (long *) arg_table[arg_number].address;
               } else if( !strncmp(arg_table[arg_number].vartype,"short",5) ) {
                  sp = (short *) arg_table[arg_number].address;
               } else if( !strncmp(arg_table[arg_number].vartype,"float",5) ) {
                  fp = (float *) arg_table[arg_number].address;
               } else if( !strncmp(arg_table[arg_number].vartype,"double",6) ) {
                  dp = (double *) arg_table[arg_number].address;
               } else if( !strncmp(arg_table[arg_number].vartype,"char*",5) ) {
                  cc = (char **) arg_table[arg_number].address;
               } else if( !strncmp(arg_table[arg_number].vartype,"string",5) ) {
                  cc = (char **) arg_table[arg_number].address;
                  if( lang == BAT_LANG )
                    fprintf(fp2,"set %s=%s\n",arg_table[arg_number].varname,argv);
                  else if( lang == DCL_LANG )
                    fprintf(fp2,"$ %s==\"%s\"\n",arg_table[arg_number].varname,argv);
                  else if( lang == CSH_LANG ) {

                    if( !arg_table[arg_number].occurances) 
                      fprintf(fp2,"set %s=\"%s\"\n",
                              arg_table[arg_number].varname,argv);
                    else
                      fprintf(fp2,"set %s=($%s \"%s\")\n",
                              arg_table[arg_number].varname,
                              arg_table[arg_number].varname,argv);

                  } else if( lang == TCL_LANG ) {

                    if( !arg_table[arg_number].occurances) 
                      fprintf(fp2,"set %s \"%s\"\n",
                              arg_table[arg_number].varname,argv);
                    else
                      fprintf(fp2,"set %s ($%s \"%s\")\n",
                              arg_table[arg_number].varname,
                              arg_table[arg_number].varname,argv);

                  } else
                     fatal_error(1,"argtool(pb): Unknown language code %d\n",lang);
               } else if( !strncmp(arg_table[arg_number].vartype,"char",4) ) {
                  cp = (char *) arg_table[arg_number].address;
               } else {
                  fatal_error(1,"pb: Unrecognized variable type %s\n",arg_table[arg_number].vartype);
               }

            } else {

               fprintf(stderr,"argtool(pb): ARG Error - Unable to convert command line args\n");
               fatal_error(1,USAGE,PROGRAM_NAME);

            }

            arg_table[arg_number].occurances++;
            if(!arg_table[arg_number].repeated) arg_number++;
         }

      }

      /* Get next arg */
      i++;
      argv = parse_line(arglist,i,lang);

   }

   /* Has everything been specified correctly ? */
   for(n=0; n<arg_entries; n++){
/*      print_table_entry(arg_table+n);         */

      /* Clear the vars at the end */
      if( lang == BAT_LANG ) {
         if(arg_table[n].flag) 
            fprintf(fp3,"set %s_m=\n",arg_table[n].uname+1);
         else
            fprintf(fp3,"set %s=\n",arg_table[n].varname);
      }

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

	    } else {

	      /* No - set it to its default */
/*	      printf("set @%s@ = @%s@\n",arg_table[n].varname,
	      arg_table[n].initvalue); */
	      fprintf(fp2,"set %s=%s\n",arg_table[n].varname,
		      arg_table[n].initvalue);

            }

         /* Must have been a required unflagged arg */
         } else {
            fprintf(stderr,"\nToo few unflagged arguments\n");
            //printf("HEY \n");
            //printf("HEY USAGE=%s\n",USAGE);
            fatal_error(1,USAGE,PROGRAM_NAME);
         }

      /* Set defaults for non-specified args */
      } else if( !arg_table[n].occurances ) {
         if(arg_table[n].flag) {
            if( lang == BAT_LANG )
               fprintf(fp2,"set %s_m=\n",arg_table[n].uname+1);
            else if( lang == DCL_LANG )
               fprintf(fp2,"$ %s_m==0\n",arg_table[n].uname+1);
            else if( lang == CSH_LANG )
               fprintf(fp2,"set %s_m=\n",arg_table[n].uname+1);
            else if( lang == TCL_LANG )
               fprintf(fp2,"set %s_m 0\n",arg_table[n].uname+1);
            else
               fatal_error(1,"argtool(pb): Unknown language code %d\n",lang);
         } else {
	   fprintf(fp2,"set %s=%s\n",arg_table[n].varname,
		    arg_table[n].initvalue);
         }
      }

   }

}





/* Routine to print out help info */
argtool_help_message_bat(HELP_MSG,arg_entries,arg_table,USAGE,PROGRAM_NAME)

   char *HELP_MSG,*USAGE;
   int arg_entries;
   struct table_entry *arg_table ;


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
