/*
	Test source for the '$xxx$' library by $AUTHOR_NAME$ ($AUTHOR_EMAIL$).
	Copyright (C) $COPYRIGHT_DATES$$COPYRIGHT_OWNER$.

$NOTICE$
*/

//	$$Id$$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "$xxx$.h"

#include "err.h"
#include "cio.h"

using cio::CIn;
using cio::COut;
using cio::CErr;

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
ERRFProlog
ERRFBegin
	COut << "Test of library " << $XXX$Tutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( COut );
			break;
		}
	default:
		COut << txf::commit;
		CErr << "\nBad arguments.\n";
		COut << "Usage: " << $XXX$Tutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
