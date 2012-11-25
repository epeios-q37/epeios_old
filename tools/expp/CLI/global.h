/*
	Header for the 'global' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'xxx_app' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'expp'.

    'expp' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'expp' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'expp'.  If not, see <http://www.gnu.org/licenses/>.
*/
// $Id$


#ifndef GLOBAL__INC
# define GLOBAL__INC

# include "expp.h"
# include "registry.h"

# include "lcl.h"
# include "cio.h"

namespace global {
	using cio::CErr;
	using cio::COut;
	using cio::CIn;

	extern lcl::locale Locale;
	

//	using registry::value_;
//	E_AUTO( value )

	enum message__ {
		mProcessCommandDescription,
		mEncryptCommandDescription,
		mNamespaceOptionDescription,
		mNoIndentOptionDescription,
		mSourceFileArgumentDescription,
		mDestFileArgumentDescription,
		m_amount,
		m_Undefined
	};

# define GLOBAL__MESSAGE_AMOUNT	6

	const char *Label( message__ Message );

	inline const str::string_ &GetTranslation(
		message__ Message,
		str::string_ &Translation )
	{
		Locale.GetTranslation( Label( Message ), "", Translation );

		return Translation;
	}

	void Display(
		message__ Message,
		... );

	enum error__ {
		eErrorParsingConfigurationFile,
		eErrorParsingLocaleFile,
		eUnableToOpenFile,
		eProcessingError,
		eEncryptionError,
		e_amount,
		e_Undefined
	};

# define GLOBAL__ERROR_AMOUNT	5

	const char *Label( error__ Error );

	inline const str::string_ &GetTranslation(
		error__ Error,
		str::string_ &Translation )
	{
		Locale.GetTranslation( Label( Error ), "", Translation );

		return Translation;
	}

	void Report(
		error__ Error,
		... );

	void CreateBackupFile( const char *FileName );

	void RecoverBackupFile( const char *FileName );
}

// Program entry point (to define).
void Main(
	int argc,
	const char *argv[] );

#endif
