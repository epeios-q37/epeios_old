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

#include "err.h"
#include "flw.h"
#include "nsxpcm.h"
#include "frdkrn.h"

#include "xulfui.h"
#include "xulfkl.h"

namespace xulftk {
	using xulfkl::status__;

	E_ROW( trow__ );	// 'trunk row', see below.

	class _repository_functions__
	{
	protected:
		virtual void XULFKLExpose( trow__ Row ) = 0;
		virtual void XULFKLRevoke( trow__ Row ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		_repository_functions__( void )
		{
			reset( false );
		}
		~_repository_functions__( void )
		{
			reset();
		}
		void Init( void )
		{
			// Standardisation.
		}
		void Expose( trow__ Row )
		{
			XULFKLExpose( Row );
		}
		void Revoke( trow__ Row )
		{
			XULFKLRevoke( Row );
		}
	};

	template <typename repository> class repository_functions__
	: public _repository_functions__
	{
	private:
		repository *_Repository;
	protected:
		virtual void XULFKLExpose( trow__ Row )
		{
			_Repository->SetCurrentRow( Row );
		}
		virtual void XULFKLRevoke( trow__ Row )
		{
			if ( _Repository->GetCurrentRow() != Row )
				ERRc();

			_Repository->DismissCurrentObject();
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Repository = NULL;
		}
		repository_functions__( void )
		{
			reset( false );
		}
		~repository_functions__( void )
		{
			reset();
		}
		void Init( repository &Repository )
		{
			_Repository = &Repository;
		}
	};

	class trunk___
	{
	private:
		trow__ _TRow;
		xulfui::ui___ *_UI;
		xulfkl::kernel___ *_Kernel;
		_repository_functions__ *_RepositoryFunctions;
		const char *_TargetName;
	protected:
		virtual void XULFTKFormatedInformations( str::string_ &Informations )
		{
			ERRu();	// Si pas surchargé, alors 'xulfmn::about_command__::NSXPCMOnEvent(...)' doir être redéfini.
		}
		virtual void XULFTKSiteURL( str::string_ &URL )
		{
			ERRu();	// Si pas surchargé, alors 'xulfmn::web_site_command__::NSXPCMOnEvent') doit être redéfini.
		}
		// Normalement appelé par la redéfintion de la fonciton qui suit.
		void _ApplySession(
			const str::string_ &FileName,
			const char *CypherKey,
			frdfbc::data___ &LibraryData )
		{
			if ( Kernel().LoadProject( FileName, _TargetName, CypherKey, LibraryData ) != frdkrn::sOK )
				UI().Alert( Kernel().Message() );

			UpdateUI();
		}
		virtual void XULFTKApplySession(
			const str::string_ &FileName,
			const char *CypherKey )
		{
			ERRu();	//	Si pas surchargé, alors xulfmn::open_project_command__::NSXPCMOnEvent') doit être redéfini.
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_UI = NULL;
			_Kernel =  NULL;
			_TRow = NONE;
			_RepositoryFunctions = NULL;
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
			trow__ TRow,
			const char *TargetName,	// ATTENTION : pointeur copié, contenu NON dupliqué.
			xulfui::ui___ &UI,
			xulfkl::kernel___ &Kernel,
			_repository_functions__ &RepositoryFunctions )
		{
			_TRow = TRow;
			_UI = &UI;
			_Kernel = &Kernel;
			_TargetName = TargetName;
			_RepositoryFunctions = &RepositoryFunctions;

			return frdkrn::sOK;
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
		void Expose( void )
		{
#ifdef XULFKL_DBG
			if ( _RepositoryFunctions == NULL )
				ERRu();
#endif
			_RepositoryFunctions->Expose( _TRow );
		}
		void Revoke( void )
		{
#ifdef XULFKL_DBG
			if ( _RepositoryFunctions == NULL )
				ERRu();
#endif
			_RepositoryFunctions->Revoke( _TRow );
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
		void  ApplySession( const char *CypherKey )
		{
		ERRProlog
			str::string Translation;
			str::string FileName;
		ERRBegin
			Translation.Init();
			FileName.Init();

			if ( nsxpcm::XPRJFileOpenDialogBox( UI().Main().Window, Kernel().GetTranslation( xulfkl::mSelectProjectFile, Translation ), Kernel().LocaleRack(), FileName ) )
				XULFTKApplySession( FileName, CypherKey );
		ERRErr
		ERREnd
		ERREpilog
		}
		void CloseSession( void )
		{
		ERRProlog
			STR_BUFFER___ Buffer;
		ERRBegin
			if ( nsxpcm::Confirm( UI().Main().Window, Kernel().GetTranslation( xulfkl::mExitConfirmation, Buffer) ) )
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
