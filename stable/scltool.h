/*
	Header for the 'scltool' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

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

#ifndef SCLTOOL__INC
#define SCLTOOL__INC

#define SCLTOOL_NAME		"SCLTOOL"

#define	SCLTOOL_VERSION	"$Revision$"

#define SCLTOOL_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &SCLTOOLTutor;

#if defined( XXX_DBG ) && !defined( SCLTOOL_NODBG )
#define SCLTOOL_DBG
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
//D SoCLe TOOL 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "lcl.h"
# include "clnarg.h"

# include "sclmisc.h"
# include "scllocale.h"
# include "sclerror.h"


namespace scltool {
	using sclmisc::CreateBackupFile;
	using sclmisc::RecoverBackupFile;
	using sclmisc::ReportFileOpeningErrorAndExit;

	enum command {
		cHelp,
		cVersion,
		cLicense,
		c_amount
	};


	// A définir par l'utilisateur.
	extern const char *TargetName;

	const char *GetLanguage( void );

	void AddDefaultCommands( clnarg::description_ &Description );

	void PrintDefaultCommandDescriptions(
		const char *ProgramName,
		const clnarg::description_ &Description );

	// A définir par l'utilisateur.
	void Main(
		int argc,
		const char *argv[] );

	inline const str::string_ &GetTranslation(
		const char *Text,
		str::string_ &Translation )
	{
		return scllocale::GetTranslation( Text, GetLanguage(), Translation );
	}


	inline const str::string_ &GetTranslation(
		const lcl::meaning_ &Meaning,
		str::string_ &Translation )
	{
		return scllocale::GetTranslation( Meaning, GetLanguage(), Translation );
	}

	inline void ReportAndExit(
		const lcl::meaning_ &Meaning,
		int ExitValue = EXIT_FAILURE )
	{
		sclerror::SetMeaning( Meaning );
		ERRExit( ExitValue );
	}

	void ReportAndExit(
		const char *Text,
		int ExitValue = EXIT_FAILURE );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
