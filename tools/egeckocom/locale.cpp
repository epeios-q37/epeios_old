/*
	'locale' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'eGeckoCOM' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'eGeckoCOM'.

    'eGeckoCOM' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eGeckoCOM' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eGeckoCOM'.  If not, see <http://www.gnu.org/licenses/>.
*/	
// $Id$

#include "locale.h"

#include <stdarg.h>

#include "flf.h"
#include "xtf.h"
#include "xml.h"
#include "fnm.h"
#include "dir.h"

#include "scllocale.h"

using namespace locale;

using cio::CErr;
using cio::COut;
using cio::CIn;

const char *locale::GetLabel( message__ Message )
{
#if	GLOBAL__MESSAGE_AMOUNT != 0
#	error "Amount of 'message__' entries changed ! Update !"
#endif

	switch( Message ) {
	default:
		ERRPrm();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

static struct global_cdtor {
	global_cdtor( void )
	{
		if ( LOCALE__MESSAGE_AMOUNT != m_amount )
			ERRChk();
	}
	~global_cdtor( void )
	{
	}
} GLOBALCDTor_;

