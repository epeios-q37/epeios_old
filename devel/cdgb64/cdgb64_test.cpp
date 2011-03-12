/*
	Test source for the 'cdgb64' library by Claude SIMON (csimon at zeusw dot org).
	Copyright (C) 20112004 Claude SIMON.

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

#include "cdgb64.h"
#include "crptgr.h"

#include "err.h"
#include "cio.h"
#include "str.h"
#include "flx.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

#define KEY	"jhgkjgghgjkghkjghj"

void Encode(
	const str::string_ &Source,
	str::string_ &Target )
{
ERRProlog
	flx::E_STRING_OFLOW___ OFlow;
	cdgb64::encoding_oflow___ Encoder;
	crptgr::encrypt_oflow___ Encrypter;
	epeios::row__ Row = NONE;
ERRBegin
	OFlow.Init( Target );
	Encoder.Init( OFlow );
	Encrypter.Init( Encoder, KEY );

	Row = Source.First();

	while ( Row != NONE ) {
		Encrypter.Put( Source( Row ) );

		Row = Source.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

void Decode(
	bso::size__ Amount,
	const str::string_ &Source,
	str::string_ &Target )
{
ERRProlog
	flx::E_STRING_IFLOW__ IFlow;
	crptgr::decrypt_iflow___ Decrypter;
	cdgb64::decoding_iflow___ Decoder;
ERRBegin
	IFlow.Init( Source );
	Decoder.Init( IFlow );
	Decrypter.Init( Decoder, KEY );

	while ( Amount-- )
		Target.Append( Decrypter.Get() );
ERRErr
ERREnd
ERREpilog
}

void EncodeDecode(
	const str::string_ &Source,
	str::string_ &Target )
{
ERRProlog
	str::string Coded;
ERRBegin
	Coded.Init();

	Encode( Source, Coded );

	cout << "Encodé : " << Coded << txf::nl;

	Decode( Source.Amount(),Coded, Target );
ERRErr
ERREnd
ERREpilog
}

//#define TEXT	"!"
//#define TEXT	"!"
#define TEXT	"Hello, the world, and thank you for all the fish (42) !"

void Test( void )
{
ERRProlog
	str::string Source,	Target;
ERRBegin
	Source.Init( TEXT );
	Target.Init();

	cout << "Source : '" << Source << '\'' << txf::nl;

	EncodeDecode( Source, Target );

	cout << "Target : '" << Target << '\'' << txf::nl << txf::commit;
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << CDGB64Tutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Test();
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
		cout << "Usage: " << CDGB64Tutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
