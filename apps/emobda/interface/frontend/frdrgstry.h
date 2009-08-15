/*
	'frdrgstry.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef FRDRGSTRY__INC
#define FRDRGSTRY__INC

#include "rgstry.h"

#define FRDRGSTRY_PATH_SEPARATOR	"/"	// Path item separator

#define FRDRGSTRY_PATH_TAG_CHAR		'$'	// Joker for tags as character. NOTA : if modified, modify below too !
#define FRDRGSTRY_PATH_TAG_STRING	"$"	// Joker for tags as string

#define FRDRGSTRY_PARAMETERS_PATH	"Parameters" FRDRGSTRY_PATH_SEPARATOR
#define FRDRGSTRY_PROFILES_PATH		"Profiles" FRDRGSTRY_PATH_SEPARATOR

namespace frdrgstry {

	struct paths {
		static struct parameters {
			static struct backend {
				static const char
					*Location,
					*Configuration,
					*AccessMode,
					*Type;
			} Backend;
			static struct database {
				static const char
					* Path,
					* AccessMode;
			} Database;
		} Parameters;
		static struct profiles {
			static const char *CurrentTable;
			static const char *FallbackProfile;
			static const char *DefaultProfile;
			static const char *UserProfile;
		} Profiles;
	};

	typedef rgstry::overloaded_unique_registry___ registry___;

	bso::bool__ FillRegistry(
		const char *FileName,
		const char *RootPath,
		rgstry::registry_ &Registry,
		rgstry::nrow__ &RegistryRoot,
		txf::text_oflow__ &Flow );

	inline bso::bool__ GetPathValue(
		const char *Path,
		const registry___ &Registry,
		str::string_ &Value )
	{
		return Registry.GetPathValue( rgstry::term( Path ), Value );
	}

	bso::bool__ GetProfileValue(
		const char *Path,
		const registry___ &Registry,
		str::string_ &Value );

	bso::bool__ GetProfileIntegerValue(
		const char *Path,
		const registry___ &Registry,
		bso::ulong__ &Id );

	void SetProfileValue(
		const char *Path,
		registry___ &Registry,
		const str::string_ &Value );

	inline void SetProfileIntegerValue(
		const char *Path,
		registry___ &Registry,
		bso::ulong__ Id )
	{
		bso::integer_buffer__ Buffer;

		SetProfileValue( Path, Registry, str::string( bso::Convert( Id, Buffer ) ) );
	}


}

#endif