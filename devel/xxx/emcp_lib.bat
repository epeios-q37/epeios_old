ECHO OFF

REM
REM 'setd_lib.bat' by Claude L. Simon (epeios@epeios.org).
REM
REM Put a development library to stable directory under DOS/Windows.
REM
REM Copyright (C) 2000 Claude L. Simon.
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
ECHO "Usage: emcp_lib <library name>"
goto end
:cont

SET RTD=H:
SET LNM=%1
SET TMP=%RTD%\temp
SET SRC=%RTD%\cvs\epeios\devel\xxx
SET CRP=%TMP%\%LNM%.crp
SET DST=..\..\stable

attrib %DST%\%LNM%.h -r
attrib %DST%\%LNM%.cpp -r

reveal %SRC%\all_libs.crp %SRC%\gen_crp.bat %TMP%\gen_crp.bat
call %TMP%\gen_crp.bat %LNM%

reveal --text-marker + %CRP% %LNM%.h %DST%\%LNM%.h
reveal --text-marker + %CRP% %LNM%.cpp %DST%\%LNM%.cpp

attrib %DST%\%LNM%.h +r
attrib %DST%\%LNM%.cpp +r

:end
echo on