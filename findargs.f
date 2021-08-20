c *****************************************************************************
c
c     findargs.f   -  Routine to associate cmd line args of a fortran program
c                     with its variables
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

#ifdef linux
      integer function number_of_args()
      number_of_args=iargc()
      return
      end
#endif
      
c *****************************************************************************

      subroutine find_args

      include 'argtool.inc'

      character*80 argv(0:MAX_ENTRIES)
      character*1024 cmdlin         
c      integer*2 i2, j2              
      integer   i, j            
      logical quote
      character*1 q

      character*1 USAGE
      common   /usage_stuff/usage

      character*1 HELP_MSG
      common   /help_msg_stuff/help_msg

      character*1 PROGRAM_NAME
      common   /program_name_stuff/program_name

      integer  m,k
#ifdef linux
      integer  machine_type,istring_length
      external machine_type,istring_length
#else
      integer  machine_type,number_of_args,istring_length
      external machine_type,number_of_args,istring_length
#endif
c
c Replace tabs & new lines in USAGE message
      call cvt_special_chars(USAGE)
      call cvt_special_chars(HELP_MSG)
c
c Get command line args & put into 2d array
      if( machine_type().eq.DOS .or. machine_type().eq.SUN 
     $     .or. machine_type().eq.MIPS ) then

         m = number_of_args()
         i = 0
         do while  ( i .le. m )
            if( machine_type().eq.DOS ) then
               stop 'FINDARGS - Need special treatment for DOS'
c               i2=i
c               call getarg ( i2, argv(i) ,j2) 
c               j=j2
            elseif( machine_type().eq.SUN .or. 
     $	      machine_type().eq.MIPS ) then
c               stop 'FINDARGS - Need special treatment for SUN'
               call getarg ( i, argv(i))
               j = istring_length( argv(i) )
            end if
            argv(i)(j+1:j+1) = char(0)
            i = i + 1
         end do
         if(machine_type().eq.SUN .or. machine_type().eq.MIPS ) m=m+1 

      else
c
c        For VMS, we need to get the entire command line 
         if(.true.) stop 'FINDARGS - Need special treatment for VMS'
c         call lib$get_foreign(cmdlin)
c
c        Remove enclosing quotes if they're there
cc         i=istring_length(cmdlin)
cc         if( cmdlin(1:1).eq.'"' .and. cmdlin(i:i).eq.'"' ) then
cc              cmdlin(1:1)=' '
cc              cmdlin(i:i)=' '
cc         end if
c
c        Parse it
         argv(0)=PROGRAM_NAME//char(0)
         k=1
         q = '"'            ! q is single quote
         do m=1,MAX_ENTRIES
c
c           Look for start of next word
            do j=k,1024
               if(cmdlin(j:j) .ne.' ') goto 10
            end do
            goto 100                  ! Must not be any more
c
c           Look for end of the word
10          k=j
            quote = (cmdlin(k:k).eq.q)
            do j=k+1,1024
               if(cmdlin(j:j) .eq. q ) quote = .not. quote
               if(.not.quote .and. cmdlin(j:j) .eq.' ') goto 20
            end do
c
c           Got it - Take off quotes if they're there
20          continue
            j=j-1
            if( cmdlin(k:k).eq.q .and. cmdlin(j:j).eq.q ) then
            j=j-1
            k=k+1
          end if
          argv(m) = cmdlin(k:j)//char(0)
            k=j+2
         end do

100      continue
      end if

      if( machine_type().eq.DOS .or. machine_type().eq.SUN
     $     .or. machine_type().eq.MIPS ) then
         call parse_cmd_line_ftn(arg_entries,arg_table,m,argv,
     &                            USAGE,HELP_MSG,PROGRAM_NAME)
      else
         stop 'FINDARGS - Need special treatment for VMS'
c         call parse_cmd_line_ftn(%REF(arg_entries),%REF(arg_table),
c     &       %REF(m),%REF(argv(0)),%REF(USAGE),%REF(HELP_MSG),
c     &       %REF(PROGRAM_NAME))      ! VMS
      end if

      return
      end

c *****************************************************************************

c Routine to convert special chars in a C-string to their actuals 
      subroutine cvt_special_chars(string)

      character*1 string(*),ch,slash

      integer i,j

      j=1
      i=1
      ch='X'
      slash=char(92)
      do while( ch.ne.char(0) )
         ch = string(i)
         if(ch.eq.slash) then
            ch = string(i+1)
            if(ch.eq.'t') then
               ch = char(9)
               i=i+1
            else if(ch.eq.'n') then
               string(j) = char(13)
               j=j+1
               ch = char(10)
               i=i+1
            else
               ch = slash
            end if
         end if
         string(j) = ch
         j=j+1
         i=i+1
      end do

      return
      end


c *****************************************************************************

      integer function istring_length(s)
      character*(*) s

      integer i

      do i=len(s),1,-1
         if(s(i:i).ne.' ') then
            istring_length = i
            return
         end if
      end do

      istring_length = 0

      return
      end


c *****************************************************************************

c Routine to get number of time a var or flag is specified
      integer*4 function marker_ftn(flag)

      character*(*) flag

      character*80 flag2

      include 'argtool.inc'

      integer*4 get_marker_ftn

      integer  machine_type
      external machine_type 

      flag2=flag//char(0)
      if( machine_type().eq.DOS .or. machine_type().eq.SUN
     $     .or. machine_type().eq.MIPS ) then
         marker_ftn = get_marker_ftn(flag2,arg_entries,arg_table)
      else
         stop 'MARKER_FTN - Need special treatment for VMS'
c         marker_ftn = get_marker_ftn(%REF(flag2),%REF(arg_entries),
c     &      %REF(arg_table))                  ! VMS
      end if

      return
      end

