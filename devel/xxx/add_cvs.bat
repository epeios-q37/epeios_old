@ECHO OFF

REM
REM 'add_cvs.bat' by Claude L. Simon (simon@epeios.org).
REM
REM Create or update a library development directory under DOS/Windows.
REM
REM Copyright (C) 2001 Claude L. Simon.
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

if "%1"=="" goto no_argument
if not exist %1\%1.h goto no_directory
echo on
cvs add %1
@cd %1
cvs add %1.cpp %1.h %1_test.cpp Makefile
goto end
:no_argument
echo usage: addcvs <library name>
goto end
:no_directory
echo Don't find directory for library '%1'.
goto end
:end