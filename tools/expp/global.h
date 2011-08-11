/*
	Header for the 'global' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'xxx_app' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/
// $Id$


#ifndef GLOBAL__INC
# define GLOBAL__INC

# include "expp.h"
# include "registry.h"

# include "lcl.h"
# include "cio.h"

namespace global {
	extern cio::cerr___ cerr;
	extern cio::cout___ cout;
	extern cio::cin___ cin;

	extern lcl::rack__ LocaleRack;
	

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
		LocaleRack.GetTranslation( Label( Message ), "", Translation );

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
		LocaleRack.GetTranslation( Label( Error ), "", Translation );

		return Translation;
	}

	void Report(
		error__ Error,
		... );

	void Initialize( void );
	void Release();

	void CreateBackupFile( const char *FileName );

	void RecoverBackupFile( const char *FileName );
}

#endif
