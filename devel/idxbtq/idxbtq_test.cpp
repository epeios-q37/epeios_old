/*
	Test source for the 'idxbtq' library by Claude SIMON (csimon@epeios.org).
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

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

#include "idxbtq.h"

#include "err.h"
#include "cio.h"

using cio::cin;
using cio::cout;
using cio::cerr;

void DisplayTree(
	const idxbtr::E_IBTREE_ &Index,
	epeios::row__ Root )
{
	epeios::row__ Row = Index.First( Root );

	while ( Row != NONE ) {
		cout << *Row << txf::tab << txf::sync;

		Row = Index.Next( Row );
	}

	cout << txf::nl;

	Index.PrintStructure( cout, Root );

}

void DisplayQueue(
	const idxque::E_IQUEUE_ &Index,
	epeios::row__ Root )
{
	epeios::row__ Row = Root;

	while ( Root != NONE ) {
		Row = Root;

		Root = Index.Previous( Root );
	}

	while ( Row != NONE ) {
		cout << *Row << txf::tab << txf::sync;

		Row = Index.Next( Row );
	}

	cout << txf::nl;
}


void Generic( int argc, char *argv[] )
{
ERRProlog
	idxbtq::E_INDEX Index;
	epeios::row__ R0 = 0, R1 = 1, R2 = 2, R3 = 3, Root = R0;
	bso::ubyte__ Dummy;
ERRBegin
	Index.Init();

	Index.Allocate( 4 );

	
	Root = Index.BecomeLesser( R1, Root, Root );

	Root = Index.BecomeGreater( R2, Index.BaseTree.SearchMostLeftNode( Root, Dummy ), Root );

	Root = Index.BecomeGreater( R3, Index.BaseTree.SearchMostRightNode( Root, Dummy ), Root );

	DisplayQueue( Index, Root );

	DisplayTree( Index, Root );
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << IDXBTQTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		cout << "Usage: " << IDXBTQTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
