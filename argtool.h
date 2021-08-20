/*****************************************************************************

   argtool.h   -  Include file for argtool 

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

#ifdef mips
extern char *strdup();
#endif

#ifdef __cplusplus
#define EXTERN extern "C" 
#else
#define EXTERN extern
#endif

EXTERN char *parse_line();
EXTERN char *detab();
EXTERN char *detab2();
EXTERN int MARKER();

/* Structure to hold table entry data */
typedef struct table_entry {
   char  *uname;                 /* Name for USAGE statement */
   char  *vartype;               /* Variable type */
   char  *varname;               /* Variable name */
   char  *format;                /* scanf format for value input */
   char  *initvalue;             /* Initial value */
   char  *comment;               /* Comment */

   int   flag;                   /* =1 if this entry is a flag and 0 if a variable */
   int   required;               /* =1 if this entry is required & 0 if not */
   int   repeated;               /* =1 if this entry is repeated & 0 if not */

   int   flagged;                /* =flag entry # if this variable is flagged, 0 if unflagged var */
   int   param_number;           /* Talbe entry number ???? */
   char   *address;              /* variable address - used by parser */
   int   occurances;             /* # times this entry has occured - used by parser */
   int   pointer;                /* Depth of pointering */
   int   size;                   /* No. bytes per element */
} dum1;

typedef struct table_entry_ftn {
  char  uname[80];                 /* Name for USAGE statement */
  char  vartype[80];               /* Variable type */
  char  varname[80];               /* Variable name */
  char  format[80];                /* scanf format for value input */
  char  initvalue[80];             /* Initial value */
  char  comment[80];               /* Comment */
  
  int   flag;                   /* =1 if this entry is a flag and 0 if a variable */
  int   required;               /* =1 if this entry is required & 0 if not */
  int   repeated;               /* =1 if this entry is repeated & 0 if not */
  
  int   flagged;                /* =flag entry # if this variable is flagged, 0 if unflagged var */
  int   param_number;           /* Talbe entry number ???? */
  int   address;                /* variable address - used by parser */
  int   occurances;             /* # times this entry has occured - used by parser */
  int   pointer;                /* Depth of pointering */
  int   size;                   /* No. bytes per element */ 
} dum2;

#define  C_LANG   1
#define  FTN_LANG 2
#define  BAT_LANG 3
#define  DCL_LANG 4
#define  CSH_LANG 5
#define  TCL_LANG 6

#if defined(sun) || defined(mips) || defined(vms) || defined(__PARAGON__) || defined(linux)
#define _MAX_DRIVE 3
#define _MAX_DIR   130
#define _MAX_FNAME 144
#define _MAX_EXT   50
#define _MAX_PATH  144
#define strcmpi strcasecmp
//#define MAX_ENTRIES  200
#define fortran 
#endif

#ifdef MSDOS
//#define  MAX_ENTRIES  200
#ifdef WINNT
#define fortran 
#endif
#endif

#if 1
#define MAX_ENTRIES  200
#endif

EXTERN int Parse_Cmd_Line(int argc,char **argv);

