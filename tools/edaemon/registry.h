/*
	Header for the 'registry' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of th 'edaemon' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'edaemon'.

    'edaemon' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'edaemon' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'edaemon'.  If not, see <http://www.gnu.org/licenses/>.
*/	
// $Id$


#ifndef REGISTRY__INC
# define REGISTRY__INC

# include "edaemon.h"
# include "sclrgstry.h"

#define REGISTRY_TAG_MARKER_STRING	"%"
#define REGISTRY_TAG_MARKER_CHAR	'%'

namespace registry {
	using namespace sclrgstry;

	extern rgstry::entry___ LogFileName;
	extern rgstry::entry___ LogMode;

	extern rgstry::entry___ ModuleId;
	extern rgstry::entry___ ModuleService;
	extern rgstry::entry___ ModuleFileName;
	extern rgstry::entry___ ModuleConnectionType;

	const str::string_ &GetModuleFileName(
		const str::string_ &Id,
		str::string_ &FileName )
	{
		return sclrgstry::GetMandatoryRegistryValue( rgstry::tentry___( ModuleFileName, Id ), FileName );
	}

	bso::ushort__ GetModuleRawService(
		const str::string_ &Id,
		str::string_ &FileName );

	bso::ubyte__ &GetModuleRawConnectionType(
		const str::string_ &Id,
		str::string_ &FileName );
}

#endif
