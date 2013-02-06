/*
	'esketchbkd.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "scldaemon.h"
#include "sclrgstry.h"
#include "scllocale.h"

#include "sktinf.h"

#include "frdkernl.h"

#include "cgigat.h"
#include "ssnmng.h"

#define NAME	SKTINF_LC_AFFIX	"cgi"
#define VERSION	"0.1.0"

const char *scldaemon::TargetName = NAME;

typedef cgigat::cgi_gate__ _gate__;

typedef frdkernl::kernel___ kernel___;

typedef frdkrn::reporting_functions__ _reporting_functions__;

class reporting_functions__
: public _reporting_functions__
{
protected:
	virtual void FRDKRNReportBackendError( const str::string_ &Message ) {}
	virtual void FRDKRNReportFrontendError( const str::string_ &Message ) {}
};

namespace sclcgi {
	enum session_handling__ 
	{
		shKeepAlive,
		shClose,
		sh_amount,
		sh_Undefined
	};

	void *SCLCGINewSession( const cgiarg::arguments_ &Arguments );
	session_handling__ SCLCGIHandleQuery(
		void *UP,
		const cgiarg::arguments_ &Arguments,
		xml::writer_ &Writer,
		str::string_ &XMLFileName );
	void SCLCGIReportEpiredSession(
		xml::writer_ &Writer,
		str::string_ &XMLFileName );
	void SCLCGIDeleteSession( void *UP );
}

class instance___
{
public:
	void Init( void )
	{}
};

void *sclcgi::SCLCGINewSession( const cgiarg::arguments_ &Arguments )
{
	instance___ *Instance = NULL;
ERRProlog
ERRBegin
	Instance = new instance___;

	if ( Instance == NULL )
		ERRa();

	Instance->Init();
ERRErr
	if ( Instance != NULL )
		delete Instance;

	Instance = NULL;
ERREnd
ERREpilog
	return Instance;
}

sclcgi::session_handling__ sclcgi::SCLCGIHandleQuery(
		void *UP,
		const cgiarg::arguments_ &Arguments,
		xml::writer_ &Writer,
		str::string_ &XMLFileName )
{
	Writer.PushTag( "Coucou" );
	XMLFileName = "Test";

	return sclcgi::shKeepAlive;
}

void sclcgi::SCLCGIReportEpiredSession(
	xml::writer_ &Writer,
	str::string_ &XMLFileName )
{
	Writer.PushTag( "Coucou" );
	XMLFileName = "Test";
}


void sclcgi::SCLCGIDeleteSession( void *UP )
{
	if( UP == NULL )
		ERRc();

	delete (instance___ *)UP;
}

typedef ssnmng::user_functions__ _user_functions__;

class user_functions__
: public _user_functions__
{
protected:
	virtual void SSNMNGDelete( void *UP )
	{
		sclcgi::SCLCGIDeleteSession( UP );
	}
};


static class gate__
: public _gate__
{
private:
	kernel___ _Kernel;
	ssnmng::sessions_manager _Sessions;
	::user_functions__ _UserFunctions;
	reporting_functions__ _ReportingFunctions;
protected:
	virtual void CGIDATProcess(
		const cgiarg::arguments_ &Arguments,
		xml::writer_ &Writer,
		str::string_ &XSLFileName )
	{
	ERRProlog
		void *UP = NULL;
		ssnmng::row__ SessionRow = NONE;
		cgiarg::row__ ArgRow = NONE;
		str::string RawSession;
		ssnmng::session_id__ Session;
	ERRBegin
		ArgRow = Arguments.Locate( "_session" );
		RawSession.Init();

		if ( ArgRow == NONE )
			SessionRow = _Sessions.New( sclcgi::SCLCGINewSession( Arguments ) );
		else if ( ( SessionRow = _Sessions.Search( Arguments.GetValue( ArgRow, RawSession ) ) ) == NONE )
			sclcgi::SCLCGIReportEpiredSession( Writer, XSLFileName );
		else if ( _Sessions.IsExpired( SessionRow ) ) {
			_Sessions.Close( SessionRow );	// Appelle 'sclcgi::SCLCGI::DeleteSession.
			sclcgi::SCLCGIReportEpiredSession( Writer, XSLFileName );
			SessionRow = NONE;
		}

		if ( SessionRow != NONE ) {
			Session.Init();
			_Sessions.SessionID( SessionRow );

			switch ( sclcgi::SCLCGIHandleQuery( UP, Arguments, Writer, XSLFileName ) ) {
			case sclcgi::shKeepAlive:
				break;
			case sclcgi::shClose:
				_Sessions.Close( SessionRow );	// Appelle 'sclcgi::SCLCGI::DeleteSession.
				break;
			default:
				ERRc();
				break;
			}
		}
	ERRErr
	ERREnd
	ERREpilog
	}
public:
	void reset( bso::bool__ P = true )
	{
		_gate__::reset( P );
		_ReportingFunctions.reset( P );
		_Sessions.reset( P );
		_UserFunctions.reset( P );
		_Kernel.reset( P );
	}
	E_CVDTOR( gate__ )
	void Init( void )
	{
		_gate__::Init();
		_ReportingFunctions.Init( _Kernel );
		_Kernel.Init( sclrgstry::GetRegistry(), sclrgstry::GetRoot(), NAME, scllocale::GetLocale(), "en", _ReportingFunctions );
		_UserFunctions.Init();
		_Sessions.Init( 60, 60, _UserFunctions );
	}
} _Gate;

csdleo::user_functions__ *scldaemon::RetrieveSteering(
	fblbur::mode__ Mode,
	void *UP,
	const lcl::locale_ &Locale )
{
	csdleo::user_functions__ *Functions = NULL;
ERRProlog
ERRBegin
	_Gate.Init();

	Functions = &_Gate;
ERRErr
	ERRRst();

	Functions = NULL;
ERREnd
ERREpilog
	return Functions;
}


void scldaemon::ReleaseSteering( csdleo::user_functions__ *Steering )
{
	if ( Steering != NULL )
		delete Steering;
}

