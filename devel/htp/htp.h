/*
	Header for the 'htp' library by Claude SIMON (csimon@epeios.org)
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

#ifndef HTP__INC
#define HTP__INC

#define HTP_NAME		"HTP"

#define	HTP_VERSION	"$Revision$"

#define HTP_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &HTPTutor;

#if defined( XXX_DBG ) && !defined( HTP_NODBG )
#define HTP_DBG
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
//D Hypertext Transfert Protocol 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Hypertext Transfer Protocol
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "bso.h"
#include "str.h"
#include "txf.h"
#include "ctn.h"

namespace htp {
	//e Status of the parsing.
	enum status__ {
		//i OK (HTTP code 200).
		sOK,
		//i Continue (HTTP code 100)
		sContinue,
		//i NotFound (HTTP code 404).
		sFound,
		//i Found (HTTP code 302).
		sNotFound,
		//i Unhandle HTTP response code.
		sUnhandledCode,
		//i Not HTTP protocol or bad version.
		sUnknowProtocol,
		//i Amount of status.
		s_amount,
		s_unknow,
	};

	enum field_name__ {
		fnURL,
		fnHost,
		fnReferrer,
		fnAccept,
		fnContentType,
		fn_amount,
		fn_unknow,
		fn_end
	};

	typedef str::string_	field_value_;
	typedef str::string		field_value;

	class field_ {
	public :
		struct s {
			field_name__ Name;
			field_value_::s Value;
		} &S_;
		field_value_ Value;
		field_( s &S )
		: S_( S ),
		  Value( S.Value )
		{}
		void reset( bso::bool__ P = true )
		{
			Value.reset( P );
			S_.Name = fn_unknow;
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			Value.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Value.plug( MM );
		}
		field_ &operator =( const field_ &F )
		{
			Value = F.Value;
			S_.Name = F.S_.Name;

			return *this;
		}
		void Init( void )
		{
			S_.Name = fn_unknow;
			Value.Init();
		}
		void Init(
			field_name__ Name,
			const str::string_ &Value )
		{
			S_.Name = Name;
			this->Value.Init( Value );
		}
		void Init(
			field_name__ Name,
			const char *Value )
		{
			S_.Name = Name;
			this->Value.Init( Value );
		}
		E_RODISCLOSE_( field_name__, Name )
	};

	E_AUTO( field );

	typedef ctn::E_XMCONTAINER_( field_ )	fields_;
	E_AUTO( fields )


	typedef bso::ulong__ content_length__;

	//c An http 1.1 header.
	class header_
	{
	public:
		struct s
		{
			content_length__ ContentLength;
			str::string_::s Location;
		} &S_;
		header_( s &S )
		: S_( S ),
		  Location ( S.Location )
		{}
		str::string_ Location;
		void reset( bso::bool__ P = true )
		{
			S_.ContentLength = 0;
			Location.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Location.plug( MM );
		}
		header_ &operator =( const header_ &H )
		{
			S_.ContentLength = H.S_.ContentLength;
			Location = H.Location;

			return *this;
		}
		void Init( void )
		{
			S_.ContentLength = 0;
			Location.Init();
		}
		E_RODISCLOSE_( content_length__, ContentLength );
	};

	E_AUTO( header )

	//f Parse 'flow' and fill 'Header' with it. Return status. 'Header' is only fill when return value is 'sOK'.
	status__ Parse(
		flw::iflow__ &IFlow,
		header_ &Header );

	void Post( 
		const str::string_ &URL,
		const fields_ &Fields,
		const str::string_ &Content,
		txf::text_oflow__ &Flow );



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
