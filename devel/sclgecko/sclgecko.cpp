/*
	'sclgecko' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sclgecko' header file ('sclgecko.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

#define SCLGECKO__COMPILATION

#include "sclgecko.h"

class sclgeckotutor
: public ttr_tutor
{
public:
	sclgeckotutor( void )
	: ttr_tutor( SCLGECKO_NAME )
	{
#ifdef SCLGECKO_DBG
		Version = SCLGECKO_VERSION "\b\bD $";
#else
		Version = SCLGECKO_VERSION;
#endif
		Owner = SCLGECKO_OWNER;
		Date = "$Date$";
	}
	virtual ~sclgeckotutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "cio.h"

#include "sclmisc.h"
#include "scllocale.h"

using namespace sclgecko;

static bso::bool__ IsInitialized_ = false;

geckoo::user_functions__ *geckob::GECKOBCreateSteering( geckoo::shared_data__ *Data )
{
	geckoo::user_functions__ *Steering = NULL;
ERRProlog
ERRBegin
	if ( !IsInitialized_ )	{
		cio::COutDriver.Init( *Data->COut, fdr::ts_Default );
		cio::CErrDriver.Init( *Data->CErr, fdr::ts_Default );
		cio::CInDriver.Init( flx::VoidIFlowDriver, fdr::ts_Default );

		cio::Initialize( cio::tUser );

		// Do not work when placed in 'global_cdtor'.
		sclmisc::Initialize( TargetName, Data->Path );
		IsInitialized_ = true;
	}

	Steering = CreateSteering( Data->LauncherIdentification, scllocale::GetRack().Locale(), Data->Language, Data->UP );
ERRErr
	Steering = NULL;
	ERRRst();
ERREnd
	if ( cio::IsInitialized() ) {
		cio::COut << txf::commit;
		cio::CErr << txf::commit;
	}
ERREpilog
	return Steering;
}

void geckob::GECKOBDeleteSteering( geckoo::user_functions__ *Steering )
{
	DeleteSteering( Steering );
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclgeckopersonnalization
: public sclgeckotutor
{
public:
	sclgeckopersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclgeckopersonnalization( void )
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

static sclgeckopersonnalization Tutor;

ttr_tutor &SCLGECKOTutor = Tutor;
