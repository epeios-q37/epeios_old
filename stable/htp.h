/*
	Header for the 'htp' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2002, 2004 Claude SIMON (csimon@epeios.org).

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

namespace htp {
	//e Status of the parsing.
	enum status {
		//i Unknow status.
		sUnknow,
		//i OK (HTTP code 200).
		sOK,
		//i Continue (HTTP code 100)
		sContinue,
		//i NotFound (HTTP code 404).
		sNotFound,
		//i Unhandle HTTP response code.
		sUnhandledCode,
		//i Not HTTP protocol or bad version.
		sUnknowProtocol,
		//i Amount of status.
		s_amount,
	};

	//c An http 1.1 header.
	class http_header__
	{
	public:
		// The 'Content-Length' field.
		bso::ulong__ ContentLength;
		http_header__( void )
		{
			ContentLength = 0;
		}
	};

	//f Parse 'flow' and fill 'Header' with it. Return status. 'Header' is only fill when return value is 'sOK'.
	status Parse(
		flw::iflow__ &IFlow,
		http_header__ &Header );

	//f Post
	void Post(
		const str::string_ &URL,
		const str::string_ &Host,
		const str::string_ &Accept,
		const str::string_ &ContentType,
		const str::string_ &Content,
		txf::text_oflow__ &Flow );



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
