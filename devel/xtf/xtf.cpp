/*
  'xtf' library by Claude SIMON (csimon@epeios.org)
  Requires the 'xtf' header file ('xtf.h').
  Copyright (C) 2000-2002 Claude SIMON (csimon@epeios.org).

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

#define XTF__COMPILATION

#include "xtf.h"

class xtftutor
: public ttr_tutor
{
public:
	xtftutor( void )
	: ttr_tutor( XTF_NAME )
	{
#ifdef XTF_DBG
		Version = XTF_VERSION "\b\bD $";
#else
		Version = XTF_VERSION;
#endif
		Owner = XTF_OWNER;
		Date = "$Date$";
	}
	virtual ~xtftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xtf;

void xtf::extended_text_iflow___::GetLine( str::string_ &Line )
{
	bso::bool__ Cont = true;
	char C;

	if ( !EOX() ) {
		C = Get();

		if ( ( C == '\r' ) || ( C == '\n' ) )
			if ( EOL_ != 0 )
				Cont = false;
			else if ( !EOX() )
				C = Get();
			else
				Cont = false;

		while( Cont ) {

			if ( Column() == 1 )
				Cont = false;
			else if ( &Line != NULL )
				Line.Add( C );

			if ( EOX() )
				Cont = false;

			if ( Cont == true )	
				C = Get();
		}
	}
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xtfpersonnalization
: public xtftutor
{
public:
	xtfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xtfpersonnalization( void )
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

static xtfpersonnalization Tutor;

ttr_tutor &XTFTutor = Tutor;
