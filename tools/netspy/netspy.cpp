/*
	'netspy' by Claude L. SIMON (simon@epeios.org)
	Copyright (C) 2001 Claude L. SIMON

	This file is part of the Epeios project (http://www.epeios.org/).

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:

                    	Free Software Foundation, Inc.,
    	   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// $Id$

#include "err.h"
#include "stf.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "sck.h"
#include "srv.h"
#include "clt.h"
#include "mtk.h"

#define NAME			"netspy"
#define VERSION			"0.1.0"
//#define COPYRIGHT		"Copyright (c) 2001 IDEALX (" EPSMSC_IDEALX_URL ")."
#define COPYRIGHT		"Copyright (c) 2001 Claude L. SIMON (" AUTHOR_EMAIL ")."
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define CVS_DETAILS		("$Id$\b " + 5)

/* Beginning of the part which handles command line arguments. */

enum command {
	cHelp,
	cVersion,
	cLicense
};

enum option {
};

struct parameters {
	const char *PortToListen;
	const char *Target;
	parameters( void )
	{
		PortToListen = "8080";
		Target = "194.51.246.50:80";
//		Target = "www.epeios.org:80";
//		Target = "linuxfr.org:80";
		
	}
};

void PrintUsage( const clnarg::description_ &Description )
{
	fout << "Usage: " << NAME << " [command] [options] ..." << txf::nl;
	fout << "(description)" << txf::nl;
	fout << "Command:" << txf::nl;
//	clnarg::PrintCommandUsage( Description, c, "", false, true );
	clnarg::PrintCommandUsage( Description, cVersion, "print version of " NAME " components.", false, false );
	clnarg::PrintCommandUsage( Description, cLicense, "print text about the license.", true, false );
	clnarg::PrintCommandUsage( Description, cHelp, "print this message.", true, false );
	fout << "Options:" << txf::nl;
//	clnarg::PrintOptionUsage( Description, o, "", false );
}

void PrintHeader( void )
{
	fout << NAME " V" VERSION " ("__DATE__ " " __TIME__ ") by " AUTHOR_NAME " (" << AUTHOR_EMAIL << ") " << txf::nl << COPYRIGHT << txf::nl;
//	fout << EPSMSC_IDEALX_TEXT <<txf::nl;
//	fout << EPSMSC_EPEIOS_TEXT <<txf::nl;
	fout << "CVS file details : " << CVS_DETAILS << txf::nl;
}


static void AnalyzeOptions(
	clnarg::analyzer___ &Analyzer,
	parameters &Parameters )
{
ERRProlog
	epeios::row__ P;
	clnarg::option_list Options;
	clnarg::id__ Option;
	const char *Unknow = NULL;
	clnarg::argument Argument;
ERRBegin
	Options.Init();

	if ( ( Unknow = Analyzer.GetOptions( Options ) ) != NULL ) {
		ferr << '\'' << Unknow << "': unknow option." << txf::nl;
		fout << HELP << txf::nl;
		ERRt();
	}

	P = Options.First();

	while( P != NONE ) {
		Argument.Init();

		switch( Option = Options( P ) ) {
//		case o:
		default:
			ERRc();
		}

		P = Options.Next( P );
	}
	
ERRErr
ERREnd
ERREpilog
}

static void AnalyzeFreeArguments(
	clnarg::analyzer___ &Analyzer,
	parameters &Parameters )
{
ERRProlog
	clnarg::arguments Free;
	epeios::row__ P;
ERRBegin
	Free.Init();

	Analyzer.GetArguments( Free );

	P = Free.Last();

	switch( Free.Amount() ) {
	case 0:
		break;
	default:
		ferr << "Too many arguments." << txf::nl;
		fout << HELP << txf::nl;
		ERRt();
		break;
	}

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeArgs(
	int argc,
	const char *argv[],
	parameters &Parameters )
{
ERRProlog
	clnarg::description Description;
	clnarg::analyzer___ Analyzer;
ERRBegin
	Description.Init();

//	Description.AddCommand( '', "", c );
	Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
	Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );
	Description.AddCommand( CLNARG_NO_SHORT, "license", cLicense );
//	Description.AddOption( '', "", o );

	Analyzer.Init( argc, argv, Description );

	switch ( Analyzer.GetCommand() ) {
	case cVersion:
		PrintHeader();
		TTR.Advertise();
		ERRt();
		break;
	case cHelp:
		PrintUsage( Description );
		ERRt();
		break;
	case cLicense:
		epsmsc::PrintLicense();
		ERRt();
		break;
//	case c:
	case CLNARG_NONE:
		break;
	default:
		ERRc();
	}

	AnalyzeOptions( Analyzer, Parameters );

	AnalyzeFreeArguments( Analyzer, Parameters );

ERRErr
ERREnd
ERREpilog
}

/* End of the part which handles command line arguments. */

struct data__ {
	sck::socket_ioflow___ *Server, *Client;
};

void Server( void *UP )
{
	data__ &D = *(data__ *)UP;
	flw::data__ Buffer[100];
	flw::size__ Size;
	unsigned char C;
	
	for(;;) {
		Size = D.Server->GetUpTo( sizeof( Buffer ), Buffer );
		fout.Put( Buffer, Size );
		fout << txf::sync;
		D.Client->Put( Buffer, Size );
		D.Client->Synchronize();

/*		C = D.Server->Get();
		fout.Put( C );
		fout << txf::sync;
		D.Client->Put( C );
*/

	}
}
	
void Client( void *UP )
{
	data__ &D = *(data__ *)UP;
	flw::data__ Buffer[100];
	unsigned char C;
	flw::size__ Size;
	
	for(;;) {
/*		Size = D.Client->GetUpTo( sizeof( Buffer ), Buffer );
		fout.Put( Buffer, Size );
		fout << txf::sync;
		D.Server->Put( Buffer, Size );
*/
		C = D.Client->Get();
		fout.Put( C );
		fout << txf::sync;
		D.Server->Put( C );
		D.Server->Synchronize();
	}
}

class connection__
: public srv::socket_functions__
{
private:
const char *Target_;
protected:
	virtual void SRVProcess( sck::socket__ Socket )
	{
	ERRProlog
		sck::socket_ioflow___ Server, Client;
		data__ Data;
	ERRBegin
		fout << "Connection ..." << txf::nl;
		
		Server.Init( Socket );
		Client.Init( clt::Connect( Target_ ) );
			
		Data.Server = &Server;
		Data.Client = &Client;
			
		mtk::Launch( ::Server, &Data );
		::Client( &Data );
	ERRErr
	ERREnd
	ERREpilog
	}
public:
	void Init( const char *Target )
	{
		Target_ = Target;
	}
};

	
void Go( const parameters &Parameters )
{
ERRProlog
	srv::listener___ Listener;
	connection__ Connection;
ERRBegin
	Connection.Init( Parameters.Target );
	
	Listener.Init( atoi( Parameters.PortToListen ) );
	
	Listener.Process( Connection );
ERRErr
ERREnd
ERREpilog
}

static inline void Main(
	int argc,
	const char *argv[] )
{
ERRProlog
	parameters Parameters;
ERRBegin
	AnalyzeArgs( argc, argv, Parameters );

	Go( Parameters );
ERRErr
ERREnd
ERREpilog
}

int main(
	int argc,
	const char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	Main( argc, argv );
ERRFErr
	ExitValue = EXIT_FAILURE;

	if ( ERRMajor == err::thw )
		ERRRst();

ERRFEnd
ERRFEpilog
	fout << txf::sync;

	return ExitValue;
}
