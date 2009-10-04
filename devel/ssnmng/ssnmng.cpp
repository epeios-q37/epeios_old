/*
	'ssnmng' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'ssnmng' header file ('ssnmng.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
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

#define SSNMNG__COMPILATION

#include "ssnmng.h"

class ssnmngtutor
: public ttr_tutor
{
public:
	ssnmngtutor( void )
	: ttr_tutor( SSNMNG_NAME )
	{
#ifdef SSNMNG_DBG
		Version = SSNMNG_VERSION "\b\bD $";
#else
		Version = SSNMNG_VERSION;
#endif
		Owner = SSNMNG_OWNER;
		Date = "$Date$";
	}
	virtual ~ssnmngtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#define DIGITS "azertyuiopmlkjhgfdsqwxcvbnNBVCXWQSDFGHJKLMPOIUYTREZA9876543210"

using namespace ssnmng;

void ssnmng::session_id__::New( void )
{
	int i;

	for( i = 0; i < SSNMNG_SIZE; i++ )
		Raw_[i] = DIGITS[rand() % ( sizeof( DIGITS ) - 1 )];

	Raw_[i] = 0;
}

static inline bso::sign__ Test_(
	const char *S1,
	const char *S2 )
{
	return strcmp( S1, S2 );
}

static bso::sign__ Search_(
	const bch::E_BUNCHt_( session_id__, row__ ) &T,
	const char *S,
	idxbtq::E_ISEEKERt__( row__ ) &Seeker )
{
	bso::sign__ Test;
	row__ Row = Seeker.GetCurrent();

	while ( ( Row != NONE )
			&& ( ( Test = Test_( T( Row ).Value(), S ) ) != 0 ) ) {
		switch( Test ) {
		case 1:
			Row = Seeker.SearchGreater();
			break;
		case -1:
			Row = Seeker.SearchLesser();
			break;
		default:
			ERRc();
			break;
		}
	}

	return Test;
}

row__ ssnmng::base_sessions_manager_::Open( void )
{
	row__ P = _list_::New();
	session_id__ SessionID;
	chrono__ C;

	do {
		SessionID.New();
	} while( Position( SessionID ) != NONE );

	Table.Store( SessionID, P );

	if ( S_.Root == NONE ) {
		S_.Root = P;
		_queue_::Create( P );
	} else {
		idxbtq::E_ISEEKERt__( row__ ) Seeker;

		Seeker.Init( Index, S_.Root );

		switch ( Search_( Table, SessionID.Value(), Seeker ) ) {
		case 1:
			S_.Root = Index.BecomeGreater( P, Seeker.GetCurrent(), S_.Root );
			break;
		case -1:
			S_.Root = Index.BecomeLesser( P, Seeker.GetCurrent(), S_.Root );
			break;
		default:
			ERRc();
			break;
		}

		_queue_::BecomeNext( P, _queue_::Tail() );
	}


	if ( time( &C.Relative ) == -1 )
		ERRs();

	if ( time( &C.Absolute ) == -1 )
		ERRs();

	C.AlwaysValid = false;

	Chronos.Store( C, P );

	return P;
}

row__ ssnmng::base_sessions_manager_::Position( const char *SessionID ) const
{
	if ( S_.Root != NONE )	{
		idxbtq::E_ISEEKERt__( row__ ) Seeker;

		Seeker.Init( Index, S_.Root );

		if ( Search_( Table, SessionID, Seeker ) == 0 )
			return Seeker.GetCurrent();
		else
			return NONE;
	}

	return NONE;
}

row__ ssnmng::base_sessions_manager_::Position( const str::string_ &SessionID ) const
{
	char Buffer[SSNMNG_SIZE+1];

	if ( SessionID.Amount() != SSNMNG_SIZE )
		return NONE;

	SessionID.Recall( 0, SSNMNG_SIZE, Buffer );

	Buffer[SSNMNG_SIZE] = 0;

	return Position( Buffer );
}

void ssnmng::base_sessions_manager_::GetExpired( rows_ &Expired ) const
{
	row__ Row = First();

	while ( Row != NONE ) {
		if ( IsExpired( Row ) )
			Expired.Append( Row );

		Row = Next( Row );
	}
}

void ssnmng::base_sessions_manager_::GetAll( rows_ &Rows ) const
{
	row__ Row = First();

	while ( Row != NONE ) {
		Rows.Append( Row );

		Row = Next( Row );
	}
}

void ssnmng::sessions_manager_::_Close( const rows_ &Rows )
{
	epeios::row__ Row = Rows.First();

	while ( Row != NONE ) {
		Close( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}


void ssnmng::sessions_manager_::CloseAll( void )
{
ERRProlog
	rows Rows;
ERRBegin
	Rows.Init();

	GetAll( Rows );

	_Close( Rows );
ERRErr
ERREnd
ERREpilog
}

void ssnmng::sessions_manager_::CloseExpired( void )
{
ERRProlog
	rows Rows;
ERRBegin
	Rows.Init();

	GetExpired( Rows );

	_Close( Rows );
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class ssnmngpersonnalization
: public ssnmngtutor
{
public:
	ssnmngpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ssnmngpersonnalization( void )
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

static ssnmngpersonnalization Tutor;

ttr_tutor &SSNMNGTutor = Tutor;
