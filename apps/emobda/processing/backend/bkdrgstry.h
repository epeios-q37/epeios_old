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

	struct paths {
		static const char *RootPath;
	};

	bso::bool__ FillRegistry(
		const char *ConfigurationFileName,
		const char *PathRoot,
		txf::text_oflow__ &ErrFlow );

	bso::bool__ GetPathValue(
		const char *Path,
		str::string_ &Value );

	inline bso::bool__ GetRootPath( str::string_ &Value )
	{
		return GetPathValue( paths::RootPath, Value );
	}

	bso::bool__ GetNumber(
		const char *Path,
		bso::ulong__ &ULong );

	bso::bool__ GetNumber(
		const char *Path,
		bso::ushort__ &UShort );

	bso::bool__ GetNumber(
		const char *Path,
		bso::ubyte__ &UByte );
}

#endif