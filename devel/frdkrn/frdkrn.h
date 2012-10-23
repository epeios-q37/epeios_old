/*
	Header for the 'frdkrn' library by Claude SIMON (csimon at zeusw dot org)
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
#include "csdleo.h"

#define FRDKRN_CONFIGURATION_FILE_EXTENSION	"xcfg"
#define FRDKRN_PROJECT_FILE_EXTENSION		"xprj"
#define FRDKRN_LOCALE_FILE_EXTENSION		"xlcl"

namespace frdkrn {
	using namespace frdbkd;

	enum project_type__
	{
		ptNew,
		ptPredefined,
		ptUser,
		pt_amount,
		pt_Undefined
	};

	project_type__ GetProjectType(
		const str::string_ &Label,
		err::handling__ ErrHandling = err::h_Default );

	const str::string_ & GetPredefinedProjectLocation(
		const str::string_ &Id,
		const frdrgy::registry_ &Registry,
		str::string_ &Location );

	enum backend_extended_type__ {
		bxtNone,	// Non utilisation de 'backend'.
		bxtPredefined,
		bxtDaemon,
		bxtEmbedded,
		bxt_amount,
		bxt_Undefined
	};

	backend_extended_type__ GetBackendExtendedType( const str::string_ &RawType );

	backend_extended_type__ GetBackendExtendedType( const frdrgy::registry_ &Registry );

	void SetBackendExtendedType(
		frdrgy::_registry_ &Registry,
		backend_extended_type__ Type );

	csducl::type__ GetBackendTypeAndLocation(
		const frdrgy::registry_ &Registry,
		str::string_ &Location );

	class log_functions__
	: public csdsnc::log_functions__
	{
	protected:
		virtual void CSDSNCLog(
			csdsnc::log__ Log,
			const void *Flow,
			mdr::size__ Amount )
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
	enum recap__ {
		rProjectParsingError,		// Error during project file handling. See 'ErrorSet' for more details.
		rSettingsParsingError,		// Error during settings data handling. See 'ErrorSet' for more details.
		rNoOrBadProjectId,
		rNoOrBadBackendDefinition,
		rNoBackendLocation,
		rUnableToConnect,
		rIncompatibleBackend,
		rBackendError,
		rUnableToOpenFile,
		r_amount,
		r_Undefined,
		r_OK,
	};

#define FRDKRN__R_AMOUNT	9	// Pour détecter les fonctions devant être modifiée si le nombre d'entrée de 'report__' est modifié.

	const char *GetLabel( recap__ Recap );

	inline const str::string_ &GetTranslation(
		recap__ Recap,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation )
	{
		LocaleRack.GetTranslation( GetLabel( Recap ), "FRDKRN_", Translation );

		return Translation;
	}

	struct error_set___
	{
	public:
		rgstry::context___ Context;
		incompatibility_informations IncompatibilityInformations;	// Quand survient un 'report__::rIncompatibleBackend'.
		str::string Misc;
		void reset( bso::bool__ P = true )
		{
			Context.reset( P );
			IncompatibilityInformations.reset( P );
			Misc.reset( P );
		}
		E_CDTOR( error_set___ );
		void Init( void )
		{
			Context.Init();
			IncompatibilityInformations.Init();
			Misc.Init();
		}
	};

	inline const str::string_ &GetTranslation(
		const error_set___ &ErrorSet,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation )
	{
		return rgstry::GetTranslation( ErrorSet.Context, LocaleRack, Translation );
	}

	const str::string_ &GetTranslation(
		recap__ Recap,
		const error_set___ &ErrorSet,
		const lcl::rack__ &LocaleRack,
		str::string_ &Translation );


	bso::bool__ GetDefaultConfigurationFileName(
		const char *Affix,
		str::string_ &FileName );

	typedef fblfrd::reporting_functions__ _backend_error_reporting_functions__;

	class reporting_functions__
	: public _backend_error_reporting_functions__
	{
	protected:
		void FBLFRDReport(
			fblovl::reply__ Reply,
			const char *Message )
		{
			ReportBackendError( Reply, Message );
		}
		virtual void FRDKRNReportBackendError(
			fblovl::reply__ Reply,
			const char *Message ) = 0;
		virtual void FRDKRNReportFrontendError( const char *Message ) = 0;
	public:
		void reset ( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_VDTOR( reporting_functions__ )
		void Init( void )
		{
			// Standardisation.
		}
		void ReportBackendError(
			fblovl::reply__ Reply,
			const char *Message )
		{
			FRDKRNReportBackendError( Reply, Message );
		}
		void ReportFrontendError( const char *Message )
		{
			FRDKRNReportFrontendError( Message );
		}
	};



	class kernel___
	{
	private:
		lcl::rack__ _LocaleRack;
		csducl::universal_client_core _ClientCore;
		frdrgy::registry _Registry;
# if 0
		frdbkd::_backend___ _Backend;
# else
		frdbkd::backend___ _Backend;
# endif
		str::string _Message;
		time_t _ProjectOriginalTimeStamp;	// Horodatage de la créationn du chargement du projet ou de sa dernière sauvegarde. Si == 0, pas de projet en cours d'utilisation.
		time_t _ProjectModificationTimeStamp;	// Horodatage de la dernière modification du projet.
		reporting_functions__ *_ReportingFunctions;
		recap__ _Connect(
			const str::string_ &RemoteHostServiceOrLocalLibraryPath,
			const compatibility_informations__ &CompatibilityInformations,
			csducl::type__ Type,
			reporting_functions__ &ReportingFunctions,
			error_set___ &ErrorSet,
			csdsnc::log_functions__ &LogFunctions );
		void _CloseConnection( void )
		{
			if ( !IsConnected() )
				ERRu();

			FRDKRNDisconnection();

			_Backend.reset();
			_ClientCore.reset();
		}
	protected:
		recap__ _FillProjectRegistry(
			const str::string_ &FileName,
			const char *TargetName,
			const xpp::criterions___ &Criterions,
			str::string_ &Id,
			error_set___ &ErrorSet );
		recap__ _DumpProjectRegistry(
			const str::string_ &FileName,
			const char *TargetName,
			const str::string_ &Id,
			error_set___ &ErrorSet );
		recap__ _Connect(
			const char *RemoteHostServiceOrLocalLibraryPath,
			const compatibility_informations__ &CompatibilityInformations,
			csducl::type__ Type,
			reporting_functions__ &ReportingFunctions,
			error_set___ &ErrorSet,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL );
		recap__ _Connect( // Try to connect using registry content.
			const compatibility_informations__ &CompatibilityInformations,
			reporting_functions__ &ReportingFunctions,
			error_set___ &ErrorSet,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL );
		virtual void FRDKRNConnection( fblfrd::backend_access___ &BackendAccess ) = 0;	// Appelé lors aprés connection au 'backned'.
		virtual void FRDKRNDisconnection( void ) = 0;	// Appelé avant déconnexion du 'backend'.
		void _Report( const char *Message )const
		{
			if( _ReportingFunctions == NULL )
				ERRc();

			return _ReportingFunctions->ReportFrontendError( Message );
		}
		void _Report( const str::string_ &Message ) const
		{
		ERRProlog
			STR_BUFFER___ Buffer;
		ERRBegin
			_Report( Message.Convert( Buffer ) );
		ERRErr
		ERREnd
		ERREpilog
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				CloseProject();

			_Backend.reset( P );
			_ClientCore.reset( P );
			_Registry.reset( P );
			_LocaleRack.reset( P );
			_Message.reset( P );
			_ProjectOriginalTimeStamp = 0;
			_ProjectModificationTimeStamp = 0;
			_ReportingFunctions = NULL;
		}
		kernel___( void )
		{
			reset( false );
		}
		~kernel___( void )
		{
			reset();
		}
		status__ Init(
			const rgstry::registry_ &ConfigurationRegistry,
			rgstry::row__ ConfigurationRegistryRoot,
			const char *TargetName,
			const lcl::locale_ &Locale,
			const char *Language,
			reporting_functions__ &ReportingFunctions )
		{
			_Registry.Init( ConfigurationRegistry, ConfigurationRegistryRoot );
			_Message.Init();
			_LocaleRack.Init( Locale, Language );
			_ProjectOriginalTimeStamp = 0;
			_ReportingFunctions = &ReportingFunctions;
			// L'initialisation de '_Backend' et '_ClientCore' se fait à la connection.

			return sOK;
		}
		fblfrd::backend_access___ &BackendAccess( void )
		{
			if ( !_Backend.IsConnected() )
				ERRc();

			return _Backend.BackendAccess();
		}
		const frdrgy::registry_ &Registry( void ) const
		{
			return _Registry;
		}
		frdrgy::registry_ &Registry( void )
		{
			return _Registry;
		}
		recap__ LoadProject(
			const str::string_ &FileName,
			const char *TargetName,
			const xpp::criterions___ &Criterions,
			str::string_ &Id,
			error_set___ &ErrorSet )	// Ne réalise qu'une mise à jour de la registry avec le contenu du projet. Ne réalise pas de connection.
		{
			return _FillProjectRegistry( FileName, TargetName, Criterions, Id, ErrorSet );
		}
		recap__ LaunchProject(
			const compatibility_informations__ &CompatibilityInformations,
			reporting_functions__ &ReportingFunctions,
			error_set___ &ErrorSet )	// Les paramètres de connection sont récupèrés de la 'registry'.
		{
			recap__ Recap = r_OK;
			
			if ( GetBackendExtendedType( Registry() ) != bxtNone )
				Recap = _Connect( CompatibilityInformations, ReportingFunctions, ErrorSet );

			if ( Recap == r_OK ) {
				_ProjectOriginalTimeStamp = time( NULL );
				_ProjectModificationTimeStamp = 0;
			}

			return Recap;
		}
		status__ LoadProject(
			const str::string_ &FileName,
			const char *TargetName,
			const xpp::criterions___ &Criterions,
			str::string_ &Id );
		status__ LaunchProject(
			const compatibility_informations__ &CompatibilityInformations,
			str::string_ &Id,
			reporting_functions__ &ReportingFunctions );
		recap__ SaveProject(
			const str::string_ &FileName,
			const char *TargetName,
			const str::string_ &Id, 
			error_set___ &ErrorSet );
		status__ SaveProject(
			const str::string_ &FileName,
			const char *TargetName,
			const str::string_ &Id );
		recap__ FillSettingsRegistry(
			xtf::extended_text_iflow__ &SettingsXFlow,
			const xpp::criterions___ &Criterions,
			error_set___ &ErrorSet );
		status__ FillSettingsRegistry(
			xtf::extended_text_iflow__ &SettingsXFlow,
			const xpp::criterions___ &Criterions );
		recap__ DumpSettingsRegistry( xml::writer_ &Writer ) const
		{
			_Registry.DumpSettings( true, Writer);

			return r_OK;
		}
		void Touch( void )
		{
			_ProjectModificationTimeStamp = time( NULL );
		}
		time_t ProjectModificationTimeStamp( void ) const
		{
			return _ProjectModificationTimeStamp;
		}
		bso::bool__ IsProjectInProgress( void ) const
		{
			return _ProjectOriginalTimeStamp != 0;
		}
		bso::bool__ IsProjectModified( void ) const
		{
			return _ProjectOriginalTimeStamp < ProjectModificationTimeStamp();
		}
		E_RWDISCLOSE__( str::string_, Message );
		void AboutBackend(
			str::string_ &ProtocolVersion,
			str::string_ &BackendLabel,
			str::string_ &APIVersion,
			str::string_ &BackendInformations )
		{
			_Backend.About( ProtocolVersion, BackendLabel, APIVersion, BackendInformations );
		}
		void ThrowError( void )
		{
			_Backend.ThrowError();
		}
		status__ CloseProject( void )
		{
			if ( IsConnected() ) {
				_CloseConnection();
			}

			_ProjectOriginalTimeStamp =	_ProjectModificationTimeStamp = 0;

			return sOK;
		}
		bso::bool__ IsConnected( void ) const
		{
			return _Backend.IsConnected();
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
		const char *Language( void ) const
		{
			return _LocaleRack.Language();
		}
	};

	enum authentication_prompt_mode__ 
	{
		apmNone,	// Pas de prompt, 'login' et 'password' envoyés sont vides (droits minimaux).
		apmAuto,	// Pas de prompt, 'login' et 'password' envoyés sont ceux stockés dans le fichier de configuration.
		apmEmpty,	// Prompt affiché, avec champs vides.
		apmPartial,	// Prompt affiché, avec champ 'login' prérenpli.
		apmFull,	// Prompt affiché, avec champs 'login' et 'password' préremplis.
		apm_amount,
		apm_Undefined
	};

	authentication_prompt_mode__ GetAuthenticationPromptMode( const frdrgy::registry_ &Registry );

	inline fblfrd::id32__ _ExtractId32( const str::string_ &Value )
	{
		return Value.ToUL();
	}

	inline fblfrd::id16__ _ExtractId16( const str::string_ &Value )
	{
		return Value.ToUS();
	}

	inline fblfrd::id8__ _ExtractId8( const str::string_ &Value )
	{
		return Value.ToUB();
	}

	void GetPredefinedProjects(
		const frdrgy::registry_ &Registry,
		const lcl::rack__ &Locale,
		xml::writer_ &Writer );

	void GetPredefinedBackends(
		const frdrgy::registry_ &Registry,
		const lcl::rack__ &Locale,
		xml::writer_ &Writer );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
