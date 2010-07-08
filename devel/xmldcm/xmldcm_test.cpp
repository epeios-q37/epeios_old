/*
  Test source for the 'xmldcm' library by Claude SIMON (http://zeusw.org/intl/contact.html).
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

#include "xmldcm.h"

#include "err.h"
#include "stf.h"

void Generic( int argc, char *argv[] )
{
ERRProlog
ERRBegin
ERRErr
ERREnd
ERREpilog
}

void Test( int argc, char *argv[] )
{
ERRProlog
	xmldcm::document D;
	xmldcm::document DD;
	xmldcm::document_filler__ DF;
ERRBegin
	D.Init();
	DF.Init( D );

	DF.PushTag( "root" );
	DF.PushTag( "sub" );
	DF.PutAttribute( "Attr", "Value" );
	DF.PutValue( "El Val" );
	DD.Init();
	DD = D;
	DF.Put( DD );
	DF.PopTag();
	DF.PopTag();

	xmldcm::WriteXML( D, stf::fout );
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
	int ExitCode = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
stf::fout << "Test of library " << XMLDCMTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		Test( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise();
			break;
		}
	default:
		stf::fout << txf::sync;
		stf::ferr << "\nBad arguments.\n";
		stf::fout << "Usage: " << XMLDCMTutor.Name << " [/i]\n\n";
		ERRt();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	stf::fout << "\nEnd of program " << XMLDCMTutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
