/*****************************************************************************

   ARGTOOL.ARG - Argument Processing Tool - Version 2.5

         Copyright (C) 1993 - 2021 by Joseph B. Attili, aa2il AT arrl DOT net

   This program provides a nice way of accessing command line arguments.
   It converts .arg or .arf source into C, FORTRAN, DOS Batch files,
   DCL command files, Unix C-shell source or TCL/TK scripts with
   the acquisition and association of the command line arguments
   transparent to the user.  See the demo files DEMO.ARG,
   DEMO2.ARF, DEMO3.BAT, DEMO4.COM and DEMO5 for
   examples on how to use this tool.

   This tool was originally developed to run in a variety of environments
   including numerous flavors of unix & linux, PC DOC, DEC VMS ...  Many of
   these machines are now only found in museums so only linux compatability 
   under gcc/gfortran & csh/tcsh is being actively maintained.

******************************************************************************

   Installation:
      All systems - Unpack source into a single dir
                  - All program that use this facility will need to be
                    linked with the argtool library.

      Linux:
        The Makefile still works!

      MSDOS:   
         nmake 
         copy argtol.lib c:\lib    (or wherever the system libs are)

      Unix (Sun, Sgi, Paraoon or Decstation):
         It is assumed that you have the following dir structure:
            ~/argtool   - Source for argtool
            ~/progs     - Program source
            ~/lib       - Lib source
            ~/incl      - Include files
         cd ~/argtool
         build          - Puts links to source code in right places & compiles it
                          (See me for build & jba_mkmf scripts)

      VMS:
         @build
         edit LOGIN.COM to put DCL FOREIGN alias for argtool

******************************************************************************

   Revision history:
      May  1993 - V 0.00 - First version functioning on IBM PC developed
      June 1993 - V 1.00 - Functional version of PC, Sun, & Decstation developed
      June 1993 - V 1.10 - Added support for FORTRAN & VMS
      June 1993 - V 1.11 - Bug fixes
      July 1993 - V 1.12 - More bug fixes
      Aug  1993 - V 1.13 - More bug fixes
      Oct  1993 - V 2.0  - Added BAT, DCL, and CSH support
      Mar  1996 - V 2.1  - Code was ported to SGI, Paragon, & Linux
      May  1997 - V 2.2  - Code was ported to Wiondows NT
      Nov  2000 - V 2.3  - Added conditional argument lists
      July 2006 - V 2.4  - Added TCL support
      Aug  2021 - V 2.5  - Dusted off and brought up to date

******************************************************************************

   Known Bugs:
      - Something is writing too far on dup'ed strings bx MSDOS version
        crashes when use the DOS strdup.  (The strdup used here adds an
        extra char so this doesn't happen but sure would be nice to find
        out what's going on !)
      - If the command fails (e.g. syntax error in .ARG/F source), the
        converted file is not deleted. (Will fix after product is stable.)
      - It would be nice to create symbols for the VAX libs & foriegn cmd
      - Would be nice to have inits for repeated ftn args (Can do e.g. /5*0/)
      - Can't put arg vars in common.  Need to have executable stuff later ??

*****************************************************************************

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

*****************************************************************************/


/* Start of ARG TABLE Defs */
#define ARGTOOLED
char* 	  argfile  ; 	 /*  .ARG file name */
char* 	  outfile  ; 	 /*  Output file name */
char* 	  cond_sym  = ""  ; 	 /*  Symbol for which condition is true */
char* 	  setfile1  ; 	 /*  Place to put batch set commands */
char* 	  clrfile1  ; 	 /*  Place to put batch clear commands */
char* 	  arglist1  ; 	 /*  List of args for batch files */
char* 	  setfile2  ; 	 /*  Place to put DCL set commands */
char* 	  arglist2  ; 	 /*  List of args for DCL files */
char* 	  setfile3  ; 	 /*  Place to put C-shell set commands */
char* 	  arglist3  ; 	 /*  List of args for shell scripts */
char* 	  setfile4  ; 	 /*  Place to put TCL-shell set commands */
char* 	  arglist4  ; 	 /*  List of args for TCL scripts */

