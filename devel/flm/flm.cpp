/*
	'flm' library by Claude SIMON (csimon@epeios.org)
	Requires the 'flm' header file ('flm.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

#define FLM__COMPILATION

#include "flm.h"

class flmtutor
: public ttr_tutor
{
public:
	flmtutor( void )
	: ttr_tutor( FLM_NAME )
	{
#ifdef FLM_DBG
		Version = FLM_VERSION "\b\bD $";
#else
		Version = FLM_VERSION;
#endif
		Owner = FLM_OWNER;
		Date = "$Date$";
	}
	virtual ~flmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "lstbch.h"
#include "que.h"
#include "bch.h"

#ifdef CPE__T_MT
#	define MT
#endif

#ifdef MT
#	include "mtx.h"
#	include "mtk.h"
#endif

using namespace flm;

epeios::size__ flm::MaxFileAmount = FLM__MAX_FILE_AMOUNT;

#ifdef MT
static mtx::mutex_handler__ Mutex_;
#endif

static inline void Lock_( void )
{
#ifdef MT
	mtx::Lock( Mutex_ );
#endif
}

static inline void Unlock_( void )
{
#ifdef MT
	mtx::Unlock( Mutex_ );
#endif
}

static inline bso::bool__ IsLocked_( void )
{
#ifdef MT
	return mtx::IsLocked( Mutex_ );
#else
	return true;
#endif
}

struct _data__ {
	bso::bool__ ToFlush;	// Doit être 'flushé' si à 'true'.
	memoire_fichier_base___ *File;
	id__ ID;
	void reset( bso::bool__ = true )
	{
		ToFlush = false;
		File = NULL;
		ID = FLM_UNDEFINED_ID;
	}
};

static lstbch::E_LBUNCHt( _data__, row__ ) List_;
static que::E_MQUEUEt( row__ ) Queue_;

typedef ids::E_IDS_STORE_( id__ )	_ids_;
E_AUTO( _ids );

static _ids	IDs_;

id__ flm::GetId( void )
{
	id__ ID = FLM_UNDEFINED_ID;

	Lock_();

	ID = IDs_.New();

	Unlock_();

	return ID;
}

void flm::ReleaseId( id__ ID )
{
	Lock_();

	IDs_.Release( ID );

	Unlock_();
}

row__ flm::_Register(
	memoire_fichier_base___ &MFB,
	id__ ID )
{
	row__ Row = NONE;
	_data__ Data = { false, &MFB, ID };

	Lock_();

	Row = List_.New();

	if ( Queue_.Amount() < List_.Extent() )	// On teste 'Amount' parce que ce qui est entre 'Amount' et 'Extent' n'est pas initialisé dans la file.
		Queue_.Allocate( List_.Extent() );

	List_.Store( Data, Row );

	Unlock_();

	return Row;
}

void flm::_Unregister(
	row__ Row,
	id__ ID )
{
	_data__ Data;

	Lock_();

	if ( List_( Row ).ID != ID )
		ERRu();

	List_.Store( Data, Row );
	List_.Delete( Row );

	if ( Queue_.IsMember( Row ) )
		Queue_.Delete( Row );

	Unlock_();
}

#define DELAY	1000	// en ms.

struct _flusher_data__
{
	row__ Row;
	time_t LastFileAccessTime;
	 _flusher_data__( void )
	{
		Row = NONE;
		LastFileAccessTime = tol::Clock( false );
	}
} FlusherData_;

static inline void Flusher_( void * )
{
	Lock_();

	_data__ Data;

	while ( FlusherData_.Row != NONE ) {

		Unlock_();
		tht::Defer();
		Lock_();

		while ( ( tol::Clock( false ) - FlusherData_.LastFileAccessTime ) < DELAY ) {
			Unlock_();
			tht::Defer( DELAY );
			Lock_();
		}

		List_.Recall( FlusherData_.Row, Data );

		if ( Data.ToFlush ) {
			Data.File->Flush();

			Data.ToFlush = false;

			List_.Store( Data, FlusherData_.Row );
		}

		FlusherData_.Row = Queue_.Previous( FlusherData_.Row );
	}

	Unlock_();
}

static void LaunchFlusher_( void )
{
#ifdef FLM_DBG
	if ( !IsLocked_() )
		ERRc();
#endif
	if ( FlusherData_.Row == NONE )
		mtk::Launch( Flusher_, NULL );

	FlusherData_.Row = Queue_.Last();
}


void flm::_ReportFileUsing(
	row__ Row,
	bso::bool__ ToFlush )
{
	Lock_();

	if ( ToFlush ) {
		_data__ Data;

		List_.Recall( Row, Data );

		if ( !Data.ToFlush ) {
			Data.ToFlush = true;
			List_.Store( Data, Row );
		}

		LaunchFlusher_();
	}

	if ( Queue_.IsMember( Row ) )
		Queue_.Delete( Row );
	else if ( Queue_.Amount() >= FLM__MAX_FILE_AMOUNT ) {
		List_( Queue_.Tail() ).File->ReleaseFile( false );
		Queue_.Delete( Queue_.Tail() );
	}

	if ( Queue_.IsEmpty() )
		Queue_.Create( Row );
	else
		Queue_.BecomePrevious( Row, Queue_.Head() );

	FlusherData_.LastFileAccessTime = tol::Clock( false );

	Unlock_();
}

void flm::_ReportFileClosing( row__ Row )
{
	Lock_();

	if ( Queue_.IsMember( Row ) )
		Queue_.Delete( Row );

	Unlock_();
}

static void _Search(
	id__ ID,
	bch::E_BUNCH_( row__ ) &Rows )
{
	row__ Row = List_.First();

	while ( Row != NONE ) {
		if ( List_( Row ).ID == ID )
			Rows.Append( Row );

		Row = List_.Next( Row );
	}
}

static void _Release( const bch::E_BUNCH_( row__ ) &Rows )
{
	epeios::row__ Row = Rows.First();

	while ( Row != NONE ) {
		List_( Rows( Row ) ).File->ReleaseFile( false );

		if ( Queue_.IsMember( Rows( Row ) ) )
			Queue_.Delete( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

void flm::ReleaseFiles( id__ ID )
{
ERRProlog
	bch::E_BUNCH( row__ ) Rows;
ERRBegin
	Lock_();

	Rows.Init();

	_Search( ID, Rows );

	_Release( Rows );
ERRErr
ERREnd
	Unlock_();
ERREpilog
}


void flm::ReleaseInactiveFiles(
	time_t Delay,
	bso::ulong__ MaxAmount )
{
	Lock_();

	time_t Now = tol::Clock( false );

	while ( MaxAmount-- && ( Queue_.Tail() != NONE ) && ( ( Now - List_( Queue_.Tail() ).File->GetLastAccessTime() ) <= Delay ) ) {
		List_( Queue_.Tail() ).File->ReleaseFile( false );
		Queue_.Delete( Queue_.Tail() );
	}

	Unlock_();
}

/*
void flm::ReleaseAllFiles( void )
{
	Lock_();

	flm::row__ Row = List.First();
	flm::row__ RowBuffer;

	while ( Row != NONE ) {
		RowBuffer = Row;

		Row = List.Next( Row );

		List( RowBuffer )->ReleaseFile( false );

		if ( Queue.IsMember( RowBuffer ) )
			Queue.Delete( RowBuffer );
	}

	Unlock_();

}
*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class flmpersonnalization
: public flmtutor
{
public:
	flmpersonnalization( void )
	{
		List_.Init();
		Queue_.Init();
		IDs_.Init();

		flm::MaxFileAmount = FLM__MAX_FILE_AMOUNT;

#ifdef MT
		Mutex_ = mtx::Create( mtx::mOwned );
#endif

		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~flmpersonnalization( void )
	{

#ifdef MT
		mtx::Delete( Mutex_ );
#endif
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

static flmpersonnalization Tutor;

ttr_tutor &FLMTutor = Tutor;
