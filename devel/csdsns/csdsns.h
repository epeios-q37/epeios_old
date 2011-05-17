/*
	Header for the 'csdsns' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
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

#ifndef CSDSNS__INC
#define CSDSNS__INC

#define CSDSNS_NAME		"CSDSNS"

#define	CSDSNS_VERSION	"$Revision$"

#define CSDSNS_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &CSDSNSTutor;

#if defined( XXX_DBG ) && !defined( CSDSNS_NODBG )
#define CSDSNS_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Client-Server Standard Network Server.
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "csdscm.h"
#include "csdbns.h"
#include "csdsnb.h"
#include "lstbch.h"
#include "mtx.h"

namespace csdsns {

	using namespace csdscm;
	using namespace csdsnb;
	using csdbns::port__;

	enum log__ {
		lNew,
		lStore,
		lTestAndGet,
		lDelete,
		l_amount,
		l_Undefined
	};

	const char *GetLogLabel( log__ Log );

	typedef void *_user_pointer__;

	class log_functions__ {
	protected:
		virtual void CSDSNSLog(
			log__ Log,
			id__ Id,
			void *UP,
			epeios::size__ Amount ) = 0;
	public:
		void Log(
			log__ Log,
			id__ Id,
			_user_pointer__ UP,
			epeios::size__ Amount )
		{
			CSDSNSLog( Log, Id, UP, Amount );
		}
	};

	typedef lstbch::E_LBUNCH_( _user_pointer__ ) user_pointers_;

	class core_
	{
	private:
		bso::bool__ _Exists( id__ Id ) const
		{
			return UPs.Exists( Id );
		}
		void _Log(
			log__ Log,
			id__ Id,
			void *UP ) const
		{
			if ( S_.Log.Functions != NULL ) {
ERRProlog
ERRBegin
				mtx::Lock( S_.Log.Mutex );
				S_.Log.Functions->Log( Log, Id, UP, UPs.Amount() );
ERRErr
ERREnd
				mtx::Unlock( S_.Log.Mutex );
ERREpilog
			}
		}
	public:
		struct s
		{
			user_pointers_::s UPs;
			mtx::mutex_handler__ Mutex;
			struct log__ {
				log_functions__ *Functions;
				mtx::mutex_handler__ Mutex;
			} Log;
		} &S_;
		user_pointers_ UPs;
		core_ ( s &S )
		: S_( S ),
		  UPs( S.UPs )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( S_.Mutex != MTX_INVALID_HANDLER )
					mtx::Delete( S_.Mutex );
			}

			UPs.reset( P );
			S_.Mutex = MTX_INVALID_HANDLER;
			S_.Log.Mutex = MTX_INVALID_HANDLER;
			S_.Log.Functions = NULL;

		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			UPs.plug( MM );
		}
		core_ &operator =( const core_ &C )
		{
			ERRu();

			return *this;
		}
		void Init( log_functions__ &LogFunctions )
		{
			reset();

			UPs.Init();
			S_.Mutex = mtx::Create( mtx::mFree );
			S_.Log.Mutex = mtx::Create( mtx::mFree );
			S_.Log.Functions = &LogFunctions;
		}
		id__ New( void )
		{
			mtx::Lock( S_.Mutex );

			epeios::row__ Row = UPs.New();

			if ( *Row >= BSO_USHORT_MAX )
				ERRl();

			mtx::Unlock( S_.Mutex );

			_Log( lNew, (id__)*Row, NULL );

			return (id__)*Row;
		}
		void Store(
			_user_pointer__ UP,
			id__ Id )
		{
#ifdef CSDSNS_DBG
			if ( Id == CSDSNB_UNDEFINED )
				ERRu();
#endif
			mtx::Lock( S_.Mutex );
			UPs.Store( UP, Id );
			mtx::Unlock( S_.Mutex );

			_Log( lStore, Id, UP );
		}
		bso::bool__ Exists( id__ Id ) const
		{
			bso::bool__ Result;

			mtx::Lock( S_.Mutex );

			Result =  _Exists( Id );

			mtx::Unlock( S_.Mutex );

			return Result;
		}
		bso::bool__ TestAndGet(
			id__ Id,
			void *&UP ) const
		{
			bso::bool__ Result = false;

			mtx::Lock( S_.Mutex );

			Result = _Exists( Id );

			if ( Result )
				UP = UPs( Id );

			mtx::Unlock( S_.Mutex );

			_Log( lTestAndGet, Id, UP );

			return Result;
		}
		bso::bool__ Delete( id__ Id )
		{
			bso::bool__ Result = false;

			mtx::Lock( S_.Mutex );

			Result = _Exists( Id );

			if ( Result )
				UPs.Delete( Id );

			mtx::Unlock( S_.Mutex );

			_Log( lDelete, Id, NULL );

			return Result;
		}

	};

	E_AUTO( core)

	class _functions___
	: public csdbns::user_functions__
	{
	private:
		core _Core;
		user_functions__ *_Functions;
		const char *_Origin;
		void _Clean( void );	// Appelle tout le 'postProcess' pour tous les objets utilisateurs.
	protected:
		virtual void *CSDPreProcess( const char *Origin )
		{
			_Origin = Origin;

			return NULL;
		}
		virtual action__ CSDProcess(
			flw::ioflow__ &Flow,
			void *UP )
		{
#ifdef CSDSNS_DBG
			if ( UP != NULL )
				ERRc();
#endif
			id__ Id = CSDSNB_UNDEFINED;
			action__ Action = aContinue;

			UP = NULL;

			Id = GetId( Flow );

			if ( Id == CSDSNB_UNDEFINED ) {
				Id = _Core.New();
				PutId( Id, Flow );
				UP = _Functions->PreProcess( _Origin );
				_Core.Store( UP, Id );
				Action = _Functions->Process( Flow, UP );
			} else if ( Id == CSDSNB_PING ) {
				Flow.Put( (flw::datum__)0 );
				Flow.Commit();
			} else if ( !_Core.TestAndGet( Id, UP ) ) {
					Flow.Put( (flw::datum__)-1 );
					Flow.Commit();
					Action = aStop;
			} else {
				Flow.Put( 0 );
				Action = _Functions->Process( Flow, UP );
			}

			switch ( Action ) {
			case aContinue:
				break;
			case aStop:
				_Functions->PostProcess( UP );
				if ( Id < CSDSNB_RESERVED )
					_Core.Delete( Id );
				break;
			default:
				ERRu();
				break;
			}

			return Action;
		}
		virtual void CSDPostProcess( void *UP )
		{
			if ( UP != NULL )
				ERRc();
		}
		virtual void CSDExit( void )
		{
			_Functions->Exit();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				_Clean();

			_Core.reset( P );
			_Functions = NULL;
			_Origin = NULL;
		}
		_functions___( void)
		{
			reset( false );
		}
		~_functions___( void)
		{
			reset();
		}
		void Init(
			user_functions__ &Functions,
			log_functions__ &LogFunctions )
		{
			reset();

			_Core.Init( LogFunctions );
			_Functions = &Functions;
		}
	};

	// Pour l'utilisation en tant que service Windows, voir csdbns::server__'. 
	class server___
	{
	private:
		csdbns::server___ _Server;
		_functions___ _Functions;
	public:
		void Init(
			port__ Port,
			user_functions__ &UserFunctions,
			log_functions__ &LogFunctions )
		{
			_Functions.Init( UserFunctions, LogFunctions );

			_Server.Init( Port, _Functions );
		}
		bso::bool__ LaunchService( const char *ServiceName )
		{
			return _Server.LaunchService( ServiceName );
		}
		void Process( sck::duration__ TimeOut = SCK_INFINITE )
		{
			_Server.Process( TimeOut );
		}
	};


}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
