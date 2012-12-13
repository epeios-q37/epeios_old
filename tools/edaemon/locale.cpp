/*
	'locale' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
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

#include "locale.h"

#include "scllocale.h"

#include <stdarg.h>

#include "flf.h"
#include "xtf.h"
#include "xml.h"
#include "fnm.h"
#include "dir.h"

#include "sclrgstry.h"

using namespace locale;

#define CASE( name )			LCL_CASE( name, m )
#define CASE_N( name, count )	LCL_CASE_N( name, m, count )

const char *locale::Label( message__ Message )
{
#if	GLOBAL__MESSAGE_AMOUNT != 0
#	error "Amount of 'message__' entries changed ! Update !"
#endif

	switch( Message ) {
	default:
		ERRc();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

const lcl::meaning_ &locale::GetMeaning_(
	message__ Message,
	lcl::meaning_ *Meaning,
	... )
{
#if	GLOBAL__MESSAGE_AMOUNT != 0
#	error "Amount of 'message__' entries changed ! Update !"
#endif
	va_list Args;
	const str::string_ *Translation = NULL;

	va_start( Args, Message );

	Meaning->SetValue( Label( Message ) );

	switch ( Message ) {
	default:
		ERRc();
		break;
	}

	return *Meaning;
}

static struct global_cdtor {
	global_cdtor( void )
	{
		if ( GLOBAL__MESSAGE_AMOUNT != m_amount )
			ERRc();
	}
	~global_cdtor( void )
	{
	}
} GLOBALCDTor_;
