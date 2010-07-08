/*
	Header for the 'xmllpr' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XMLLPR__INC
#define XMLLPR__INC

#define XMLLPR_NAME		"XMLLPR"

#define	XMLLPR_VERSION	"$Revision$"

#define XMLLPR_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XMLLPRTutor;

#if defined( XXX_DBG ) && !defined( XMLLPR_NODBG )
#define XMLLPR_DBG
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
//D XML Light PaRser 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#error	"Use 'lxmlpr'"

#include "err.h"
#include "flw.h"
#include "xtf.h"
#include "str.h"
#include "lstctn.h"
#include "stk.h"

namespace xmllpr {

	using xtf::location__;

	E_ROW( srow__ );

	typedef lstctn::E_LXMCONTAINERt_( str::string_, srow__ ) strings_;

	struct tag__ {
		srow__ Prefix;
		srow__ Name;
	};

	typedef stk::E_STACK_( tag__ ) tags_;

	struct callback__
	{
		virtual void XMLLPRTag(
			const str::string_ &Prefix,
			const str::string_ &Name ) = 0;
		virtual void XMLLPRValue( const str::string_ &Value ) = 0;
		virtual void XMLLPRAttribute(
			const str::string_ &Prefix,
			const str::string_ &Name,
			const str::string_ &Value ) = 0;
		virtual void XMLLPRTagClosed( void ) = 0;
		virtual void XMLLPRError(
			location__ Line,
			location__ Column ) = 0;

	};

	class parser_ {
	public:
		struct s {
			strings_ ::s Strings;
			tags_::s Tags;
		};
		strings_  Strings;
		tags_ Tags;
		parser_( s &S )
		: Strings( S.Strings ),
		  Tags( S.Tags )
		{}
		void reset( bso::bool__ P = true )
		{
			Strings.reset( P );
			Tags.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Strings.plug( MM );
			Tags.plug( MM );
		}
		parser_ &operator =( const parser_ &P )
		{
			Strings = P.Strings;
			Tags = P.Tags;

			return *this;
		}
		void Init( void )
		{
			Strings.Init();
			Tags.Init();
		}
		void Parse(
			xtf::extended_text_iflow__ &Flow,
			callback__ &Callback );
	};

	E_AUTO( parser )



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
