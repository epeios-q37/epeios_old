/*
  'mtk' library by Claude L. Simon (simon@epeios.org)
  Requires the 'mtk' header file ('mtk.h').
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

#define MTK__COMPILATION

#include "mtk.h"

class mtktutor
: public ttr_tutor
{
public:
	mtktutor( void )
	: ttr_tutor( MTK_NAME )
	{
#ifdef MTK_DBG
		Version = MTK_VERSION "\b\bD $";
#else
		Version = MTK_VERSION;
#endif
		Owner = MTK_OWNER;
		Date = "$Date$";
	}
	virtual ~mtktutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "mtx.h"

using namespace mtk;

#ifdef MTK__UNIX_LIKE

namespace {
	struct thread_struct__
	{
		mtk__routine R;
		void *UP;
		mtx::mutex_handler__ MH;
		thread_struct__( void )
		{
			R = NULL;
			UP = NULL;
			MH = MTX_INVALID_HANDLER;
		}
	};
};

#	ifdef CPE__BEOS

#	include <be/kernel/scheduler.h>

static int32 ThreadFunction_( void *D )
{
ERRFProlog
	mtk__thread_struct TS = *((mtk__thread_struct *)D);
ERRFBegin
	mtx::Unlock( TS.MH );
	TS.R( TS.UP );
ERRFErr
ERRFEnd
ERRFEpilog
	return 0;
}

static void LaunchThread_(
	mtk__routine Routine,
	void *UP )
{
ERRProlog
	thread_id ID;
	mtk__thread_struct TS;
	sph__semaphore_ S;
ERRBegin
	TS.R = Routine;
	TS.UP = UP;
	TS.SD = SPHCreate();

	S.Init( TS.SD );

	S.Lock();	// Unlocked by 'ThreadFunction'.
	
	if ( ( ID = spawn_thread( ThreadFunction_, NULL, suggest_thread_priority(), &TS ) ) < B_OK )
		ERRs();
		
	if ( resume_thread( ID ) != B_OK )
		ERRs();

	S.Lock();
	S.Unlock();

ERRErr
ERREnd
	if ( TS.SD != SPH_INVALID )
		SPHDelete( TS.SD );
ERREpilog
}

#	else

static void *ThreadFunction_( void *D )
{
ERRFProlog
	thread_struct__ TS = *((thread_struct__ *)D);
ERRFBegin
	mtx::Unlock( TS.MH );
	TS.R( TS.UP );
#ifdef CPE__THREADS_REMAINS
	for(;;)
		TOLYield();
#endif
ERRFErr
ERRFEnd
ERRFEpilog
	return NULL;
}

static void LaunchThread_(
	mtk__routine Routine,
	void *UP )
{
ERRProlog
	pthread_t TID;
	thread_struct__ TS;
ERRBegin
	TS.R = Routine;
	TS.UP = UP;
	TS.MH = mtx::Create();
	
	mtx::Lock( TS.MH );	// Unlocked by 'ThreadFunction'.

	if ( pthread_create( &TID, NULL, ThreadFunction_, &TS ) )
		ERRs();

	mtx::Lock( TS.MH );
	mtx::Unlock( TS.MH );

ERRErr
ERREnd
	if ( TS.MH != MTX_INVALID_HANDLER )
		mtx::Delete( TS.MH );
ERREpilog
}
#	endif

#endif

void mtk::LaunchAndKill(
	mtk__routine Routine,
	void *UP )
{
ERRProlog
ERRBegin
#ifdef CPE__MS
	if ( _beginthread( Routine, 4096, UP ) == (unsigned long)-1 )
		ERRs();
#elif defined( MTK__UNIX_LIKE )
	LaunchThread_( Routine, UP );
#else
#error "Unknow compilation enviroment"
#endif
ERRErr
ERREnd
ERREpilog
}

namespace {
	struct {
		// Lock to ensure that data was red before reusing them.
		mtx::mutex_handler__ Data;
		// Lock for new thread to block caller.
		mtx::mutex_handler__ Thread;
		// Lock for thread launcher to block new created thread.
		mtx::mutex_handler__ Store;
		// Lock to handle concurrent calls to 'LauncAndKeep()'.
		mtx::mutex_handler__ Exclusion;
		bso__bool Continue;
		void *UP;
		bso__ulong Amount;
		routine__ Routine;
	} Common;
	
	void Create_( void )
	{
	ERRProlog
		mtx::mutex___ Mutex;
	ERRBegin
		Common.Thread = Common.Data = Common.Store = Common.Exclusion = MTX_INVALID_HANDLER;

		Common.Continue = true;
		Common.UP = NULL;
		Common.Amount = 0;
		Common.Routine = NULL;
		Common.Data = mtx::Create();
		Common.Thread = mtx::Create();
		Common.Store = mtx::Create();
		Common.Exclusion = mtx::Create();

		mtx::Lock( Common.Thread );

		mtx::Lock( Common.Store );
	ERRErr
		if ( Common.Data != MTX_INVALID_HANDLER )
			mtx::Delete( Common.Data );

		if ( Common.Thread != MTX_INVALID_HANDLER )
			mtx::Delete( Common.Thread );

		if ( Common.Store != MTX_INVALID_HANDLER )
			mtx::Delete( Common.Store );
			
		if ( Common.Exclusion != MTX_INVALID_HANDLER )
			mtx::Delete( Common.Exclusion );
	ERREnd
	ERREpilog
	}

	void Close_( void )
	{
		Common.Continue = false;

		mtx::Lock( Common.Data );

		while ( Common.Amount != 0 ){

			mtx::Unlock( Common.Data );

			mtx::Unlock( Common.Thread );

			mtx::Lock( Common.Store );

			mtx::Lock( Common.Data );		
		}
		
		mtx::TryToLock( Common.Data);
		mtx::Unlock(Common.Data );
		
		mtx::TryToLock( Common.Thread);
		mtx::Unlock(Common.Thread);
		
		mtx::TryToLock(Common.Store);
		mtx::Unlock(Common.Store);
		
		mtx::TryToLock(Common.Exclusion);
		mtx::Unlock(Common.Exclusion);
		
		mtx::Delete( Common.Data );
		mtx::Delete( Common.Thread );
		mtx::Delete( Common.Store );
		mtx::Delete( Common.Exclusion );
	}
	
	void Launcher_( void * )
	{
	ERRFProlog
		void *RUP;
		mtk::routine__ Routine;
	ERRFBegin
		mtx::Lock( Common.Data );	// To ensure that data are available.
	
		do {
			RUP = Common.UP;
			Routine = Common.Routine;
			
			mtx::Unlock( Common.Data );	// I have what I need.
			
			mtx::Unlock( Common.Store );// Calling function can continue.
			
			Routine( RUP );	// Launching user function.
			
			mtx::Lock( Common.Data );	// To ensure that nobody else is accessing data.
			
			if ( Common.Amount == BSO_USHORT_MAX )
				ERRl();
				
			Common.Amount++;// One more thread available.
			
			mtx::Unlock( Common.Data );	// I'm finished with the data.
			
			mtx::Lock( Common.Thread );	// Waiting for new work.
			
			mtx::Lock( Common.Data );	// To ensure that nobody else is accessing data.
			
			Common.Amount--;// One less thread available.
			
		} while ( Common.Continue );
		
		mtx::Unlock( Common.Data );
		
		mtx::Unlock( Common.Store );
		
	ERRFErr
	ERRFEnd
	ERRFEpilog
	}
}

void mtk::LaunchAndKeep(
	routine__ Routine,
	void *UP )
{
	mtx::Lock( Common.Exclusion );

	mtx::Lock( Common.Data );
	
	Common.Routine = Routine;
	Common.UP = UP;
	
	if ( Common.Amount == 0 )
		mtk::LaunchAndKill( Launcher_, NULL ); 
	else
		mtx::Unlock( Common.Thread );
		
	mtx::Unlock( Common.Data );
	
	mtx::Lock( Common.Store );
	
	mtx::Unlock( Common.Exclusion );
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class mtkpersonnalization
: public mtktutor
{
public:
	mtkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
		Create_();
	}
	~mtkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
		Close_();
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static mtkpersonnalization Tutor;

ttr_tutor &MTKTutor = Tutor;
