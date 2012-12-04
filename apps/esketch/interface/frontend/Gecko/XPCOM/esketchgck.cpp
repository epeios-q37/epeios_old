/*
	'esketchgck.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "trunk.h"
#include "ui.h"

#include "sclgecko.h"
#include "sclrgstry.h"
#include "scllocale.h"

#include "binding.h"

const char *sclgecko::TargetName = APP_NAME;

geckoo::steering_callback__ *sclgecko::CreateSteering(
	const char *LauncherIdentification,
	const lcl::locale_ &Locale,
	const char *Language,
	void *UP )
{
	binding::binding_callback__ *Callback = NULL;
ERRProlog
ERRBegin
	if ( ( Callback = new binding::binding_callback__ ) == NULL )
		ERRa();

	Callback->Init( LauncherIdentification, sclrgstry::GetRegistry(), sclrgstry::GetRoot(), Locale, Language );
ERRErr
	if ( Callback != NULL )
		delete Callback;

	Callback = NULL;

	ERRRst();	// Error catched to avoid that it goes further.
				// Error reported by the fact that the returned value is 'NULL'.
ERREnd
ERREpilog
	return Callback;
}


void sclgecko::DeleteSteering( geckoo::steering_callback__ *Steering )
{
	if ( Steering != NULL )
		delete Steering;
}
