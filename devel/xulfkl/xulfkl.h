/*
	Header for the 'xulfkl' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFKL__INC
#define XULFKL__INC

#define XULFKL_NAME		"XULFKL"

#define	XULFKL_VERSION	"$Revision$"

#define XULFKL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFKLTutor;

#if defined( XXX_DBG ) && !defined( XULFKL_NODBG )
#define XULFKL_DBG
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
//D XUL Frontend KerneL 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#include "frdkrn.h"

#define XULFKL_M_AMOUNT	2

namespace xulfkl {

	using frdkrn::status__;

	enum message__ 
	{
		mSelectProjectFile,
		mExitConfirmation,
		m_amount,
		m_Undefined
	};

	typedef frdkrn::kernel___ _kernel___;

	class kernel___
	: public _kernel___
	{
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				Close();

			_kernel___::reset( P );
		}
		kernel___( void )
		{
			reset( false );
		}
		~kernel___( void )
		{
			reset();
		}
		status__ Init(
			const str::string_ &ConfigurationFileName,
			const char *TargetName,
			const char *Language,
			const xpp::criterions___ &Criterions )
		{
			return _kernel___::Init( ConfigurationFileName, TargetName, Language, Criterions );
		}
		status__ LoadProject(
			const str::string_ &FileName,
			const char *TargetName,
			const xpp::criterions___ &Criterions,
			const frdkrn::compatibility_informations__ &CompatibilityInformations,
			csdleo::shared_data__ &LibrarySharedData )
		{
			return _kernel___::LoadProject( FileName, TargetName, Criterions, CompatibilityInformations, LibrarySharedData );
		}
		const str::string_ &GetTranslation(
			message__ Message,
			str::string_ &Translation );
		const char *GetTranslation(
			message__ Message,
			STR_BUFFER___ &Buffer );
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
