/*
	'locale' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
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

#include "locale.h"

#include "scllocale.h"

#include <stdarg.h>

#include "flf.h"
#include "xtf.h"
#include "xml.h"
#include "fnm.h"
#include "dir.h"

#include "sclrgstry.h"
#include "scltool.h"

using namespace locale;

#define CASE( l )\
	case m##l:\
	return #l;\
	break

const char *locale::Label( message__ Message )
{
#if	LOCALE__MESSAGE_AMOUNT != 8
#	error "Amount of 'message__' entries changed ! Update !"
#endif

	switch( Message ) {
	CASE( ProcessCommandDescription );
	CASE( EncryptCommandDescription );
	CASE( NamespaceOptionDescription );
	CASE( NoIndentOptionDescription );
	CASE( SourceFileArgumentDescription );
	CASE( DestFileArgumentDescription );
	CASE( ProcessingError );
	CASE( EncryptionError );
	default:
		ERRCcp();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

const lcl::meaning_ &locale::GetMeaning_(
	message__ Message,
	lcl::meaning_ *Meaning,
	... )
{
#if	LOCALE__MESSAGE_AMOUNT != 8
#	error "Amount of 'message__' entries changed ! Update !"
#endif
ERRProlog
	va_list Args;
	lcl::meaning MeaningBuffer;
ERRBegin
	va_start( Args, Meaning );

	switch ( Message ) {
	case mProcessCommandDescription:
	case mEncryptCommandDescription:
	case mNamespaceOptionDescription:
	case mNoIndentOptionDescription:
		Meaning->SetValue( Label( Message ) );

		Meaning->AddTag( va_arg( Args, const char *) );
		break;
	case mSourceFileArgumentDescription:
	case mDestFileArgumentDescription:
		Meaning->SetValue( Label( Message ) );
		break;
	case mProcessingError:
		Meaning->SetValue( Label( Message ) );

		MeaningBuffer.Init();
		xpp::GetMeaning( *va_arg( Args, const xpp::context___ *), MeaningBuffer );

		Meaning->AddTag( MeaningBuffer );
		break;
	case mEncryptionError:
		Meaning->SetValue( Label( Message ) );

		MeaningBuffer.Init();
		xpp::GetMeaning( *va_arg( Args, const xpp::context___ *), MeaningBuffer );

		Meaning->AddTag( MeaningBuffer );
		break;
	default:
		ERRCcp();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return *Meaning;
}

static struct locale_cdtor {
	locale_cdtor( void )
	{
		if ( LOCALE__MESSAGE_AMOUNT != m_amount )
			ERRCcp();
	}
	~locale_cdtor( void )
	{
	}
} GLOBALCDTor_;
