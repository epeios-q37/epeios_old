/*
	'scltool' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'scltool' header file ('scltool.h').
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

#define SCLTOOL__COMPILATION

#include "scltool.h"

class scltooltutor
: public ttr_tutor
{
public:
	scltooltutor( void )
	: ttr_tutor( SCLTOOL_NAME )
	{
#ifdef SCLTOOL_DBG
		Version = SCLTOOL_VERSION "\b\bD $";
#else
		Version = SCLTOOL_VERSION;
#endif
		Owner = SCLTOOL_OWNER;
		Date = "$Date$";
	}
	virtual ~scltooltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "cio.h"

#include "sclrgstry.h"
#include "scllocale.h"
#include "sclmisc.h"

using namespace scltool;

int main(
	int argc,
	const char *argv[] )
{
ERRFProlog
ERRFBegin
	sclmisc::Initialize( TargetName, NULL );
	Main( argc, argv, scllocale::GetRack() );
ERRFErr
ERRFEnd
	sclmisc::Terminate();
ERRFEpilog
	return ERRExitValue;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class scltoolpersonnalization
: public scltooltutor
{
public:
	scltoolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~scltoolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static scltoolpersonnalization Tutor;

ttr_tutor &SCLTOOLTutor = Tutor;
