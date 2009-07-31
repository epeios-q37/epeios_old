/*
	'mbdrgstry.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef MBDRGSTRY__INC
#define MBDRGSTRY__INC

#include "rgstry.h"

#define MBDRGSTRY_PATH_SEPARATOR	"/"	// Path item separator
#define MBDRGSTRY_PATH_TAG			"$"	// Joker for tags.

#define MBDRGSTRY_PARAMETERS_PATH	 "Parameters" MBDRGSTRY_PATH_SEPARATOR

namespace mbdrgstry {

	struct paths {
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
	};

	typedef rgstry::overloaded_unique_registry___ registry___;

	bso::bool__ FillRegistry(
		const char *FileName,
		const char *RootPath,
		rgstry::registry_ &Registry,
		rgstry::nrow__ &RegistryRoot,
		txf::text_oflow__ &Flow );
}

#endif