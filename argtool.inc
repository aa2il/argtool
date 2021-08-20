c *****************************************************************************
c
c    argtool.inc   -  Fortran include file for argtool 
c
c    Copyright (C) 1993 - 2021 by Joseph B. Attili, aa2il AT arrl DOT net
c
c *****************************************************************************
c
c   This program is free software: you can redistribute it and/or modify
c   it under the terms of the GNU General Public License as published by
c   the Free Software Foundation, either version 3 of the License, or
c   (at your option) any later version.
c
c   This program is distributed in the hope that it will be useful,
c   but WITHOUT ANY WARRANTY; without even the implied warranty of
c   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
c   GNU General Public License for more details.
c
c *****************************************************************************

      integer MAX_ENTRIES
      parameter (MAX_ENTRIES = 100)
      integer ITABLE_SIZE
      parameter (ITABLE_SIZE = (6*80 + 9*4)*MAX_ENTRIES )

      integer*4 arg_entries

c Structure to hold table entry data 
      structure /table_entry/
         character*80  uname                 ! Name for USAGE statement 
         character*80  vartype               ! Variable type 
         character*80  varname               ! Variable name 
         character*80   format               ! scanf format for value input 
         character*80  initvalue             ! Initial value 
         character*80  comment               ! Comment 

         integer   flag                   ! =1 if this entry is a flag and 0 if a variable 
         integer   required               ! =1 if this entry is required & 0 if not 
         integer   repeated               ! =1 if this entry is repeated & 0 if not 

         integer   flagged                ! =flag entry # if this variable is flagged, 0 if unflagged var 
         integer   param_number           ! Talbe entry number ???? 
         integer*4 address                ! variable address - used by parser 
         integer   occurances             ! # times this entry has occured - used by parser 
         integer   pointer                ! Depth of pointing 
         integer   size                   ! No. bytes per element 
      end structure

      integer DOS,SUN,MIPS,VMS
      parameter (DOS=1)
      parameter (SUN=2)
      parameter (MIPS=3)
      parameter (VMS=4)

c      integer*4 MARKER_FTN      ! These two lines must be commented out when
c      external  MARKER_FTN      ! building argtool

      record/table_entry/arg_table(MAX_ENTRIES)
c      character*1 arg_table
      common /arg_stuff/arg_entries,arg_table