char *PROGRAM_NAME="argtool";
char *USAGE;
char *USAGE1="Usage:\t%s\t <argfile>\n			 [output_file]\n			 [-c]\n			 [-ftn]\n			 [-D <condition>]\n			 [-v]\n			 [-bat <set_file> <clear_file> [arglist_bat]]\n			 [-dcl <set_file> [arglist_dcl]]\n			 [-csh <set_file> [arglist_csh]]\n			 [-tcl <set_file> [arglist_tcl]]\n			 [-nh]\n			 [-help]\n";
char *HELP_MSG=" Arg Tool Demo Program";
#include "argtool.h"
int arg_entries=22;
struct table_entry arg_table[22] = {
{ "argfile" , "char*" , " argfile" , "%s" , "" ,  "  .ARG file name " , 0 , 1 , 0 , 0 , 0 , (char *)& argfile , 0 , 1 , 1 } ,
{ "output_file" , "char*" , " outfile" , "%s" , "" ,  "  Output file name " , 0 , 0 , 0 , 0 , 1 , (char *)& outfile , 0 , 1 , 1 } ,
{ "-c" , " " , " " , " " , " " ,  "  C Language  " , 1 , 0 , 0 , 2 , 0 ,  0 , 0 , 0 , 0 } ,
{ "-ftn" , " " , " " , " " , " " ,  "  Fortran " , 1 , 0 , 0 , 3 , 0 ,  0 , 0 , 0 , 0 } ,
{ "-D" , " " , " " , " " , " " ,  "  Selection for conditional arg lists " , 1 , 0 , 0 , 4 , 0 ,  0 , 0 , 0 , 0 } ,
{ "condition" , "char*" , " cond_sym" , "%s" , "" ,  "  Symbol for which condition is true " , 0 , 1 , 0 , 4 , 0 , (char *)& cond_sym , 0 , 1 , 1 } ,
{ "-v" , " " , " " , " " , " " ,  "  Verbose mode " , 1 , 0 , 0 , 6 , 0 ,  0 , 0 , 0 , 0 } ,
{ "-bat" , " " , " " , " " , " " ,  "  DOS Batch files " , 1 , 0 , 0 , 7 , 0 ,  0 , 0 , 0 , 0 } ,
{ "set_file" , "char*" , " setfile1" , "%s" , "" ,  "  Place to put batch set commands " , 0 , 1 , 0 , 7 , 0 , (char *)& setfile1 , 0 , 1 , 1 } ,
{ "clear_file" , "char*" , " clrfile1" , "%s" , "" ,  "  Place to put batch clear commands " , 0 , 1 , 0 , 7 , 1 , (char *)& clrfile1 , 0 , 1 , 1 } ,
{ "arglist_bat" , "char*" , " arglist1" , "%s" , "" ,  "  List of args for batch files " , 0 , 0 , 0 , 7 , 2 , (char *)& arglist1 , 0 , 1 , 1 } ,
{ "-dcl" , " " , " " , " " , " " ,  "  DCL scripts " , 1 , 0 , 0 , 11 , 0 ,  0 , 0 , 0 , 0 } ,
{ "set_file" , "char*" , " setfile2" , "%s" , "" ,  "  Place to put DCL set commands " , 0 , 1 , 0 , 11 , 0 , (char *)& setfile2 , 0 , 1 , 1 } ,
{ "arglist_dcl" , "char*" , " arglist2" , "%s" , "" ,  "  List of args for DCL files " , 0 , 0 , 0 , 11 , 1 , (char *)& arglist2 , 0 , 1 , 1 } ,
{ "-csh" , " " , " " , " " , " " ,  "  C-shell scripts " , 1 , 0 , 0 , 14 , 0 ,  0 , 0 , 0 , 0 } ,
{ "set_file" , "char*" , " setfile3" , "%s" , "" ,  "  Place to put C-shell set commands " , 0 , 1 , 0 , 14 , 0 , (char *)& setfile3 , 0 , 1 , 1 } ,
{ "arglist_csh" , "char*" , " arglist3" , "%s" , "" ,  "  List of args for shell scripts " , 0 , 0 , 0 , 14 , 1 , (char *)& arglist3 , 0 , 1 , 1 } ,
{ "-tcl" , " " , " " , " " , " " ,  "  TCL scripts " , 1 , 0 , 0 , 17 , 0 ,  0 , 0 , 0 , 0 } ,
{ "set_file" , "char*" , " setfile4" , "%s" , "" ,  "  Place to put TCL-shell set commands " , 0 , 1 , 0 , 17 , 0 , (char *)& setfile4 , 0 , 1 , 1 } ,
{ "arglist_tcl" , "char*" , " arglist4" , "%s" , "" ,  "  List of args for TCL scripts " , 0 , 0 , 0 , 17 , 1 , (char *)& arglist4 , 0 , 1 , 1 } ,
{ "-nh" , " " , " " , " " , " " ,  "  No header " , 1 , 0 , 0 , 20 , 0 ,  0 , 0 , 0 , 0 } ,
{ "-help" , " " , " " , " " , " " ,  " Display this HELP message " , 1 , 0 , 0 , 21 , 0 ,  0 , 0 , 0 , 0 }
};

