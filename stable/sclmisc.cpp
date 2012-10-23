/*
	'sclmisc' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sclmisc' header file ('sclmisc.h').
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

#define SCLMISC__COMPILATION

#include "sclmisc.h"

class sclmisctutor
: public ttr_tutor
{
public:
	sclmisctutor( void )
	: ttr_tutor( SCLMISC_NAME )
	{
#ifdef SCLMISC_DBG
		Version = SCLMISC_VERSION "\b\bD $";
#else
		Version = SCLMISC_VERSION;
#endif
		Owner = SCLMISC_OWNER;
		Date = "$Date$";
	}
	virtual ~sclmisctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "str.h"
#include "cio.h"
#include "fnm.h"
#include "dir.h"
#include "lcl.h"

#include "scllocale.h"
#include "sclrgstry.h"

using namespace sclmisc;

#define DEFAULT_LANGUAGE	"en"

#define REGISTRY_DEFAULT_FILENAME_SUFFIX ".xcfg"

#define LOCALE_DEFAULT_FILENAME_SUFFIX ".xlcl"

const lcl::rack__ *sclrgstry::LocaleRack = &scllocale::GetRack();

static inline const str::string_ &GetLanguage_( str::string_ &Language )
{
	bso::bool__ Missing = false;

	if ( !sclrgstry::IsRegistryReady() )
		ERRc();

	if ( !sclrgstry::GetRegistry().GetValue( sclrgstry::Language, sclrgstry::GetRoot(), Language  ) )
		Language = DEFAULT_LANGUAGE;

	return Language;
}


static void Initialize_(
	flw::iflow__ &LocaleFlow,
	flw::iflow__ &RegistryFlow,
	const char *LocaleRootPath,
	const char *RegistryRootPath,
	const char *LocaleDirectory,
	const char *RegistryDirectory )
{
ERRProlog
	str::string Language;
ERRBegin
	scllocale::Load( LocaleFlow, LocaleDirectory, LocaleRootPath );

	sclrgstry::Load( RegistryFlow, RegistryDirectory, RegistryRootPath );

	Language.Init();
	GetLanguage_( Language );

	scllocale::SetLanguage( Language );
ERRErr
ERREnd
ERREpilog
}

static void BuildRootPath_(
	const char *Subject,
	const char *Target,
	str::string_ &Path )
{
	Path.Append( Subject );
	Path.Append( "s/" );
	Path.Append( Subject );
	Path.Append( "[target=\"" );
	Path.Append( Target );
	Path.Append( "\"]" );
}

void sclmisc::Initialize(
	flw::iflow__ &LocaleFlow,
	flw::iflow__ &RegistryFlow,
	const char *Target,
	const char *LocaleDirectory,
	const char *RegistryDirectory )
{
ERRProlog
	str::string LocaleRootPath, RegistryRootPath;
	STR_BUFFER___ LocaleBuffer, RegistryBuffer;
ERRBegin
	LocaleRootPath.Init();
	BuildRootPath_( "Locale", Target, LocaleRootPath );

	RegistryRootPath.Init();
	BuildRootPath_( "Configuration", Target, RegistryRootPath );

	Initialize_( LocaleFlow, RegistryFlow, LocaleRootPath.Convert( LocaleBuffer ), RegistryRootPath.Convert( RegistryBuffer ), LocaleDirectory, RegistryDirectory );
ERRErr
ERREnd
ERREpilog
}

static bso::bool__ GuessFileName_(
	const char *Affix,
	const char *Suffix,
	const char *SuggestedDirectory,
	str::string_ &FileName )
{
	bso::bool__ Success = false;
ERRProlog
	STR_BUFFER___ STRBuffer;
	STR_BUFFER___ PathBuffer;
	FNM_BUFFER___ FNMBuffer;
ERRBegin
	FileName.Init( Affix );
	FileName.Append( Suffix );

	if ( !fil::FileExists( FileName.Convert( STRBuffer ) ) ) {
		FileName.Init( fnm::BuildFileName( SuggestedDirectory, Affix, Suffix, FNMBuffer ) );

		if ( !fil::FileExists( FileName.Convert( STRBuffer ) ) ) {
				FileName.Init( fnm::BuildFileName( dir::GetSelfPath( PathBuffer ), Affix, Suffix, FNMBuffer ) );

				Success = fil::FileExists( FileName.Convert( STRBuffer ) );
		} else
			Success = true;
	} else
		Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ InitializeFlow_(
	const char *Target,
	const char *Suffix,
	const char *SuggestedDirectory,
	flf::file_iflow___ &Flow,
	str::string_ &Directory )
{
	bso::bool__ Success = false;
ERRProlog
	str::string FileName;
	STR_BUFFER___ FileNameBuffer, LocationBuffer;
ERRBegin
	FileName.Init();
	Success = GuessFileName_( Target, Suffix, SuggestedDirectory, FileName );

	if ( Success )
		if ( Flow.Init( FileName.Convert( FileNameBuffer ), err::hUserDefined ) != fil::sSuccess )
			Success = false;

	Directory.Append( fnm::GetLocation( FileName.Convert( FileNameBuffer ), LocationBuffer ) );
ERRErr
ERREnd
ERREpilog
	return Success;
}

static flw::iflow__ &InitializeLocaleFlow_(
	const char *Target,
	const char *SuggestedDirectory,
	flf::file_iflow___ &Flow,
	str::string_ &Directory )
{
	if ( !InitializeFlow_( Target, LOCALE_DEFAULT_FILENAME_SUFFIX, SuggestedDirectory, Flow, Directory ) ) {
		cio::CErr << "Unable to open locale file !" << txf::nl;
		ERRExit( EXIT_FAILURE );
	}

	return Flow;
}

static flw::iflow__ &InitializeRegistryFlow_(
	const char *Target,
	const char *SuggestedDirectory,
	flf::file_iflow___ &Flow,
	str::string_ &Directory )
{
ERRProlog
	str::string Translation;
ERRBegin
	if ( !InitializeFlow_( Target, REGISTRY_DEFAULT_FILENAME_SUFFIX, SuggestedDirectory, Flow, Directory ) ) {
		Translation.Init();
		cio::CErr << scllocale::GetTranslation( SCLMISC_NAME "_UnableToOpenRegistryFile", Translation );
		ERRExit( EXIT_FAILURE );
	}

ERRErr
ERREnd
ERREpilog
	return Flow;
}

void sclmisc::Initialize(
	const char *Target,
	const char *SuggestedDirectory )
{
ERRProlog
	flf::file_iflow___ LocaleFlow, RegistryFlow;
	str::string LocaleDirectory, RegistryDirectory;
	STR_BUFFER___ LocaleBuffer, RegistryBuffer;
ERRBegin
	LocaleDirectory.Init();
	InitializeLocaleFlow_( Target, SuggestedDirectory, LocaleFlow, LocaleDirectory );

	RegistryDirectory.Init();
	InitializeRegistryFlow_( Target, SuggestedDirectory, RegistryFlow, RegistryDirectory );

	Initialize( LocaleFlow, RegistryFlow, Target, LocaleDirectory.Convert( LocaleBuffer ), RegistryDirectory.Convert( RegistryBuffer ) ); 
ERRErr
ERREnd
ERREpilog
}

void sclmisc::Terminate( void )
{
	cio::COut << txf::commit;
	cio::CErr << txf::commit;
}


void sclmisc::CreateBackupFile(
	const char *FileName,
	fil::backup_mode__ Mode )
{
ERRProlog
	fil::backup_status__ Status = fil::bs_Undefined;
	str::string Translation;
ERRBegin
	if ( ( Status = fil::CreateBackupFile( FileName, Mode, err::hUserDefined ) )!= fil::bsOK ) {
		Translation.Init();
		fil::GetTranslation( Status, FileName, scllocale::GetRack(), Translation );
		cio::CErr << Translation << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
}

void sclmisc::RecoverBackupFile( const char *FileName )
{
ERRProlog
	fil::recover_status__ Status = fil::rs_Undefined;
	str::string Translation;
ERRBegin
	if ( ( Status = fil::RecoverBackupFile( FileName, err::hUserDefined ) )!= fil::rsOK ) {
		Translation.Init();
		fil::GetTranslation( Status, FileName, scllocale::GetRack(), Translation );
		cio::CErr << Translation << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclmiscpersonnalization
: public sclmisctutor
{
public:
	sclmiscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclmiscpersonnalization( void )
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

static sclmiscpersonnalization Tutor;

ttr_tutor &SCLMISCTutor = Tutor;
