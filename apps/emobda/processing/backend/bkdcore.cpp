/*
	'bkdcore.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "bkdcore.h"

using namespace bkdcore;

#ifdef CPE__T_LIBRARY
#	define BKDCORE__LIBRARY
#endif

#ifdef BKDCORE__LIBRARY
#	include "bkdlrm.h"
#else
#	include "bkdrrm.h"
#	include "cio.h"
#endif

typedef bkdmng::text_log_functions__	log_functions__;

struct data {
	backend Backend;
	log_functions__ LogFunctions;
	cio::aware_cout___ LogFlow;
	data( void )
	: LogFunctions( LogFlow )
	{
	}
};

typedef data	data_;

static bso::bool__ Process_(
	flw::ioflow__ &Client,
	backend_ &Backend,
	bkdmng::log_functions__ &LogFunctions )
{
	bso::bool__ ReturnValue;
ERRProlog
#ifdef BKDCORE__LIBRARY
	bkdlrm::local_request_manager Request;
#else
	bkdrrm::remote_request_manager Request;
#endif
ERRBegin
	Request.Init( Client );

	ReturnValue = Backend.Handle( Request, NULL, LogFunctions );
ERRErr
ERREnd
ERREpilog
	return ReturnValue;
}

void *backend_functions__::CSDPreProcess( flw::ioflow__ &Flow )
{
	data_ *Data = NULL;
ERRProlog
ERRBegin
	Data = new data();

	if ( Data == NULL )
		ERRa();

	Data->Backend.Init( *_Locales );
ERRErr
	if ( Data != NULL )
		delete Data;
ERREnd
ERREpilog
	return Data;
}

csdscm::action__ backend_functions__::CSDProcess(
	flw::ioflow__ &Flow,
	void *UP )
{
	data_ &Data = *(data *)UP;

	if ( Process_( Flow, Data.Backend, Data.LogFunctions ) )
		return csdscm::aContinue;
	else
		return csdscm::aStop;
}

void backend_functions__::CSDPostProcess( void *UP )
{
	delete (data *)UP;
}





