/*
	Header for the 'mscmdx' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef MSCMDX__INC
#define MSCMDX__INC

#define MSCMDX_NAME		"MSCMDX"

#define	MSCMDX_VERSION	"$Revision$"

#define MSCMDX_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &MSCMDXTutor;

#if defined( XXX_DBG ) && !defined( MSCMDX_NODBG )
#define MSCMDX_DBG
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
//D MuSiC MiDi Xml features 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#include "mscmdm.h"
#include "mscmdf.h"
#include "xml.h"

namespace mscmdx {
	using mscmdm::origin__;

	bso::bool__ MIDIToXMID(
		flw::iflow__ &IFlow,
		origin__ Origin,
		xml::writer_ &Writer,
		err::handling__ ErrHandling = err::h_Default );

	xml::status__ XMIDToMIDI(
		xtf::extended_text_iflow__ &IFlow,
		flw::oflow__ &OFlow );

	enum status__ {
		sOK,
		sUnexpectedTag,
		sUnexpectedValue,
		sBadDataAttributeValue,
		sBadIdValue,
		sXML,
		s_amount,
		s_Undefined
	};

	status__ ParseEvents(
		xml::parser___ &Parser,
		mscmdm::events_ &Events );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
