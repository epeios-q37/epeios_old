/*
	'xulftk' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulftk' header file ('xulftk.h').
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

#define XULFTK__COMPILATION

#include "xulftk.h"

class xulftktutor
: public ttr_tutor
{
public:
	xulftktutor( void )
	: ttr_tutor( XULFTK_NAME )
	{
#ifdef XULFTK_DBG
		Version = XULFTK_VERSION "\b\bD $";
#else
		Version = XULFTK_VERSION;
#endif
		Owner = XULFTK_OWNER;
		Date = "$Date$";
	}
	virtual ~xulftktutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xulftk;

#define PREFIX	XULFKL_NAME " " 

void xulftk::error_reporting_functions__::FBLFRDReportError(
	fblovl::reply__ Reply,
	const char *RawMessage )
{
ERRProlog
	str::string Message;
STR_BUFFER___ Buffer;
ERRBegin
	if ( _UI == NULL )
		ERRc();

	Message.Init( _Rack.GetTranslation( "ErrorType", PREFIX, Buffer ) );

	Message.Append( " : " );

	Message.Append( fblovl::GetTranslation( Reply, _Rack, Buffer ) );

	Message.Append( '\n' );

	Message.Append( _Rack.GetTranslation( "ErrorMessage", PREFIX, Buffer ) );

	Message.Append( " : " );

	Message.Append( RawMessage );

	_UI->LogAndPrompt( Message );
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulftkpersonnalization
: public xulftktutor
{
public:
	xulftkpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulftkpersonnalization( void )
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

static xulftkpersonnalization Tutor;

ttr_tutor &XULFTKTutor = Tutor;
