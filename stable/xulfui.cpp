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

typedef pseudo_event_callback__	*_handler__;

E_ROW( _hrow__ );	// Handler row.

typedef lstbch::E_LBUNCHt_( _handler__ , _hrow__ ) _handlers_;
E_AUTO( _handlers );

static _handlers Handlers_;

typedef lstctn::E_LXMCONTAINERt_( str::string_, _hrow__ ) _ids_;
E_AUTO( _ids );

static _ids Ids_;

static _hrow__ Search_( const char *TargetId )
{
	ctn::E_CMITEMt( str::string_, _hrow__ ) Id;
	_hrow__ Row = Ids_.First();

	Id.Init( Ids_ );

	while ( ( Row != NONE ) && ( Id( Row ) != TargetId ) )
		Row = Ids_.Next( Row );

	return Row;
}

void xulfui::Add(
	const char *Id,
	pseudo_event_callback__ &Callback )
{
	_hrow__ Row = Search_( Id );

	if ( Row != NONE )
		ERRc();

	Row = Ids_.New();

	Ids_.Store( str::string( Id ), Row );

	if ( Handlers_.Add( &Callback ) != Row )
		ERRc();
}

bso::bool__ xulfui::Launch(
	const char *Id,
	nsIDOMElement *Element )
{
	_hrow__ Row = Search_( Id );
	_handler__ Handler = NULL;

	if ( Row == NONE )
		ERRc();

	Handlers_.Recall( Row, Handler );

	if ( Handler == NULL )
		ERRc();

	Handler->Handle( Element );

	return true;
}

void xulfui::Remove( const char *Id )
{
	_hrow__ Row = Search_( Id );

	if ( Row == NONE )
		ERRc();

	Ids_.Delete( Row );
	Handlers_.Delete( Row );
}

bso::bool__ xulfui::steering_callback__::GECKOORegister(
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

nsIAutoCompleteResult *xulfui::steering_callback__::GECKOOGetAutoCompleteResult( const nsAString &RawSearchParam )
{
	nsIAutoCompleteResult *Target = NULL;
ERRProlog
	str::string SearchParam;
ERRBegin
	SearchParam.Init();

	nsxpcm::Transform( RawSearchParam, SearchParam );

	Target = nsxpcm::Get( SearchParam);

	if ( Target == NULL )
		ERRc();
ERRErr
ERREnd
ERREpilog
	return Target;	
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

		Handlers_.Init();
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
