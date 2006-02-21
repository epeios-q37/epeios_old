/*
	'flw' library by Claude SIMON (csimon@epeios.org)
	Requires the 'flw' header file ('flw.h').
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

#define FLW__COMPILATION

#include "flw.h"

class flwtutor
: public ttr_tutor
{
public:
	flwtutor( void )
	: ttr_tutor( FLW_NAME )
	{
#ifdef FLW_DBG
		Version = FLW_VERSION "\b\bD $";
#else
		Version = FLW_VERSION;
#endif
		Owner = FLW_OWNER;
		Date = "$Date$";
	}
	virtual ~flwtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#ifdef FLW__IGNORE_SIGPIPE
#	include <signal.h>
#endif

using namespace flw;

void flw::oflow__::_Write(
	const datum__ *Buffer,
	bsize__ Amount )
{
	bsize__ AmountWritten = _WriteUpTo( Buffer, Amount );

	while( AmountWritten < Amount )
		AmountWritten += _WriteUpTo( Buffer + AmountWritten, Amount - AmountWritten );
}

bool flw::GetString(
	iflow__ &Flot,
	char *Tampon,
	size__ NombreMax )
{
	size__ Position = 0;

	while( ( Position < NombreMax ) && ( ( Tampon[Position] = (char)Flot.Get() ) != 0 ) )
		Position++;

	return Position < NombreMax;
}


bsize__ flw::iflow__::_Read(
	bsize__ Minimum,
	datum__ *Buffer,
	bsize__ Wanted )
{
	bsize__ Amount = 0;
ERRProlog
ERRBegin
	if ( _Size == 0 )	// There was an error before. See below, in 'ERRErr'.
		ERRd();

	Amount = _Functions.Read( Minimum, Buffer, Wanted );

	_Red += Amount;

	if ( _Red > _AmountMax )
		ERRf();
ERRErr
	_Size = _Available = 0;	// To avoid further reading from cache. Next reading will generate an error. 
ERREnd
ERREpilog
	return Amount;
}

bsize__ flw::oflow__::_DirectWrite(
	const datum__ *Buffer,
	bsize__ Wanted,
	bsize__ Minimum )
{
	bsize__ Amount = 0;
ERRProlog
ERRBegin
	if ( ( Size_ == 0 ) && ( Cache_ != NULL ) )	// There was an error before. See below, in 'ERRErr'.
		ERRd();

#ifdef FLW_DBG
		if ( Wanted < Minimum )
			ERRu();
#endif

	Amount = _Functions.Write( Buffer, Wanted, Minimum );

#ifdef FLW_DBG
		if ( Amount > Wanted )
			ERRu();

		if ( Amount < Minimum )
			ERRu();
#endif

	Written_ += Amount;

	if ( Written_ >= AmountMax_ )
		ERRf();
ERRErr
	Size_ = Free_ = 0;	// To avoid further writing in cache. Next writing will generate an error. 
	Synchronize();	// N'écrit rien (à priori) ; juste pour déverouiiler.
ERREnd
ERREpilog
	return Amount;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class flwpersonnalization
: public flwtutor
{
public:
	flwpersonnalization( void )
	{
#ifdef FLW__IGNORE_SIGPIPE
//		signal( SIGPIPE, SIG_IGN );	// Witout this, an 'broken pipe' would terminate the program.
#endif	
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~flwpersonnalization( void )
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

static flwpersonnalization Tutor;

ttr_tutor &FLWTutor = Tutor;
