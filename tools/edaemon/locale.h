/*
	Header for the 'locale' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'edaemon' tool.
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


#ifndef LOCALE__INC
# define LOCALE__INC

# include "edaemon.h"
# include "scllocale.h"

# include "lcl.h"
# include "cio.h"

namespace locale {

	enum message__ {
		mConfigurationFileParsingError,
		mLocaleFileParsingError,
		mBadOrNoValueForRegistryEntry,
		mUnableToLoadBackend,
		mUnableToOpenLogFile,
		m_amount,
		m_Undefined
	};

// To adjust if above 'enum' is modified.
# define GLOBAL__MESSAGE_AMOUNT	0

	using scllocale::GetTranslation;

	const char *Label( message__ Message );

	const str::string_ &GetTranslation(
		message__ Message,
		... );
}

#endif
