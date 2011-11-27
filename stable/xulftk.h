/*
	Header for the 'xulftk' library by Claude SIMON (csimon at zeusw dot org)
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
		xulfui::ui___ *_UI;
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
			xulfui::ui___ &UI,
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
			ERRu();	// Si pas surcharg�, alors 'xulfmn::about_command__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		virtual void XULFTKSiteURL( str::string_ &URL )
		{
			ERRu();	// Si pas surcharg�, alors 'xulfmn::web_site_command__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		// Normalement appel�e par la red�fintion de 'XULFTKApplySession()'. Charge le projet correspondant au fichier 'FileName'.
		void _ApplySession(
			const str::string_ &FileName,
			const xpp::criterions___ &Criterions,
			const frdkrn::compatibility_informations__ &CompatibilityInformations,
			const rgstry::multi_level_registry_ &Registry );	// 'registry' qui contient la configuration de l'application.
		virtual void XULFTKApplySession(
			const str::string_ &FileName,
			const xpp::criterions___ &XMLPreprocessorCriterions )
		{
			ERRu();	//	Si pas surcharg�, alors 'xulfmn::open_project_command__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		// Demande de confirmation de la fermeture d'une session (projet). Normalement appel� par la red�fintion de 'XULFTKDropSession()' lorsque projet modifi�.
		bso::bool__ _DefendSession( void );
		void _DropSession( void );
		virtual bso::bool__ XULFTKDropSession( void )	// Retourne 'true' si la session aeffectivement �t� ferm�e, 'false' sinon.
		{
			ERRu();	//	Si pas surcharg�, alors 'xulfmn::close_project_command__::NSXPCMOnEvent()' doit �tre red�fini.

			return false;	// Pour �viter un 'warning'.
		}
		// Ferme l'application. Normalement appel� par la red�finition de 'XULFTKExit()'.
		void _Exit( void )
		{
			UI().Main().Widgets.Window.Close();
		}
		virtual void XULFTKExit( void )
		{
			ERRu();	//	Si pas surcharg�, alors 'xulfmn::exit_command__::NSXPCMOnEvent()' doit �tre red�fini.
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
			const char *TargetName,	// ATTENTION : pointeur copi�, contenu NON dupliqu�.
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
		void UpdateUI( void )
		{
			UI().Update();
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

			if ( nsxpcm::XPRJFileOpenDialogBox( UI().Main().Window(), Kernel().GetTranslation( xulfkl::mSelectProjectFile, Translation ), Kernel().LocaleRack(), FileName ) ) {
				XULFTKApplySession( FileName, XMLPreprocessorCriterions );
				UpdateUI();
			}
		ERRErr
		ERREnd
		ERREpilog
		}
		bso::bool__ DropSession( void )
		{
			if ( XULFTKDropSession() ) {
				UpdateUI();
				return true;
			} else
				return false;
		}
		void Exit( void )
		{
		ERRProlog
			STR_BUFFER___ Buffer;
		ERRBegin
			if ( nsxpcm::Confirm( UI().Main().Window(), Kernel().GetTranslation( xulfkl::mExitConfirmation, Buffer) ) )
				XULFTKExit();
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
