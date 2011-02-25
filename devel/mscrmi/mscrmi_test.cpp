/*
	Test source for the 'mscrmi' library by Claude SIMON (csimon at zeusw dot org).
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
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

//	$Id$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpe.h"

#ifdef CPE__T_LINUX
#	define LINUX
#endif

#include "mscrmi.h"
#ifndef LINUX
#	include "mscmdd.h"
#endif
#include "mscmdm.h"

#include "err.h"
#include "cio.h"
#include "xpp.h"
#include "lcl.h"

cio::cin___ cin;
cio::cout___ cout;
cio::cerr___ cerr;

#define FILE	"test.xml"

void PrintAddress( mscrmi::address__ Address )
{
	char AddressBuffer[] = "12345678";

	sprintf( AddressBuffer, "%08lX", Address );
	cout << AddressBuffer;
}

static inline mscrmi::xaddress__ Sum_(
	mscrmi::xaddress__ Address,
	mscrmi::size__ Size )
{
	bso::ubyte__ StencilSize = mscrmi::_StencilSize( Address );

	Address = mscrmi::_Address( Address );

	if ( ( Address & 0x80808080 ) != 0 )
		ERRc();

	Address = ( Address & 0x7f ) + ( ( Address & 0x7f00 ) >> 1 ) + ( ( Address & 0x7f0000 ) >> 2 ) + ( ( Address & 0x7f000000 ) >> 3 ) + Size;

	Address = ( Address & 0x7f ) + ( ( Address & 0x3f80) << 1 ) + ( ( Address & 0x1fc000 ) << 2 ) + ( ( Address & 0xfe00000 ) << 3 );

	Address = mscrmi::_SetStencilSize( Address, StencilSize );

	return Address;
}


void Print( const mscrmi::blocs_ &Blocs )
{
	epeios::row__ Row = Blocs.First();

	while ( Row != NONE ) {
		PrintAddress( Blocs( Row ).Address );
		cout << " - ";

		PrintAddress( Sum_( Blocs( Row ).Address, Blocs( Row ).Size -1 ) );
		cout << txf::nl;

		Row = Blocs.Next( Row );
	}
}

void GetBlocs( const mscrmi::midi_implementations_ &Implementations )
{
ERRProlog
	ctn::E_CITEM( mscrmi::midi_implementation_ ) Implementation;
	epeios::row__ Row = NONE;
	mscrmi::blocs Blocs;
ERRBegin
	Implementation.Init( Implementations );

	Row = Implementations.First();

	while ( Row != NONE ) {
		Blocs.Init();
		mscrmi::GetBlocs( Implementation( Row ).Definitions, Blocs );

		Print( Blocs );

		cout << txf::nl;

		Row = Implementations.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void ReadMI( void )
{
ERRProlog
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	xpp::preprocessing_iflow___ XPPFlow;
	xml::browser___ Browser;
	mscrmi::midi_implementations Implementations;
	mscrmi::fill_status__ Status = mscrmi::fs_Undefined;
	lcl::locale Locale;
	str::string Message;
	xml::writer Writer;
	mscrmi::blocs Blocs;
ERRBegin
	FFlow.Init( FILE );
	FFlow.EOFD( XTF_EOXT );
	XPPFlow.Init( FFlow, str::string( "" ) );
	XFlow.Init( XPPFlow );
	Browser.Init( XFlow, xml::eh_Default );

	Implementations.Init();

	Status = mscrmi::Fill( Browser, Implementations );

	switch ( Status ) {
	case mscrmi::fsOK:
		break;
	case mscrmi::fsBrowserError:
		if ( Browser.Status() == xml::sOK )
			ERRc();

		Message.Init();
		Locale.Init();

		xpp::GetTranslation( XPPFlow.Status(), str::string(""), Locale, Message );
		cerr << "Error : " << xpp::GetLabel( XPPFlow.Status() ) << " at " << XPPFlow.Coord().Line << " , " << XPPFlow.Coord().Column << txf::nl;
		ERRReturn;
		break;
	default:
		cerr << "Error : " << mscrmi::Label( Status ) << " at " << XPPFlow.Coord().Line << " , " << XPPFlow.Coord().Column << txf::nl;
		ERRReturn;
		break;
	}

	Writer.Init( cout, xml::oIndent );
	Writer.PushTag( "MIDIImplementations" );
	mscrmi::Print( Implementations, Writer );
	Writer.PopTag();

	Blocs.Init();

	GetBlocs( Implementations );

ERRErr
ERREnd
ERREpilog
}

void ParseImplementation( mscrmi::midi_implementation_ &Implementation )
{
ERRProlog
	flf::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	xpp::preprocessing_iflow___ XPPFlow;
	xml::browser___ Browser;
	mscrmi::fill_status__ Status = mscrmi::fs_Undefined;
	lcl::locale Locale;
	str::string Message;
ERRBegin
	FFlow.Init( "V-Piano.xml" );
	FFlow.EOFD( XTF_EOXT );
	XPPFlow.Init( FFlow, str::string( "" ) );
	XFlow.Init( XPPFlow );
	Browser.Init( XFlow, xml::eh_Default );

	Status = mscrmi::Fill( Browser, Implementation );

	switch ( Status ) {
	case mscrmi::fsOK:
		break;
	case mscrmi::fsBrowserError:
		if ( Browser.Status() == xml::sOK )
			ERRc();

		Message.Init();
		Locale.Init();

		xpp::GetTranslation( XPPFlow.Status(), str::string(""), Locale, Message );
		cerr << "Error : " << xpp::GetLabel( XPPFlow.Status() ) << " at " << XPPFlow.Coord().Line << " , " << XPPFlow.Coord().Column << txf::nl;
		ERRReturn;
		break;
	default:
		cerr << "Error : " << mscrmi::Label( Status ) << " at " << XPPFlow.Coord().Line << " , " << XPPFlow.Coord().Column << txf::nl;
		ERRReturn;
		break;
	}
ERRErr
ERREnd
ERREpilog
}

void RetrieveDataSet(
	const mscrmi::blocs_ &Blocs,
	const str::string_ &ModelID,
	mscrmi::adata_set_ &DataSet )
{
ERRProlog
#ifdef LINUX
	flf::file_iflow___ IFlow;
	flf::file_oflow___ OFlow;
#else
	mscmdd::midi_iflow___<> IFlow;
	mscmdd::midi_oflow___<> OFlow;
#endif
	mscrmi::adata Data;
ERRBegin
#ifdef LINUX
	IFlow.Init( "/dev/midi1" );
	OFlow.Init( "/dev/midi1", fil::mAppend );
#else
	OFlow.Init( 5 );
	IFlow.Init( 4 );
	IFlow.Start();
#endif
	Data.Init();
	mscrmi::RetrieveDataSet( OFlow, IFlow, Blocs, ModelID, DataSet );
ERRErr
ERREnd
ERREpilog
}

void Print(
	const mscrmi::parameters_ &Parameters,
	const mscrmi::midi_implementation_ &Implementation )
{
ERRProlog
	xml::writer Writer;
ERRBegin
	Writer.Init( cout, xml::oIndent );
	Writer.PushTag( "AllParameters" );
	mscrmi::Print( Parameters, Implementation, Writer );
	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

void RetrieveAll( void )
{
ERRProlog
	mscrmi::midi_implementation Implementation;
	mscrmi::blocs Blocs;
	mscrmi::adata_set DataSet;
	mscrmi::parameters Parameters;
ERRBegin
	Implementation.Init();
	ParseImplementation( Implementation );

	Blocs.Init();
	mscrmi::GetBlocs( Implementation, Blocs );

	DataSet.Init();
	RetrieveDataSet( Blocs, Implementation.ModelID, DataSet );

	Parameters.Init();
	mscrmi::Fill( DataSet, Implementation, Parameters );

	Print( Parameters, Implementation );

ERRErr
ERREnd
ERREpilog
}

void Communicate( void )
{
ERRProlog
#ifdef LINUX
	flf::file_iflow___ IFlow;
	flf::file_oflow___ OFlow;
#else
	mscmdd::midi_oflow___<> OFlow;
	mscmdd::midi_iflow___<> IFlow;
#endif
	mscrmi::adata Data;
ERRBegin
#ifdef LINUX
	IFlow.Init( "/dev/midi1" );
	OFlow.Init( "/dev/midi1", fil::mAppend );
#else
	OFlow.Init( 5 );
	IFlow.Init( 4 );
	IFlow.Start();
#endif
	Data.Init();
	mscrmi::RetrieveData( OFlow, IFlow, 0x03000000, 20, str::string( "\x0\x0\x39", 3 ), Data );
ERRErr
ERREnd
ERREpilog
}

void RetrieveSettings(
	const mscrmi::midi_implementation_ &Implementation,
	mscrmi::adata_set_ &DataSet )
{
ERRProlog
	flf::file_iflow___ Flow;
	xtf::extended_text_iflow__ XFlow;
	xml::browser___ Browser;
ERRBegin
	Flow.Init( "Settings.xml" );
	XFlow.Init( Flow );
	Browser.Init( XFlow, xml::eh_Default );

	mscrmi::Fill( Browser, Implementation, DataSet );
ERRErr
ERREnd
ERREpilog
}

void Send(
	const mscrmi::adata_set_ &DataSet,
	const mscrmi::midi_implementation_ &Implementation )
{
ERRProlog
	mscmdd::midi_oflow___<> Flow;
ERRBegin
	Flow.Init( 5 );
	mscrmi::SendData( Implementation.ModelID, DataSet, Flow );
ERRErr
ERREnd
ERREpilog
}

void Send( void )
{
ERRProlog
	mscrmi::midi_implementation Implementation;
	mscrmi::adata_set DataSet;
ERRBegin
	Implementation.Init();
	ParseImplementation( Implementation );

	DataSet.Init();

	RetrieveSettings( Implementation, DataSet );

	Send( DataSet, Implementation );
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cin.Init();
	cout.Init();
	cerr.Init();
	cout << "Test of library " << MSCRMITutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
//		Communicate();
//		ReadMI();
//		RetrieveAll();
		Send();
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( cout );
			break;
		}
	default:
		cout << txf::commit;
		cerr << "\nBad arguments.\n";
		cout << "Usage: " << MSCRMITutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
	cout << txf::commit;
	cerr << txf::commit;
ERRFEpilog
	return ERRExitValue;
}
