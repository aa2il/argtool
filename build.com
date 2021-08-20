$ ! ***************************************************************************
$ !                                                                           *
$ !   BUILD.COM - Build file for VMS ARGTOOL - JBA - June 1993                *
$ !                                                                           *
$ !      Copyright (C) 1993 - 2021 by Joseph B. Attili, aa2il AT arrl DOT net *
$ !                                                                           *
$ ! ***************************************************************************
$ ! 
$ !    This program is free software: you can redistribute it and/or modify
$ !    it under the terms of the GNU General Public License as published by
$ !    the Free Software Foundation, either version 3 of the License, or
$ !    (at your option) any later version.
$ ! 
$ !    This program is distributed in the hope that it will be useful,
$ !    but WITHOUT ANY WARRANTY; without even the implied warranty of
$ !    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
$ !    GNU General Public License for more details.
$ ! 
$ ! *****************************************************************************
$ !
$ ! Compiler opts & macros
$     cflags   == "/list"
$     fflags   == "/list"
$     echo     == "write sys$output "
$
$ ! Define executables as foreign so can get command line args (DCL requires this)
$     quote    =  """
$     argpath  == f$parse("*.*",,,"device")+f$directory()
$     argtool  == "$ "+argpath+"argtool.exe"
$     argtool1 == "$ "+argpath+"argtool_1.exe"
$     demo     == "$ "+argpath+"demo.exe"
$     demo2    == "$ "+argpath+"demo2.exe"
$ echo " "
$ echo "Don't forget to add the following line to your LOGIN.COM script:"
$ echo "argtool == "+quote+argtool+quote
$ echo " "
$
$ goto skip
$
$ ! Remove old objects
$ echo "Cleaning up the old mess ..."
$     delete *.lib;*
$     delete *.exe;*
$     delete *.obj;*
$     delete *.lis;*
$     purge
$
$ ! Build the library
$ echo "Compiling library ..."
$     cc 'cflags' machine
$     cc 'cflags' collapse
$     cc 'cflags' detab
$     cc 'cflags' error
$     cc 'cflags' ftnfmt
$     cc 'cflags' marker
$     cc 'cflags' parsecmd
$     cc 'cflags' parselin
$     cc 'cflags' spltpath
$     cc 'cflags' strdup
$     cc 'cflags' parsetab
$     cc 'cflags' pf
$     cc 'cflags' pb
$     cc 'cflags' markerf
$     cc 'cflags' saveinfo
$     for 'fflags' findargs
$
$     libr/create argtool
$     libr/replace argtool *.obj
$
$skip:
$
$ ! Build the argtool
$ echo "Linking argtool..."
$     argtool argtool.arg -c
$     cc 'cflags' argtool
$     link argtool,argtool/lib,sys$library:vaxcrtl.olb/lib
$
$ ! Build demos
$ echo "Creating DCL demo ..."
$ !argtool demo4.com -dcl setargs.com 
$ @demo4
$ @demo4 abcef
$! exit
$
$ echo "Creating C demo ..."
$     argtool demo.arg -c
$     cc 'cflags' demo.c
$     link demo,argtool/lib,sys$library:vaxcrtl.olb/lib
$
$     echo "Running demo with no args ..."
$     demo
$     echo "RUNNING demo abc def xyz -j 10 20 30 -n 23 4.5"
$     demo abc def xyz -j 10 20 30 -n 23 4.5
$
$ echo "Creating FORTRAN demo2 ..."
$     argtool demo2.arf -ftn
$     for 'fflags' demo2
$     link demo2,argtool/lib,sys$library:vaxcrtl.olb/lib
$
$     echo "Running demo2 with no args ..."
$     demo2
$     echo "Running demo2 abc xyz 12 3.4 -junk 22 33 44 -test wxyz mnopqrst"
$     demo2 "abc xyz" 12 3.4 -junk 22 33 44 -test wxyz mnopqrst
$
$done:
$ echo "Purging ..."
$     purge
$ echo " "
$ echo "Don't forget to add the following line to your LOGIN.COM script:"
$ echo "argtool == "+quote+argtool+quote
