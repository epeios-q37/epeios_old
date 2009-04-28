@echo off
REM
REM	'getapi.bat' by Claude SIMON (http://zeusw.org/).
REM
REM	 This file is part of 'emobda' software.
REM
REM    'emobda' is free software: you can redistribute it and/or modify
REM    it under the terms of the GNU General Public License as published by
REM    the Free Software Foundation, either version 3 of the License, or
REM    (at your option) any later version.
REM
REM    'emobda' is distributed in the hope that it will be useful,
REM    but WITHOUT ANY WARRANTY; without even the implied warranty of
REM    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
REM    GNU General Public License for more details.
REM
REM    You should have received a copy of the GNU General Public License
REM    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
REM
REM
REM	 $Id$
REM
@echo on

getbkdapi localhost:1234 mbdapi.xml
sabcmd file://H:\cvs\ebkd\stable\bkd4cpp.xsl mbdapi.xml mbdapi.h