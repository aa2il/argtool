$ ! ***************************************************************************
$ !                                                                           *
$ !   DEMO4.COM - Demo batch job for DCL argtool 
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
$ ! The file is broken into four parts
$ ! *** We first declare the args - We need to skip this table to avoid errors ***
$ ! 
goto start

$ARG_TABLE

<dummy>        string dummy      # Required string

[-test]                          # Test flag

$END_TABLE

$ ! *********************************************************************
$ ! 
$ ! *** This is where we get the args ***
$ ! 
$ start:
$ args:=""" ''p1' ''p2' ''p3' ''p4' ''p5' ''p6' ''p7' ''p8'"""
$ echo args
$ argtool demo4.com -dcl sys$scratch:setargs.com 'args' -nh
$!IF ERRORLEVEL==1 goto done
$ @sys$scratch:setargs.com
$ delete sys$scratch:setargs.com;*
$
$ ! *********************************************************************
$ ! 
$ ! *** This is the heart of the batcfh file - We would use the args here ***
$ ! 
$
$ write sys$output "dummy="+dummy
$ write sys$output "test_m="+test_m
$
$ ! *********************************************************************
$ ! 
$ ! *** We need an exit point here ***
$ ! 
$done:

