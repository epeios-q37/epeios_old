/*
  Test source for the 'brkanl' library by Claude L. Simon (epeios@epeios.org).
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
*/
#include "brkanl.h"

using namespace brkanl;

#include "err.h"
#include "stf.h"
#include "clt.h"
#include "salcsm.h"
#include "broker.h"

class manager
: public salcsm::manager___
{
protected:
	//v Client process function.
	virtual salcsm::behavior SALCSMCP(
		flw::ioflow___ &Client,
		flw::ioflow___ &Server,
		void *UP )
	{
		::broker::broker &Broker = *(::broker::broker *)UP;
		
		Broker.Handle( Client ); 
	
		return salcsm::bContinue;
	}
	//v Client initialization function.
	virtual void *SALCSMCI(
		flw::ioflow___ &Client,
		flw::ioflow___ &Server )
	{
		::broker::broker *Broker;
		
		if ( ( Broker = new ::broker::broker ) == NULL )
			ERRa();
			
		Broker->Init();
		
		return Broker;
	}
	//v Client ending functions.
	virtual void SALCSMCE( void *UP )
	{
		if ( UP != NULL )
			delete (::broker::broker *)UP;
	}
};


void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

#if 0

void Print(	const parameters_ &Parameters )
{
	POSITION__ P = Parameters.First();

	while( P != NONE ) {
		fout << txf::tab << brkcst::CastsNames[Parameters( P )];
		P = Parameters.Next( P );
	}

	fout << txf::nl;
}

void Print(	const commands_ &Commands )
{
	CITEM( command_ ) Command;
	POSITION__ P = Commands.First();

	Command.Init( Commands );

	while( P != NONE ) {
		fout << Command( P ).Identification.Value << " (" << Command( P ).Identification.ID() << "):" << txf::nl;
		Print( Command( P ).Parameters );
		P = Commands.Next( P );
	}

	fout << txf::nl;
}

void Print( types_ &Types )
{
	CITEM( type_ ) Type;
	POSITION__ P = Types.First();

	Type.Init( Types );

	while( P != NONE ) {
		fout << Type( P ).Identification.Value << " (" << Type( P ).Identification.ID() << "):" << txf::nl;
		Print( Type( P ).Commands );
		P = Types.Next( P );
	}
}


/*****************************************************/

void GenFunctionParameter(
	brkcst::cast Cast,
	bso__bool In,
	bso__ulong Indice,
	txf::text_oflow___ &Flow)
{
	Flow << txf::tab << txf::tab << txf::tab;

	if ( In ) 
		Flow << "const ";

	Flow << brkcst::CastsNames[Cast] << " &";

	if ( In )
		Flow << "In";
	else
		Flow << "Out";

	Flow << Indice;
}


void GenFunctionParameters(
	const parameters_ &Parameters,
	txf::text_oflow___ &Flow)
{
	POSITION__ P = Parameters.First();
	bso__bool In = true;
	brkcst::cast Cast;
	bso__ulong Indice = 1;

	if ( (brkcst::cast)Parameters( P ) == brkcst::cEnd ) {
		In = false;
		P = Parameters.Next( P );
	}

	if ( P == NONE )
		Flow << " void";
	else {
		GenFunctionParameter( (brkcst::cast)Parameters( P ), In, Indice++, Flow );
		P = Parameters.Next( P );
	}

	while( P != NONE ) {
		if ( ( Cast = (brkcst::cast)Parameters( P) ) == brkcst::cEnd ) {
			In = false;
			Indice = 1;
		} else {
			Flow << ',' << txf::nl;

			GenFunctionParameter( Cast, In, Indice++, Flow );
		}

		P = Parameters.Next( P );
	}

	fout << " )" << txf::nl;
}


void GenFunctionBody(
	const parameters_ &Parameters,
	txf::text_oflow___ &Flow)
{
	POSITION__ P = Parameters.First();
	bso__bool In = true;
	brkcst::cast Cast;
	bso__ulong Indice = 1;

	while ( ( P != NONE ) && ( ( Cast = (brkcst::cast)Parameters( P) ) != brkcst::cEnd ) ) {
		Flow << txf::tab << txf::tab << txf::tab << "Broker->Push" << brkcst::CastsNames[Cast] << "( In" << Indice++ << " );" << txf::nl;
		P = Parameters.Next( P );
	}

	if ( P == NONE )
		ERRf();

	Flow << txf::tab << txf::tab << txf::tab
		<< "if ( ( Return = Broker_->Send() ) ) {" << txf::nl;

	Indice = 0;

	P = Parameters.Next( P );

	while( P != NONE ) {
		Flow << txf::tab << txf::tab << txf::tab << txf::tab
			 << "Broker->Pop" << brkcst::CastsNames[Parameters( P)] << "( Out" << Indice++ << " );" << txf::nl;
		P = Parameters.Next( P );
	}

	Flow << txf::tab << txf::tab << txf::tab << "}" << txf::nl;
	Flow << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "return Return;" << txf::nl;
	Flow << txf::tab << txf::tab << "}" << txf::nl;
}

void GenRemoteCommandParameters(
	const parameters_ &Parameters,
	txf::text_oflow___ &Flow )
{
	POSITION__ P;
	
	Flow << txf::tab << txf::tab << txf::tab << txf::tab;
	
	P = Parameters.First();
	
	while( P != NONE ) {
		Flow << (bso__ulong)Parameters( P ) << ", " ;
		P = Parameters.Next( P );
	}
	
	Flow << txf::nl;
}
	
	
	

void GenAPI(
	const commands_ &Commands,
	txf::text_oflow___ &Flow)
{
	CITEM( command_ ) Command;
	POSITION__ P = NONE;
	bso__ushort I = 0;
	bso__ulong Cumul = 0;

	Command.Init( Commands );

	
	Flow << txf::tab << txf::tab << "void Init( void )" << txf::nl;
	Flow << txf::tab << txf::tab << "{" << txf::nl;
	
	Flow << txf::tab << txf::tab << txf::tab << "commands_details CommandsDetails;" << txf::nl;
	Flow << txf::nl;
	
	Flow << txf::tab << txf::tab << txf::tab << "command_detail CommandsDetail;" << txf::nl;
	Flow << txf::nl;
	
	Flow << txf::tab << txf::tab << txf::tab << "ids16 Commands;" << txf::nl;
	Flow << txf::nl;
	
	Flow << txf::tab << txf::tab << txf::tab << "bso__ushort Parameters[] = {" << txf::nl;
	Flow << txf::nl;
	
	P = Commands.First();
	
	while( P != NONE ) {
		GenRemoteCommandParameters( Command( P ).Parameters, Flow );
		P = Commands.Next( P );
	}
		
	Flow << txf::tab << txf::tab << txf::tab << "};" << txf::nl;
	
	Flow << txf::tab << txf::tab << txf::tab << "CommandsDetails.Init();" << txf::nl;
	Flow << txf::nl;

	P = Commands.First();

	while( P != NONE ) {
		
		Flow << txf::tab << txf::tab << txf::tab << "CommandDetail.Init();" << txf::nl;
		Flow << txf::tab << txf::tab << txf::tab << "CommandDetail.Name = \"" << Command( P ).Identification.Value << "\";" << txf::nl;
		Flow << txf::tab << txf::tab << txf::tab << "CommandDetail.Casts.Add( Parameters + " << Cumul << ", " << Command( P ).Parameters.Amount() << " );" << txf::nl;
		Cumul += Command( P ).Parameters.Amount();
		Flow << txf::tab << txf::tab << txf::tab << "CommandsDetails.Add( CommandDetail );" << txf::nl;
		Flow << txf::nl;
		
		P = Commands.Next( P );
	}
	
	Flow << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "Commands.Init();" << txf::nl;
	Flow << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "Broker->GetCommands( ..., ComandsDetails, Commands );" << txf::nl;
	Flow << txf::nl;
	
	Flow << txf::tab << txf::tab << txf::tab << "Commands.Read( Commands_ );" << txf::nl;
	Flow << txf::nl;
	
	P = Commands.First();

	while( P != NONE ) {
		Flow << txf::tab << txf::tab << "bso__bool " << Command( P ).Identification.Value << "(" << txf::nl;
		GenFunctionParameters( Command( P ).Parameters, Flow );
		Flow << txf::tab << txf::tab << "{" << txf::nl;
		Flow << txf::tab << txf::tab << txf::tab << "Broker->PushHeader( ID_, Commands_[" << I++ << "] );" << txf::nl;
		GenAPIBody( Command( P ).Parameters, Flow );
		P = Commands.Next( P );
	}

	fout << txf::nl;
}

