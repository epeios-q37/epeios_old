/*
	Header for the 'csdsnc' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef CSDSNC__INC
#define CSDSNC__INC

#define CSDSNC_NAME		"CSDSNC"

#define	CSDSNC_VERSION	"$Revision$"

#define CSDSNC_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CSDSNCTutor;

#if defined( XXX_DBG ) && !defined( CSDSNC_NODBG )
#define CSDSNC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Standard Network Client 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "sck.h"
#include "stk.h"
#include "csdbnc.h"
#ifdef CPE__MT
#	include "mtx.h"
#endif
#include "bso.h"

#define CSDSNC_UNDEFINED			BSO_USHORT_MAX
#define CSDSNC_DEFAULT_CACHE_SIZE	100

namespace csdsnc {
#ifdef CPE__MT
	typedef mtx::mutex_handler__	mutex__;
#	define CSDSNC_NO_MUTEX			MTX_INVALID_HANDLER
#else
	typedef void *mutex__;
#	define CSDSNC_NO_MUTEX			NULL
#endif

	inline void _Lock( mutex__ Mutex )
	{
#ifdef CPE__MT
		mtx::Lock( Mutex );
#endif
	}

	inline void _Unlock( mutex__ Mutex )
	{
#ifdef CPE__MT
		mtx::Unlock( Mutex );
#endif
	}

	inline void _Delete( mutex__ Mutex )
	{
#ifdef CPE__MT
		mtx::Delete( Mutex );
#endif
	}

	inline mutex__ _Create( void )
	{
#ifdef CPE__MT
		return mtx::Create();
#else
		return CSDSNC_NO_MUTEX;
#endif
	}

	typedef sck::socket_ioflow___ _flow___;

	typedef stk::E_STACK_( _flow___ * )	flows_;

	enum log__ {
		lCreation,
		lRetrieval,
		lRelease,
		l_amount,
		l_Undefined
	};

	const char *GetLogLabel( log__ Log );

	class log_functions__
	{
	protected:
		virtual void CSDSNCLog(
			log__ Log,
			const void *Flow,
			epeios::size__ Amount ) = 0;
	public:
		void Log(
			log__ Log,
			const void *Flow,
			epeios::size__ Amount )
		{
			CSDSNCLog( Log ,Flow, Amount );
		}
	};

	class core_ {
	private:
		void _DeleteFlows( void );
		void _Log(
			log__ Log,
			const _flow___ *Flow )
		{
			if ( S_.Log.Functions != NULL ) {
ERRProlog
ERRBegin
				_Lock( S_.Log.Mutex );
				S_.Log.Functions->Log( Log, Flow, Flows.Amount() );
ERRErr
ERREnd
				_Unlock( S_.Log.Mutex );
ERREpilog
			}
		}
	public:
		struct s {
			tol::E_FPOINTER___( char ) Host;
			tol::E_FPOINTER___( char ) Service;
			mutex__ Mutex;
			struct log__ {
				log_functions__ *Functions;
				mutex__ Mutex;
			} Log;
			flows_::s Flows;
		} &S_;
		flows_ Flows;
		core_( s &S )
		: S_( S ),
		  Flows( S.Flows )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( S_.Mutex != CSDSNC_NO_MUTEX )
					_Delete( S_.Mutex );
				if ( S_.Log.Mutex != CSDSNC_NO_MUTEX )
					_Delete( S_.Log.Mutex );
			}

			S_.Host.reset( P );
			S_.Service.reset( P );
			S_.Mutex = CSDSNC_NO_MUTEX;
			S_.Log.Mutex = CSDSNC_NO_MUTEX;
			S_.Log.Functions = NULL;
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			Flows.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Flows.plug( MM );
		}
		core_ &operator =( const core_ &C )
		{
			ERRu();

			return *this;
		}
		void Init( 
			const char *Host,
			const char *Service,
			log_functions__ &LogFunctions )
		{
			reset();

			S_.Host = malloc( strlen( Host ) + 1 );

			if ( S_.Host == NULL )
				ERRa();

			strcpy( S_.Host, Host );

			S_.Service = malloc( strlen( Service ) + 1 );

			if ( S_.Service == NULL )
				ERRa();

			strcpy( S_.Service, Service );

			S_.Mutex = _Create();
			S_.Log.Mutex = _Create();
			S_.Log.Functions = &LogFunctions;

			Flows.Init();
		}
		_flow___ *Get( void )
		{
			_flow___ *Flow = NULL;
		ERRProlog
			log__ Log = l_Undefined;
			bso::bool__ Locked = false;
		ERRBegin
			_Lock( S_.Mutex );
			Locked = true;

			if ( Flows.Amount() ) {
				Flow = Flows.Pop();
				Log = lRetrieval;
			} else {
				Flow = new _flow___;

				if ( Flow == NULL )
					ERRa();

				Flow->Init( csdbnc::Connect( S_.Host, S_.Service ) );

				Log = lCreation;
			}

			_Unlock( S_.Mutex );
			Locked = false;

			_Log( Log, Flow );
		ERRErr
		ERREnd
			if ( Locked )
				_Unlock( S_.Mutex );
		ERREpilog

			return Flow;
		}
		void Release( _flow___ *Flow )
		{
			_Lock( S_.Mutex );

			Flows.Push( Flow );

			_Unlock( S_.Mutex );

			_Log( lRelease, Flow );
		}
	};

	E_AUTO( core );

	class client_flow___
	: public flw::ioflow__
	{
	private:
		_flow___ *_Flow;
		core_ *_Core;
		bso::ushort__ _Id;
		flw::datum__ _Cache[CSDSNC_DEFAULT_CACHE_SIZE];
		bso::bool__ _Prepare( void )	// Return true if has already a flow, false otherwise.
		{
			bso::bool__ Created = _Flow == NULL;

			if ( Created ) {
				_Flow = _Core->Get();

				_Flow->Write( &_Id, sizeof( _Id ) );
			}

			return !Created;
		}
		void _Synchronize( void )
		{
#ifdef CSDSNC_DBG
			if ( _Flow == NULL )
				ERRu();
#endif

			_Flow->Synchronize();

			if ( _Id == CSDSNC_UNDEFINED )
				_Flow->Read( sizeof( _Id ), &_Id );
			else if ( _Flow->Get() != 0 )
				ERRf();
		}
	protected:
		virtual flw::bsize__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::bsize__ Wanted,
			flw::bsize__ Minimum,
			bool Synchronization )
		{
			flw::size__ Amount = 0;

			_Prepare();

			return _Flow->WriteRelay( Buffer, Wanted, Minimum, Synchronization );
		}
		virtual void FLWSynchronizing( void )
		{
			_Synchronize();
		}
		virtual flw::bsize__ FLWRead(
			flw::bsize__ Minimum,
			flw::datum__ *Buffer,
			flw::bsize__ Wanted )
		{
			flw::bsize__ Amount = 0;

			if ( !_Prepare() )
				_Synchronize();


			Amount = _Flow->ReadUpTo( Wanted, Buffer );

			while ( Amount < Minimum ) {
				Amount += _Flow->ReadUpTo( Wanted - Amount, Buffer + Amount );
			}

			return Amount;
		}
		virtual void FLWDismiss( void )
		{
			if ( _Flow != NULL )
				_Core->Release( _Flow );

			_Flow->Dismiss();

			_Flow = NULL;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Flow != NULL )
					delete _Flow;
			}

			_Flow = NULL;
			_Id = CSDSNC_UNDEFINED;
			_Core = NULL;
		}
		client_flow___( flw::mutex__ Mutex = FLW_NO_MUTEX )
		: ioflow__( _Cache, sizeof( _Cache ), FLW_SIZE_MAX, Mutex, Mutex )
		{
			reset( false );
		}
		void Init( core_ &Core )
		{
			reset();

			_Core = &Core;

		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
