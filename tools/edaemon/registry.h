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

	extern rgstry::entry___ ModuleService;
	extern rgstry::entry___ ModuleServiceType;
	extern rgstry::entry___ ModuleFileName;
	extern rgstry::entry___ ModuleLogFileName;
	extern rgstry::entry___ ModuleLogMode;

	inline const char *GetModuleLogFileName(
		const str::string_ &Id,
		STR_BUFFER___ &FileName )
	{
		return sclrgstry::GetOptionalValue( rgstry::tentry___( ModuleLogFileName, Id ), FileName );
	}

	inline const str::string_ &GetRawModuleLogMode(
		const str::string_ &Id,
		str::string_ &Mode )
	{
		return sclrgstry::GetOptionalValue( rgstry::tentry___( ModuleLogMode, Id ), Mode );
	}

	bso::bool__ GetModulesIds( rgstry::values_ &Values );

	inline const str::string_ &GetModuleFileName(
		const str::string_ &Id,
		str::string_ &FileName )
	{
		return sclrgstry::GetMandatoryValue( rgstry::tentry___( ModuleFileName, Id ), FileName );
	}

	inline bso::ushort__ GetRawModuleService( const str::string_ &Id )
	{
		return sclrgstry::GetMandatoryUShort( rgstry::tentry___( ModuleService, Id ) );
	}

	inline const str::string_ &GetRawModuleServiceType(
		const str::string_ &Id,
		str::string_ &Type )
	{
		return sclrgstry::GetMandatoryValue( rgstry::tentry___( ModuleServiceType, Id ), Type );
	}
}

#endif
