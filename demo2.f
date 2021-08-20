c *****************************************************************************
c
c    DEMO.ARF - Argument Tool test prog - FORTRANN Version
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
c
      program  demo2
c
c *****************************************************************************
c
c Var declarations
      use, intrinsic :: ISO_C_BINDING
      integer  dummy,i
c
c The table must be after local var defs but before and executable code (including DATA statements)
c
c The convention is <> for required params, [] for optional, ... for repeated
c
c Vars are declared as
c  <usage_name>   <type>   <var_name>  [init_value]   [# Comment]
c
c Flags are declared as
c  <-flag> [# Comment]
c
c There is a blank line betwen flag groups.  
c Here is an example.....
      character*60 pgm_name / 'This is a test' /  	 !  Required string
      integer n / 5 /  	 !  Required integer
      real f / 1.0 /  	 !  Optional float
      integer i99(6) 	 !  Repeated integer
      character*20 st(5) 	 !  Repeated string

      character PROGRAM_NAME*6
      common/program_name_stuff/program_name
      character USAGE*200
      common/usage_stuff/usage
      character HELP_MSG*2
      common/help_msg_stuff/help_msg

      include 'argtool.inc'
      PROGRAM_NAME='demo2'//char(0)
      USAGE='Usage:\t%s\t <prog_name>\n                        [n]\n    
     &                    [f]\n                        [-junk <i99>...]\
     &n                        [-test <st>...]\n                       [
     &-help]\n'//char(0)
      HELP_MSG=' '//char(0)
      arg_entries=7

      call set_table_ftn(arg_table,0,
     &'prog_name'//char(0),
     &'character*60'//char(0),
     &'pgm_name'//char(0),
     &'%s'//char(0),
     &'This is a test'//char(0),
     &' Required string'//char(0),
     &0,1,0,0,0,
     &loc(pgm_name),0,0,60)

      call set_table_ftn(arg_table,1,
     &'n'//char(0),
     &'integer'//char(0),
     &'n'//char(0),
     &'%D'//char(0),
     &char(0),
     &' Required integer'//char(0),
     &0,0,0,1,0,
     &loc(n),0,0,8)

      call set_table_ftn(arg_table,2,
     &'f'//char(0),
     &'real'//char(0),
     &'f'//char(0),
     &'%f'//char(0),
     &'1.0'//char(0),
     &' Optional float'//char(0),
     &0,0,0,2,0,
     &loc(f),0,0,4)

      call set_table_ftn(arg_table,3,
     &'-junk'//char(0),
     &' '//char(0),
     &' '//char(0),
     &' '//char(0),
     &char(0),
     &' '//char(0),
     &1,0,0,0,3,
     &0,0,0,0)

      call set_table_ftn(arg_table,4,
     &'i99'//char(0),
     &'integer'//char(0),
     &'i99'//char(0),
     &'%D'//char(0),
     &char(0),
     &' Repeated integer'//char(0),
     &0,1,6,0,3,
     &loc(i99),0,1,8)

      call set_table_ftn(arg_table,5,
     &'-test'//char(0),
     &' '//char(0),
     &' '//char(0),
     &' '//char(0),
     &char(0),
     &' '//char(0),
     &1,0,0,0,5,
     &0,0,0,0)

      call set_table_ftn(arg_table,6,
     &'st'//char(0),
     &'character*20'//char(0),
     &'st'//char(0),
     &'%s'//char(0),
     &char(0),
     &' Repeated string'//char(0),
     &0,1,5,0,5,
     &loc(st),0,1,20)
c
c Executable starts after the table
c
c This program just reports the arg vars after they are retrieved
      write(*,*) 'Before:'
      write(*,*) 'pgm_name=',pgm_name
      write(*,*) 'n=',n
      write(*,*) 'f=',f
c
c This line retrives the command line ars & associates them with the local vars
      call find_args
c
c MARKER_FTN tells how many times a repeated arg was given
      write(*,*) 'After:'
      write(*,*) 'pgm_name=',pgm_name
      write(*,*) 'n=',n
      write(*,*) 'f=',f
      write(*,*) 'MARKER(i99)=',MARKER_FTN('i99')
      do i=1,5
         write(*,*) 'i99(',i,')=',i99(i)
      end do
      write(*,*) 'MARKER(st)= ',MARKER_FTN('st')
      do i=1,5
         write(*,*) 'st( ',i,')=',st(i)
      end do

      end

