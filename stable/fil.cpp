/*
	'fil' library by Claude SIMON (csimon@epeios.org)
	Requires the 'fil' header file ('fil.h').
	Copyright (C) 2000-2001, 2003 Claude SIMON (csimon@epeios.org).

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

#define FIL__COMPILATION

#include "fil.h"

class filtutor
: public ttr_tutor
{
public:
	filtutor( void )
	: ttr_tutor( FIL_NAME )
	{
#ifdef FIL_DBG
		Version = FIL_VERSION "\b\bD $";
#else
		Version = FIL_VERSION;
#endif
		Owner = FIL_OWNER;
		Date = "$Date$";
	}
	virtual ~filtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "fnm.h"
#include "cpe.h"

using namespace fil;

status fil::file_iflow___::Init(
	const char *FileName,
	err::handle ErrHandle )
{
	status Status = sSuccess;

	Stream_.close();
	Stream_.clear();

	istream_iflow___::Init();

	Stream_.open( FileName, std::ios::binary | std::ios::in );

	if ( !Stream_ )
	{
		if ( ErrHandle == err::hUsual )
			ERRd();
		else if ( ErrHandle != err::hSkip )
			ERRu();

		Status = sFailure;
	}

	return Status;
}


status fil::file_oflow___::Init(
	const char *FileName,
	mode Mode,
	err::handle ErrHandle )
{
	status Status = sSuccess;

	Stream_.close();
	Stream_.clear();

	ostream_oflow___::Init();

	if ( Mode == fil::mAppend )
		Stream_.open( FileName, std::ios::binary | std::ios::out | std::ios::app  );
	else
		Stream_.open( FileName, std::ios::binary | std::ios::out );

	if ( Stream_.fail() )
	{
		if ( ErrHandle == err::hUsual )
			ERRd();
		else if ( ErrHandle != err::hSkip )
			ERRu();

		Status = sFailure;
	}
	return Status;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class filpersonnalization
: public filtutor
{
public:
	filpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~filpersonnalization( void )
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

static filpersonnalization Tutor;

ttr_tutor &FILTutor = Tutor;
