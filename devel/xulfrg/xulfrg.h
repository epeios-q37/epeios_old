/*
	Header for the 'xulfrg' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFRG__INC
#define XULFRG__INC

#define XULFRG_NAME		"XULFRG"

#define	XULFRG_VERSION	"$Revision$"

#define XULFRG_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFRGTutor;

#if defined( XXX_DBG ) && !defined( XULFRG_NODBG )
#define XULFRG_DBG
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
//D XUL Fro,te,d ReGistryWiDGets 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "frdrgy.h"

namespace xulfrg {

	extern rgstry::entry AnnexTarget;
	extern rgstry::entry AnnexTargetType;

	extern rgstry::entry BackendSelection;
	extern rgstry::entry BackendSelectionMode;

	struct paths
	: public frdrgy::paths
	{
		struct annex {
			static const char *Type;
			static const char *Target;
		};
		struct backend_selection
		{
			static const char *Mode;
		};
	};

	using frdrgy::registry_;
	using frdrgy::registry;

	inline bso::bool__ GetRawAnnexType(
		const rgstry::multi_level_registry_ &Registry,
		str::string_ &Value )
	{
		return Registry.GetValue( paths::annex::Type, Value );
	}

	inline bso::bool__ GetAnnexTarget(
		const rgstry::multi_level_registry_ &Registry,
		str::string_ &Value )
	{
		return Registry.GetValue( paths::annex::Target, Value );
	}

	inline bso::bool__ GetRawBackendSelectionMode(
		const rgstry::multi_level_registry_ &Registry,
		str::string_ &Value )
	{
		return Registry.GetValue( paths::backend_selection::Mode, Value );
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
