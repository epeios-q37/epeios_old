/*
  'flw' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'flw' header file ('flw.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the:
  
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
		Version = FLW_VERSION " (DBG)";
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

using namespace flw;

void flw::oflow___::ForceWriting_(
	const data__ *Buffer,
	amount__ Amount )
{
	amount__ AmountWritten = PutUpTo( Buffer, Amount );

	while( AmountWritten < Amount )
		AmountWritten += PutUpTo( Buffer + AmountWritten, Amount - AmountWritten );
}

void flw::oflow___::ForceDumpingOfCache_( bool Synchronisation )
{
	while( !DumpCache_( Synchronisation ) );
}


bool flw::GetString(
	iflow___ &Flot,
	char *Tampon,
	size__ NombreMax )
{
	size_t Position = 0;

	while( ( Position < NombreMax ) && ( ( Tampon[Position] = Flot.Get() ) != 0 ) )
		Position++;

	return Position < NombreMax;
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class flwpersonnalization
: public flwtutor
{
public:
	flwpersonnalization( void )
	{
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
