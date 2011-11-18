/*
	'geckob' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'geckob' header file ('geckob.h').
	Copyright (C) 20112004 Claude SIMON.

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

#define GECKOB__COMPILATION

#include "geckob.h"

class geckobtutor
: public ttr_tutor
{
public:
	geckobtutor( void )
	: ttr_tutor( GECKOB_NAME )
	{
#ifdef GECKOB_DBG
		Version = GECKOB_VERSION "\b\bD $";
#else
		Version = GECKOB_VERSION;
#endif
		Owner = GECKOB_OWNER;
		Date = "$Date$";
	}
	virtual ~geckobtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "mtx.h"

using namespace geckob;

static mtx::mutex_handler__ Mutex_ = MTX_INVALID_HANDLER;
static geckoo::user_functions__ *LoneSteering_ = NULL;

#ifdef CPE__T_MS
# define FUNCTION_SPEC __declspec(dllexport)
#else
#define FUNCTION_SPEC
# endif

#define DEF( name, function ) extern "C" FUNCTION_SPEC function name

DEF( GECKOO_RETRIEVE_STEERING_FUNCTION_NAME, geckoo::retrieve_steering );
DEF( GECKOO_RETRIEVE_LONE_STEERING_FUNCTION_NAME, geckoo::retrieve_lone_steering );
DEF( GECKOO_RELEASE_STEERING_FUNCTION_NAME, geckoo::release_steering );

#if 0
#ifdef CPE__T_MS

#include <windows.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		default:
			break;
    }
    return TRUE;
}
#endif
#endif

geckoo::user_functions__ *GECKOORetrieveSteering( geckoo::shared_data__ *Data )
{
	if ( Data == NULL )
		ERRu();

	if ( strcmp( Data->Version, GECKOO_SHARED_DATA_VERSION ) )
		ERRu();

	if ( Data->Control != Data->ControlComputing() )
		ERRc();

	return geckob::GECKOBRetrieveSteering( Data );
}

geckoo::user_functions__ *GECKOORetrieveLoneSteering( void )
{
	geckoo::user_functions__ *Steering = NULL;

	if ( !mtx::IsLocked( Mutex_ ) )
		ERRc();

	if ( LoneSteering_ == NULL )
		ERRc();

	Steering = LoneSteering_;
	LoneSteering_ = NULL;

	mtx::Unlock( Mutex_ );

	return Steering;

}

void GECKOOReleaseSteering( geckoo::user_functions__ *Steering )
{
	geckob::GECKOBReleaseSteering( Steering );
}

void geckob::SetLoneSteering( geckoo::user_functions__ &Steering )
{
	mtx::Lock( Mutex_ );

	if ( LoneSteering_ != NULL )
		ERRc();

	LoneSteering_ = &Steering;
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class geckobpersonnalization
: public geckobtutor
{
public:
	geckobpersonnalization( void )
	{
		Mutex_ = mtx::Create( mtx::mFree );
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~geckobpersonnalization( void )
	{
		if ( Mutex_ != MTX_INVALID_HANDLER )
			mtx::Delete( Mutex_ );
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

static geckobpersonnalization Tutor;

ttr_tutor &GECKOBTutor = Tutor;
