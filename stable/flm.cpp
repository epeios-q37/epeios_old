/*
	'flm' library by Claude SIMON (csimon@epeios.org)
	Requires the 'flm' header file ('flm.h').
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

#ifdef CPE__T_MT
#	define MT
#endif

#ifdef MT
#	include "mtx.h"
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

static lstbch::E_LBUNCHt( memoire_fichier_base___ *, row__ ) List;
static que::E_MQUEUEt( row__ ) Queue;

row__ flm::_Register( memoire_fichier_base___ &MFB )
{
	row__ Row = NONE;

	Lock_();

	Row = List.New();

	if ( Queue.Amount() < List.Extent() )	// On teste 'Amount' parce que ce qui est entre 'Amount' et 'Extent' n'est pas initialisé dans la queue.
		Queue.Allocate( List.Extent() );

	List.Store( &MFB, Row );

	Unlock_();

	return Row;
}

void flm::_Unregister( row__ Row )
{
	Lock_();

	List.Store( NULL, Row );
	List.Delete( Row );

	if ( Queue.IsMember( Row ) )
		Queue.Delete( Row );

	Unlock_();
}

void flm::_ReportFileUsing( row__ Row )
{
	Lock_();

	if ( Queue.IsMember( Row ) )
		Queue.Delete( Row );
	else if ( Queue.Amount() >= FLM__MAX_FILE_AMOUNT ) {
		List( Queue.Tail() )->ReleaseFile( false );
		Queue.Delete( Queue.Tail() );
	}

	if ( Queue.IsEmpty() )
		Queue.Create( Row );
	else
		Queue.BecomePrevious( Row, Queue.Head() );

	Unlock_();
}

void flm::_ReportFileClosing( row__ Row )
{
	Lock_();

	if ( Queue.IsMember( Row ) )
		Queue.Delete( Row );

	Unlock_();
}

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




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class flmpersonnalization
: public flmtutor
{
public:
	flmpersonnalization( void )
	{
		List.Init();
		Queue.Init();

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
