/*
	'emobdadmn.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "../mbdbkd.h"
#include "epsmsc.h"
#include "csdsns.h"

#define VERSION			BACKEND_VERSION
#define COPYRIGHT_YEAR	BACKEND_COPYRIGHT_YEAR

#define NAME			"emobdadmn"
#define DESCRIPTION		"Backend daemon for 'emobda' software"
#define INFO			EPSMSC_EPEIOS_TEXT
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		EPSMSC_COPYRIGHT( COPYRIGHT_YEARS )
#define CVS_DETAILS		("$Id$\b " + 5)

class log_functions__
: public csdsns::log_functions__
{
	virtual void CSDSNSLog(
		csdsns::log__ Log,
		csdsns::id__ Id,
		void *UP,
		epeios::size__ Amount )
	{
	}
public:
} LogFunctions;

class kernel___
{
public:
	csdsns::core Core;
	csdsns::server___ Server;
	mbdbkd::backend_functions__ Functions;
	void Init( csdsns::service__ Service )
	{
		Functions.Init( );
		Core.Init( ::LogFunctions );
		Server.Init( Service, Functions, Core );
	}
	void Process( sck::duration__ TimeOut )
	{
		Server.Process( TimeOut );
	}

} Kernel_;

void Main(
	int argc,
	const char *argv[] )
{
	::Kernel_.Init(1234);

	::Kernel_.Process( SCK_INFINITE );
}

int main(
	int argc,
	const char *argv[] )
{
ERRFProlog
ERRFBegin
	Main( argc, argv );
ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
