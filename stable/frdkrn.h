/*
	Header for the 'frdkrn' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef FRDKRN__INC
#define FRDKRN__INC

#define FRDKRN_NAME		"FRDKRN"

#define	FRDKRN_VERSION	"$Revision$"

#define FRDKRN_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FRDKRNTutor;

#if defined( XXX_DBG ) && !defined( FRDKRN_NODBG )
#define FRDKRN_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D FRontenD KeRNel 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"

#include "frdbkd.h"
#include "frdrgy.h"
#include "frdfbc.h"

#define FRDKRN_CONFIGURATION_FILE_EXTENSION	"xcfg"
#define FRDKRN_PROJECT_FILE_EXTENSION		"xprj"
#define FRDKRN_LOCALE_FILE_EXTENSION		"xlcl"

namespace frdkrn {
	using namespace frdbkd;

	csducl::type__ GetBackendType( const frdrgy::registry_ &Registry );

	class log_functions__
	: public csdsnc::log_functions__
	{
	protected:
		virtual void CSDSNCLog(
			csdsnc::log__ Log,
			const void *Flow,
			epeios::size__ Amount )
		{}
	};

	enum status__ {
		sOK,
		sError,		// An error occured; it must be corrected.
		sWarning,	// There was a problem, but the program can continue.
		s_amount,
		s_Undefined
	};

	// Si modifié, modifier 'GetLabel_(...)' en conséquence ainsi que le '.xlcl' associé.
	enum report__ {
		rOK,
		rConfigurationParsingError,	// Error during configuration file parsing. See 'ErrorSet' for more details.
		rLocaleParsingError,		// Error during locales file handling. See 'ErrorSet' for more details.
		rNoLocaleFileDefined,		// No locales file is defined. 
		rProjectParsingError,		// Error during project file handling. See 'ErrorSet' for more details.
		rNoOrBadBackendDefinition,
		rNoBackendLocation,
		rUnableToConnect,
		r_amount,
		r_Undefined
	};

#define FRDKRN__R_AMOUNT	8	// Pour détecter les fonctions devant être modifiée si le nombre d'entrée de 'report__' est modifié.

	const char *GetLabel( report__ Report );

	inline const str::string_ &GetTranslation(
		report__ Report,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation )
	{
		LocaleRack.GetTranslation( GetLabel( Report ), "FRDKRN_", Translation );

		return Translation;
	}

	struct error_set___
	{
	public:
		rgstry::status__ Status;
		rgstry::error_details Details;
		void reset( bso::bool__ P = true )
		{
			Status = rgstry::s_Undefined;
			Details.reset( P );
		}
		error_set___( void )
		{
			reset( false );
		}
		~error_set___( void )
		{
			reset();
		}
		void Init( void )
		{
			Status = rgstry::s_Undefined;

			Details.Init();
		}
	};

	inline bso::bool__ IsErrorSetRelevant( report__ Report )
	{
#if FRDKRN__R_AMOUNT != 8
#	error "'report__' modified !"
#endif
		switch ( Report  ) {
		case rOK:
			return false;
			break;
		case rConfigurationParsingError:
		case rLocaleParsingError:
			return true;
			break;
		case rNoLocaleFileDefined:
			return false;
			break;
		case rProjectParsingError:
			return true;
			break;
		case rNoOrBadBackendDefinition:
		case rNoBackendLocation:
		case rUnableToConnect:
			return false;
		default:
			ERRu();
			break;
		}

		return false;	// Pour éviter un 'warning'.
	}


	inline const str::string_ &GetTranslation(
		const error_set___ &ErrorSet,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation )
	{
		return rgstry::GetTranslation( ErrorSet.Status, ErrorSet.Details, LocaleRack, Translation );
	}

	const str::string_ &GetTranslation(
		report__ Report,
		const error_set___ &ErrorSet,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation );


	bso::bool__ GetDefaultConfigurationFileName(
		const char *Affix,
		str::string_ &FileName );

	class kernel___
	{
	private:
		lcl::locale _LocaleForLibrary;	// Uniquement utilisé en mode 'library'.
		lcl::locale _Locale;
		lcl::rack__ _LocaleRack;
		csducl::universal_client_core _ClientCore;
		frdrgy::registry _Registry;
		frdbkd::_backend___ _Backend;
		str::string _Message;
		time_t _ProjectOriginalTimeStamp;	// Horodatage de la création ou du chargement du projet. Si == 0, pas de projet en cours d'utilisation.
		report__ _Connect(
			const str::string_ &RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type,
			frdfbc::data___ &LibraryData,
			error_reporting_functions___ &ErrorReportingFunctions,
			csdsnc::log_functions__ &LogFunctions );
		void _CloseConnection( void )
		{
			if ( !IsConnected() )
				ERRu();

			_Backend.reset();
			_ClientCore.reset();
		}
	protected:
		report__ _LoadConfiguration(
			const str::string_ &ConfigurationFileName,
			const char *TargetName,
			const char *CypherKey,
			error_set___ &ErrorSet );
		report__ _LoadLocale(
			const str::string_ &FileName,
			const char *TargetName,
			error_set___ &ErrorSet );
		report__ _LoadLocale(
			const char *TargetName,
			error_set___ &ErrorSet );
		report__ _LoadConfigurationAndLocale(
			const str::string_ &ConfigurationFileName,
			const char *TargetName,
			const char *CypherKey,
			error_set___ &ErrorSet )
		{
			report__ Report = r_Undefined;

			if ( ( Report = _LoadConfiguration( ConfigurationFileName, TargetName, CypherKey, ErrorSet ) ) == rOK )
				Report = _LoadLocale( TargetName, ErrorSet );

			return Report;
		}
		report__ _FillProjectRegistry(
			const str::string_ &FileName,
			const char *TargetName,
			const char *CypherKey,
			error_set___ &ErrorSet );
		report__ _Connect(
			const char *RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type,
			frdfbc::data___ &LibraryData,
			error_reporting_functions___ &ErrorReportingFunctions = *(error_reporting_functions___ *)NULL,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL );
		report__ _Connect( // Try to connect using registry content.
			frdfbc::data___ &LibraryData,
			error_reporting_functions___ &ErrorReportingFunctions = *(error_reporting_functions___ *)NULL,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				Close();

			_Backend.reset( P );
			_ClientCore.reset( P );
			_Registry.reset( P );
			_LocaleForLibrary.reset();
			_Locale.reset( P );
			_LocaleRack.reset( P );
			_Message.reset( P );
			_ProjectOriginalTimeStamp = 0;
		}
		kernel___( void )
		{
			reset( false );
		}
		~kernel___( void )
		{
			reset();
		}
		report__ Init(
			const str::string_ &ConfigurationFileName,
			const char *TargetName,
			const str::string_ &Language,
			const char *CypherKey,
			error_set___ &ErrorSet )
		{
			_Registry.Init();
			_Message.Init();
			_LocaleForLibrary.reset();
			_Locale.Init();
			_LocaleRack.Init( _Locale, Language );	// Initialisé dés mantenant bien que '_Locale' vide, pour pouvoir être utilisé par fonction appelante.
			_ProjectOriginalTimeStamp = 0;

			return _LoadConfigurationAndLocale( ConfigurationFileName, TargetName, CypherKey, ErrorSet );

			// L'initialisation de '_Backend' et '_ClientCore' se fait à la connection.
		}
		status__ Init(
			const str::string_ &ConfigurationFileName,
			const char *TargetName,
			const char *CypherKey,
			const str::string_ &Language );
		const frdrgy::registry_ &Registry( void ) const
		{
			return _Registry;
		}
		report__ LoadProject(
			const str::string_ &FileName,
			const char *TargetName,
			const char *CypherKey,
			frdfbc::data___ &LibraryData,
			error_set___ &ErrorSet )
		{
			report__ Report = r_Undefined;

			if ( ( Report = _FillProjectRegistry( FileName, TargetName, CypherKey, ErrorSet ) ) == rOK )
				Report = _Connect( LibraryData );

			if ( Report == rOK )
				_ProjectOriginalTimeStamp = time( NULL );

			return Report;
		}
		status__ LoadProject(
			const str::string_ &FileName,
			const char *TargetName,
			const char *CypkerKey,
			frdfbc::data___ &LibraryData );
		bso::bool__ IsProjectInProgress( void ) const
		{
			return _ProjectOriginalTimeStamp != 0;
		}
		bso::bool__ IsProjectModified( void ) const
		{
			return _ProjectOriginalTimeStamp < ProjectTimeStamp();
		}
		E_RWDISCLOSE__( str::string_, Message );
		const void AboutBackend(
			str::string_ &BackendInformations,
			str::string_ &PublisherInformations )
		{
			_Backend.About( BackendInformations, PublisherInformations );
		}
		void Close( void )
		{
			if ( IsConnected() ) {
				_CloseConnection();
			}

			_Registry.Init();
		}
		bso::bool__ IsConnected( void ) const
		{
			return _Backend.IsConnected();
		}
		void FillUserRegistry(
			flw::iflow__ &User,
			const char *CypherKey );	// To call after 'Init()'. 'User' contains the 'XML' tree containing the user configuration.
		void DumpConfigurationRegistry( txf::text_oflow__ &OFlow ) const
		{
			_Registry.DumpConfiguration( OFlow );
		}
		void DumpProjectRegistry( txf::text_oflow__ &OFlow ) const
		{
			_Registry.DumpProject( OFlow );
		}
		void DumpUserRegistry( txf::text_oflow__ &OFlow ) const
		{
			_Registry.DumpUser( OFlow );
		}
		time_t ProjectTimeStamp( void ) const
		{
			return ProjectTimeStamp();
		}
		bso::bool__ GetRegistryValue(
			const char *Path,
			str::string_ &Value )
		{
			return _Registry.GetValue( str::string( Path ), Value );
		}
		const lcl::rack__ &LocaleRack( void ) const
		{
			return _LocaleRack;
		}
	};

	inline bkdacc::id32__ _ExtractId32( const str::string_ &Value )
	{
		return Value.ToUL();
	}

	inline bkdacc::id16__ _ExtractId16( const str::string_ &Value )
	{
		return Value.ToUS();
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