#endif

///////////////////////////////////

inline void GenCommandCastsList(
	const parameters_ &Parameters,
	txf::text_oflow___ &Flow )
{
	POSITION__ P;
	
	Flow << txf::tab << txf::tab << txf::tab << txf::tab;
	
	P = Parameters.First();
	
	while( P != NONE ) {
		Flow << (bso__ulong)Parameters( P ) << ", " ;
		P = Parameters.Next( P );
	}
	
	Flow << txf::nl;
}

inline void GenFillingInstruction(
	const command_ &Command,
	bso__ulong Position,
	txf::text_oflow___ &Flow )
{
	Flow << txf::tab << txf::tab << txf::tab << "CommandDetail.Init();" << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "CommandDetail.Name = \"" << Command.Identification.Value << "\";" << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "CommandDetail.Casts.Add( Parameters + " << Position << ", " << Command.Parameters.Amount() << " );" << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "CommandsDetails.Add( CommandDetail );" << txf::nl;
	Flow << txf::nl;
}
	

//////////////////////////////////////

inline void GenCommandsCastsLists(
	const commands_ &Commands,
	txf::text_oflow___ &Flow )
{
	CITEM( command_ ) Command;
	POSITION__ P = NONE;
	
	Command.Init( Commands );
	
	P = Commands.First();
	
	while( P != NONE ) {
		GenCommandCastsList( Command( P ).Parameters, Flow );
		P = Commands.Next( P );
	}
}

inline void GenFillingInstructions(
	const commands_ &Commands,
	txf::text_oflow___ &Flow )
{
	CITEM( command_ ) Command;
	POSITION__ P = NONE;
	bso__ulong Cumul = 0;

	Command.Init( Commands );

	P = Commands.First();

	while( P != NONE ) {
		GenFillingInstruction( Command( P ), Cumul, Flow );
		Cumul += Command( P ).Parameters.Amount();
		P = Commands.Next( P );
	}
}

void GenCommandFunctionParameter(
	brkcst::cast Cast,
	bso__bool In,
	bso__ulong Indice,
	txf::text_oflow___ &Flow)
{
	Flow << txf::tab << txf::tab << txf::tab;

	if ( In ) 
		Flow << "const ";

	Flow << brkcst::CastsNames[Cast] << " &";

	if ( In )
		Flow << "In";
	else
		Flow << "Out";

	Flow << Indice;
}

POSITION__ GenPushInstructions(
	const parameters_ &Parameters,
	txf::text_oflow___ &Flow )
{
	POSITION__ P = Parameters.First();
	bso__ulong Indice = 1;
	brkcst::cast Cast;
	
	while ( ( P != NONE ) && ( ( Cast = (brkcst::cast)Parameters( P ) ) != brkcst::cEnd ) ) {
		Flow << txf::tab << txf::tab << txf::tab << "Broker->Push" << brkcst::CastsNames[Cast] << "( In" << Indice++ << " );" << txf::nl;
		P = Parameters.Next( P );
	}

	if ( P == NONE )
		ERRf();
	
	return Parameters.Next( P );
}

POSITION__ GenPopInstructions(
	const parameters_ &Parameters,
	POSITION__ P,
	txf::text_oflow___ &Flow )
{
	bso__ulong Indice = 1;

	while( P != NONE ) {
		Flow << txf::tab << txf::tab << txf::tab << txf::tab
			 << "Broker->Pop" << brkcst::CastsNames[Parameters( P)] << "( Out" << Indice++ << " );" << txf::nl;
		P = Parameters.Next( P );
	}
}

	
//////////////////////////////////////

inline void GenInitFunctionDeclarations(
	const commands_ &Commands,
	txf::text_oflow___ &Flow )
{
	Flow << txf::tab << txf::tab << txf::tab << "commands_details CommandsDetails;" << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "command_detail CommandsDetail;" << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "ids16 Commands;" << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "bso__ushort Parameters[] = {" << txf::nl;
	
	GenCommandsCastsLists( Commands, Flow );
	
	Flow << txf::tab << txf::tab << txf::tab << "};" << txf::nl;
	Flow << txf::nl;
}


