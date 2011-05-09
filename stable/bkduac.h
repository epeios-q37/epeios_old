/*
	Header for the 'bkduac' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef BKDUAC__INC
#define BKDUAC__INC

#define BKDUAC_NAME		"BKDUAC"

#define	BKDUAC_VERSION	"$Revision$"

#define BKDUAC_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &BKDUACTutor;

#if defined( XXX_DBG ) && !defined( BKDUAC_NODBG )
#define BKDUAC_DBG
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
//D BacKenD Universal ACcess 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bkdlac.h"
#include "bkdrac.h"

namespace bkduac {

	enum type__
	{
		tLocal,
		tRemote,
		t_Amount,
		t_Undefined
	};

	typedef bkdacc::parameters_handling_functions__ _parameters_handling_functions__;


	class universal_parameters_handling_functions__
	: public _parameters_handling_functions__
	{
	private:
		bkdlac::backend_local_access_base__ _Local;
		bkdrac::backend_remote_access_base___ _Remote;
		type__ _Type;
	protected:
		virtual void BKDACCIn(
			bkdcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			switch ( _Type ) {
			case tLocal:
				_Local.In( Cast, Pointer, Flow );
				break;
			case tRemote:
				_Remote.In( Cast, Pointer, Flow );
				break;
			default:
				ERRu();
				break;
			}

			
		}
		virtual void BKDACCOut(
			flw::ioflow__ &Flow,
			bkdcst::cast Cast,
			void *Pointer )
		{
			switch ( _Type ) {
			case tLocal:
				_Local.Out( Flow, Cast, Pointer );
				break;
			case tRemote:
				_Remote.Out( Flow, Cast, Pointer );
				break;
			default:
				ERRu();
				break;
			}
		}
		virtual void BKDACCPostProcess( flw::ioflow__ &Flow )
		{
			switch ( _Type ) {
			case tLocal:
				_Local.PostProcess( Flow );
				break;
			case tRemote:
				_Remote.PostProcess( Flow );
				break;
			default:
				ERRu();
				break;
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Local.reset( P );
			_Remote.reset( P );
			_parameters_handling_functions__::reset( P );
			_Type = t_Undefined;
		}
		universal_parameters_handling_functions__( void )
		{
			reset( false );
		}
		virtual ~universal_parameters_handling_functions__( void )
		{
			reset();
		}
		void Init( type__ Type )
		{
			reset();

			switch ( Type ) {
			case tLocal:
				_Local.Init();
				break;
			case tRemote:
				_Remote.Init();
				break;
			default:
				ERRu();
				break;
			}

			_Type = Type;

			_parameters_handling_functions__::Init();
		}
	};

	typedef bkdacc::backend_access___ _backend_access___;

	class backend_universal_access___
	: public _backend_access___
	{
	private:
		universal_parameters_handling_functions__ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			_backend_access___::reset( P );
			_Functions.reset( P );
		}
		backend_universal_access___( void )
		{
			reset( false );
		}
		~backend_universal_access___( void )
		{
			reset();
		}
		void Init(
			const char *Language,
			const char *APIVersion,
			flw::ioflow__ &Flow,
			type__ Type,
			bkdacc::error_handling_functions__ &ErrorHandlingFunctions,
			str::string_ &Message,
			str::string_ &URL )
		{
			_Functions.Init( Type );

			_backend_access___::Init( Language, APIVersion, Flow, _Functions, ErrorHandlingFunctions, Message, URL  );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
