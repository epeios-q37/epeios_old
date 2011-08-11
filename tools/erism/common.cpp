/*
	'common' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'erism' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/
// $Id$


#include "common.h"

#include "flf.h"
#include "xtf.h"
#include "xml.h"
#include "fnm.h"
#include "dir.h"

#include "mscmdd.h"

using namespace common;

cio::cerr___ common::cerr;
cio::cout___ common::cout;
cio::cin___ common::cin;

static lcl::locale _Locale;
STR_BUFFER___ _Language;
lcl::rack__ common::LocaleRack( _Locale, _Language() );

#define DEFAULT_LANGUAGE	"en-US"


#define CASE( name )			LCL_CASE( name, m )
#define CASE_N( name, count )	LCL_CASE_N( name, m, count )

const char *common::Label( message__ Message )
{
#if	COMMON__MESSAGE_AMOUNT != 17
#	error "Amount of 'message__' entries changed ! Update !"
#endif

	switch( Message ) {
	CASE( DevicesCommandDescription )
	CASE( IdentifyCommandDescription )
	CASE( RetrieveCommandDescription )
	CASE( WriteCommandDescription )
	CASE( RandomCommandDescription )
	CASE( DetailCommandDescription )
	CASE( DInOptionDescription )
	CASE( DOutOptionDescription )
	CASE( TargetArgumentDescription )
	CASE( SettingsArgumentDescription )
	CASE( AddressRangeArgumentDescription )
	CASE( ValueRangeArgumentDescription )
	CASE( NoMIDIOutDevicesAvailable )
	CASE( AvailableMIDIOutDevices )
	CASE( NoMIDIInDevicesAvailable )
	CASE( AvailableMIDIInDevices )
	CASE( DeviceModel )
	default:
		ERRc();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

void common::Display(
		message__ Message
		... )
{
#if	COMMON__MESSAGE_AMOUNT != 17
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
	case mTargetArgumentDescription:
	case mSettingsArgumentDescription:
	case mAddressRangeArgumentDescription:
	case mValueRangeArgumentDescription:
	case mDevicesCommandDescription:
	case mIdentifyCommandDescription:
	case mRetrieveCommandDescription:
	case mWriteCommandDescription:
	case mRandomCommandDescription:
	case mDetailCommandDescription:
	case mNoMIDIOutDevicesAvailable:
	case mAvailableMIDIOutDevices:
	case mNoMIDIInDevicesAvailable:
	case mAvailableMIDIInDevices:
		break;
	case mDInOptionDescription:
	case mDOutOptionDescription:
		TagValue.Init( va_arg( Args, const char *) );
		lcl::ReplaceTag( Translation, 1, TagValue );
		break;
	case mDeviceModel:
		TagValue.Init( *va_arg( Args, const str::string *) );
		lcl::ReplaceTag( Translation, 1, TagValue );
		break;
	default:
		ERRc();
		break;
	}

	cout << Translation;
ERRErr
ERREnd
ERREpilog

}

#undef CASE
#undef CASE_N

#define CASE( label )	LCL_CASE( label, e )

const char *common::Label( error__ Error )
{
#if	COMMON__ERROR_AMOUNT != 18
#	error "Amount of 'error__' entries changed ! Update !"
#endif

	switch( Error ) {
	CASE( MissingDOutID )
	CASE( MissingDInID )
	CASE( BadDOutID )
	CASE( BadDInID )
	CASE( UnableToOpenMIDIDevices )
	CASE( MIDIImplementationsFileNameNotDefined )
	CASE( MIDIImplementationFileError )
	CASE( BadDeviceID )
	CASE( UnableToIdentifyDevice )
	CASE( ErrorParsingConfigurationFile )
	CASE( ErrorParsingLocaleFile )
	CASE( MissingSettingsFileName )
	CASE( SettingsFileError )
	CASE( UnableToOpenFile )
	CASE( TransmissionError )
	CASE( BadAddressRange )
	CASE( BadValueRange )
	CASE( UnableToFindParameterForAddress )

	default:
		ERRc();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

void common::Report(
		error__ Error,
		... )
{
#if	COMMON__ERROR_AMOUNT != 18
#	error "Amount of 'error__' entries changed ! Update !"
#endif

ERRProlog
	str::string Translation;
	va_list Args;
	str::string TagValue;
	xpp::context___ Context;
ERRBegin
	va_start( Args, Error );

	Translation.Init();

	switch ( Error ) {
	case eMissingDOutID:
	case eMissingDInID:
	case eBadDOutID:
	case eBadDInID:
	case eMIDIImplementationsFileNameNotDefined:
	case eMissingSettingsFileName:
	case eBadDeviceID:
	case eSettingsFileError:
	case eBadAddressRange:
	case eBadValueRange:
		// Pas de param�tre.
		GetTranslation( Error, Translation );
		break;
	case eUnableToOpenMIDIDevices:
		// Parameter : 'mscmdd::status__'.
		mscmdd::GetTranslation( *va_arg( Args, const mscmdd::status__ *), LocaleRack, Translation );
		break;
	case eUnableToOpenFile:
		// Parameter : 'const char *'.
		GetTranslation( Error, Translation );
		lcl::ReplaceTag( Translation, 1, str::string( va_arg( Args, const char *) ) );

		va_end( Args );
		break;
	case eMIDIImplementationFileError:
	{
		mscrmi::parse_status__ &Status = *va_arg( Args, mscrmi::parse_status__ *);
		const xpp::preprocessing_iflow___ &Flow = *va_arg( Args, const xpp::preprocessing_iflow___ *);
		
		if ( Status == mscrmi::psParserError )
			xpp::GetTranslation( Flow, LocaleRack, Translation );
		else {
			Context.Init();
			Flow.GetContext( Context );

			mscrmi::GetTranslation( Status, LocaleRack, Context.Coord, Context.FileName, Translation );
		}
		break;
	}
	case eUnableToIdentifyDevice:
		GetTranslation( Error, Translation );

		TagValue.Init();
		mscrmi::ToString( *va_arg( Args, const mscrmi::device_family__ *), TagValue );
		lcl::ReplaceTag( Translation, 1, TagValue );

		TagValue.Init();
		mscrmi::ToString( *va_arg( Args, const mscrmi::software_revision__ *), TagValue );
		lcl::ReplaceTag( Translation, 2, TagValue );
		break;
	case eErrorParsingConfigurationFile:
	case eErrorParsingLocaleFile:
	{
		const rgstry::status__ &Status = *va_arg(Args, const rgstry::status__ *);

		rgstry::GetTranslation( *va_arg( Args, const rgstry::context___ *), LocaleRack, Translation );
		break;
	}
	case eTransmissionError:
		// Parameter : 'mscrmi::transmission_status__'
		mscrmi::GetTranslation( *va_arg( Args, const mscrmi::transmission_status__ *), LocaleRack, Translation );
		break;
	case eUnableToFindParameterForAddress:
	{
		// Parameter : 'mscrmi::address__'.
		char Buffer[]  ="12345678";

		GetTranslation( Error, Translation );
		sprintf( Buffer, "%08lX", va_arg( Args, mscrmi::address__ ) );

		TagValue.Init( Buffer );

		lcl::ReplaceTag( Translation, 1, TagValue );
		break;
	}
	default:
		ERRc();
		break;
	}

	cerr << Translation << " !" << txf::nl << txf::commit;
	ERRExit( EXIT_FAILURE );
ERRErr
ERREnd
ERREpilog

}


#if 1
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
			Report( eErrorParsingConfigurationFile,&Context );
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
		Report( eErrorParsingLocaleFile, &Status, &Context );

	Language.Init();

	GetLanguage_( Language );

	LocaleRack.Init( _Locale, Language.Convert( _Language ) );
ERRErr
ERREnd
ERREpilog
}


void common::Initialize( void )
{
	cerr.Init();
	cout.Init();
	cin.Init();

	registry::Registry.Init();

	_Locale.Init();
	_Language.Init();

	LoadTemporaryLocale_();

	LoadRegistry_();

	LoadLocale_();
}

void common::Release( void )
{
	cerr.reset();
	cout.reset();
	cin.reset();
}

#else	// Old
static inline const str::string_ &GetLanguage_( str::string_ &Language )
{
	if ( !registry::GetLanguage( Language ) )
		Language = DEFAULT_LANGUAGE;

	return Language;
}


void common::GlobalInitialization( void )
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
	cerr.Init();
	cout.Init();

	registry::Registry.Init();
	Context.Init();

	_Locale.Init();
	_Language.Init();

	LocaleRack.Init( _Locale, _Language );	// Even if no 'locale' loaded, to display raw messages (not translated). Language parameter doesn't matter.

	if ( ( Status = registry::FillRegistry( fnm::BuildFileName( dir::GetSelfPath( DIRBuffer ), NAME, ".xcfg", FNMBuffer ), Context ) ) != rgstry::sOK )
		if ( Status == rgstry::sUnableToOpenFile ) {
			registry::Registry.Init();
			Context.Init();
			if ( ( Status = registry::FillRegistry( fnm::BuildFileName( "", NAME, ".xcfg", FNMBuffer ), Context ) ) != rgstry::sOK )
				Report( eErrorParsingConfigurationFile, &Status, &Context );
		} else
			Report( eErrorParsingConfigurationFile, &Status, &ErrorDetails );

	LocaleFileName.Init();

	if ( !registry::GetLocaleFileName( LocaleFileName ) ) {
		LocaleFileName.Init( fnm::BuildFileName( dir::GetSelfPath( PathBuffer ), NAME, LCL_DEFAULT_FILENAME_SUFFIX, FNMBuffer ) );

		if ( !fil::FileExists( LocaleFileName.Convert( STRBuffer ) ) )
			LocaleFileName.Init( NAME LCL_DEFAULT_FILENAME_SUFFIX );
	}

	if ( ( Status = _Locale.Init( LocaleFileName.Convert( STRBuffer ), "Locales/Locale[target=\"" NAME "\"]", ErrorDetails ) ) != rgstry::sOK )
		Report( eErrorParsingLocaleFile, &Status, &ErrorDetails );

	Language.Init();
	GetLanguage_( Language );

	LocaleRack.Init( _Locale, Language.Convert( _Language ) );	// To configure with the correct language, because the configuration is now available.
ERRErr
ERREnd
ERREpilog
}

void common::GlobalRelease( void )
{
	cerr.reset();
	cout.reset();
}
#endif


mscrmi::device_id__ common::GetDeviceID( const char *RawDeviceID )
{
	mscrmi::device_id__ DeviceID = MSCRMI_UNDEFINED_DEVICE_ID;

	if ( RawDeviceID != NULL ) {
		epeios::row__ Error = NONE;

		DeviceID = str::string( RawDeviceID ).ToUB( &Error, str::bAuto, 127 );	// As this value is transmitted through a MIDI channel, it can't be > 127.

		if ( Error != NONE )
			Report( eBadDeviceID );

		if ( DeviceID == MSCRMI_UNDEFINED_DEVICE_ID )	// Normally, can't happen (see above).
			Report( eBadDeviceID );
	} else {
		bso::bool__ Error = false;

		DeviceID = rgstry::GetUB( registry::Registry, str::string( registry::path::DefaultDeviceID ), MSCRMI_DEFAULT_DEVICE_ID, &Error, 0, 127 );

		if ( Error )
			Report( eBadDeviceID );
	}

	return DeviceID;
}

void common::Normalize(
	const char *RawDeviceID,
	const char *RawDIn,
	const char *RawDOut,
	mscrmi::device_id__ &DeviceID,
	int &DIn,
	int &DOut )
{
	epeios::row__ Error = NONE;

	DeviceID = GetDeviceID( RawDeviceID );

	if ( RawDIn == NULL )
		Report( eMissingDInID );

	if ( RawDOut == NULL )
		Report( eMissingDOutID );

	DIn = str::string( RawDIn ).ToUB( &Error );

	if ( Error != NONE )
		common::Report( eBadDInID );

	DOut = str::string( RawDOut ).ToUB( &Error );

	if ( Error != NONE )
		common::Report( eBadDOutID );
}

void common::CreateBackupFile( const char *FileName )
{
ERRProlog
	fil::backup_status__ Status = fil::bs_Undefined;
	str::string Translation;
ERRBegin
	if ( ( Status = fil::CreateBackupFile( FileName, fil::bmDuplicate, err::hUserDefined ) )!= fil::bsOK ) {
		Translation.Init();
		fil::GetTranslation( Status, FileName, LocaleRack, Translation );
		cerr << Translation << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
}

void common::RecoverBackupFile( const char *FileName )
{
ERRProlog
	fil::recover_status__ Status = fil::rs_Undefined;
	str::string Translation;
ERRBegin
	if ( ( Status = fil::RecoverBackupFile( FileName, err::hUserDefined ) )!= fil::rsOK ) {
		Translation.Init();
		fil::GetTranslation( Status, FileName, LocaleRack, Translation );
		cerr << Translation << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
}


const mscrmi::device_family__ &common::GetDeviceFamilyAndSoftwareRevision(
	mscrmi::device_id__ DeviceID,
	flw::ioflow__ &Flow,
	mscrmi::device_family__ &DeviceFamily,
	mscrmi::software_revision__ &SoftwareRevision )
{
	mscrmi::transmission_status__ Status = mscrmi::GetDeviceFamilyAndSoftwareRevision( DeviceID, Flow, DeviceFamily, SoftwareRevision );

	if ( Status != mscrmi::tsOK )
		Report( eTransmissionError, &Status );

	return DeviceFamily;
}


const mscrmi::device_family__ &common::GetDeviceFamilyAndSoftwareRevision(
	mscrmi::device_id__ DeviceID,
	int DIn,
	int DOut,
	mscrmi::device_family__ &DeviceFamily,
	mscrmi::software_revision__ &SoftwareRevision )
{
ERRProlog
	mscmdd::midi_ioflow___ Flow;
ERRBegin
	GetDeviceFamilyAndSoftwareRevision( DeviceID, common::OpenDevices( DIn, DOut, Flow ), DeviceFamily, SoftwareRevision );
ERRErr
ERREnd
ERREpilog
	return DeviceFamily;
}

epeios::row__ common::Identify(
	mscrmi::device_id__ DeviceID,
	flw::ioflow__ &Flow,
	const mscrmi::midi_implementations_ &Implementations,
	mscrmi::device_family__ &DeviceFamily,
	mscrmi::software_revision__ &SoftwareRevision )
{
	epeios::row__ Row = NONE;

	common::GetDeviceFamilyAndSoftwareRevision( DeviceID, Flow, DeviceFamily, SoftwareRevision );

	if ( ( Row = mscrmi::GetCorrespondingMIDIImplementation( DeviceFamily, Implementations ) ) == NONE )
		common::Report( common::eUnableToIdentifyDevice, &DeviceFamily, &SoftwareRevision );

	return Row;
}

epeios::row__ common::Identify(
	mscrmi::device_id__ DeviceID,
	int DIn,
	int DOut,
	const mscrmi::midi_implementations_ &Implementations,
		mscrmi::device_family__ &DeviceFamily,
	mscrmi::software_revision__ &SoftwareRevision )
{
	epeios::row__ Row = NONE;
ERRProlog
	mscmdd::midi_ioflow___ Flow;
ERRBegin
	Row = Identify( DeviceID, common::OpenDevices( DIn, DOut, Flow ), Implementations, DeviceFamily, SoftwareRevision );
ERRErr
ERREnd
ERREpilog
	return Row;
}

static void GetImplementations_(
	const char *RawFileName,
	mscrmi::midi_implementations_ &Implementations )
{
ERRProlog
	flf::file_iflow___ FFlow;
	xpp::preprocessing_iflow___ PFlow;
	xtf::extended_text_iflow__ XFlow;
	xml::parser___ Parser;
	mscrmi::parse_status__ Status = mscrmi::ps_Undefined;
	str::string FileName;
	STR_BUFFER___ STRBuffer;
	DIR_BUFFER___ DIRBuffer;
	FNM_BUFFER___ FNMBuffer;
ERRBegin

	switch ( fnm::Type( RawFileName ) ) {
	case fnm::tEmpty:
		ERRc();
		break;
	case fnm::tAbsolute:
	case fnm::tRelative:
	case fnm::tPath:
		FileName.Init( RawFileName );
		break;
	case fnm::tSuffixed:
	case fnm::tFree:
		if ( !fil::FileExists( RawFileName ) )
			FileName.Init( fnm::BuildFileName( dir::GetSelfPath( DIRBuffer ), RawFileName, "", FNMBuffer ) );
		else
			FileName.Init( RawFileName );
		break;
	default:
		ERRc();
		break;
	}	

	if ( FFlow.Init( FileName.Convert( STRBuffer ), err::hUserDefined ) != fil::sSuccess )
		Report( eUnableToOpenFile, STRBuffer() );

	PFlow.Init( FFlow, xpp::criterions___( str::string( fnm::GetLocation( FileName.Convert( STRBuffer ), FNMBuffer ) ) ) );
	XFlow.Init( PFlow );

	Parser.Init( XFlow, xml::eh_Default );

	switch ( Status = mscrmi::Parse( Parser, Implementations ) ) {
	case mscrmi::psOK:
		break;
	default:
		Report( eMIDIImplementationFileError, &Status, &PFlow );
		break;
	}

ERRErr
ERREnd
ERREpilog
}

void common::GetImplementations( mscrmi::midi_implementations_ &Implementations )
{
ERRProlog
	value FileName;
	STR_BUFFER___ Buffer;
ERRBegin
	FileName.Init();

	if ( !registry::GetMIDIImplementationsFileName( FileName ) )
		Report( eMIDIImplementationsFileNameNotDefined );

	GetImplementations_( FileName.Convert( Buffer ), Implementations );
ERRErr
ERREnd
ERREpilog
}

const mscrmi::midi_implementation_ &common::GetImplementation(
	mscrmi::device_id__ DeviceID,
	flw::ioflow__ &Flow,
	mscrmi::midi_implementation_ &Implementation )
{
ERRProlog
	mscrmi::midi_implementations Implementations;
	epeios::row__ Row = NONE;
	mscrmi::device_family__ DeviceFamily;
	mscrmi::software_revision__ SoftwareRevision;
ERRBegin
	Implementations.Init();

	GetImplementations( Implementations );

	Row = Identify( DeviceID, Flow, Implementations, DeviceFamily, SoftwareRevision );

	Implementation = Implementations( Row );
ERRErr
ERREnd
ERREpilog
	return Implementation;
}

const mscrmi::midi_implementation_ &common::GetImplementation(
	mscrmi::device_id__ DeviceID,
	int DIn,
	int DOut,
	mscrmi::midi_implementation_ &Implementation )
{
ERRProlog
	mscmdd::midi_ioflow___ Flow;
ERRBegin
	GetImplementation( DeviceID, common::OpenDevices( DIn, DOut, Flow ), Implementation );
ERRErr
ERREnd
ERREpilog
	return Implementation;
}

static struct common__cdtor {
	common__cdtor( void )
	{
		if ( COMMON__ERROR_AMOUNT != e_amount )
			ERRc();

		if ( COMMON__MESSAGE_AMOUNT != m_amount )
			ERRc();
	}
} CommonCDTor;