/*
	Header for the 'clthvy' library by Claude SIMON (csimon@epeios.org)
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

#ifndef CLTHVY__INC
#define CLTHVY__INC

#define CLTHVY_NAME		"CLTHVY"

#define	CLTHVY_VERSION	"$Revision$"

#define CLTHVY_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CLTHVYTutor;

#if defined( XXX_DBG ) && !defined( CLTHVY_NODBG )
#define CLTHVY_DBG
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
//D CLienT for HeaVY operations. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "sck.h"
#include "stk.h"
#include "clt.h"
#include "mtx.h"
#include "bso.h"

#define CLTHVY_UNDEFINED			BSO_USHORT_MAX
#define CLTHVY_DEFAULT_CACHE_SIZE	100

namespace clthvy {
	typedef sck::socket_ioflow___ _flow___;

	typedef stk::E_STACK_( _flow___ * )	flows_;

	class core_ {
	private:
		void _DeleteFlows( void );
	public:
		struct s {
			tol::E_FPOINTER___( char ) Host;
			tol::E_FPOINTER___( char ) Service;
			mtx::mutex_handler__ Mutex;
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
				if ( S_.Mutex != MTX_INVALID_HANDLER )
					mtx::Delete( S_.Mutex );
			}

			S_.Host.reset( P );
			S_.Service.reset( P );
			S_.Mutex = MTX_INVALID_HANDLER;
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
			const char *Service )
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

			S_.Mutex = mtx::Create();

			Flows.Init();
		}
		_flow___ *Get( void )
		{
			_flow___ *Flow = NULL;

			mtx::Lock( S_.Mutex );

			if ( Flows.Amount() )
				Flow = Flows.Pop();
			else {
				Flow = new _flow___;

				if ( Flow == NULL )
					ERRa();

				Flow->Init( clt::Connect( S_.Host, S_.Service ) );
			}

			mtx::Unlock( S_.Mutex );

			return Flow;
		}
		void Release( _flow___ *Flow )
		{
			mtx::Lock( S_.Mutex );

			Flows.Push( Flow );

			mtx::Unlock( S_.Mutex );
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
		flw::datum__ _Cache[CLTHVY_DEFAULT_CACHE_SIZE];
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
#ifdef CLTHVY_DBG
			if ( _Flow == NULL )
				ERRu();
#endif

			_Flow->Synchronize();

			if ( _Id == CLTHVY_UNDEFINED )
				_Flow->Read( sizeof( _Id ), &_Id );
			else if ( _Flow->Get() != 0 )
				ERRf();
		}
	protected:
		virtual flw::size__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::size__ Wanted,
			flw::size__ Minimum,
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
		virtual flw::size__ FLWRead(
			flw::size__ Minimum,
			flw::datum__ *Buffer,
			flw::size__ Wanted )
		{
			flw::size__ Amount = 0;

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
			_Id = CLTHVY_UNDEFINED;
			_Core = NULL;
		}
		client_flow___( flw::mutex__ Mutex = FLW_NO_MUTEX )
		: ioflow__( _Cache, sizeof( _Cache ), FLW_AMOUNT_MAX, Mutex, Mutex )
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
