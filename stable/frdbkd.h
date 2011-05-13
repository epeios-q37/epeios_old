/*
	Header for the 'frdbkd' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

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
	class _backend___
	{
	private:
		csducl::universal_client_ioflow___ _Flow;
		bkduac::backend_universal_access___ _BackendAccess;
	public:
		void reset( bso::bool__ P = true )
		{
			_BackendAccess.reset( P );
			_Flow.reset( P );
		}
		bso::bool__ Init(
			const char *Language,
			const char *TargetLabel,
			const char *APIVersion,
			csducl::universal_client_core &ClientCore,
			bkdacc::error_handling_functions__ &ErrorHandlingFunctions,
			str::string_ &Message,
			str::string_ &URL )
		{
			bkduac::mode__ Mode = bkduac::m_Undefined;

			_Flow.Init( ClientCore );

			switch ( ClientCore.GetType() ) {
			case csducl::tLibrary:
				Mode = bkduac::mLocal;
				break;
			case csducl::tDaemon:
				Mode = bkduac::mRemote;
				break;
			default:
				ERRu();
				break;
			}

			return _BackendAccess.Init( Language, TargetLabel, APIVersion,_Flow, Mode, ErrorHandlingFunctions, Message, URL );
		}
		bkdacc::backend_access___ &BackendAccess( void )
		{
			return _BackendAccess;
		}
		bso::bool__ IsConnected( void ) const
		{
			return _BackendAccess.IsConnected();
		}
		const char *GetMessage( void ) const
		{
			return _BackendAccess.GetMessage();
		}
		void About(
			str::string_ &ProtocolVersion,
			str::string_ &TargetLabel,
			str::string_ &APIVersion,
			str::string_ &BackendInformations,
			str::string_ &PublisherInformations )
		{
			_BackendAccess.About( ProtocolVersion, TargetLabel, APIVersion, BackendInformations, PublisherInformations );
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
		E_CVDTOR( backend___ )
		bso::bool__ Init(
			const char *Language,
			const char *TargetLabel,
			const char *APIVersion,
			csducl::universal_client_core &ClientCore,
			bkdacc::error_handling_functions__ &ErrorHandlingFunctions,
			str::string_ &Message,
			str::string_ &URL )
		{
			statics::Init( BackendAccess() );
			return _backend___::Init( Language, TargetLabel, APIVersion,ClientCore, ErrorHandlingFunctions, MEssage, URL );
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
