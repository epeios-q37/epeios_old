/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'xxx.cpp' template file V2.8 */
/* 
	See the 'ssnmng.h' file  for more details.
*/

#define SSNMNG__COMPILATION

#include "ssnmng.h"

#ifdef SSNMNG__LINE
#line 12
#endif

class ssnmngtutor
: public ttr_tutor
{
public:
	ssnmngtutor( void )
	: ttr_tutor( SSNMNG_NAME )
	{
#ifdef SSNMNG_DBG
		Version = SSNMNG_VERSION " (DBG)";
#else
		Version = SSNMNG_VERSION;
#endif
		Owner = SSNMNG_OWNER;
		Date = "26/05/2000";
	}
	virtual ~ssnmngtutor( void ){}
};

//  	Substitution is not applied to the rest.
/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

#define DIGITS "azertyuiopmlkjhgfdsqwxcvbnNBVCXWQSDFGHJKLMPOIUYTREZA9876543210"

void ssnmng__session_id::New( void )
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
	const SET_( ssnmng__session_id ) &T,
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

POSITION__ ssnmng_sessions_manager_::Open( void )
{
	POSITION__ P = LIST_::CreateEntry();
	ssnmng__session_id SessionID;
	ssnmng__chrono C;

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

POSITION__ ssnmng_sessions_manager_::Position( const char *SessionID ) const
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

/* end of 'xxx.cpp' template file */