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

	typedef frdkrn::reporting_functions__ _reporting_functions__;

	class reporting_functions__
	: public _reporting_functions__
	{
	private:
		trunk___ *_Trunk;
	protected:
		virtual void FRDKRNReportBackendError( const str::string_ &Message );
		virtual void FRDKRNReportFrontendError( const str::string_ &Message );
	public:
		void reset( bso::bool__ P = true )
		{
			_reporting_functions__::reset( P );
			_Trunk = NULL;
		}
		E_CVDTOR( reporting_functions__ );
		void Init( trunk___ &Trunk );
	};

	class _user_callback__
	{
	private:
		trunk___ *_Trunk;
		const char *_LauncherIdentification;	// Identification du lanceur du composant (normalement 'egeckcom' avec sa version).
	protected:
		trunk___ &_T( void )
		{
			if ( _Trunk == NULL )
				ERRc();

			return *_Trunk;
		}
		void _GoToHome( void )
		{}	// Standadisation.
		void _DefineSession(
			const str::string_ &FileName,
			const xpp::criterions___ &Criterions );
		void _ApplySession( const frdkrn::compatibility_informations__ &CompatibilityInformations );
		void _CancelSession( void );
		// Demande de confirmation de la fermeture d'une session (projet). Normalement appel� par la red�fintion de 'XULFTKDropSession()' lorsque projet modifi�.
		bso::bool__ _DefendSession( void );
		void _DropSession( void );
		// Ferme l'application. Normalement appel� par la red�finition de 'XULFTKExit()'.
		void _Exit( void );
		virtual void XULFTKFormatedInformations(
			const char *LauncherIdentification,
			str::string_ &Informations )
		{
			ERRc();	// Si pas surcharg�, alors 'xulfmn::about_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		virtual void XULFTKSiteURL( str::string_ &URL )
		{
			ERRc();	// Si pas surcharg�, alors 'xulfmn::web_site_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		virtual void XULFTKGoToHome( void ) = 0;
		virtual void XULFTKDefineSession(
			const str::string_ &ProjectFileName,
			const xpp::criterions___ &XMLPreprocessorCriterions )
		{
			ERRc();	//	Si pas surcharg�, alors 'xulfsf::open_project_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		virtual void XULFTKApplySession( void )
		{
			ERRc();	//	Si pas surcharg�, alors 'xulfsf::apply_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		virtual void XULFTKCancelSession( void )
		{
			ERRc();	//	Si pas surcharg�, alors 'xulfsf::cancel_eh__::NSXPCMOnEvent()' doit �tre red�fini.
		}
		virtual bso::bool__ XULFTKDropSession( void )	// Retourne 'true' si la session aeffectivement �t� ferm�e, 'false' sinon.
		{
			ERRc();	//	Si pas surcharg�, alors 'xulfmn::close_project_eh__::NSXPCMOnEvent()' doit �tre red�fini.

			return false;	// Pour �viter un 'warning'.
		}
		virtual bso::bool__ XULFTKExit( void )
		{
			ERRc();	//	Si pas surcharg�, alors 'xulfmn::exit_eh__::NSXPCMOnEvent()' doit �tre red�fini.

			return true;	// Pour �viter un 'warning'.
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Trunk = NULL;
			_LauncherIdentification = NULL;
		}
		E_CVDTOR( _user_callback__ );
		void Init(
			const char *LauncherIdentification,	// Contenu NON dupliqu� !
			trunk___ &Trunk )
		{
			_Trunk = &Trunk;
			_LauncherIdentification = LauncherIdentification;
		}
		void FormatedInformations( str::string_ &Informations )
		{
			XULFTKFormatedInformations( _LauncherIdentification, Informations );
		}
		void SiteURL( str::string_ &URL )
		{
			XULFTKSiteURL( URL );
		}
		void GoToHome( void )
		{
			XULFTKGoToHome();
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
		void CancelSession( void )
		{
			XULFTKCancelSession();
		}
		bso::bool__ DropSession( void )	// Retourne 'true' si la session aeffectivement �t� ferm�e, 'false' sinon.
		{
			return XULFTKDropSession();
		}
		bso::bool__ Exit( void )
		{
			return XULFTKExit();
		}
	};

	template <typename trunk> class user_callback__
	: public _user_callback__
	{
	protected:
		trunk &_T( void )
		{
			return *(trunk *)&_user_callback__::_T();	// On peut caster, parce que 'Init(...)' garantit que l'objet est du bon type.
		}
	public:
		void Init(
			const char *LauncherIdentification,
			trunk &Trunk )
		{
			_user_callback__::Init( LauncherIdentification, Trunk );
		}
	};

	class trunk___
	{
	private:
		xulfui::ui___ *_UI;
		xulfkl::kernel___ *_Kernel;
		_user_callback__ *_UserCallback;
		geckoo::steering_callback__  *_Steering;
		const char *_TargetName;
		const char *_DefaultXSLRootPath;
		reporting_functions__ _DefaultReportingFunctions;
		_user_callback__ &_UF( void )
		{
			if ( _UserCallback == NULL )
				ERRc();

			return *_UserCallback;
		}
	protected:
		void Handle_( frdkrn::status__ Status );
		void _DefineSession(
			const str::string_ &ProjectFileName,	// Si non vide, contient le nom du fichier projet avec lequel pr�remplir le 'SessionForm'.
			const xpp::criterions___ &Criterions );	// 'registry' qui contient la configuration de l'application.
		// Normalement appel�e par la red�fintion de 'XULFTKApplySession()'. Charge le projet correspondant au fichier 'FileName'.
		void _ApplySession(	const frdkrn::compatibility_informations__ &CompatibilityInformations );
		// Normalement appel�e par la red�fintion de 'XULFTKCacnelSession()', m�me si ne fait rien (standardisation).
		void _CancelSession( void )
		{}
		// Demande de confirmation de la fermeture d'une session (projet). Normalement appel� par la red�fintion de 'XULFTKDropSession()' lorsque projet modifi�.
		bso::bool__ _DefendSession( void );
		void _DropSession( void );
		// Ferme l'application. Normalement appel� par la red�finition de 'XULFTKExit()'.
		void _Exit( void )
		{
			UI().Main().Close();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_UI = NULL;
			_Kernel =  NULL;
			_UserCallback = NULL;
			_Steering = NULL;
			_DefaultXSLRootPath = NULL;
			_DefaultReportingFunctions.reset( P );
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
			const char *DefaultXSLRootPath,	// ATTENTION : pointeur copi�, contenu NON dupliqu�.
			xulfui::ui___ &UI,
			xulfkl::kernel___ &Kernel,
			_user_callback__ &UserCallback,
			geckoo::steering_callback__ &Steering )
		{
			_UI = &UI;
			_Kernel = &Kernel;
			_TargetName = TargetName;
			_UserCallback = &UserCallback;
			_Steering = &Steering;
			_DefaultXSLRootPath = DefaultXSLRootPath;
			_DefaultReportingFunctions.Init( *this );

			return frdkrn::sOK;
		}
		void XULWDGReport( const char *Message )	// Pseudo m�thode virtuelle.
		{
			_UI->LogAndPrompt( Message );
		}
		_reporting_functions__ &DefaultReportingFunctions( void )
		{
			return _DefaultReportingFunctions;
		}
		void ExposeSteering( void )
		{
			geckob::SetSteering( *_Steering );
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
		bso::bool__ FinalizeLaunching( void )
		{
			bso::bool__ Defined = false;
		ERRProlog
			str::string DefaultProjectFileName;
		ERRBegin
			DefaultProjectFileName.Init();

			if ( Registry().GetValue( frdrgy::DefaultProject, DefaultProjectFileName ) ) {
				DefineSession( DefaultProjectFileName, xpp::criterions___() );
				Defined = true;
			} else
				GoToHome();

			UpdateUI();

		ERRErr
		ERREnd
		ERREpilog
			return Defined;
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
		void GoToHome( void )
		{
			_UF().GoToHome();
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
		void CancelSession( void )
		{
			_UF().CancelSession();
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
			str::string Translation;
		ERRBegin
			Translation.Init();
			Kernel().Locale().GetTranslation( xulfkl::GetLabel( xulfkl::mExitConfirmation ), Kernel().Language(), Translation );

			if ( Confirmation = nsxpcm::Confirm( UI().Main().Window(), Translation ) )
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
		const str::string_ &BuildXSLDefaultLocalizedFileName(
			const char *XSLFileNameAffix,
			str::string_ &LocalizedFileName );
		friend class _user_callback__;
	};

	inline void _user_callback__::_DefineSession(
		const str::string_ &ProjectFileName,
		const xpp::criterions___ &Criterions )
	{
		_T()._DefineSession( ProjectFileName, Criterions );
	}

	inline void _user_callback__::_ApplySession( const frdkrn::compatibility_informations__ &CompatibilityInformations )
	{
		_T()._ApplySession( CompatibilityInformations );
	}

	inline void _user_callback__::_CancelSession( void )
	{
		_T()._CancelSession();
	}

	inline bso::bool__ _user_callback__::_DefendSession( void )
	{
		return _T()._DefendSession();
	}

	inline void _user_callback__::_DropSession( void )
	{
		_T()._DropSession();
	}

	inline void _user_callback__::_Exit( void )
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
