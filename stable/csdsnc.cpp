/*
	'csdsnc' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'csdsnc' header file ('csdsnc.h').
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



//	$Id$

#define CSDSNC__COMPILATION

#include "csdsnc.h"

class csdsnctutor
: public ttr_tutor
{
public:
	csdsnctutor( void )
	: ttr_tutor( CSDSNC_NAME )
	{
#ifdef CSDSNC_DBG
		Version = CSDSNC_VERSION "\b\bD $";
#else
		Version = CSDSNC_VERSION;
#endif
		Owner = CSDSNC_OWNER;
		Date = "$Date$";
	}
	virtual ~csdsnctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "mtk.h"

using namespace csdsnc;

#define CASE( n )\
	case l##n:\
		return #n;\
		break

const char *csdsnc::GetLogLabel( log__ Log )
{
	switch ( Log ) {
		CASE( Creation );
		CASE( Retrieval );
		CASE( Release );
	default:
		ERRu();
		return NULL;	// Pour éviter un 'warning'.
		break;
	}
}

static void Ping_(
	_flow___ &Flow,
	time_t Delay )
{
	if ( ( tol::Clock( false ) - Flow.TimeStamp() ) >= Delay )
		if ( !Flow.OFlowIsLocked() ) {
			csdsnb::PutId( CSDSNB_PING, Flow );
			Flow.Commit();

			if ( Flow.Get() != 0 )
				ERRf();

			Flow.Dismiss();
		}
}

void csdsnc::core_::Ping( void )
{
	_Lock( S_.Mutex );

	stk::row__ Row = Flows.First();
	
	while ( Row != NONE )
	{
		Ping_( *Flows( Row ), S_.PingDelay );

		Row = Flows.Next( Row );
	}

	_Unlock( S_.Mutex );
}

static void KeepAlive_( void *UP )
{
	csdsnc::core_ &Core = *(csdsnc::core_ *)UP;

	while ( 1 ) {
		tht::Suspend( CSDSNC_PING_RESOLUTION );	// Les test sont réalisés toutes les minutes.

		Core.Ping();
	}
}

void csdsnc::core_::_KeepAlive( time_t Delay )
{
	if ( Delay <= CSDSNC_PING_RESOLUTION )	// Le test est réalisé toutes les minutes.
		ERRu();

	mtk::Launch( ::KeepAlive_, this );
}

void csdsnc::core_::_DeleteFlows( void )
{
	while ( Flows.Amount() != 0 )
	{
		PutId( CSDSNB_CLOSE, *Flows.Top() );
		delete Flows.Pop();
	}
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdsncpersonnalization
: public csdsnctutor
{
public:
	csdsncpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csdsncpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static csdsncpersonnalization Tutor;

ttr_tutor &CSDSNCTutor = Tutor;
