/*
  'brkrqm' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'brkrqm' header file ('brkrqm.h').
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

#define BRKRQM__COMPILATION

#include "brkrqm.h"

class brkrqmtutor
: public ttr_tutor
{
public:
	brkrqmtutor( void )
	: ttr_tutor( BRKRQM_NAME )
	{
#ifdef BRKRQM_DBG
		Version = BRKRQM_VERSION " (DBG)";
#else
		Version = BRKRQM_VERSION;
#endif
		Owner = BRKRQM_OWNER;
		Date = "$Date$";
	}
	virtual ~brkrqmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace brkrqm;

void brkrqm::description_::Init(
	const char *Name,
	const cast *Casts )
{
	int i = 0;
	cast Cast = cInvalid;

	this->Name.Init();
	this->Casts.Init();

	this->Name = Name;

	do
	{
		Cast = Casts[i++];
		this->Casts.Add( Cast );
	} while ( Cast != cEnd );

	while ( ( Cast = Casts[i++] ) != cEnd ) {
		this->Casts.Add( Cast );
	}

#ifdef RQM_DBG
	if ( Array != 0 )
		ERRc();
#endif

}


POSITION__ brkrqm::descriptions_::Position( const description_ &Description ) const
{
	POSITION__ Position = CONTAINER_( description_ )::First();
ERRProlog
	CITEM( description_ ) D;
ERRBegin

	D.Init( *this );

	while( ( Position != NONE )
		    && ( ( D( Position ).Name != Description.Name )
		         || ( D( Position ).Casts != Description.Casts ) ) )
		Position = CONTAINER_( description_ )::Next( Position );
ERRErr
ERREnd
ERREpilog
	return Position;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class brkrqmpersonnalization
: public brkrqmtutor
{
public:
	brkrqmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~brkrqmpersonnalization( void )
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

static brkrqmpersonnalization Tutor;

ttr_tutor &BRKRQMTutor = Tutor;
