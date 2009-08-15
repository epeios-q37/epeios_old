/*
	'bkdrgstry.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa ReGistrY

#ifndef BKDRGSTRY__INC
#define BKDRGSTRY__INC

#include "rgstry.h"

#define BKDRGSTRY_PATH_SEPARATOR	"/"	// Path item separator

namespace bkdrgstry {

	typedef rgstry::overloaded_unique_registry___ registry___;

	extern registry___ Registry;

	struct paths {
		static const char *RootPath;
	};

	inline bso::bool__ GetPathValue(
		const char *Path,
		const registry___ &Registry,
		str::string_ &Value )
	{
		return Registry.GetPathValue( rgstry::term( Path ), Value );
	}

	inline bso::bool__ GetRootPath(
		const registry___ &Registry,
		str::string_ &Value )
	{
		return GetPathValue( paths::RootPath, Registry, Value );
	}

	bso::bool__ GetInteger(
		const char *Path,
		const registry___ &Registry,
		bso::ulong__ &Value );
}

#endif