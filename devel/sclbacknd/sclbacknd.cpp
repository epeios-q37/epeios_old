/*
	'sclbacknd' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sclbacknd' header file ('sclbacknd.h').
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

#define SCLBACKND__COMPILATION

#include "sclbacknd.h"

class sclbackndtutor
: public ttr_tutor
{
public:
	sclbackndtutor( void )
	: ttr_tutor( SCLBACKND_NAME )
	{
#ifdef SCLBACKND_DBG
		Version = SCLBACKND_VERSION "\b\bD $";
#else
		Version = SCLBACKND_VERSION;
#endif
		Owner = SCLBACKND_OWNER;
		Date = "$Date$";
	}
	virtual ~sclbackndtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "sclmisc.h"

#include "scllocale.h"

#include  "lcl.h"
#include "cio.h"

#include "csdleo.h"
#include "csdles.h"

#include "fblbur.h"

using namespace sclbacknd;

static bso::bool__ IsInitialized_ = false;

csdleo::user_functions__ *csdles::CSDLESRetrieveSteering( csdleo::shared_data__ *Data )
{
	csdleo::user_functions__ *Steering = NULL;
ERRProlog
	fblbur::mode__ Mode = fblbur::m_Undefined;
ERRBegin
	if ( !IsInitialized_ )	{
		cio::COutDriver.Init( *Data->COut, fdr::ts_Default );
		cio::CErrDriver.Init( *Data->CErr, fdr::ts_Default );
		cio::CInDriver.Init( flx::VoidIFlowDriver, fdr::ts_Default );

		cio::Initialize( cio::tUser );

		// Do not work when placed in 'global_cdtor'.
		sclmisc::Initialize( TargetName );
		IsInitialized_ = true;
	}

	switch ( Data->Mode ) {
	case csdleo::mEmbedded:
		Mode = fblbur::mEmbedded;
		break;
	case csdleo::mRemote:
		Mode = fblbur::mRemote;
		break;
	default:
		ERRc();
		break;
	}

	Steering = RetrieveSteering( Mode, Data->User, scllocale::GetRack().Locale() );
ERRErr
ERREnd
	if ( cio::IsInitialized() ) {
		cio::COut << txf::commit;
		cio::CErr << txf::commit;
	}
ERREpilog
	return Steering;
}

void csdles::CSDLESReleaseSteering( csdleo::user_functions__ *Steering )
{
	ReleaseSteering( Steering );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclbackndpersonnalization
: public sclbackndtutor
{
public:
	sclbackndpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclbackndpersonnalization( void )
	{
		sclmisc::Terminate();
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

static sclbackndpersonnalization Tutor;

ttr_tutor &SCLBACKNDTutor = Tutor;
