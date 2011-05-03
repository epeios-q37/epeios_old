/*
	Header for the 'csdsnc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#ifndef CSDSNC__INC
#define CSDSNC__INC

#define CSDSNC_NAME		"CSDSNC"

#define	CSDSNC_VERSION	"$Revision$"

#define CSDSNC_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &CSDSNCTutor;

#if defined( XXX_DBG ) && !defined( CSDSNC_NODBG )
#define CSDSNC_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
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
#include "csdsnb.h"
#include "bso.h"
#include "cpe.h"
#include "mmm.h"

#ifdef CPE__T_MT
#	define CSDSNC__MT
#endif

#ifdef CSDSNC__MT
#	include "mtx.h"
#endif

#define CSDSNC_DEFAULT_CACHE_SIZE	100

namespace csdsnc {
	using namespace csdsnb;
#ifdef CSDSNC__MT
	typedef mtx::mutex_handler__	mutex__;
#	define CSDSNC_NO_MUTEX			MTX_INVALID_HANDLER
#else
	typedef void *mutex__;
#	define CSDSNC_NO_MUTEX			NULL
#endif

	inline void _Lock( mutex__ Mutex )
	{
#ifdef CSDSNC__MT
		mtx::Lock( Mutex );
#endif
	}

	inline void _Unlock( mutex__ Mutex )
	{
#ifdef CSDSNC__MT
		mtx::Unlock( Mutex );
#endif
	}

	inline void _Delete( mutex__ Mutex )
	{
#ifdef CSDSNC__MT
		mtx::Delete( Mutex );
#endif
	}

	inline mutex__ _Create( void )
	{
#ifdef CSDSNC__MT
		return mtx::Create( mtx::mFree );
#else
		return CSDSNC_NO_MUTEX;
#endif
	}

	typedef sck::socket_ioflow___ _flow___;

	typedef stk::E_BSTACK_( _flow___ * )	flows_;

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
			char *HostService;
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
				_DeleteFlows();

				if ( S_.HostService != NULL )
					free( S_.HostService );
				if ( S_.Mutex != CSDSNC_NO_MUTEX )
					_Delete( S_.Mutex );
				if ( S_.Log.Mutex != CSDSNC_NO_MUTEX )
					_Delete( S_.Log.Mutex );
			}

			Flows.reset( P );

			S_.HostService = NULL;
			S_.Mutex = CSDSNC_NO_MUTEX;
			S_.Log.Mutex = CSDSNC_NO_MUTEX;
			S_.Log.Functions = NULL;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
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
		bso::bool__ Init( 
			const char *HostService,
			log_functions__ &LogFunctions )
		{
			reset();

			if ( ( S_.HostService = (char *)malloc( strlen( HostService ) + 1 ) ) == NULL )
				ERRa();

			strcpy( S_.HostService, HostService );

			S_.Mutex = _Create();
			S_.Log.Mutex = _Create();
			S_.Log.Functions = &LogFunctions;

			Flows.Init();

			_flow___ *Flow = Get( err::hUserDefined );

			if ( Flow == NULL )
				return false;

			Release( Flow );

			return true;
		}
		_flow___ *Get( err::handling__ ErrorHandling = err::h_Default )
		{
			_flow___ *Flow = NULL;
		ERRProlog
			log__ Log = l_Undefined;
			bso::bool__ Locked = false;
			sck::socket__ Socket = SCK_INVALID_SOCKET;
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

				if ( ( Socket = csdbnc::Connect( S_.HostService, ErrorHandling ) ) == SCK_INVALID_SOCKET ) {
					delete Flow;
					Flow = NULL;
					ERRReturn;
				}

				Flow->Init( Socket );

				Log = lCreation;
			}

			_Unlock( S_.Mutex );
			Locked = false;

			_Log( Log, Flow );
		ERRErr
			if ( Flow != NULL ) {
				delete Flow;
				Flow = NULL;
			}
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

	class _functions___
	: public fwf::ioflow_functions___<>
	{
	private:
		_flow___ *_Flow;
		core_ *_Core;
		id__ _Id;
		bso::bool__ _Prepare( void )	// Return true if has already a flow, false otherwise.
		{
			bso::bool__ Created = _Flow == NULL;

			if ( Created ) {
				_Flow = _Core->Get();

				PutId( _Id, *_Flow );
			}

			return !Created;
		}
		void _Commit( void )
		{
#ifdef CSDSNC_DBG
			if ( _Flow == NULL )
				ERRu();
#endif

			_Flow->Commit();

			if ( _Id == CSDSNB_UNDEFINED )
				_Id = GetId( *_Flow );
			else if ( _Flow->Get() != 0 )
				ERRf();
		}
	protected:
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Buffer,
			fwf::size__ Maximum )
		{
			fwf::size__ Amount = 0;

			_Prepare();

			return _Flow->WriteUpTo( Buffer, Maximum );
		}
		virtual void FWFCommit( void )
		{
			if ( _Flow != NULL )
				_Commit();
		}
		virtual fwf::size__ FWFRead(
			fwf::size__ Maximum,
			fwf::datum__ *Buffer )
		{
			return _Flow->ReadUpTo( Maximum, Buffer );
		}
		virtual void FWFDismiss( void )
		{
			if ( _Flow != NULL ) {
				_Core->Release( _Flow );
				_Flow->Dismiss();
			}

			_Flow = NULL;
		}
		public:
			void reset( bso::bool__ P = true )
			{
				ioflow_functions___::reset( P );

				if ( P ) {
					if ( _Flow != NULL )
						delete _Flow;
				}

				_Flow = NULL;
				_Id = CSDSNB_UNDEFINED;
				_Core = NULL;
			}
			_functions___( void )
			{
				reset( false );
			}
			~_functions___( void )
			{
				reset();
			}
			void Init(
				core_ &Core,
				fwf::thread_safety__ ThreadSafety )
			{
				reset();

				ioflow_functions___::Init( ThreadSafety );
				_Core = &Core;
			}
	};

	class client_ioflow___
	: public flw::ioflow__
	{
	private:
		_functions___ _Functions;
		flw::datum__ _Cache[CSDSNC_DEFAULT_CACHE_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			flw::ioflow__::reset( P );
			_Functions.reset( P );
		}
		client_ioflow___( void )
		{
			reset( false );
		}
		void Init( core_ &Core )
		{
			reset();

			_Functions.Init( Core, fwf::tsDisabled );
			ioflow__::Init( _Functions, _Cache, sizeof( _Cache ), FLW_SIZE_MAX );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
