/*
	Header for the 'frdbkd' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef FRDBKD__INC
#define FRDBKD__INC

#define FRDBKD_NAME		"FRDBKD"

#define	FRDBKD_VERSION	"$Revision$"

#define FRDBKD_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FRDBKDTutor;

#if defined( XXX_DBG ) && !defined( FRDBKD_NODBG )
#define FRDBKD_DBG
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
//D FRontenD BacKenD 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#include "csducl.h"
#include "bkduac.h"

namespace frdbkd {
	struct error_reporting_functions___
	{
	protected:
		virtual void FRDBCKNDReportError( const char *Message ) = 0 ;
	public:
		void reset( bso::bool__ = true )
		{
			// A des fins de standardisation.
		}
		void Init( void )
		{
			// A des fins de standardisation.
		}
		void ReportError( const char *Message )
		{
			FRDBCKNDReportError( Message );
		}
	};

	class _backend___
	{
	private:
		error_reporting_functions___ *_ErrorReportingFunctions;
		csducl::universal_client_ioflow___ _Flow;
		bkduac::backend_universal_access_functions__ _Functions;
		bkdacc::backend_access___ _BackendAccess;
		void _ReportError( const char *Message )
		{
			if ( _ErrorReportingFunctions != NULL )
				_ErrorReportingFunctions->ReportError( Message );
			else
				ERRb();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_BackendAccess.reset( P );
			_Flow.reset( P );
			_ErrorReportingFunctions = NULL;
		}
		void Init(
			csducl::universal_client_core &ClientCore,
			error_reporting_functions___ &ErrorReportingFunctions )
		{
			_Flow.Init( ClientCore );

			switch ( ClientCore.GetType() ) {
			case csducl::tLibrary:
				_Functions.Init( bkduac::tLocal );
				break;
			case csducl::tDaemon:
				_Functions.Init( bkduac::tRemote );
				break;
			default:
				ERRu();
				break;
			}

			_BackendAccess.Init( _Flow, _Functions );

			_ErrorReportingFunctions = &ErrorReportingFunctions;
		}
		bkdacc::backend_access___ &BackendAccess( void )
		{
			return _BackendAccess;
		}
		bso::bool__ IsConnected( void ) const
		{
			return _BackendAccess.IsConnected();
		}
		const char *GetRawMessage( void ) const
		{
			return _BackendAccess.GetRawMessage();
		}
		const char *GetI18Message( void ) const
		{
			return _BackendAccess.GetI18Message();
		}
		bso::bool__ Handle( bso::bool__ Result )
		{
		ERRProlog
			STR_BUFFER___ Buffer;
			str::string Message;
		ERRBegin
			if ( !Result ) {
				Message.Init();
				Message.Append( GetRawMessage() );
				Message.Append( '\n' );
				Message.Append( GetI18Message() );

//				nsxpcm::Alert( UI.Main.Window, Message.Convert( Buffer ) );
				_ReportError( Message.Convert( Buffer ) );
				ERRReturn;
			}
		ERRErr
		ERREnd
		ERREpilog
			return Result;
		}
		void About(
			str::string_ &BackendInformations,
			str::string_ &PublisherInformations )
		{
			_BackendAccess.About( BackendInformations, PublisherInformations );
		}
	};

	template <typename statics> class backend___
	: public _backend___,
	  public statics
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_backend___::reset( P );
			statics::reset( P );
		}
		void Init(
			csducl::universal_client_core &ClientCore,
			error_reporting_functions___ &ErrorReportingFunctions )
		{
			_backend___::Init( ClientCore, ErrorReportingFunctions );
			statics::Init( BackendAccess() );
		}
	};

	template <typename common> class backend_object_common___
	{
	protected:
		common Common;
	public:
		void reset( bso::bool__ P = true )
		{
			Common.reset( P );
		}
		void Init( _backend___ &Backend )
		{
			Common.Init( Backend.BackendAccess() );
		}
	};

	template <typename object> E_TTYPEDEF__( object, backend_object___ );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
