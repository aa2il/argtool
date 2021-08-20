/*****************************************************************************

      DEMO.ARG - Argument Tool test prog - C version

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

/* The table must be before main() 

The convention is <> for required params, [] for optional, ... for repeated

Vars are declared as
<usage_name>   <type>   <var_name>  [init_value]   [# Comment]

Flags are declared as
<-flag> [# Comment]

There is a blank line betwen flag groups.  

Here is an example..... */

/* Start of ARG TABLE Defs */
#define ARGTOOLED
char** 	  fname  ; 	 /*  Repeated string */
int* 	 junk  ; 	 /*  Repeated integer */
int 	 n  = 5  ; 	 /*  Required integer */
float 	 f  = 1.0  ; 	 /*  Optional float */

char *PROGRAM_NAME="demo";
char *USAGE;
char *USAGE1="Usage:\t%s\t <prog_name>...\n			 [-j <junk>...]\n			 [-n <n> [f]]\n			 [-test]\n			 [-help]\n";
char *HELP_MSG=" Arg Tool Demo Program";
#include "argtool.h"
int arg_entries=8;
struct table_entry arg_table[8] = {
{ "prog_name" , "char**" , " fname" , "%s" , "" ,  "  Repeated string " , 0 , 1 , 1 , 0 , 0 , (char *)& fname , 0 , 2 , 1 } ,
{ "-j" , " " , " " , " " , " " ,  "  Optional flag " , 1 , 0 , 0 , 1 , 0 ,  0 , 0 , 0 , 0 } ,
{ "junk" , "int*" , "junk" , "%d" , "" ,  "  Repeated integer " , 0 , 1 , 1 , 1 , 0 , (char *)&junk , 0 , 1 , 4 } ,
{ "-n" , " " , " " , " " , " " ,  "  Optional flag " , 1 , 0 , 0 , 3 , 0 ,  0 , 0 , 0 , 0 } ,
{ "n" , "int" , "n" , "%d" , "5" ,  "  Required integer " , 0 , 1 , 0 , 3 , 0 , (char *)&n , 0 , 0 , 4 } ,
{ "f" , "float" , "f" , "%f" , "1.0" ,  "  Optional float " , 0 , 0 , 0 , 3 , 1 , (char *)&f , 0 , 0 , 4 } ,
{ "-test" , " " , " " , " " , " " ,  "   " , 1 , 0 , 0 , 6 , 0 ,  0 , 0 , 0 , 0 } ,
{ "-help" , " " , " " , " " , " " ,  " Display this HELP message " , 1 , 0 , 0 , 7 , 0 ,  0 , 0 , 0 , 0 }
};

/* End of ARG TABLE Defs */

#include <stdio.h>

main(argc,argv)

   int   argc;
   char  *argv[];

{
   int i;

/* This line retrives the command line args & associates them with the local vars */
   Parse_Cmd_Line(argc,argv);

/* This program just reports the arg vars before and after they are retrieved */
/* MARKER tells how many times a repeated arg was given */
#ifdef DUMMY
   printf("dummy            = %s\n",dummy);
#endif
   printf("MARKER(prog_name)     = %d\n",MARKER("prog_name"));
   for(i=0;i<MARKER("prog_name");i++) printf("\tfname[%d]=%s\n",i,fname[i]);  
   printf("MARKER(-j)       = %d\n",MARKER("-j"));
   for(i=0;i<MARKER("junk");i++) printf("i=%d \t junk[%d]=%d\n",i,i,junk[i]);    
   printf("MARKER(-help)    = %d\n",MARKER("-help"));
   printf("MARKER(-n)       = %d\n",MARKER("-n"));
   printf("n                = %d\n",n);
   printf("f                = %f\n",f);
}

