/*
	Header for the 'xulftk' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFTK__INC
#define XULFTK__INC

#define XULFTK_NAME		"XULFTK"

#define	XULFTK_VERSION	"$Revision$"

#define XULFTK_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFTKTutor;

#if defined( XXX_DBG ) && !defined( XULFTK_NODBG )
#define XULFTK_DBG
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
//D XUL Frontend TrunK 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "nsxpcm.h"
# include "frdkrn.h"

# include "geckob.h"

# include "xulfui.h"
# include "xulfkl.h"

namespace xulftk {
	using xulfkl::status__;

	typedef frdkrn::error_reporting_functions__ _error_reporting_functions__;

	class error_reporting_functions__
	: public _error_reporting_functions__
	{
	private:
		const xulfui::ui___ *_UI;
		lcl::rack__ _Rack;
	protected:
		virtual void FBLFRDReportError(
			fblovl::reply__ Reply,
			const char *Message );
	public:
		void reset( bso::bool__ P = true )
		{
			_error_reporting_functions__::reset( P );
		}
		E_CVDTOR( error_reporting_functions__ );
		void Init(
			const xulfui::ui___ &UI,
			const lcl::locale_ &Locale,
			const char *Language )
		{
			reset();

			_error_reporting_functions__::Init();
			_UI = &UI;
			_Rack.Init( Locale, Language );
		}
	};


	class trunk___
	{
	private:
		xulfui::ui___ *_UI;
		xulfkl::kernel___ *_Kernel;
		geckoo::user_functions__  *_Steering;
		const char *_TargetName;
		error_reporting_functions__ _DefaultErrorReportingFunctions;
	protected:
		virtual void XULFTKFormatedInformations( str::string_ &Informations )
		{
			ERRu();	// Si pas surchargé, alors 'xulfmn::about_command__::NSXPCMOnEvent(...)' doit être redéfini.
		}
		virtual void XULFTKSiteURL( str::string_ &URL )
		{
			ERRu();	// Si pas surchargé, alors 'xulfmn::web_site_command__::NSXPCMOnEvent') doit être redéfini.
		}
		// Normalement appelé par la redéfintion de la fonciton qui suit.
		void _ApplySession(
			const str::string_ &FileName,
			const xpp::criterions___ &Criterions,
			const frdkrn::compatibility_informations__ &CompatibilityInformations )
		{
			if ( Kernel().LoadProject( FileName, _TargetName, Criterions, CompatibilityInformations ) != frdkrn::sOK )
				UI().Alert( Kernel().Message() );

			UpdateUI();
		}
		virtual void XULFTKApplySession(
			const str::string_ &FileName,
			const xpp::criterions___ &XMLPreprocessorCriterions )
		{
			ERRu();	//	Si pas surchargé, alors xulfmn::open_project_command__::NSXPCMOnEvent') doit être redéfini.
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_UI = NULL;
			_Kernel =  NULL;
			_Steering = NULL;
			_DefaultErrorReportingFunctions.reset( P );
//			_TrunkFunctions.reset();
		}
		trunk___( void )
		{
			reset( false );
		}
		~trunk___( void )
		{
			reset();
		}
		status__ Init(
			const char *TargetName,	// ATTENTION : pointeur copié, contenu NON dupliqué.
			xulfui::ui___ &UI,
			xulfkl::kernel___ &Kernel,
			geckoo::user_functions__ &Steering,
			const lcl::locale_ &Locale,
			const char *Language )
		{
			_UI = &UI;
			_Kernel = &Kernel;
			_TargetName = TargetName;
			_Steering = &Steering;
			_DefaultErrorReportingFunctions.Init( UI, Locale, Language );

			return frdkrn::sOK;
		}
		_error_reporting_functions__ &DefaultErrorReportingFunctions( void )
		{
			return _DefaultErrorReportingFunctions;
		}
		void ExposeSteering( void )
		{
			geckob::SetLoneSteering( *_Steering );
		}
		const xulfkl::kernel___ &Kernel( void ) const
		{
#ifdef XULFTK_DBG
			if ( _Kernel == NULL )
				ERRu();
#endif
			return *_Kernel;
		}
		xulfkl::kernel___ &Kernel( void )
		{
#ifdef XULFTK_DBG
			if ( _Kernel == NULL )
				ERRu();
#endif
			return *_Kernel;
		}
		const xulfui::ui___ &UI( void ) const
		{
#ifdef XULFTK_DBG
			if ( _Kernel == NULL )
				ERRu();
#endif
			return *_UI;
		}
		xulfui::ui___ &UI( void )
		{
#ifdef XULFTK_DBG
			if ( _Kernel == NULL )
				ERRu();
#endif
			return *_UI;
		}
		const frdrgy::registry_ &Registry( void ) const
		{
			return Kernel().Registry();
		}
		const str::string_ &Message( void ) const
		{
			return Kernel().Message();
		}
		void SaveUserRegistry( void )
		{
		ERRProlog
			str::string Data;
			flx::E_STRING_OFLOW___ SOFlow;
			txf::text_oflow__ TOFlow;
		ERRBegin
			Data.Init();
			TOFlow.Init( SOFlow );
			SOFlow.Init( Data );

			Kernel().DumpUserRegistry( TOFlow );

			SOFlow.reset();	// To flush buffer.

			UI().SaveUserData( Data );
		ERRErr
		ERREnd
		ERREpilog
		}
		bso::bool__ Exit( void )
		{
			SaveUserRegistry();
			UI().Main().Widgets.Window.Close();
			// nsxpcm::Close( UI().Main.Window );
			Kernel().Close();

			return true;
		}
		void UpdateUI( void )
		{
			UI().Main().Widgets.Commands.CloseProject.Enable( Kernel().IsProjectInProgress() );
//			UI().Debug.UpdateUI();	// Comme il s'agit d'une boîte de dialogue, n'est mis à jour qu'à son ouverture.
		}
		void BrowseInformations( void )
		{
		ERRProlog
			str::string Informations;
		ERRBegin
			Informations.Init();

			XULFTKFormatedInformations( Informations ),

			UI().Alert( Informations );
		ERRErr
		ERREnd
		ERREpilog
		}
		void BrowseWEBSite( void )
		{
		ERRProlog
			str::string URL;
		ERRBegin
			URL.Init();

			XULFTKSiteURL( URL );

			nsxpcm::LaunchURI( URL );
		ERRErr
		ERREnd
		ERREpilog
		}
		void  ApplySession( const xpp::criterions___ &XMLPreprocessorCriterions )
		{
		ERRProlog
			str::string Translation;
			str::string FileName;
		ERRBegin
			Translation.Init();
			FileName.Init();

			if ( nsxpcm::XPRJFileOpenDialogBox( UI().Main().Window(), Kernel().GetTranslation( xulfkl::mSelectProjectFile, Translation ), Kernel().LocaleRack(), FileName ) )
				XULFTKApplySession( FileName, XMLPreprocessorCriterions );
		ERRErr
		ERREnd
		ERREpilog
		}
		void CloseSession( void )
		{
		ERRProlog
			STR_BUFFER___ Buffer;
		ERRBegin
			if ( nsxpcm::Confirm( UI().Main().Window(), Kernel().GetTranslation( xulfkl::mExitConfirmation, Buffer) ) )
				Exit();
		ERRErr
		ERREnd
		ERREpilog
		}
		void DefineSession( void )
		{
			UpdateUI();
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
