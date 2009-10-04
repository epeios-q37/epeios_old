/*
	'uif' library by Claude SIMON (csimon@epeios.org)
	Requires the 'uif' header file ('uif.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

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

#define UIF__COMPILATION

#include "uif.h"

class uiftutor
: public ttr_tutor
{
public:
	uiftutor( void )
	: ttr_tutor( UIF_NAME )
	{
#ifdef UIF_DBG
		Version = UIF_VERSION "\b\bD $";
#else
		Version = UIF_VERSION;
#endif
		Owner = UIF_OWNER;
		Date = "$Date$";
	}
	virtual ~uiftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#ifdef CPE__T_MT
#	include "mtx.h"
#endif

#include "ids.h"

using namespace uif;

E_TYPEDEF( int, id__ );

static ids::ids_store<id__> Ids_;

#ifdef CPE__T_MT
static mtx::mutex_handler__ Mutex_;
#endif

id__ GetId_( void )
{
	id__ Id;
#ifdef CPE__T_MT
	mtx::Lock( ::Mutex_ );
#endif

	Id = Ids_.New();

#ifdef CPE__T_MT
	mtx::Unlock( ::Mutex_ );
#endif

	return Id;
}

static void ReleaseId_( int Id )
{
#ifdef CPE__T_MT
	mtx::Lock( ::Mutex_ );
#endif
	Ids_.Release( Id );
#ifdef CPE__T_MT
	mtx::Unlock( ::Mutex_ );
#endif
}

menu_item_id__ uif::eMenuItem::_GetId( void )
{
	return *::GetId_();
}

void uif::eMenuItem::_ReleaseId( menu_item_id__ Id )
{
	ReleaseId_( *Id );
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class uifpersonnalization
: public uiftutor
{
public:
	uifpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */

		Ids_.Init();
#ifdef CPE__T_MT
		::Mutex_ = mtx::Create( mtx::mFree );
#endif
	}
	~uifpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
#ifdef CPE__T_MT
		mtx::Delete( ::Mutex_ );
#endif
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static uifpersonnalization Tutor;

ttr_tutor &UIFTutor = Tutor;