/* End of ARG TABLE Defs */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef vms
#include <malloc.h>
#endif

char                 name[_MAX_PATH],usemsg[1024],*hlp_msg;
int                  help_found;
int                  num_entries;      /* No. of entries in the table */
struct table_entry   entries[MAX_ENTRIES];      /* Parsed table entries */
char                 *s,*arglist;

char                  *OUTFILE;
int                   ARGTOOL=7474;

#define VERSION "2.5"

main(argc,argv)

   int   argc;
   char  *argv[];

{
   char  line[1024],*lp,cond_flag[1024];

   int   i,j,istart,iend,flagged;
   int   n,nflg,k;
   FILE  *fp1,*fp2,*fp3;
   char  drive[_MAX_DRIVE],dir[_MAX_DIR],fname[_MAX_FNAME],ext[_MAX_EXT];
   int   lang,conditional=0;

   /* Init */
   flagged = 0;
   help_found = 0;
   cond_flag[0]=0;

   /* Retrive the command line args */
   Parse_Cmd_Line(argc,argv);
   if(!MARKER("-nh")) 
     fprintf(stderr,"\nArgument Processing Tool - Version %s\n\n",
             VERSION);

   /* Open source file */
   drive[0]=0;
   split_path(argfile,dir,fname,ext);
   if( !(fp1 = fopen(argfile,"r")) ) 
      fatal_error(1,"Unable to open input source file %s\n",argfile);

   /* Check language */
   if(MARKER("-c")) {
      lang = C_LANG ;
      if(!MARKER("output_file") )
	sprintf(name,"%s%s%s.c",drive,dir,fname);
      else
	sprintf(name,"%s",outfile);
	
   } else if(MARKER("-ftn")) {
      lang = FTN_LANG ;
#if defined(sun) || defined(mips) || defined(__PARAGON__) || defined(linux)
      sprintf(name,"%s%s%s.f",drive,dir,fname);
#else
      sprintf(name,"%s%s%s.for",drive,dir,fname);
#endif

   } else if(MARKER("-bat")) {
      lang = BAT_LANG ;
      sprintf(name,"%s",setfile1);
      arglist = arglist1;
   } else if(MARKER("-dcl")) {
      lang = DCL_LANG ;
      sprintf(name,"%s",setfile2);
      arglist = arglist2;

/*      if( MARKER("arglist_dcl") ) {
        arglist = strdup(arglist2[0]);
        for(i=1; i<MARKER("arglist_dcl"); i++) {
          strcat( arglist," ");
          strcat( arglist,arglist2[i]);
        }
      } else {
        arglist = 0;
      } */

   } else if(MARKER("-csh")) {
      lang = CSH_LANG ;
      sprintf(name,"%s",setfile3);
      arglist = arglist3;
   } else if(MARKER("-tcl")) {
      lang = TCL_LANG ;
      sprintf(name,"%s",setfile4);
      arglist = arglist4;
   } else
      fatal_error(1,"argtool: Lanuage flag must be specified\n");

    /* VMS doesn't like null args */   
    if( !arglist) {
	arglist = malloc(3);
	arglist[0] = 32;
	arglist[1]= 0;
   }

   /* Open output file(s) */
   if(MARKER("-v")) fprintf(stderr,"Opening ...\n");
   if(!MARKER("-nh")) 
      fprintf(stderr,"Converting %s to %s ...\n",argfile,name);
   OUTFILE = strdup(name);
   if( !(fp2 = fopen(name,"w")) ) 
      fatal_error(1,"Unable to open output source file %s\n",name);
   if(MARKER("-bat")) {
      if( !(fp3 = fopen(clrfile1,"w")) ) 
         fatal_error(1,"Unable to open output source file %s\n",clrfile1);
   }

   /* Start to create useage message */
   if(MARKER("-v")) fprintf(stderr,"Creating usage ...\n");
#if 0
   strcpy(usemsg,"Usage:\\t");
   strcat(usemsg,fname);
   strcat(usemsg,"\\t");
   if( strlen(fname) < 8 ) strcat(usemsg,"\\t");       /* Make sure args line up vertically */
#else
   strcpy(usemsg,"Usage:\\t%s\\t");
#endif

   /* Spin through input file & look for table def */
   if(MARKER("-v")) fprintf(stderr,"Looking for table ... fp1=%d\n",fp1);
   i=0;
   istart = 0;
   iend = 0;
   if(MARKER("-v")) fprintf(stderr,"Looking for table 2 ...\n");
   fgets(line,1024,fp1);
   if(MARKER("-v")) fprintf(stderr,"Looking for table 3 ...\n");
   while( !feof(fp1) ) {
      i++;
      if(MARKER("-v")) fprintf(stderr,"i=%d ...\n",i);

      /* Is this the start of the table ? */
      if( !istart && strstr(line,"$ARG_TABLE") ){
	   if(MARKER("-v")) fprintf(stderr,"Found table start...\n");
         istart = i;
         if(lang==C_LANG) {
		fprintf(fp2,"\n/* Start of ARG TABLE Defs */\n#define ARGTOOLED\n");
	 }

      /* Is this the end of the table ? */
      } else if( !iend && strstr(line,"$END_TABLE") ){
	   if(MARKER("-v")) fprintf(stderr,"Found table end...\n");
         iend = i;

         /* Finish useage message */
         if(flagged) {
            if(entries[flagged].required) 
               strcat(usemsg,">");
            else
               strcat(usemsg,"]");
            if(entries[flagged].repeated) 
               strcat(usemsg,"...");
         }
         strcat(usemsg,"\\n");

         /* Was there a -help ?  If not, it will be done automajically */
         if( !help_found ) {
            hlp_msg = strdup( " " );
            strcat(usemsg,"\t\t\t[-help]\\n");
         }

         /* For batch files, finish up */
         if( lang == BAT_LANG || lang==DCL_LANG || lang==CSH_LANG || lang==TCL_LANG) {
            s=detab2(usemsg);
            parse_cmd_line_bat(fp2,fp3,arglist,entries,num_entries,
                               fname,s,hlp_msg,lang);
            free(s);
         } else {
            save_argtool_info(fp2,lang,fname,usemsg,hlp_msg);
         }
         if(lang==C_LANG) {
		fprintf(fp2,"\n/* End of ARG TABLE Defs */\n");
	 }


      /* Is this line the start of a conditional arg list ? */
      } else if( istart && !iend && !strncmp(line,"$IF",3) ){

	/* Strip off cond level */
	conditional++;
	for(i=3;i<strlen(line);i++){
	  if(line[i]>' ') 
	    break;
	}
	for(j=0;i<strlen(line);i++,j++){
	  if(line[i]>' ')
	    cond_flag[j]=line[i];
	  else
	    break;
	}
	cond_flag[j]=0;
	if(MARKER("-v")) fprintf(stderr,"Conditional found %s ...\n",cond_flag);

	/* Define a symbol in C-routine for this */
	if(lang == C_LANG && !strcmp(cond_flag,cond_sym) )
	  fprintf(fp2,"#define %s\n",cond_flag);

      /* Is this line the end of a conditional arg list ? */
      } else if( istart && !iend && strstr(line,"$ENDIF") ){

	if(MARKER("-v")) fprintf(stderr,"Conditional end found...\n");
	conditional--;

      /* Is this line part of the table ? */
      } else if( istart && !iend ) {
	 if(MARKER("-v")) fprintf(stderr,"Inside table ...\n%s\n",line);

         /* Parse this line */
         if( (!conditional  || !strcmp(cond_flag,cond_sym) ) &&
	    parse_table_entry(i,line,&entries[num_entries],num_entries,lang) ) {

            /* Add variable declares for this line */
            if( !entries[num_entries].flag ){
               if(lang == C_LANG) {

                  fprintf(fp2,"%s \t %s ",entries[num_entries].vartype,entries[num_entries].varname);
                  if( strlen(entries[num_entries].initvalue) ) fprintf(fp2," = %s ",entries[num_entries].initvalue);
                  if(entries[num_entries].comment)
                     fprintf(fp2," ; \t /* %s */\n",entries[num_entries].comment);
                  else
                     fprintf(fp2," ;\n");
                                                 
               } else if(lang == FTN_LANG) {

                  fprintf(fp2,"      %s %s",entries[num_entries].vartype,entries[num_entries].varname);
                  if( entries[num_entries].repeated ) fprintf(fp2,"(%d)",entries[num_entries].repeated);
                  if( strlen(entries[num_entries].initvalue) ) fprintf(fp2," / %s / ",entries[num_entries].initvalue);
                  if(entries[num_entries].comment)
                     fprintf(fp2," \t ! %s\n",entries[num_entries].comment);
                  else
                     fprintf(fp2,"\n");

               }

            }

            /* Finish previous line of usage message */
            if( num_entries >= 1 && flagged && entries[num_entries].flag) {
               if(entries[flagged].required) 
                  strcat(usemsg,">");
               else
                  strcat(usemsg,"]");
               if(entries[flagged].repeated) strcat(usemsg,"...");
               strcat(usemsg,"\\n\t\t\t");
            }
            if( num_entries >= 1 && !flagged ) strcat(usemsg,"\\n\t\t\t");
            if(entries[num_entries].flag) flagged = num_entries;

            /* Add stuff for this line of usage message */
            if(entries[num_entries].required) 
               strcat(usemsg," <");
            else
               strcat(usemsg," [");
            strcat(usemsg,entries[num_entries].uname);
            if(!entries[num_entries].flag) {
               if(entries[num_entries].required) 
                  strcat(usemsg,">");
               else
                  strcat(usemsg,"]");
               if(entries[num_entries].repeated) 
                  strcat(usemsg,"...");
            }

            /* If it is -help, use the comment as the help message */
            if( entries[num_entries].flag && !strcmpi(entries[num_entries].uname,"-help") ) {
               help_found = 1;
               hlp_msg = strdup( entries[num_entries].comment );
               entries[num_entries].comment = strdup("Display this HELP message");
            }

            num_entries++;
            if( num_entries >= MAX_ENTRIES )
               fatal_error(1,"Table is too big\n");

         }

      /* Copy those lines not in the table directly into output file */
      } else if( !istart || iend ) {
	 if(MARKER("-v")) fprintf(stderr,"Outside table ...\n");
         if(lang == C_LANG || lang==FTN_LANG) fprintf(fp2,"%s",line);
      }

      /* Get next line */
      fgets(line,1024,fp1);	
   }

   /* That's all folks */
   fclose(fp1);
   fclose(fp2);
   if(!MARKER("-nh")) fprintf(stderr,"Done.\n\n");
   exit(0);

}


