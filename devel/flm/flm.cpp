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

#ifdef CPE__T_MT
#	define MT
#endif

#ifdef MT
#	include "mtx.h"
#endif

using namespace flm;

/*
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
*/
/*
static lstbch::E_LBUNCHt( memoire_fichier_base___ *, row__ ) List;
static que::E_MQUEUEt( row__ ) Queue;
*/
row__ flm::concurrent_opened_files_manager::Register( memoire_fichier_base___ &MFB )
{
	row__ Row = NONE;

	_Lock();

	Row = _List.New();

	if ( _Queue.Amount() < _List.Extent() )	// On teste 'Amount' parce que ce qui est entre 'Amount' et 'Extent' n'est pas initialisé dans la queue.
		_Queue.Allocate( _List.Extent() );

	_List.Store( &MFB, Row );

	_Unlock();

	return Row;
}

void flm::concurrent_opened_files_manager::Unregister( row__ Row )
{
	_Lock();

	_List.Store( NULL, Row );
	_List.Delete( Row );

	if ( _Queue.IsMember( Row ) )
		_Queue.Delete( Row );

	_Unlock();
}

void flm::concurrent_opened_files_manager::ReportFileUsing( row__ Row )
{
	_Lock();

	if ( _Queue.IsMember( Row ) )
		_Queue.Delete( Row );
	else if ( _Queue.Amount() >= FLM__MAX_FILE_AMOUNT ) {
		_List( _Queue.Tail() )->ReleaseFile( false );
		_Queue.Delete( _Queue.Tail() );
	}

	if ( _Queue.IsEmpty() )
		_Queue.Create( Row );
	else
		_Queue.BecomePrevious( Row, _Queue.Head() );

	_Unlock();
}

void flm::concurrent_opened_files_manager::ReportFileClosing( row__ Row )
{
	_Lock();

	if ( _Queue.IsMember( Row ) )
		_Queue.Delete( Row );

	_Unlock();
}

void flm::concurrent_opened_files_manager::ReleaseAllFiles( void )
{
	_Lock();

	flm::row__ Row = _List.First();
	flm::row__ RowBuffer;

	while ( Row != NONE ) {
		RowBuffer = Row;

		Row = _List.Next( Row );

		_List( RowBuffer )->ReleaseFile( false );

		if ( _Queue.IsMember( RowBuffer ) )
			_Queue.Delete( RowBuffer );
	}

	_Unlock();

}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class flmpersonnalization
: public flmtutor
{
public:
	flmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~flmpersonnalization( void )
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

static flmpersonnalization Tutor;

ttr_tutor &FLMTutor = Tutor;
