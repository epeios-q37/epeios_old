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

	// Si modifié, modifier 'GetLabel_(...)' en conséquence ainsi que le '.xlcl' associé.
	enum status__ {
		sOK,
		sConfigurationParsingError,	// Error during configuration file parsing. See 'ErrorSet' for more details.
		sLocaleParsingError,		// Error during locales file handling. See 'ErrorSet' for more details.
		sNoLocaleFileDefined,		// No locales file is defined. 
		sProjectParsingError,		// Error during project file handling. See 'ErrorSet' for more details.
		sNoOrBadBackendDefinition,
		sNoBackendLocation,
		sUnableToConnect,
		s_amount,
		s_Undefined
	};

#define FRDKRN__S_AMOUNT	8	// Pour détecter les fonctions devant être modifiée si le nombre d'entrée de 'status__' est modifié.

	const str::string_ &GetTranslation(
		status__ Status,
		const lcl::locale_rack___ &Locale,
		str::string_ &Translation );

	struct error_set___
	{
	public:
		rgstry::error__ Error;
		rgstry::error_details Details;
		void reset( bso::bool__ P = true )
		{
			Error = rgstry::e_Undefined;
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
			Error = rgstry::e_Undefined;

			Details.Init();
		}
	};

	inline bso::bool__ IsErrorSetRelevant( status__ Status )
	{
#if FRDKRN__S_AMOUNT != 8
#	error "'status__' modified !"
#endif
		switch ( Status  ) {
		case sOK:
			return false;
			break;
		case sConfigurationParsingError:
		case sLocaleParsingError:
			return true;
			break;
		case sNoLocaleFileDefined:
			return false;
			break;
		case sProjectParsingError:
			return true;
			break;
		case sNoOrBadBackendDefinition:
		case sNoBackendLocation:
		case sUnableToConnect:
			return false;
		default:
			ERRu();
			break;
		}

		return false;	// Pour éviter un 'warning'.
	}


	inline const str::string_ &GetTranslation(
		const error_set___ &ErrorSet,
		const lcl::locale_rack___ &Locale,
		str::string_ &Translation )
	{
		return rgstry::GetTranslation( ErrorSet.Error, ErrorSet.Details, Locale, Translation );
	}

	const str::string_ &GetTranslation(
		status__ Status,
		const error_set___ &ErrorSet,
		const lcl::locale_rack___ &Locale,
		str::string_ &Translation );


	bso::bool__ GetDefaultConfigurationFileName(
		const char *Affix,
		str::string_ &FileName );

	class kernel___
	{
	private:
		lcl::locale _Locale;
		lcl::locale_rack___ _LocaleRack;
		csducl::universal_client_core _ClientCore;
		frdrgy::registry _Registry;
		frdbkd::_backend___ _Backend;
		status__ _Connect(
			const char *RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type,
			error_reporting_functions___ &ErrorReportingFunctions,
			csdsnc::log_functions__ &LogFunctions );
		status__ _Connect(
			const str::string_ &RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type,
			error_reporting_functions___ &ErrorReportingFunctions,
			csdsnc::log_functions__ &LogFunctions );
		void _CloseConnection( void )
		{
			if ( !IsConnected() )
				ERRu();

			_Backend.reset();
			_ClientCore.reset();
		}
		void _EndClosing( void )
		{
			_Registry.Init();
		}
	protected:
		virtual const str::string_ &FRDKERNLGetVersionFormatedText( str::string_ &Version ) = 0;
		virtual void FRDKERNLClose( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				Close();

			_Backend.reset( P );
			_ClientCore.reset( P );
			_Registry.reset( P );
			_Locale.reset( P );
			_LocaleRack.reset( P );
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
			const str::string_ &ConfigurationFileName,
			const char *TargetName,
			const str::string_ &Language,
			error_set___ &ErrorSet );
		status__ Connect(
			const char *RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type,
			error_reporting_functions___ &ErrorReportingFunctions = *(error_reporting_functions___ *)NULL,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL )
		{
			return _Connect( RemoteHostServiceOrLocalLibraryPath, Type, ErrorReportingFunctions, LogFunctions );
		}
		status__ Connect(
			const str::string_ &RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type,
			error_reporting_functions___ &ErrorReportingFunctions = *(error_reporting_functions___ *)NULL,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL )
		{
			return _Connect( RemoteHostServiceOrLocalLibraryPath, Type, ErrorReportingFunctions, LogFunctions );
		}
		status__ Connect( // Try to connect using registry content.
			error_reporting_functions___ &ErrorReportingFunctions = *(error_reporting_functions___ *)NULL,
			csdsnc::log_functions__ &LogFunctions = *(csdsnc::log_functions__ *)NULL );
		const void AboutBackend(
			str::string_ &BackendInformations,
			str::string_ &PublisherInformations )
		{
			_Backend.About( BackendInformations, PublisherInformations );
		}
		void Close( void )
		{
			if ( IsConnected() ) {
				FRDKERNLClose();
				_CloseConnection();
			}

			_EndClosing();
		}
		bso::bool__ IsConnected( void ) const
		{
			return _Backend.IsConnected();
		}
		status__ FillProjectRegistry(
			const str::string_ &FileName,
			const char *TargetName,
			error_set___ &ErrorSet );
		void FillUserRegistry( flw::iflow__ &User );	// To call after 'Init()'. 'User' contains the 'XML' tree containing the user configuration.
		void DumpConfigurationRegistry( txf::text_oflow__ &OFlow ) const
		{
			_Registry.DumpConfigurationRegistry( OFlow );
		}
		void DumpProjectRegistry( txf::text_oflow__ &OFlow ) const
		{
			_Registry.DumpProjectRegistry( OFlow );
		}
		void DumpUserRegistry( txf::text_oflow__ &OFlow ) const
		{
			_Registry.DumpUserRegistry( OFlow );
		}
		bso::bool__ GetRegistryValue(
			const char *Path,
			str::string_ &Value )
		{
			return _Registry.GetValue( str::string( Path ), Value );
		}
		const str::string_ &GetVersionFormatedText( str::string_ &Version )
		{
			return FRDKERNLGetVersionFormatedText( Version );
		}
		const lcl::locale_rack___ &Locale( void ) const
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
