/*
  Test source for the 'txmtbl' library by Claude SIMON (http://zeusw.org/intl/contact.html).
  Copyright (C) 2000,2001 Claude SIMON (http://zeusw.org/intl/contact.html).

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
  along with this program; if not, go to http://www.fsf.org/
  or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

#include "txmtbl.h"

#include "err.h"
#include "stf.h"
#include "fil.h"

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void Essai( void )
{
ERRProlog
	fil::file_iflow___ Flow;
	xtf::extended_text_iflow___ IFlow;
	txmtbl::table Table;
ERRBegin
//	Flow.Init( "essai.txt" );
	Flow.Init( "20010312.txt" );
	Flow.EOFT( XTF_EOXT );
	IFlow.Init( Flow );
	
	Table.Init();
	
	txmtbl::GetTable( IFlow, Table, ';' );
	
	fout << Table << txf::nl << "***************************" << txf::nl;

	Table.DeleteCommentaries( '#' );

	fout << Table << txf::nl << "***************************" << txf::nl;
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	fout << "Test of library " << TXMTBLTutor.Name << ' ' << __DATE__" "__TIME__"\n";

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
		fout << "Usage: " << TXMTBLTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	fout << "\nEnd of program " << TXMTBLTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
