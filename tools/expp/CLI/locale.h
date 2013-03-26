/*
	Header for the 'locale' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'xxx' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'xxx'.

    'xxx' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'xxx' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'xxx'.  If not, see <http://www.gnu.org/licenses/>.
*/	
// $Id$


#ifndef LOCALE__INC
# define LOCALE__INC

// # include "xxx.h"
# include "scllocale.h"

# include "lcl.h"
# include "cio.h"

namespace locale {

	enum message__ {
		mProcessCommandDescription,
		mEncryptCommandDescription,
		mNamespaceOptionDescription,
		mNoIndentOptionDescription,
		mSourceFileArgumentDescription,
		mDestFileArgumentDescription,
		mProcessingError,
		mEncryptionError,
		m_amount,
		m_Undefined
	};

// To adjust if above 'enum' is modified.
# define LOCALE__MESSAGE_AMOUNT	8

	using scllocale::GetLocale;

	const char *Label( message__ Message );

	const lcl::meaning_ &GetMeaning_(
		message__ Message,
		lcl::meaning_ *Meaning,	// '&Meaning' ne fonctionne pas avec 'va_arg'.
		... );

	// For 'meanings' with no tags.
#define GET_MEANING( name )\
	inline const lcl::meaning_ &Get##name##Meaning( lcl::meaning_ &Meaning )\
	{\
		return GetMeaning_( m##name, &Meaning );\
	}

	GET_MEANING( ProcessCommandDescription );
	GET_MEANING( EncryptCommandDescription );
	GET_MEANING( NoIndentOptionDescription );
	GET_MEANING( SourceFileArgumentDescription );
	GET_MEANING( DestFileArgumentDescription );

	inline const lcl::meaning_ &GetNamespaceOptionDescriptionMeaning(
		const char *DefaultNamespace,
		lcl::meaning_ &Meaning )
	{
		return GetMeaning_( mNamespaceOptionDescription, &Meaning, DefaultNamespace );
	}

	inline const lcl::meaning_ &GetProcessingErrorMeaning(
		const xpp::context___ &Context,
		lcl::meaning_ &Meaning )
	{
		return GetMeaning_( mProcessingError, &Meaning, &Context );
	}

	inline const lcl::meaning_ &GetEncryptionErrorMeaning(
		const xpp::context___ &Context,
		lcl::meaning_ &Meaning )
	{
		return GetMeaning_( mEncryptionError, &Meaning, &Context );
	}

}

#endif