inline void GenInitFunctionInstructions(
	const commands_ &Commands,
	txf::text_oflow___ &Flow )
{
	Flow << txf::tab << txf::tab << txf::tab << "CommandsDetails.Init();" << txf::nl;
	Flow << txf::nl;
	
	GenFillingInstructions( Commands, Flow );
	
	Flow << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "Commands.Init();" << txf::nl;
	Flow << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "Broker->GetCommands( ..., ComandsDetails, Commands );" << txf::nl;
	Flow << txf::nl;
	
	Flow << txf::tab << txf::tab << txf::tab << "Commands.Read( Commands_ );" << txf::nl;
	Flow << txf::nl;
}


void GenCommandFunctionParameters(
	const parameters_ &Parameters,
	txf::text_oflow___ &Flow)
{
	POSITION__ P = Parameters.First();
	bso__bool In = true;
	brkcst::cast Cast;
	bso__ulong Indice = 1;

	if ( (brkcst::cast)Parameters( P ) == brkcst::cEnd ) {
		In = false;
		P = Parameters.Next( P );
	}

	if ( P == NONE )
		Flow << " void";
	else {
		Flow << txf::nl;
		GenCommandFunctionParameter( (brkcst::cast)Parameters( P ), In, Indice++, Flow );
		P = Parameters.Next( P );
	}

	while( P != NONE ) {
		if ( ( Cast = (brkcst::cast)Parameters( P) ) == brkcst::cEnd ) {
			In = false;
			Indice = 1;
		} else {
			Flow << ',' << txf::nl;

			GenCommandFunctionParameter( Cast, In, Indice++, Flow );
		}

		P = Parameters.Next( P );
	}
}

inline void GenCommandFunctionDeclarations( txf::text_oflow___ &Flow )
{
	Flow << txf::tab  << txf::tab << txf::tab << "bso__bool State;" << txf::nl;
}

inline void GenCommandFunctionsInstructions(
	const parameters_ &Parameters,
	bso__ulong Indice,
	txf::text_oflow___ &Flow )
{
	POSITION__ P = NONE;

	Flow << txf::tab << txf::tab << txf::tab << "Broker->PushHeader( ID_, Commands_[" << Indice << "] );" << txf::nl;
	
	P = GenPushInstructions( Parameters, Flow );
	
	Flow << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "if ( State = Broker->Send() ) {" << txf::nl;
	
	GenPopInstructions( Parameters, P, Flow );
}

//////////////////////////////////

inline void GenInitFunctionHeader( txf::text_oflow___ &Flow )
{
	Flow << txf::tab << txf::tab << "void Init( void )" << txf::nl;
	Flow << txf::tab << txf::tab << "{" << txf::nl;
}

inline void GenInitFunctionBody( 
	const commands_ Commands,
	txf::text_oflow___ &Flow )
{
	GenInitFunctionDeclarations( Commands, Flow );
	GenInitFunctionInstructions( Commands, Flow );
}

inline void GenInitFunctionFooter( txf::text_oflow___ &Flow )
{
	Flow << txf::tab << txf::tab << "}" << txf::nl;
}	
	

inline void GenCommandFunctionHeader(
	const command_ &Command,
	txf::text_oflow___ &Flow )
{
	Flow << txf::tab << txf::tab << "bso__bool " << Command.Identification.Value << "(";
	GenCommandFunctionParameters( Command.Parameters, Flow );
	Flow << " )" << txf::nl;
	Flow << txf::tab << txf::tab << '{' << txf::nl;
}

inline void GenCommandFunctionBody(
	const parameters_ &Parameters,
	bso__ulong Indice,
	txf::text_oflow___ &Flow )
{
	GenCommandFunctionDeclarations( Flow );
	Flow << txf::nl;
	GenCommandFunctionsInstructions(  Parameters, Indice, Flow );
}

inline void GenCommandFunctionFooter( txf::text_oflow___ &Flow )
{
	Flow << txf::tab << txf::tab << txf::tab << "}" << txf::nl;
	Flow << txf::nl;
	Flow << txf::tab << txf::tab << txf::tab << "return State;" << txf::nl;
	Flow << txf::tab << txf::tab << "}" << txf::nl;
}	


//////////////////////////

void GenInitFunction(
	const commands_ &Commands,
	txf::text_oflow___ &Flow)
{
	GenInitFunctionHeader( Flow );
	GenInitFunctionBody( Commands, Flow );
	GenInitFunctionFooter( Flow );
}

