/*
	'global' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'erism' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'expp'.

    'expp' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'expp' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'expp'.  If not, see <http://www.gnu.org/licenses/>.
*/
// $Id$

#include "global.h"

#include <stdarg.h>

#include "flf.h"
#include "xtf.h"
#include "xml.h"
#include "fnm.h"
#include "dir.h"

using namespace global;

static lcl::locale _Locale;
STR_BUFFER___ _Language;
lcl::rack__ global::LocaleRack( _Locale, _Language() );

#define DEFAULT_LANGUAGE	"en-US"

#define CASE( name )			LCL_CASE( name, m )
#define CASE_N( name, count )	LCL_CASE_N( name, m, count )

const char *global::Label( message__ Message )
{
#if	GLOBAL__MESSAGE_AMOUNT != 6
#	error "Amount of 'message__' entries changed ! Update !"
#endif

	switch( Message ) {
	CASE( ProcessCommandDescription );
	CASE( EncryptCommandDescription );
	CASE( NamespaceOptionDescription );
	CASE( NoIndentOptionDescription );
	CASE( SourceFileArgumentDescription );
	CASE( DestFileArgumentDescription );
	default:
		ERRc();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

void global::Display(
		message__ Message
		... )
{
#if	GLOBAL__MESSAGE_AMOUNT != 6
#	error "Amount of 'message__' entries changed ! Update !"
#endif

ERRProlog
	str::string Translation;
	va_list Args;
	str::string TagValue;
ERRBegin
	va_start( Args, Message );

	Translation.Init();

	GetTranslation( Message, Translation );

	switch ( Message ) {
	case mProcessCommandDescription:
	case mEncryptCommandDescription:
		break;
	case mNamespaceOptionDescription:
		lcl::ReplaceTag( Translation, 1, str::string( XPP__PREPROCESSOR_DEFAULT_NAMESPACE ) );
		break;
	case mNoIndentOptionDescription:
	case mSourceFileArgumentDescription:
	case mDestFileArgumentDescription:
		break;
	default:
		ERRc();
		break;
	}

	COut << Translation;
ERRErr
ERREnd
ERREpilog

}

#undef CASE
#undef CASE_N

#define CASE( label )	LCL_CASE( label, e )

const char *global::Label( error__ Error )
{
#if	GLOBAL__ERROR_AMOUNT != 5
#	error "Amount of 'error__' entries changed ! Update !"
#endif

	switch( Error ) {
	CASE( ErrorParsingConfigurationFile );
	CASE( ErrorParsingLocaleFile );
	CASE( UnableToOpenFile );
	CASE( ProcessingError );
	CASE( EncryptionError );
	default:
		ERRc();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

void global::Report(
		error__ Error,
		... )
{
#if	GLOBAL__ERROR_AMOUNT != 5
#	error "Amount of 'error__' entries changed ! Update !"
#endif

ERRProlog
	str::string Translation;
	str::string EmbededMessage;
	va_list Args;
	str::string TagValue;
ERRBegin
	va_start( Args, Error );

	Translation.Init();

	GetTranslation( Error, Translation );

	switch ( Error ) {
	case eErrorParsingConfigurationFile:
	case eErrorParsingLocaleFile:
	{
		const rgstry::context___ &Context = *va_arg(Args, const rgstry::context___ *);

		EmbededMessage.Init();
		rgstry::GetTranslation( Context, LocaleRack, EmbededMessage );

		lcl::ReplaceTag( Translation, 1, EmbededMessage );
		break;
	}
	case eUnableToOpenFile:
	{
		const char *FileName = va_arg( Args, const char *);

		lcl::ReplaceTag( Translation, 1, str::string( FileName ) );
		break;
	}
	case eProcessingError:
	case eEncryptionError:
	{
		const xpp::context___ &Context = *va_arg( Args, const xpp::context___ *);

		EmbededMessage.Init();
		xpp::GetTranslation( Context, LocaleRack, EmbededMessage );

		lcl::ReplaceTag( Translation, 1, EmbededMessage );
		break;
	}
	default:
		ERRc();
		break;
	}

	CErr << Translation << " !" << txf::nl << txf::commit;

	ERRExit( EXIT_FAILURE );
ERRErr
ERREnd
ERREpilog

}

/*
	Nothing should be modified below this line
	(standard behaviour of loading configuration
	and locales files)
*/	

static inline const str::string_ &GetLanguage_( str::string_ &Language )
{
	if ( !registry::GetLanguage( Language ) )
		Language = DEFAULT_LANGUAGE;

	return Language;
}

// Try to find a locale file name, to avoid cryptic message until we have loadthe correct one.
static void LoadTemporaryLocale_( void )
{
ERRProlog
	rgstry::status__ Status = rgstry::s_Undefined;
	rgstry::context___ Context;
	str::string LocaleFileName;
	DIR_BUFFER___ DIRBuffer;
	FNM_BUFFER___ FNMBuffer;
	STR_BUFFER___ STRBuffer;
	STR_BUFFER___ PathBuffer;
	str::string Language;
ERRBegin
	LocaleFileName.Init( NAME LCL_DEFAULT_FILENAME_SUFFIX );

	if ( !fil::FileExists( LocaleFileName.Convert( STRBuffer ) ) )
		LocaleFileName.Init( fnm::BuildFileName( dir::GetSelfPath( PathBuffer ), NAME, LCL_DEFAULT_FILENAME_SUFFIX, FNMBuffer ) );

	Context.Init();
	_Locale.Init( LocaleFileName.Convert( STRBuffer ), "Locales/Locale[target=\"" NAME "\"]", Context );

	Language.Init();

	GetLanguage_( Language );

	LocaleRack.Init( _Locale, Language.Convert( _Language ) );
ERRErr
ERREnd
ERREpilog
}

void LoadRegistry_( void )
{
ERRProlog
	rgstry::status__ Status = rgstry::s_Undefined;
	rgstry::context___ Context;
	DIR_BUFFER___ DIRBuffer;
	FNM_BUFFER___ FNMBuffer;
ERRBegin
	registry::Registry.Init();
	Context.Init();

	if ( ( Status = registry::FillRegistry( fnm::BuildFileName( "", NAME, ".xcfg", FNMBuffer ), Context ) ) != rgstry::sOK )
		if ( Status == rgstry::sUnableToOpenFile ) {
			registry::Registry.Init();
			Context.Init();
			if ( ( Status = registry::FillRegistry( fnm::BuildFileName( dir::GetSelfPath( DIRBuffer ), NAME, ".xcfg", FNMBuffer ), Context ) ) != rgstry::sOK )
				Report( eErrorParsingConfigurationFile, &Context );
		} else
			Report( eErrorParsingConfigurationFile, &Context );
ERRErr
ERREnd
ERREpilog
}

static void LoadLocale_( void )
{
ERRProlog
	rgstry::status__ Status = rgstry::s_Undefined;
	rgstry::context___ Context;
	str::string LocaleFileName;
	FNM_BUFFER___ FNMBuffer;
	STR_BUFFER___ STRBuffer;
	STR_BUFFER___ PathBuffer;
	str::string Language;
ERRBegin
	LocaleFileName.Init();

	if ( !registry::GetLocaleFileName( LocaleFileName ) ) {
		LocaleFileName.Init( NAME LCL_DEFAULT_FILENAME_SUFFIX );

		if ( !fil::FileExists( LocaleFileName.Convert( STRBuffer ) ) )
			LocaleFileName.Init( fnm::BuildFileName( dir::GetSelfPath( PathBuffer ), NAME, LCL_DEFAULT_FILENAME_SUFFIX, FNMBuffer ) );
	}

	Context.Init();

	if ( ( Status = _Locale.Init( LocaleFileName.Convert( STRBuffer ), "Locales/Locale[target=\"" NAME "\"]", Context ) ) != rgstry::sOK )
		Report( eErrorParsingLocaleFile, &Context );

	Language.Init();

	GetLanguage_( Language );

	LocaleRack.Init( _Locale, Language.Convert( _Language ) );
ERRErr
ERREnd
ERREpilog
}


static void _Initialize( void )
{
	registry::Registry.Init();

	_Locale.Init();
	_Language.Init();

	LoadTemporaryLocale_();

	LoadRegistry_();

	LoadLocale_();
}

static void _Release( void )
{
}

void global::CreateBackupFile( const char *FileName )
{
ERRProlog
	fil::backup_status__ Status = fil::bs_Undefined;
	str::string Translation;
ERRBegin
	if ( ( Status = fil::CreateBackupFile( FileName, fil::bmDuplicate, err::hUserDefined ) )!= fil::bsOK ) {
		Translation.Init();
		fil::GetTranslation( Status, FileName, LocaleRack, Translation );
		CErr << Translation << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
}

void global::RecoverBackupFile( const char *FileName )
{
ERRProlog
	fil::recover_status__ Status = fil::rs_Undefined;
	str::string Translation;
ERRBegin
	if ( ( Status = fil::RecoverBackupFile( FileName, err::hUserDefined ) )!= fil::rsOK ) {
		Translation.Init();
		fil::GetTranslation( Status, FileName, LocaleRack, Translation );
		CErr << Translation << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
}

int main(
	int argc,
	const char *argv[] )
{
ERRFProlog
ERRFBegin
	_Initialize();
	Main( argc, argv );
ERRFErr
ERRFEnd
	_Release();
	COut << txf::commit;
	CErr << txf::commit;
ERRFEpilog
	return ERRExitValue;
}

static struct global__cdtor {
	global__cdtor( void )
	{
		if ( GLOBAL__ERROR_AMOUNT != e_amount )
			ERRc();

		if ( GLOBAL__MESSAGE_AMOUNT != m_amount )
			ERRc();
	}
	~global__cdtor( void )
	{
	}
} GlobalCDTor;
