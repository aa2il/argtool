@echo off
rem *********************************************************************
rem 
rem    demo3.bat   -  Demo batch job for DOS argtool
rem
rem    Copyright (C) 1993 - 2021 by Joseph B. Attili, aa2il AT arrl DOT net
rem 
rem *********************************************************************
rem
rem   This program is free software: you can redistribute it and/or modify
rem   it under the terms of the GNU General Public License as published by
rem   the Free Software Foundation, either version 3 of the License, or
rem   (at your option) any later version.
rem
rem   This program is distributed in the hope that it will be useful,
rem   but WITHOUT ANY WARRANTY; without even the implied warranty of
rem   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem   GNU General Public License for more details.
rem
rem *****************************************************************************
rem
rem The file is divided into four parts
rem *** We first declare the args - We need to skip this table to avoid errors ***
rem
goto start

$ARG_TABLE

<dummy>        string dummy      # Required string

[-test]                          # Test flag

$END_TABLE

rem *********************************************************************
rem
rem *** This is where we get the args ***
rem
:start
argtool demo3.bat -bat setargs.bat clrargs.bat "%1 %2 %3 %4" -nh
IF ERRORLEVEL==1 goto done
call setargs.bat
del setargs.bat

rem *********************************************************************
rem
rem *** This is the heart of the batcfh file - We would use the args here ***
rem

echo dummy=%dummy%
echo test_m=%test_m%

rem *********************************************************************
rem
rem *** We need an exit point here ***
rem
call clrargs.bat
del clrargs.bat
:done

