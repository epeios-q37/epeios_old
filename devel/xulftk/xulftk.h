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

	class _user_functions__
	{
	private:
		trunk___ *_Trunk;
	protected:
		trunk___ &_T( void )
		{
			if ( _Trunk == NULL )
				ERRc();

			return *_Trunk;
		}
		void _DefineSession(
			const str::string_ &FileName,
			const xpp::criterions___ &Criterions );
		void _ApplySession(	const frdkrn::compatibility_informations__ &CompatibilityInformations );
		// Demande de confirmation de la fermeture d'une session (projet). Normalement appelé par la redéfintion de 'XULFTKDropSession()' lorsque projet modifié.
		bso::bool__ _DefendSession( void );
		void _DropSession( void );
		// Ferme l'application. Normalement appelé par la redéfinition de 'XULFTKExit()'.
		void _Exit( void );
		virtual void XULFTKFormatedInformations( str::string_ &Informations )
		{
			ERRc();	// Si pas surchargé, alors 'xulfmn::about_eh__::NSXPCMOnEvent()' doit être redéfini.
		}
		virtual void XULFTKSiteURL( str::string_ &URL )
		{
			ERRc();	// Si pas surchargé, alors 'xulfmn::web_site_eh__::NSXPCMOnEvent()' doit être redéfini.
		}
		virtual void XULFTKDefineSession(
			const str::string_ &ProjectFileName,
			const xpp::criterions___ &XMLPreprocessorCriterions )
		{
			ERRc();	//	Si pas surchargé, alors 'xulfmn::open_project_eh__::NSXPCMOnEvent()' doit être redéfini.
		}
		virtual void XULFTKApplySession( void )
		{
			ERRc();	//	Si pas surchargé, alors 'xulfmn::open_project_eh__::NSXPCMOnEvent()' doit être redéfini.
		}
		virtual bso::bool__ XULFTKDropSession( void )	// Retourne 'true' si la session aeffectivement été fermée, 'false' sinon.
		{
			ERRc();	//	Si pas surchargé, alors 'xulfmn::close_project_eh__::NSXPCMOnEvent()' doit être redéfini.

			return false;	// Pour éviter un 'warning'.
		}
		virtual bso::bool__ XULFTKExit( void )
		{
			ERRc();	//	Si pas surchargé, alors 'xulfmn::exit_eh__::NSXPCMOnEvent()' doit être redéfini.

			return true;	// Pour éviter un 'warning'.
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Trunk = NULL;
		}
		E_CVDTOR( _user_functions__ );
		void Init( trunk___ &Trunk )
		{
			_Trunk = &Trunk;
		}
		void FormatedInformations( str::string_ &Informations )
		{
			XULFTKFormatedInformations( Informations );
		}
		void SiteURL( str::string_ &URL )
		{
			XULFTKSiteURL( URL );
		}
		void DefineSession(
			const str::string_ &ProjectFileName,
			const xpp::criterions___ &XMLPreprocessorCriterions )
		{
			XULFTKDefineSession( ProjectFileName, XMLPreprocessorCriterions );
		}
		void ApplySession( void )
		{
			XULFTKApplySession();
		}
		bso::bool__ DropSession( void )	// Retourne 'true' si la session aeffectivement été fermée, 'false' sinon.
		{
			return XULFTKDropSession();
		}
		bso::bool__ Exit( void )
		{
			return XULFTKExit();
		}
	};

	template <typename trunk> class user_functions__
	: public _user_functions__
	{
	protected:
		trunk &_T( void )
		{
			return *(trunk *)&_user_functions__::_T();	// On peut caster, parce que 'Init(...)' garantit que l'objet est du bon type.
		}
	public:
		void Init( trunk &Trunk )
		{
			_user_functions__::Init( Trunk );
		}
	};

	class trunk___
	{
	private:
		xulfui::ui___ *_UI;
		xulfkl::kernel___ *_Kernel;
		_user_functions__ *_UserFunctions;
		geckoo::user_functions__  *_Steering;
		const char *_TargetName;
		const char *_DefaultXSLRootPath;
		error_reporting_functions__ _DefaultErrorReportingFunctions;
		_user_functions__ &_UF( void )
		{
			if ( _UserFunctions == NULL )
				ERRc();

			return *_UserFunctions;
		}
	protected:
		void Handle_( frdkrn::status__ Status )
		{
			switch( Status ) {
			case frdkrn::sOK:
				break;
			case frdkrn::sWarning:
				_UI->LogAndPrompt( _Kernel->Message() );
				break;
			case frdkrn::sError:
				_UI->Alert( _Kernel->Message() );
				ERRAbort();
				break;
			default:
				ERRc();
				break;
			}
		}
		void _DefineSession(
			const str::string_ &ProjectFileName,	// Si non vide, contient le nom du fichier projet avec lequel préremplir le 'SessionForm'.
			const xpp::criterions___ &Criterions );	// 'registry' qui contient la configuration de l'application.
		// Normalement appelée par la redéfintion de 'XULFTKApplySession()'. Charge le projet correspondant au fichier 'FileName'.
		void _ApplySession(	const frdkrn::compatibility_informations__ &CompatibilityInformations );
		// Demande de confirmation de la fermeture d'une session (projet). Normalement appelé par la redéfintion de 'XULFTKDropSession()' lorsque projet modifié.
		bso::bool__ _DefendSession( void );
		void _DropSession( void );
		// Ferme l'application. Normalement appelé par la redéfinition de 'XULFTKExit()'.
		void _Exit( void )
		{
			UI().Main().Widgets.wdwMain.Close();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_UI = NULL;
			_Kernel =  NULL;
			_UserFunctions = NULL;
			_Steering = NULL;
			_DefaultXSLRootPath = NULL;
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
			const char *DefaultXSLRootPath,	// ATTENTION : pointeur copié, contenu NON dupliqué.
			xulfui::ui___ &UI,
			xulfkl::kernel___ &Kernel,
			_user_functions__ &UserFunctions,
			geckoo::user_functions__ &Steering,
			const lcl::locale_ &Locale,
			const char *Language )
		{
			_UI = &UI;
			_Kernel = &Kernel;
			_TargetName = TargetName;
			_UserFunctions = &UserFunctions;
			_Steering = &Steering;
			_DefaultXSLRootPath = DefaultXSLRootPath;
			_DefaultErrorReportingFunctions.Init( UI, Locale, Language );

			return frdkrn::sOK;
		}
		void XULWDGReport( const char *Message )	// Pseudo méthode virtuelle.
		{
			_UI->LogAndPrompt( Message );
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
		frdrgy::registry_ &Registry( void )
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

			_UF().FormatedInformations( Informations ),

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

			_UF().SiteURL( URL );

			nsxpcm::LaunchURI( URL );
		ERRErr
		ERREnd
		ERREpilog
		}
		bso::bool__ DefineSession(
			const str::string_ &ProjectFileName,
			const xpp::criterions___ &Criterions )
		{
			if ( _UF().DropSession() ) {
				_UF().DefineSession( ProjectFileName, Criterions );
				return true;
			} else
				return false;
		}
		void ApplySession( void )
		{
			_UF().ApplySession();
			UpdateUI();
		}
		bso::bool__ DropSession( void )
		{
			if ( _UF().DropSession() ) {
				UpdateUI();
				return true;
			} else
				return false;
		}
		bso::bool__ Exit( void )
		{
			bso::bool__ Confirmation = false;
		ERRProlog
			STR_BUFFER___ Buffer;
		ERRBegin
			if ( Confirmation = nsxpcm::Confirm( UI().Main().Window(), GetTranslation( xulfkl::mExitConfirmation, Kernel().LocaleRack(), Buffer) ) )
				Confirmation = _UF().Exit();
		ERRErr
		ERREnd
		ERREpilog
		return Confirmation;
		}
		const char *DefaultXSLRootPath( void ) const
		{
			if ( _DefaultXSLRootPath == NULL )
				ERRu();

			return _DefaultXSLRootPath;
		}
		friend _user_functions__;
	};

	inline void _user_functions__::_DefineSession(
		const str::string_ &ProjectFileName,
		const xpp::criterions___ &Criterions )
	{
		_T()._DefineSession( ProjectFileName, Criterions );
	}

	inline void _user_functions__::_ApplySession( const frdkrn::compatibility_informations__ &CompatibilityInformations )
	{
		_T()._ApplySession( CompatibilityInformations );
	}

	inline bso::bool__ _user_functions__::_DefendSession( void )
	{
		return _T()._DefendSession();
	}

	inline void _user_functions__::_DropSession( void )
	{
		_T()._DropSession();
	}

	inline void _user_functions__::_Exit( void )
	{
		_T()._Exit();
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
