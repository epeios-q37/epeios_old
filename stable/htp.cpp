/*
	'htp' library by Claude SIMON (csimon@epeios.org)
	Requires the 'htp' header file ('htp.h').
	Copyright (C) 2002, 2004 Claude SIMON (csimon@epeios.org).

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

#define HTP__COMPILATION

#include "htp.h"

class htptutor
: public ttr_tutor
{
public:
	htptutor( void )
	: ttr_tutor( HTP_NAME )
	{
#ifdef HTP_DBG
		Version = HTP_VERSION "\b\bD $";
#else
		Version = HTP_VERSION;
#endif
		Owner = HTP_OWNER;
		Date = "$Date$";
	}
	virtual ~htptutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "str.h"

using namespace htp;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

#define CONTENT_LENGTH_STRING	"Content-Length: "
#define HTTP_SIGNATURE			"HTTP/1.1 "

static bso::bool__ TestHTTPVersion_( flw::iflow__ &IFlow )
{
	int Pos = 0;
	const char *Signature = HTTP_SIGNATURE;

	while( Signature[Pos]
		   && ( IFlow.Get() == Signature[Pos++] ) );

	return !Signature[Pos];
}

static bso::ushort__ GetHTTPResponseCode_( flw::iflow__ &IFlow )
{
	bso::char__ C;
	bso::ushort__ Code = 0;

	while( isdigit( C = IFlow.Get() ) )
		Code = Code * 10 + C - '0';

	return Code;
}

static void GetHeader_(
	flw::iflow__ &IFlow,
	str::string_ &Header )
{
	bso::bool__ Continue = true;
	bso::ubyte__ State = 0;
	bso::char__ C;

	do {
		C = IFlow.Get();

		Header.Append( C );

		if ( C == '\r' )
			if ( State == 2 )
				State = 3;
			else
				State = 1;
		else if ( C == '\n' )
			if ( State == 1 )
				State = 2;
			else if ( State == 3 )
				Continue = false;
			else
				State = 0;
		else
			State = 0;
	} while ( Continue );
}

static void GetContentLengthValue_(
	const str::string_ &RawHeader,
	http_header__ &Header )
{
	epeios::row__ P = NONE;

	Header.ContentLength = 0;

	P = *RawHeader.Search( str::string( CONTENT_LENGTH_STRING ) );

	if ( P == NONE )
		ERRf();

	*P += sizeof( CONTENT_LENGTH_STRING ) - 1;
	
	while ( ( P != NONE ) && ( isdigit( RawHeader( P ) ) ) ) {
		Header.ContentLength = Header.ContentLength * 10 + RawHeader( P ) - '0';
		P = RawHeader.Next( P );
	}
	
	if ( P == NONE )
		ERRf();
}

static void FillField_(
	flw::iflow__ &IFlow,
	http_header__ &Header )
{
ERRProlog
	str::string RawHeader;
ERRBegin
	RawHeader.Init();

	GetHeader_( IFlow, RawHeader );

	GetContentLengthValue_(	RawHeader, Header );
ERRErr
ERREnd
ERREpilog
}

static void StripHeader_( flw::iflow__ &IFlow )
{
ERRProlog
	str::string RawHeader;
ERRBegin
	RawHeader.Init();

	GetHeader_( IFlow, RawHeader );
ERRErr
ERREnd
ERREpilog
}


htp::status htp::Parse(
	flw::iflow__ &IFlow,
	http_header__ &Header )
{
	htp::status Status = htp::sUnknow;

	if ( TestHTTPVersion_( IFlow ) ) {
		switch( GetHTTPResponseCode_( IFlow ) ) {
		case 100:
			Status = htp::sContinue;
			StripHeader_( IFlow );
			break;
		case 200:
			Status = htp::sOK;
			FillField_( IFlow, Header );
			break;
		case 404:
			Status = htp::sNotFound;
			break;
		default:
			Status = htp::sUnhandledCode;
			break;
		}
	} else
		Status = sUnknowProtocol;

	return Status;
}

#define NL	"\r\n";

void htp::Post(
	const str::string_ &URL,
	const str::string_ &Host,
	const str::string_ &UserAgent,
	const str::string_ &ContentType,
	const str::string_ &Content,
	txf::text_oflow__ &Flow )
{
	Flow << "POST " << URL << " HTTP/1.0" << NL;
	Flow << "Host: " << Host << NL;
	Flow << "User-Agent: " << UserAgent << NL;
	Flow << "Content-Type: " << ContentType << NL;
	Flow << "Content-Length: " << Content.Amount() << NL;
	Flow << NL;

	Flow << Content;
}





class htppersonnalization
: public htptutor
{
public:
	htppersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~htppersonnalization( void )
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

static htppersonnalization Tutor;

ttr_tutor &HTPTutor = Tutor;
