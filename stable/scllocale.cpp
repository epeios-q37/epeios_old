/*
	'scllocale' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'scllocale' header file ('scllocale.h').
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

#define SCLLOCALE__COMPILATION

#include "scllocale.h"

class scllocaletutor
: public ttr_tutor
{
public:
	scllocaletutor( void )
	: ttr_tutor( SCLLOCALE_NAME )
	{
#ifdef SCLLOCALE_DBG
		Version = SCLLOCALE_VERSION "\b\bD $";
#else
		Version = SCLLOCALE_VERSION;
#endif
		Owner = SCLLOCALE_OWNER;
		Date = "$Date$";
	}
	virtual ~scllocaletutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "lcl.h"
#include "fnm.h"
#include "dir.h"
#include "cio.h"

using namespace scllocale;

#define DEFAULT_LANGUAGE	"en"

static lcl::locale Locale_;
static STR_BUFFER___ Language_;
static str::string Translation_;

static lcl::rack__ LocaleRack_( Locale_, Language_() );

const lcl::rack__ &scllocale::GetRack( void )
{
	return LocaleRack_;
}

static const str::string_ &GetTranslation_( const char *Message )	// NOTA : seulement à usage limité à cette bibiothèque. Ne pas confondre avec 'scllocale::GetTranslation(...)'.
{
	Translation_.Init();
	LocaleRack_.GetTranslation( Message, SCLLOCALE_NAME "_", Translation_ );

	return Translation_;
}

static void ReportLocaleFileParsingError_( const rgstry::context___ &Context )
{
ERRProlog
	str::string Message, EmbeddedMessage;
ERRBegin
	Message.Init( GetTranslation_( "LocaleFileParsingError" ) );

	EmbeddedMessage.Init();

	lcl::ReplaceTag( Message, 1, rgstry::GetTranslation( Context, LocaleRack_, EmbeddedMessage ) );

	cio::CErr << Message << txf::nl << txf::commit;
ERRErr
ERREnd
ERREpilog
}

lcl::level__ scllocale::Load(
	flw::iflow__ &Flow,
	const char *Directory,
	const char *RootPath )
{
	lcl::level__ Level = LCL_UNDEFINED_LEVEL;
ERRProlog
	STR_BUFFER___ STRBuffer;
	rgstry::context___ Context;
	str::string Language;
	xtf::extended_text_iflow__ XFlow;
ERRBegin
	XFlow.Init( Flow );

	Context.Init();

	if ( ( Level = Locale_.Push( XFlow, xpp::criterions___( str::string( Directory ) ), RootPath,  Context ) ) == LCL_UNDEFINED_LEVEL ) {
		if ( !cio::IsInitialized() )
			cio::Initialize( cio::t_Default );

		ReportLocaleFileParsingError_( Context );
		ERRExit( EXIT_FAILURE );
	}

	SetLanguage( str::string( DEFAULT_LANGUAGE ) );
ERRErr
ERREnd
ERREpilog
	return Level;
}

void scllocale::SetLanguage( const str::string_ &Language )
{
	LocaleRack_.Init( Locale_, Language.Convert( Language_ ) );
}

const str::string_ &scllocale::GetTranslation(
	const char *Message,
	str::string_ &Translation )
{
	LocaleRack_.GetTranslation( Message, "", Translation );

	return Translation;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class scllocalepersonnalization
: public scllocaletutor
{
public:
	scllocalepersonnalization( void )
	{
		Locale_.Init();
		Language_.Init();
		LocaleRack_.Init( Locale_, "" );
		Translation_.Init();
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~scllocalepersonnalization( void )
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

static scllocalepersonnalization Tutor;

ttr_tutor &SCLLOCALETutor = Tutor;
