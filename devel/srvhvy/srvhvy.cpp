/*
	'srvhvy' library by Claude SIMON (csimon@epeios.org)
	Requires the 'srvhvy' header file ('srvhvy.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

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

#define SRVHVY__COMPILATION

#include "srvhvy.h"

class srvhvytutor
: public ttr_tutor
{
public:
	srvhvytutor( void )
	: ttr_tutor( SRVHVY_NAME )
	{
#ifdef SRVHVY_DBG
		Version = SRVHVY_VERSION "\b\bD $";
#else
		Version = SRVHVY_VERSION;
#endif
		Owner = SRVHVY_OWNER;
		Date = "$Date$";
	}
	virtual ~srvhvytutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace srvhvy;

namespace {

	class functions__
	: public srv::flow_functions__
	{
	private:
		core_ *_Core;
		user_functions__ *_Functions;
	protected:
		virtual void *SRVPreProcess( flw::ioflow__ &Flow )
		{
			return NULL;
		}
		virtual action__ SRVProcess(
			flw::ioflow__ &Flow,
			void *UP )
		{
#ifdef SRVHVY_DBG
			if ( UP != NULL )
				ERRc();
#endif
			id__ Id = SRVHVY_UNDEFINED;
			action__ Action = aContinue;

			UP = NULL;

			Flow.Read( sizeof( Id ), &Id );

			if ( Id == SRVHVY_UNDEFINED ) {
				Id = _Core->New();
				Flow.Write( &Id, sizeof( Id ) );
				UP = _Functions->PreProcess( Flow );
				_Core->Store( UP, Id );
			} else {
				if ( !_Core->Exists( Id ) ) {
					Flow.Put( -1 );
					Flow.Synchronize();
					Action = aStop;
				} else {
					Flow.Put( 0 );
					Action = _Functions->Process( Flow, UP = _Core->Get( Id ) );
				}

				switch ( Action ) {
				case aContinue:
					break;
				case aStop:
					if ( UP != NULL )
						_Functions->PostProcess( UP );
					break;
				default:
					ERRu();
					break;
				}
			}

			return Action;
		}
		virtual void SRVPostProcess( void *UP )
		{
#ifdef SRVHVY_DBG
			ERRc();
#endif
		}
	public:
		void Init(
			core_ &Core,
			user_functions__ &Functions )
		{
			_Core = &Core;
			_Functions = &Functions;
		}
	};
}

void srvhvy::server___::Process(
		service__ Service,
		user_functions__ &UFunctions,
		core_ &Core )
{
	functions__ Functions;

	Functions.Init( Core, UFunctions );

	_Server.Init( Service );

	_Server.Process(  Functions );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class srvhvypersonnalization
: public srvhvytutor
{
public:
	srvhvypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~srvhvypersonnalization( void )
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

static srvhvypersonnalization Tutor;

ttr_tutor &SRVHVYTutor = Tutor;
