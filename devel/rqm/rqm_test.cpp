/*
  Test source for the 'rqm' library by Claude SIMON (http://zeusw.org/intl/contact.html).
  Copyright (C) 2000 Claude SIMON (http://zeusw.org/intl/contact.html).

  This file is part of the Epeios (http://zeusw.org/epeios/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

#include "rqm.h"

using namespace rqm;

#include "err.h"
#include "stf.h"
#include "flx.h"

#define CHAINE1	"Ceci est une chaine de caracteres."
#define CHAINE2	"Ceci est une autre chaine de caracteres."
#define CHAINEP "Ceci est la chaine de caracteres pour le pointeur ..."

class tampon
: private flx::buffer_iflow___,
  private flx::buffer_oflow___
{
public:
	void Init( flw::data__ *Tampon, size_t Taille )
	{
		buffer_iflow___::Init( Tampon, Taille );
		buffer_oflow___::Init( Tampon, Taille );
	}
	flw::amount__ Get(
		flw::amount__ Minimum,
		flw::data__ *Buffer,
		flw::amount__ Wanted )
	{
		return buffer_iflow___::FLWGet( Minimum, Buffer, Wanted);
	}
	flw::amount__ Put(
		const flw::data__ *Buffer,
		flw::amount__ Wanted,
		flw::amount__ Minimum,
		bool Synchronization )
	{
		return buffer_oflow___::FLWPut( Buffer, Wanted, Minimum, Synchronization );
	}
};

class canal
: public flw::ioflow___
{
private:
	tampon Flot;
	flw::data__ Cache[100];
protected:
	virtual flw::amount__ FLWGet(
		flw::amount__ Minimum,
		flw::data__ *Buffer,
		flw::amount__ Wanted )
	{
		return Flot.Get( Minimum, Buffer, Wanted );
	}
	virtual flw::amount__ FLWPut(
		const flw::data__ *Buffer,
		flw::amount__ Wanted,
		flw::amount__ Minimum,
		bool Synchronization )
	{
		return Flot.Put( Buffer, Wanted, Minimum, Synchronization );
	}
public:
	void Init(
		flw::data__ *Tampon,
		size_t Taille )
	{
		Flot.Init( Tampon, Taille );
		ioflow___::Init( Cache, sizeof( Cache ) );
	}
	~canal( void )
	{
		ioflow___::Synchronize();
	}
};

void Essai( void )
{
ERRProlog
	flw::data__ Tampon[500];
	description D;
	canal R;
	bso__char E;
	str_string S1, S2, SP;
	bso__raw Raw;
	str_string * P;
	request_manager___ Requete;
	CONTAINER( str_string_ )C;
	command__ Cmd;
	object__ Obj;
	const rqm::cast Casts[] = { cString, cPointer, cChar,
			cString, cArray, cChar, cEnd, cByte, cEnd, cEnd };
	bso__bool Cont = true;
ERRBegin
	D.Init( "coucou", Casts );

	SP.Init();
	SP = CHAINEP;

	R.Init( Tampon, sizeof( Tampon ) );

	PutObject( (object__)1, R );
	PutCommand( (command__)2, R );
	AddValue( cString, &str_string( CHAINE1 ), R );
//	AddValue( cString, &SP, R );
	AddValue( cPointer, &SP, R );
	AddValue( cChar, "a", R );
	AddValue( cString, &str_string( CHAINE2 ), R );
	AddCast( cArray, R );

	for ( E = 'A'; E <= 'Z'; E++ )
		AddValue( cChar, &E, R );

	AddCast( cEnd, R );

	AddValue( cByte, "0", R );
/*
	AddCast( cEnd, R );

	R.Synchroniser();
*/
	Complete( R );

	S1.Init();
	S2.Init();

	C.Init();

	Requete.Init( R );
	Requete.SetDescription( D );

	flw::Get( R, Obj );
	flw::Get( R, Cmd );

	Requete.GetValue( cString, &S1 );
	fout << S1 << txf::nl;
	Requete.GetValue( cPointer, &P );
	fout << *(str_string *)P << txf::nl;
	Requete.GetValue( cChar, &E );
	fout << E << txf::nl;

	Requete.GetValue( cString, &S2 );

	fout << S2 << txf::nl;

	while( Cont ) {
		switch( Requete.GetValue( cChar, &E )  ) {
		case kArrayLast:
			Cont = false;
		case kArray:
			fout << E << ' ';
			break;
		default:
			ERRc();
		}
	};

	fout << txf::nl;

	Requete.GetValue( cByte, &Raw );
	fout << (char)Raw << txf::nl;

	Requete.Complete();

ERRErr
	// instructions à exécuter si erreur
ERREnd
	// instructions à exécuter, erreur ou non.
ERREpilog
}


void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << RQMTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		fout << "Usage: " << RQMTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << RQMTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
