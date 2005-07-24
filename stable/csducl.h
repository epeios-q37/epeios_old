/*
	Header for the 'csducl' library by Claude SIMON (csimon@epeios.org)
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

#ifndef CSDUCL__INC
#define CSDUCL__INC

#define CSDUCL_NAME		"CSDUCL"

#define	CSDUCL_VERSION	"$Revision$"

#define CSDUCL_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CSDUCLTutor;

#if defined( XXX_DBG ) && !defined( CSDUCL_NODBG )
#define CSDUCL_DBG
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
//D Client-Server Devices Universal CLient 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "csdsnc.h"
#include "csddlc.h"

#define CSDUCL_CACHE_SIZE	1000

namespace csducl {
	enum type__ {
		tShared,
		tLibrary,
		t_amount,
		t_Undefined
	};

	class universal_client_core
	{
	private:
		type__ _Type;
		csdsnc::core _Shared;
		csddlc::dynamic_library_client_core _LibraryName;
	public:
		void Init(
			const char *Backend,
			void *UP,
			csdsnc::log_functions__ &Log,
			type__ Type )
		{
			switch ( Type ) {
			case tShared:
				_Shared.Init( Backend, Log );
				break;
			case tLibrary:
				_LibraryName.Init( Backend, UP );
				break;
			default:
				ERRu();
				break;
			}

			_Type = Type;
		}
		type__ GetType( void ) const
		{
			return _Type;
		}
		friend class universal_client;
	};

	class universal_client
	: public flw::ioflow__
	{
	private:
		flw::datum__ _Cache[CSDUCL_CACHE_SIZE];
		csdsnc::client_flow___ _Shared;
		csddlc::dynamic_library_client _Library;
		universal_client_core *_Core;
		flw::ioflow__ &_Get( void )
		{
			switch ( _Core->_Type ) {
			case tShared:
				return _Shared;
				break;
			case tLibrary:
				return _Library;
				break;
			default:
				ERRu();
				break;
			}

			return *(flw::ioflow__ *)NULL;	// Pour éviter un 'warning'.
		}
	protected:
		virtual flw::bsize__ FLWRead(
			flw::bsize__ Minimum,
			flw::datum__ *Buffer,
			flw::bsize__ Wanted )
		{
			return _Get().ReadRelay( Minimum, Buffer, Wanted );
		}
		virtual void FLWDismiss( void )
		{
			_Get().Dismiss();
		}
		virtual flw::bsize__ FLWWrite(
			const flw::datum__ *Buffer,
			flw::bsize__ Wanted,
			flw::bsize__ Minimum,
			bool Synchronization )
		{
			return _Get().WriteRelay( Buffer, Wanted, Minimum, Synchronization );
		}
		virtual void FLWSynchronizing( void )
		{
			_Get().Synchronize();
		}
	public:
		universal_client( void )
		: ioflow__( _Cache, sizeof( _Cache ), FLW_SIZE_MAX, FLW_NO_MUTEX, FLW_NO_MUTEX )
		{}

		void Init( universal_client_core &Core )
		{
			_Core = &Core;

			switch ( Core._Type ) {
			case tShared:
				_Shared.Init( Core._Shared );
				break;
			case tLibrary:
				_Library.Init( Core._LibraryName );
				break;
			default:
				ERRu();
				break;
			}
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
