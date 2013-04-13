/*
	'fblber' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblber' header file ('fblber.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#define FBLBER__COMPILATION

#include "fblber.h"

class fblbertutor
: public ttr_tutor
{
public:
	fblbertutor( void )
	: ttr_tutor( FBLBER_NAME )
	{
#ifdef FBLBER_DBG
		Version = FBLBER_VERSION "\b\bD $";
#else
		Version = FBLBER_VERSION;
#endif
		Owner = FBLBER_OWNER;
		Date = "$Date$";
	}
	virtual ~fblbertutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fblber;

static inline void *GetPointer_( flw::iflow__ &Flow )
{
	void *P = NULL;

	flw::Get( Flow, P );

	return P;
}

void fblber::embed_request_functions___::_GetAll(
	flw::iflow__ &Flow,
	const casts_ &Casts )
{
	sdr::row__ Row = NONE;

	Row = Casts.First();

	while ( ( Row != NONE )
		    && ( Casts( Row ) != cEnd ) ) {

		if ( Flow.Get() != *Casts( Row ) )
			ERRFwk();

		if ( Repository_.Append( GetPointer_( Flow ) ) != Row )
			ERRFwk();

		Row = Casts.Next( Row );
	}

	if ( Row == NONE )
		ERRFwk();

	if ( Flow.Get() != fblcst::cEnd )
		ERRFwk();

	if ( Repository_.Append( (void *)NULL ) != Row )
		ERRFwk();

	Row = Casts.Next( Row );

	while ( Row != NONE ) {
		if ( Flow.Get() != *Casts( Row ) )
			ERRFwk();

		if ( Repository_.Append( GetPointer_( Flow ) ) != Row )
			ERRFwk();

		Row = Casts.Next( Row );
	}
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblberpersonnalization
: public fblbertutor
{
public:
	fblberpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblberpersonnalization( void )
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

static fblberpersonnalization Tutor;

ttr_tutor &FBLBERTutor = Tutor;
