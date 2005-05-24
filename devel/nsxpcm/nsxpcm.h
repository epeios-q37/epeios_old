/*
	Header for the 'nsxpcm' library by Claude SIMON (csimon@epeios.org)
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

#ifndef NSXPCM__INC
#define NSXPCM__INC

#define NSXPCM_NAME		"NSXPCM"

#define	NSXPCM_VERSION	"$Revision$"

#define NSXPCM_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &NSXPCMTutor;

#if defined( XXX_DBG ) && !defined( NSXPCM_NODBG )
#define NSXPCM_DBG
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
//D from nsXPCOM 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "epeios.h"
#include "str.h"
#include "ctn.h"

#include "nsEmbedString.h"

#ifdef NSXPCM_BKD
#	define NSXPCM__BKD
#endif

#ifdef NSXPCM__BKD
#	include "bkdacc.h"
#endif

namespace nsxpcm {

	using str::string_;
	using str::string;

	typedef ctn::E_XMCONTAINER_( string_ ) strings_;
	E_AUTO( strings );

	void Convert(
		const char *CString,
		epeios::size__ Size,
		char **JString );

	void Convert(
		const char *CString,
		char **JString );

	void Convert(
		const nsEmbedCString &ECString,
		nsEmbedString &EString );

	void Convert( 
		const char *String,
		nsEmbedString &EString );

	void Convert(
		const nsEmbedString &EString,
		nsEmbedCString &ECString );

	void Convert( 
		const nsEmbedString &EString,
		char **JString );

	void Convert(
		const str::string_ &EString,
		char **JString );

	void Convert(
		const str::string_ &String,
		nsEmbedString &EString );

	void Convert( 
		const nsEmbedString &EString,
		str::string_ &String );

	void Split( 
		const string_ &Merged,
		strings_ &Splitted,
		bso::char__ Separator );

	void Split(
		const char *Merged,
		nsxpcm::strings_ &Splitted,
		bso::char__ Separator );

	void Merge(
		const strings_ &Splitted,
		str::string_ &Merged,
		bso::char__ Separator );

#ifdef NSXPCM__BKD
	void Convert(
		const strings_ &Items,
		bkdacc::ids32_ &Ids );

	void SplitAndConvert(
		const char *Merged,
		bkdacc::ids32_ &Ids,
		bso::char__ Separator );

	void ConvertAndMerge(
		const bkdacc::ids8_ &Ids,
		string_ &Merged,
		bso::char__ Separator );

	void ConvertAndMerge(
		const bkdacc::ids16_ &Ids,
		string_ &Merged,
		bso::char__ Separator );

	void ConvertAndMerge(
		const bkdacc::ids32_ &Ids,
		string_ &Merged,
		bso::char__ Separator );
#endif
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
