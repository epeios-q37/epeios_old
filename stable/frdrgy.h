/*
	Header for the 'frdrgy' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

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

#ifndef FRDRGY__INC
#define FRDRGY__INC

#define FRDRGY_NAME		"FRDRGY"

#define	FRDRGY_VERSION	"$Revision$"

#define FRDRGY_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FRDRGYTutor;

#if defined( XXX_DBG ) && !defined( FRDRGY_NODBG )
#define FRDRGY_DBG
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
//D FRontenD ReGistrY 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#include "rgstry.h"

#define FRDRGY_PATH_SEPARATOR	"/"	// Path item separator

#define FRDRGY_PATH_TAG_CHAR	'$'	// Joker for tags as character. NOTA : if modified, modify below too !
#define FRRRGY_PATH_TAG_STRING	"$"	// Joker for tags as string

#define FRDRGY_PARAMETERS_PATH	"Parameters" FRDRGY_PATH_SEPARATOR
#define FRDRGY_PROFILES_PATH	"Profiles" FRDRGY_PATH_SEPARATOR

namespace frdrgy {
	struct paths {
		static struct parameters {
			static const char *Locale;
			static struct backend {
				static const char
					*Location,
					*Configuration,
					*AccessMode,
					*Type;
			} Backend;
		} Parameters;
		static struct profiles {
			static const char *FallbackProfile;
			static const char *DefaultProfile;
			static const char *UserProfile;
		} Profiles;
	};

	typedef rgstry::multi_level_registry_ _registry_;

	class registry_
	: public _registry_
	{
	private:
		epeios::size__ _DumpRegistry(
			rgstry::level__ Level,
			txf::text_oflow__ &TFlow ) const
		{
			return _registry_::Dump( S_.Configuration, true, xml::oIndent, TFlow );
		}
	public:
		struct s
		: public _registry_::s
		{
			rgstry::level__
				Configuration,
				Project,
				User;
		} &S_;
		registry_( s &S )
		: S_( S ),
			_registry_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_registry_::reset( P );

			S_.Configuration = S_.Project = S_.User = RGSTRY_UNDEFINED_LEVEL;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_registry_::plug( MM );
		}
		registry_ &operator =( const registry_ &R )
		{
			_registry_::operator =( R );

			S_.Configuration = R.S_.Configuration;
			S_.Project = R.S_.Project;
			S_.User = R.S_.User;

			return *this;
		}
		void Init( void )
		{
			_registry_::Init();

			S_.Configuration = _registry_::AddNewLevel( rgstry::name( "Configuration" ) );
			S_.Project = _registry_::AddNewLevel( rgstry::name( "Project" ) );
			S_.User = _registry_::AddNewLevel( rgstry::name( "User" ) );
		}
		void DumpConfiguration( txf::text_oflow__ &TFlow ) const
		{
			_DumpRegistry( S_.Configuration, TFlow );
		}
		void DumpProject( txf::text_oflow__ &TFlow ) const
		{
			_DumpRegistry( S_.Project, TFlow );
		}
		void DumpUser( txf::text_oflow__ &TFlow ) const
		{
			_DumpRegistry( S_.User, TFlow );
		}
		rgstry::error__ FillConfiguration(
			const char *FileName,
			const char *RootPath,
			rgstry::error_details_ &ErrorDetails )
		{
			return _registry_::Fill( S_.Configuration, FileName, RootPath, ErrorDetails );
		}
		rgstry::error__ FillProject(
			const char *FileName,
			const char *RootPath,
			rgstry::error_details_ &ErrorDetails )
		{
			return _registry_::Fill( S_.Project, FileName, RootPath, ErrorDetails );
		}
		rgstry::error__ FillUser(
			const char *FileName,
			const char *RootPath,
			rgstry::error_details_ &ErrorDetails )
		{
			return _registry_::Fill( S_.User, FileName, RootPath, ErrorDetails );
		}
		rgstry::error__ FillUser(
			flw::iflow__ &Flow,
			const char *RootPath,
			rgstry::error_details_ &ErrorDetails )
		{
			return _registry_::Fill( S_.User, Flow, str::string( "" ), RootPath, ErrorDetails );
		}
		time_t ConfigurationTimeStamp( void ) const
		{
			return TimeStamp( S_.Configuration );
		}
		time_t ProjectTimeStamp( void ) const
		{
			return TimeStamp( S_.Project );
		}
		time_t UserTimeStamp( void ) const
		{
			return TimeStamp( S_.User );
		}
	};


	E_AUTO( registry );

	// typedef rgstry::overloaded_unique_registry___ registry___;

	inline bso::bool__ GetValue(
		const char *Path,
		const registry_ &Registry,
		str::string_ &Value )
	{
		return Registry.GetValue( str::string( Path ), Value );
	}

	bso::bool__ GetProfileValue(
		const char *Path,
		const registry_ &Registry,
		str::string_ &Value );

	bso::bool__ GetProfileIntegerValue(
		const char *Path,
		const registry_ &Registry,
		bso::ulong__ &Id );

	void SetProfileValue(
		const char *Path,
		registry_ &Registry,
		const str::string_ &Value );

	inline void SetProfileIntegerValue(
		const char *Path,
		registry_ &Registry,
		bso::ulong__ Id )
	{
		bso::integer_buffer__ Buffer;

		SetProfileValue( Path, Registry, str::string( bso::Convert( Id, Buffer ) ) );
	}

	inline bso::bool__ GetBackendType(
		const registry_ &Registry,
		str::string_ &Target )
	{
		return GetValue( paths::Parameters.Backend.Type, Registry, Target );
	}

	inline bso::bool__ GetBackendLocation(
		const registry_ &Registry,
		str::string_ &Target )
	{
		return GetValue( paths::Parameters.Backend.Location, Registry, Target );
	}

	inline bso::bool__ GetLocalesFileName(
		const registry_ &Registry,
		str::string_ &Locales )
	{
		return GetValue( paths::Parameters.Locale, Registry, Locales );
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
