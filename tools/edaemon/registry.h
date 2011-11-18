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

	struct paths
	: public sclrgstry::paths
	{
		struct modules {
			struct module {
				static const char
					*Id,
					*Service,
					*FileName,
					*ConnectionType;
			};
		};
		static const char *Service;
		struct log {
			static const char *FileName;
			static const char *Mode;
		};
	};

	SCLRGSTRY_ORV( GetLogFileName, paths::log::FileName );

	inline const str::string_ &GetModuleFileName(
		const str::string_ &Id,
		str::string_ &FileName )
	{
		return GetMandatoryRegistryValue( xpath___( paths::modules::module::FileName, Id ), FileName );
	}

	inline bso::ushort__ GetRawService( const str::string_ &Id )
	{
		return GetMandatoryRegistryUShort( xpath___( registry::paths::modules::module::Service, Id ) );
	}

	inline bso::ushort__ GetRawModuleConnectionType( const str::string_ &Id )
	{
		return GetMandatoryRegistryUShort( xpath___( registry::paths::modules::module::ConnectionType, Id ) );
	}

	SCLRGSTRY_MRV( GetRawLogFileHandling, paths::log::Mode );

	inline bso::bool__ GetModulesIds( str::strings_ &Ids )
	{
		return GetValues( paths::modules::module::Id, Ids );
	}

}

#endif
