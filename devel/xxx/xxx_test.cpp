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
#include "stf.h"

using stf::cin;
using stf::cout;
using stf::cerr;

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
	cout << "Test of library " << $XXX$Tutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise();
			break;
		}
	default:
		cout << txf::sync;
		cerr << "\nBad arguments.\n";
		cout << "Usage: " << $XXX$Tutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
	ExitCode = EXIT_FAILURE;
ERRFEnd
	cout << "\nEnd of program " << $XXX$Tutor.Name << ".\n";
ERRFEpilog
	return ExitCode;
}
