/*
	Test source for the 'dir' library by Claude SIMON (csimon@epeios.org).
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#include "dir.h"

#include "err.h"
#include "cio.h"
#include "fil.h"

using cio::cin;
using cio::cout;
using cio::cerr;

#define PATH	"c:"

void Generic( int argc, char *argv[] )
{
ERRProlog
	dir::handle___ Handle = NULL;
	const char *Name = NULL;
	char LocalizedName[MAX_PATH];
ERRBegin
/*	if ( dir::CreateDir( "coucou" ) != dir::sOK )
		ERRu();
*/

	Name = dir::GetFirstFile( PATH, Handle );

	while ( ( Name != NULL ) && *Name ) {

		sprintf( LocalizedName, "%s\\%s", PATH, Name );

		cio::cout << '"' << LocalizedName << "\" : ";

		if ( fil::IsDirectory( LocalizedName ) )
			cout << "Directory";
		else if ( fil::IsFile( LocalizedName ) )
			cout << "File";
		else
			cout << "???";

		cout << txf::nl;

		Name = dir::GetNextFile( Handle );
	}
		
ERRErr
ERREnd
	if ( Handle != NULL )
		dir::Close( Handle );
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << DIRTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( cout );
			break;
		}
	default:
		cout << txf::sync;
		cerr << "\nBad arguments.\n";
		cout << "Usage: " << DIRTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