void GenCommandFunction(
	const command_ &Command,
	bso__ulong Indice,
	txf::text_oflow___ &Flow )
{
	GenCommandFunctionHeader( Command, Flow );
	GenCommandFunctionBody( Command.Parameters, Indice, Flow );
	GenCommandFunctionFooter( Flow );
}



////////////////////////

void GenCommandFunctions(
	const commands_ &Commands,
	txf::text_oflow___ &Flow )
{
	CITEM( command_ ) Command;
	POSITION__ P = NONE;
	bso__ulong Indice = 0;
	
	Command.Init( Commands );

	P = Commands.First();

	while( P != NONE ) {
		GenCommandFunction( Command( P ), Indice++, Flow );
		P = Commands.Next( P );
	}

	fout << txf::nl;
}

////////////////////////////////////////////

inline void GenClassHeader(
	const type_ &Type,
	txf::text_oflow___ &Flow )
{
	Flow << txf::tab << "class ";
	Flow << Type.Identification.Value << txf::nl;
	Flow << txf::tab << "{" << txf::nl;
}

inline void GenClassBody(
	const type_ &Type,
	txf::text_oflow___ &Flow )
{
	Flow << txf::tab << txf::tab << "command__ Commands_[" << Type.Commands.Amount()<< "];" << txf::nl;
	GenInitFunction( Type.Commands, Flow );	
	GenCommandFunctions( Type.Commands, Flow );
}
	
inline void GenClassFooter(
	const type_ &Type,
	txf::text_oflow___ &Flow )
{
	Flow << txf::tab << "};" << txf::nl << txf::nl;
}

////////////////////////

inline void GenClass( 
	const type_ &Type,
	txf::text_oflow___ &Flow )
{
	GenClassHeader( Type, Flow );
	GenClassBody( Type, Flow );
	GenClassFooter( Type, Flow );
}

///////////////////////////
	
void GenAPI(
	const types &Types,
	txf::text_oflow___ &Flow )
{
	CITEM( type_ ) Type;
	POSITION__ P = Types.First();

	Flow << "#include \"frtend.h\"" << txf::nl << txf::nl;
	Flow << "namespace epeios {" << txf::nl;

	Type.Init( Types );

	while( P != NONE ) {
		GenClass( Type( P ), Flow );
		P = Types.Next( P );
	}
}

POSITION__ FindMasterType( const types_ &Types )
{
ERRProlog
	CITEM( type_ ) Type;
	POSITION__ P = NONE;
ERRBegin
	Type.Init( Types );
	
	P = Types.First();
	
	while( ( P != NONE ) && ( Type( P ).Identification.ID() != BROKER_MASTER_TYPE ) )
		P = Types.Next( P );
		
	if ( P == NONE )
		ERRb();
ERRErr
ERREnd
ERREpilog
}

void FilterMasterCommands( commands_ &Commands )
{
ERRProlog
	commands C;
	POSITION__ P;
	CITEM( command_ ) Command;
ERRBegin
	C.Init();
	
	Command.Init( Commands );
	
	P = Commands.First();
	
	while( P != NONE ) {
		if ( Command( P ).Identification.Value( 0 ) != '_' )
			C.Add( Command( P ) );
			
		P = Commands.Next( P );
	}
	
	
ERRErr
ERREnd
ERREpilog
}

void Essai( void )
{
ERRProlog
	frtend::frontend___ Frontend;
	manager Manager;
	types Types;
	sck::socket_ioflow___ Flow;
	POSITION__ P;
ERRBegin
	Types.Init();
#if 1
	Flow.Init( clt::Connect( "localhost:1234" ) );
	Frontend.Init( Flow );
#else
	Manager.Init();
	Frontend.Init( Manager.Process() );
#endif
	
	Analyze( Frontend, Types );

//	Print( Types);

	P = FindMasterType( Types );
	
	FilterMasterCommands( Types( P ).Commands );
	
	Types( P ).Identification.Value = "frontend";
	
	Types.Sync();

	GenAPI( Types, fout );
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << BRKANLTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Essai();
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise();
			break;
		}
	default:
		fout << txf::sync;
		ferr << "\nBad arguments.\n";
		fout << "Usage: " << BRKANLTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << BRKANLTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
