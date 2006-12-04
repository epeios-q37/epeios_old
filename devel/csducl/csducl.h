/*
	Header for the 'csducl' library by Claude SIMON (csimon@epeios.org)
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
		bso::bool__ Init(
			const char *Backend,
			void *UP,
			csdsnc::log_functions__ &Log,
			type__ Type )
		{
			bso::bool__ Success = false;

			switch ( Type ) {
			case tShared:
				Success = _Shared.Init( Backend, Log );
				break;
			case tLibrary:
				Success = _LibraryName.Init( Backend, UP );
				break;
			default:
				ERRu();
				break;
			}

			_Type = Type;

			return Success;
		}
		type__ GetType( void ) const
		{
			return _Type;
		}
		friend class universal_client_ioflow_functions___;
	};

	class universal_client_ioflow_functions___
	: public fwf::ioflow_functions___
	{
	private:
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
		virtual fwf::size__ FWFRead(
			fwf::size__ Minimum,
			fwf::datum__ *Buffer,
			fwf::size__ Wanted )
		{
			return _Get().ReadRelay( Minimum, Buffer, Wanted );
		}
		virtual void FWFDismiss( void )
		{
			_Get().Dismiss();
		}
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Buffer,
			fwf::size__ Wanted,
			fwf::size__ Minimum )
		{
			return _Get().WriteRelay( Buffer, Wanted, Minimum );
		}
		virtual void FWFSynchronize( void )
		{
			_Get().Synchronize();
		}
	public:
		void Init( universal_client_core &Core )
		{
			_Core = &Core;

			ioflow_functions___::Init();

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

	class universal_client
	: public flw::ioflow__
	{
	private:
		universal_client_ioflow_functions___ _Functions;
		flw::datum__ _Cache[CSDUCL_CACHE_SIZE];
	public:
		universal_client( flw::size__ AmountMax = FLW_SIZE_MAX )
		: ioflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{}
		void Init( universal_client_core &Core )
		{
			_Functions.Init( Core );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
