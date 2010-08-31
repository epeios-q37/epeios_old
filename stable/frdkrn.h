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

	class kernel___
	{
	private:
		lcl::locales_rack___ _Locales;
		csducl::universal_client_core _ClientCore;
		frdrgy::registry _Registry;
		frdbkd::_backend___ _Backend;
		bso::bool__ _Connect(
			const char *RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type,
			csdsnc::log_functions__ &LogFunctions,
			error_reporting_functions___ &ErrorReportingFunctions );
		bso::bool__ _Connect(
			const str::string_ &RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type,
			csdsnc::log_functions__ &LogFunctions,
			error_reporting_functions___ &ErrorReportingFunctions );
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
			_Locales.reset( P );
		}
		kernel___( void )
		{
			reset( false );
		}
		~kernel___( void )
		{
			reset();
		}
		void Init(
			const frdrgy::registry_ &Registry,
			const str::string_ &Language,
			const lcl::locales_ &Locales )	// 'Locales' n'est pas dupliqué !
		{
			// L'initialisation de '_Backend' et '_ClientCore' se fait à la connection.

			_Registry.Init();
			_Registry = Registry;

			_Locales.Init( Locales, Language );
		}
		bso::bool__ Connect(
			const char *RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type,
			csdsnc::log_functions__ &LogFunctions,
			error_reporting_functions___ &ErrorReportingFunctions )
		{
			_Connect( RemoteHostServiceOrLocalLibraryPath, Type, LogFunctions, ErrorReportingFunctions );
		}
		bso::bool__ Connect(
			const str::string_ &RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type,
			csdsnc::log_functions__ &LogFunctions,
			error_reporting_functions___ &ErrorReportingFunctions )
		{
			_Connect( RemoteHostServiceOrLocalLibraryPath, Type, LogFunctions, ErrorReportingFunctions );
		}
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
		const str::string_ &Language( void ) const
		{
			return _Locales.Language();
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
