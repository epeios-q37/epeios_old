/*
	'backend.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "backend.h"

using namespace backend;

static struct data {
	backend Backend;
};

bso::bool__ gesbib::Process(
	flw::ioflow__ &Client,
	backend_ &Backend )
{
	bso::bool__ ReturnValue;
ERRProlog
#ifdef CPE__T_LIBRARY
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

void *gesbib::server_functions__::CSDPreProcess( flw::ioflow__ &Flow )
{
	data__ *Data = NULL;
ERRProlog
ERRBegin
	Data = new data__( *_FlowFunctions );

	if ( Data == NULL )
		ERRa();

	Data->Backend.Init();
	Data->Backend.SetLanguage( bkdlgg::lFrench );
ERRErr
	if ( Data != NULL )
		delete Data;
ERREnd
ERREpilog
	return Data;
}

csdscm::action__ gesbib::server_functions__::CSDProcess(
	flw::ioflow__ &Flow,
	void *UP )
{
	data__ &Data = *(data__ *)UP;

	if ( gesbib::Process( Flow, Data.Backend, Data.LogFunctions ) )
		return csdscm::aContinue;
	else
		return csdscm::aStop;
}

void gesbib::server_functions__::CSDPostProcess( void *UP )
{
	delete (data__ *)UP;
}





