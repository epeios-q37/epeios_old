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

	inline const char *GetModuleLogFileName( STR_BUFFER___ &FileName )
	{
		return sclrgstry::GetOptionalValue( ModuleLogFileName, FileName );
	}

	inline const str::string_ &GetRawModuleLogMode( str::string_ &Mode )
	{
		return sclrgstry::GetOptionalValue( ModuleLogMode, Mode );
	}

	inline const str::string_ &GetModuleFileName( str::string_ &FileName )
	{
		return sclrgstry::GetMandatoryValue( ModuleFileName, FileName );
	}

	inline bso::u16__ GetRawModuleService( void )
	{
		return sclrgstry::GetMandatoryU16( ModuleService );
	}

	inline const str::string_ &GetRawModuleServiceType( str::string_ &Type )
	{
		return sclrgstry::GetMandatoryValue( ModuleServiceType, Type );
	}
}

#endif
