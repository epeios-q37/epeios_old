/*
	'csdsns' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'csdsns' header file ('csdsns.h').
	Copyright (C) 2004 Claude SIMON.

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

#define CSDSNS__COMPILATION

#include "csdsns.h"

class csdsnstutor
: public ttr_tutor
{
public:
	csdsnstutor( void )
	: ttr_tutor( CSDSNS_NAME )
	{
#ifdef CSDSNS_DBG
		Version = CSDSNS_VERSION "\b\bD $";
#else
		Version = CSDSNS_VERSION;
#endif
		Owner = CSDSNS_OWNER;
		Date = "$Date$";
	}
	virtual ~csdsnstutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace csdsns;

#define CASE( n )\
	case l##n:\
		return #n;\
		break


const char *csdsns::GetLogLabel( log__ Log )
{
	switch ( Log ) {
		CASE( New );
		CASE( Store );
		CASE( TestAndGet );
		CASE( Delete );
	default:
		ERRu();
		return NULL;	// Pour éviter un 'warning'.
		break;
	}
}

namespace {

	class functions__
	: public csdbns::user_functions__
	{
	private:
		core_ *_Core;
		user_functions__ *_Functions;
	protected:
		virtual void *CSDPreProcess( flw::ioflow__ &Flow )
		{
			return NULL;
		}
		virtual action__ CSDProcess(
			flw::ioflow__ &Flow,
			void *UP )
		{
#ifdef CSDSNS_DBG
			if ( UP != NULL )
				ERRc();
#endif
			id__ Id = CSDSNS_UNDEFINED;
			action__ Action = aContinue;

			UP = NULL;

			Flow.Read( sizeof( Id ), &Id );

			if ( Id == CSDSNS_UNDEFINED ) {
				Id = _Core->New();
				Flow.Write( &Id, sizeof( Id ) );
				UP = _Functions->PreProcess( Flow );
				_Core->Store( UP, Id );
				_Functions->Process( Flow, UP );
			} else {
				if ( !_Core->TestAndGet( Id, UP ) ) {
					Flow.Put( (flw::datum__)-1 );
					Flow.Synchronize();
					Action = aStop;
				} else {
					Flow.Put( 0 );
					Action = _Functions->Process( Flow, UP );
				}

				switch ( Action ) {
				case aContinue:
					break;
				case aStop:
					if ( UP != NULL )
						_Functions->PostProcess( UP );
					if ( Id != CSDSNS_UNDEFINED )
						_Core->Delete( Id );
					break;
				default:
					ERRu();
					break;
				}
			}

			return aContinue;
		}
		virtual void CSDPostProcess( void *UP )
		{
#ifdef CSDSNS_DBG
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

void csdsns::server___::Process(
		user_functions__ &UFunctions,
		core_ &Core,
		sck::duration__ TimeOut )
{
	functions__ Functions;

	Functions.Init( Core, UFunctions );

	_Server.Process( Functions, TimeOut );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class csdsnspersonnalization
: public csdsnstutor
{
public:
	csdsnspersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~csdsnspersonnalization( void )
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

static csdsnspersonnalization Tutor;

ttr_tutor &CSDSNSTutor = Tutor;
