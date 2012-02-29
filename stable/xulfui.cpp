/*
	'xulfui' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfui' header file ('xulfui.h').
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

#define XULFUI__COMPILATION

#include "xulfui.h"

class xulfuitutor
: public ttr_tutor
{
public:
	xulfuitutor( void )
	: ttr_tutor( XULFUI_NAME )
	{
#ifdef XULFUI_DBG
		Version = XULFUI_VERSION "\b\bD $";
#else
		Version = XULFUI_VERSION;
#endif
		Owner = XULFUI_OWNER;
		Date = "$Date$";
	}
	virtual ~xulfuitutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"

#include "lstbch.h"
#include "lstctn.h"

using namespace xulfui;

struct _bundle__
{
public:
	pseudo_event_handler__ Handler;
	void *UP;
	void reset( bso::bool__ = true )
	{
		Handler = NULL;
		UP = NULL;
	}
	E_CDTOR( _bundle__ );
	void Init( void )
	{
		reset();
	}
	void Init(
		pseudo_event_handler__ Handler,
		void *UP )
	{
		reset();

		this->Handler = Handler;
		this->UP = UP;
	}
};

E_ROW( _brow__ );	// Bundle row.

typedef lstbch::E_LBUNCHt_( _bundle__, _brow__ ) _bundles_;
E_AUTO( _bundles );

static _bundles Bundles_;

typedef lstctn::E_LXMCONTAINERt_( str::string_, _brow__ ) _ids_;
E_AUTO( _ids );

static _ids Ids_;

static _brow__ Search_( const char *TargetId )
{
	ctn::E_CMITEMt( str::string_, _brow__ ) Id;
	_brow__ Row = Ids_.First();

	Id.Init( Ids_ );

	while ( ( Row != NONE ) && ( Id( Row ) != TargetId ) )
		Row = Ids_.Next( Row );

	return Row;
}

void xulfui::Add(
	const char *Id,
	pseudo_event_handler__ Handler,
	void *UP )
{
	_bundle__ Bundle;
	_brow__ Row = Search_( Id );

	if ( Row != NONE )
		ERRc();

	Row = Ids_.New();

	Bundle.Init( Handler, UP );

	Ids_.Store( str::string( Id ), Row );
	Bundles_.Store( Bundle, Row );
}

bso::bool__ xulfui::Launch(
	const char *Id,
	nsIDOMElement *Element )
{
	_brow__ Row = Search_( Id );
	_bundle__ Bundle;

	if ( Row == NONE )
		ERRc();

	Bundle.Init();

	Bundles_.Recall( Row, Bundle );

	if ( Bundle.Handler == NULL )
		ERRc();

	Bundle.Handler( Element, Bundle.UP );

	return true;
}

void xulfui::Remove( const char *Id )
{
	_brow__ Row = Search_( Id );

	if ( Row == NONE )
		ERRc();

	Ids_.Delete( Row );
	Bundles_.Delete( Row );
}

bso::bool__ xulfui::user_functions__::GECKOORegister(
	nsIDOMWindow *Window,
	const str::string_ &Id )
{
	bso::bool__ Success = false;
ERRProlog
	err::buffer__ Buffer;
ERRBegin
	Success = XULFUIRegister( Window, Id );
ERRErr
	_Trunk->UI().Alert( err::Message( Buffer ) );
	ERRRst();
ERREnd
ERREpilog
	return Success;
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfuipersonnalization
: public xulfuitutor
{
public:
	xulfuipersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */

		Bundles_.Init();
		Ids_.Init();
	}
	~xulfuipersonnalization( void )
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

static xulfuipersonnalization Tutor;

ttr_tutor &XULFUITutor = Tutor;
