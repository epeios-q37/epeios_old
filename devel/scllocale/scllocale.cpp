/*
	'scllocale' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'scllocale' header file ('scllocale.h').
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

#include "sclrgstry.h"
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

const lcl::rack__ *sclrgstry::LocaleRack = &LocaleRack_;

const lcl::rack__ &scllocale::GetRack( void )
{
	return LocaleRack_;
}

static inline const str::string_ &GetLanguage_( str::string_ &Language )
{
	bso::bool__ Missing = false;

	if ( sclrgstry::IsRegistryReady() )
		sclrgstry::GetLanguage( Language, &Missing );
	else
		Missing = true;

	if ( Missing )
		Language = DEFAULT_LANGUAGE;

	return Language;
}

static bso::bool__ GuessLocaleFileName_(
	const char *Affix,
	const char *SuggestedPath,
	str::string_ &LocaleFileName )
{
	bso::bool__ Success = false;
ERRProlog
	STR_BUFFER___ STRBuffer;
	STR_BUFFER___ PathBuffer;
	FNM_BUFFER___ FNMBuffer;
ERRBegin
	LocaleFileName.Init( Affix );
	LocaleFileName.Append( LCL_DEFAULT_FILENAME_SUFFIX );

	if ( !fil::FileExists( LocaleFileName.Convert( STRBuffer ) ) ) {
		LocaleFileName.Init( fnm::BuildFileName( SuggestedPath, Affix, LCL_DEFAULT_FILENAME_SUFFIX, FNMBuffer ) );

		if ( !fil::FileExists( LocaleFileName.Convert( STRBuffer ) ) ) {
				LocaleFileName.Init( fnm::BuildFileName( dir::GetSelfPath( PathBuffer ), Affix, LCL_DEFAULT_FILENAME_SUFFIX, FNMBuffer ) );

				Success = fil::FileExists( LocaleFileName.Convert( STRBuffer ) );
		} else
			Success = true;
	} else
		Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
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

static void LoadLocale_(
	const str::string_ &SuggestedLocaleFileName,
	const char *Affix,
	const char *RegistryRootPath,
	const char *FileSuggestedPath,
	bso::bool__ IgnoreUnableToOpenFileError )
{
ERRProlog
	STR_BUFFER___ STRBuffer;
	rgstry::context___ Context;
	str::string Language;
	str::string LocaleFileName;
ERRBegin
	LocaleFileName.Init( SuggestedLocaleFileName );
	Context.Init();

	if ( ( LocaleFileName.Amount() != 0 ) || ( GuessLocaleFileName_( Affix, FileSuggestedPath, LocaleFileName ) ) )
		switch ( Locale_.Init( LocaleFileName.Convert( STRBuffer ), RegistryRootPath, Context ) ) {
		case rgstry::sOK:
			break;
		case rgstry::sUnableToOpenFile:
			if ( IgnoreUnableToOpenFileError )
				break;
		default:
			if ( !cio::IsInitialized() )
				cio::Initialize( cio::t_Default );

			ReportLocaleFileParsingError_( Context );
			ERRExit( EXIT_FAILURE );
		}

	Language.Init();

	GetLanguage_( Language );

	if ( Language.Amount() == 0 )
		Language.Init( DEFAULT_LANGUAGE );

	LocaleRack_.Init( Locale_, Language.Convert( Language_ ) );
ERRErr
ERREnd
ERREpilog
}


// Try to find a locale file name, to avoid cryptic message until we have load the correct one.
void scllocale::LoadTemporaryLocale(
	const char *Affix,
	const char *RegistryRootPath,
	const char *FileSuggestedPath )
{
	LoadLocale_( str::string(), Affix, RegistryRootPath, FileSuggestedPath, true );
}

void scllocale::LoadLocale(
	const char *Affix,
	const char *RegistryRootPath,
	const char *FileSuggestedPath )
{
ERRProlog
	str::string LocaleFileName;
ERRBegin
	LocaleFileName.Init();

	LoadLocale_( sclrgstry::GetLocaleFileName( LocaleFileName ), Affix, RegistryRootPath, FileSuggestedPath ,false );
ERRErr
ERREnd
ERREpilog
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
