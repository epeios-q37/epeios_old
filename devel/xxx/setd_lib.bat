#ECHO OFF

REM
REM 'setd_lib.bat' by Claude L. Simon (epeios@epeios.org).
REM
REM Create or update a library development directory under DOS/Windows.
REM
REM Copyright (C) 2000,2002 Claude L. Simon.
REM
REM This file is part of the Epeios project (http://www.epeios.org/).
REM
REM This script is free software; you can redistribute it and/or
REM modify it under the terms of the GNU General Public License
REM as published by the Free Software Foundation; either version 2
REM of the License, or (at your option) any later version.
REM 
REM This program is distributed in the hope that it will be useful,
REM but WITHOUT ANY WARRANTY; without even the implied warranty of
REM MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
REM GNU General Public License for more details.
REM
REM You should have received a copy of the GNU General Public License
REM along with this program; if not, go to http://www.fsf.org or write to the
REM  
REM                       Free Software Foundation, Inc.,
REM          59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

REM $Id$

IF "%1" == "" GOTO error
goto cont
:error
ECHO "Usage: setd_lib <library name>"
goto end
:cont

SET RTD=H:
SET LNM=%1
SET TMP=%RTD%\temp
SET SRC=%RTD%\cvs\epeios\devel\xxx
SET CRP=%TMP%\%LNM%.crp
SET DIR=%RTD%\cvs

reveal %SRC%\all_libs.crp %SRC%\gen_crp.bat %TMP%\gen_crp.bat
call %TMP%\gen_crp.bat %LNM%

SET ID=%PROJECT%

SET DST=%DIR%\%ID%\devel\%LNM%

%RTD%
IF NOT EXIST %DST% mkdir %DST%
cd %DST%

IF NOT EXIST %DST%\Makefile reveal --text-marker + %CRP% %SRC%\LibMakefile %DST%\Makefile
IF NOT EXIST %DST%\%LNM%_test.cpp reveal --text-marker + %CRP% %SRC%\xxx_test.cpp %DST%\%LNM%_test.cpp
IF NOT EXIST %DST%\%LNM%.dsp reveal --text-marker + %CRP% %SRC%\xxx.dsp %DST%\%LNM%.dsp
reveal --text-marker + %CRP% %SRC%\lib_mcp.xml %DST%\%LNM%_mcp.xml

reveal --text-marker + %CRP% %SRC%\xxx.cpp %TMP%\%LNM%.cpp
reveal --text-marker + %CRP% %SRC%\xxx.h %TMP%\%LNM%.h

IF EXIST %DST%\%LNM%.cpp GOTO cpp_exist
copy %TMP%\%LNM%.cpp %DST%
goto cpp_end
:cpp_exist
update %TMP%\%LNM%.cpp %DST%\%LNM%.cpp
:cpp_end

IF EXIST %DST%\%LNM%.h GOTO h_exist
copy %TMP%\%LNM%.h %DST%
goto h_end
:h_exist
update %TMP%\%LNM%.h %DST%\%LNM%.h
:h_end

del %CRP%
del %TMP%\%LNM%.cpp
del %TMP%\%LNM%.h

cd ..

:end
echo on

