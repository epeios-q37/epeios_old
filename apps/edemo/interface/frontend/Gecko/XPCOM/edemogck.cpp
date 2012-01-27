/*
	'edemogck.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eDemo' software.

    'eDemo' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eDemo' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eDemo'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "trunk.h"
#include "ui.h"

#include "sclgecko.h"
#include "sclrgstry.h"
#include "scllocale.h"

#include "binding.h"

const char *sclgecko::TargetName = NAME;

geckoo::user_functions__ *sclgecko::CreateSteering(
	const char *LauncherIdentification,
	const lcl::locale_ &Locale,
	const char *Language,
	void *UP )
{
	binding::binding_functions__ *Functions = NULL;
ERRProlog
ERRBegin
	if ( ( Functions = new binding::binding_functions__ ) == NULL )
		ERRa();

	Functions->Init( LauncherIdentification, sclrgstry::GetRegistry(), Locale, Language );
ERRErr
	if ( Functions != NULL )
		delete Functions;

	Functions = NULL;

	ERRRst();	// Error catched to avoid that it goes further.
				// Error reported by the fact that the returned value is 'NULL'.
ERREnd
ERREpilog
	return Functions;
}


void sclgecko::DeleteSteering( geckoo::user_functions__ *Steering )
{
	if ( Steering != NULL )
		delete Steering;
}

