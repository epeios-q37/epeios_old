/*
  'ssnmng' library by Claude L. Simon (simon@epeios.org)
  Requires the 'ssnmng' header file ('ssnmng.h').
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org).

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

namespace ssnmng {

	void session_id__::New( void )
	{
		int i;

		for( i = 0; i < SSNMNG_SIZE; i++ )
			Raw_[i] = DIGITS[rand() % sizeof( DIGITS )];

		Raw_[i] = 0;
	}

	static inline bso__sbyte Test_(
		const char *S1,
		const char *S2 )
	{
		return strcmp( S1, S2 );
	}

	static POSITION__ Search_( 
		const idxbtr_tree_index_ &I,
		const SET_( session_id__ ) &T,
		const char *S,
		bso__sbyte &Test )
	{
		POSITION__ P = I.Root();
		bso__bool Cont = true;

		while ( Cont )
			if ( ( Test = Test_( T( P ).Value(), S ) ) > 0 )
				Cont = !I.NextAvailable( P );
			else if ( Test < 0 )
				Cont = !I.PreviousAvailable( P );
			else
				Cont = false;

		return P;
	}

	POSITION__ sessions_manager_::Open( void )
	{
		POSITION__ P = LIST_::CreateEntry();
		session_id__ SessionID;
		chrono__ C;

		do {
			SessionID.New();
		} while( Position( SessionID ) != NONE );

		Table.Write( SessionID, P );

		if ( Index.IsEmpty() ) {
			MQUEUE_::Create( P );
			Index.Create( P );
		} else {
			bso__sbyte Test;
			POSITION__ PI = Search_( Index, Table, SessionID.Value(), Test );

			if ( Test > 0 )
				Index.BecomeNext( P, PI );
			else if ( Test < 0 )
				Index.BecomePrevious( P, PI );
			else
				ERRc();

			MQUEUE_::InsertItemAfterNode( P, MQUEUE_::Tail() );
		}


		if ( time( &C.Relative ) == -1 )
			ERRs();

		if ( time( &C.Absolute ) == -1 )
			ERRs();

		Chronos.Write( C, P );

		return P;
	}

	POSITION__ sessions_manager_::Position( const char *SessionID ) const
	{
		POSITION__ P = NONE;

		if ( !Index.IsEmpty() )	{
			bso__sbyte Test;

			P = Search_( Index, Table, SessionID, Test );

			if ( Test )
				P = NONE;
		}

		return P;
	}
	POSITION__ sessions_manager_::Position( const str_string_ &SessionID ) const
	{
		char Buffer[SSNMNG_SIZE+1];

		if ( SessionID.Amount() != SSNMNG_SIZE )
			return NONE;

		SessionID.Read( 0, SSNMNG_SIZE, Buffer );

		Buffer[SSNMNG_SIZE] = 0;

		return Position( Buffer );
	}
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
