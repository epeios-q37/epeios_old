/*
	'xulfkl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfkl' header file ('xulfkl.h').
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

#define XULFKL__COMPILATION

#include "xulfkl.h"

class xulfkltutor
: public ttr_tutor
{
public:
	xulfkltutor( void )
	: ttr_tutor( XULFKL_NAME )
	{
#ifdef XULFKL_DBG
		Version = XULFKL_VERSION "\b\bD $";
#else
		Version = XULFKL_VERSION;
#endif
		Owner = XULFKL_OWNER;
		Date = "$Date$";
	}
	virtual ~xulfkltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xulfkl;

#define CASE( message )	\
	case m##message:\
		Message = #message;\
		break

static const char *GetRawMessage_( xulfkl::message__ MessageId )
{

#if XULFKL_M_AMOUNT != 2
#	error "'message__' changed !"
#endif

	const char *Message = NULL;

	switch ( MessageId ) {
	CASE( SelectProjectFile );
	CASE( ExitConfirmation );
	default:
		ERRu();
		break;
	}

	return Message;
}

static const str::string_ &GetMessage_(
	message__ Message,
	const lcl::rack__ &LocaleRack,
	str::string_ &Translation )
{
	LocaleRack.GetTranslation( GetRawMessage_( Message ), XULFKL_NAME "_", Translation );

	return Translation;
}

static const char *GetMessage_(
	message__ Message,
	const lcl::rack__ &LocaleRack,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	GetMessage_( Message, LocaleRack, Translation );

	Translation.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer();
}

const str::string_ &xulfkl::GetTranslation(
	message__ MessageId,
	const lcl::rack__ LocaleRack,
	str::string_ &Message  )
{
	return GetMessage_( MessageId, LocaleRack, Message );
}

const char *xulfkl::GetTranslation(
	message__ MessageId,
	const lcl::rack__ LocaleRack,
	STR_BUFFER___ &Buffer  )
{
	return GetMessage_( MessageId, LocaleRack, Buffer );
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfklpersonnalization
: public xulfkltutor
{
public:
	xulfklpersonnalization( void )
	{
		if ( XULFKL_M_AMOUNT != m_amount )
			ERRc();

		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfklpersonnalization( void )
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

static xulfklpersonnalization Tutor;

ttr_tutor &XULFKLTutor = Tutor;
