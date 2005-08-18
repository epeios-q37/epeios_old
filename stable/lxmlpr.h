/*
	Header for the 'lxmlpr' library by Claude SIMON (csimon@epeios.org)
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

#ifndef LXMLPR__INC
#define LXMLPR__INC

#define LXMLPR_NAME		"LXMLPR"

#define	LXMLPR_VERSION	"$Revision$"

#define LXMLPR_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &LXMLPRTutor;

#if defined( XXX_DBG ) && !defined( LXMLPR_NODBG )
#define LXMLPR_DBG
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
//D Light XML PaRser 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "xtf.h"
#include "lstctn.h"

namespace lxmlpr {
	struct callback__
	{
		virtual void LXMLPRTag(
			const str::string_ &Prefix,
			const str::string_ &Name ) = 0;
		virtual void LXMLPRValue( const str::string_ &Value ) = 0;
		virtual void LXMLPRAttribute(
			const str::string_ &Prefix,
			const str::string_ &Name,
			const str::string_ &Value ) = 0;
		virtual void LXMLPRTagClosed( void ) = 0;
	};

	bso::bool__ Parse(
		xtf::extended_text_iflow__ &Flow,
		callback__ &Callback,
		xtf::location__ &ErrorLine,
		xtf::location__ &ErrorColumn );



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
