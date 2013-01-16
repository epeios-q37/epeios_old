/*
	Header for the 'xulfui' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFUI__INC
#define XULFUI__INC

#define XULFUI_NAME		"XULFUI"

#define	XULFUI_VERSION	"$Revision$"

#define XULFUI_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFUITutor;

#if defined( XXX_DBG ) && !defined( XULFUI_NODBG )
#define XULFUI_DBG
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
//D XUL Frontend User Interface 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "xulfdg.h"
# include "xulfsf.h"
# include "xulfsv.h"
# include "xulfmn.h"

# include "geckof.h"

namespace xulfui {

	typedef xulfmn::main__ _main__;

	class ui___
	{
	private:
		_main__ *_Main;
		xulfsf::session_form__ *_SessionForm;
		xulfsv::session_view__ *_SessionView;
		xulfdg::debug_dialog__ *_DebugDialog;
		nsIDOMWindow *_JSConsoleWindow;
	protected:
		virtual void XULFUIUpdate( void ) = 0;	// Mise à jour de l'interface.
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _JSConsoleWindow != NULL )
					nsxpcm::Close( _JSConsoleWindow );

			_Main = NULL;
			_SessionForm = NULL;
			_SessionView = NULL;
			_JSConsoleWindow = NULL;
			_DebugDialog = NULL;
		}
		E_CVDTOR( ui___ );
		void Init(
			_main__ &Main,
			xulfsf::session_form__ &SessionForm,
			xulfsv::session_view__ &SessionView )
		{
			reset();

			_Main = &Main;
			_SessionForm = &SessionForm;
			_SessionView = &SessionView;
		}
		void Update( void )
		{
			XULFUIUpdate();
		}
		_main__ &Main( void ) const
		{
#ifdef XULFUI_DBG
			if ( _Main == NULL )
				ERRu();
#endif
			return *_Main;
		}
		xulfsf::session_form__ &SessionForm( void ) const
		{
#ifdef XULFUI_DBG
			if ( _SessionForm == NULL )
				ERRu();
#endif
			return *_SessionForm;
		}
		xulfsv::session_view__ &SessionView( void ) const
		{
#ifdef XULFUI_DBG
			if ( _SessionForm == NULL )
				ERRu();
#endif
			return *_SessionView;
		}
		xulfdg::debug_dialog__ &DebugDialog( void ) const
		{
#ifdef XULFUI_DBG
			if ( _DebugDialog == NULL )
				ERRu();
#endif
			return *_DebugDialog;
		}
		void CreateDebugDialog( void )
		{
			if ( _DebugDialog != NULL )
				ERRc();

			if ( ( _DebugDialog = new xulfdg::debug_dialog__ ) == NULL )
				ERRa();
		}
		void DeleteDebugDialog( void )
		{
			if ( _DebugDialog == NULL )
				ERRc();

			delete _DebugDialog;

			_DebugDialog = NULL;
		}
		void SaveAnnex(
			const char *AttributeName,
			const str::string_ &Data )
		{
			nsxpcm::SetAttribute( nsxpcm::GetElement( Main().Window() ), AttributeName, Data );
		}
		void Alert(
			nsIDOMWindow *Window,
			const char *Message ) const
		{
			nsxpcm::Alert( Window, Message );
		}
		void Alert(
			nsIDOMWindow *Window,
			const str::string_ &Message ) const
		{
			nsxpcm::Alert( Window, Message );
		}
		void Alert( const char *Message ) const
		{
			Alert( Main().Window(), Message );
		}
		void Alert( const str::string_ &Message ) const
		{
			Alert( Main().Window(), Message );
		}
		bso::bool__ Confirm( const char *Message )
		{
			return nsxpcm::Confirm( Main().Window(), Message );
		}
		bso::bool__ Confirm( const str::string_ &Message )
		{
			return nsxpcm::Confirm( Main().Window(), Message );
		}
		void LogQuietly( const str::string_ &Text ) const
		{
			nsxpcm::Log( Text );
		}
		void LogQuietly( const char *Text ) const
		{
			nsxpcm::Log( Text );
		}
		void GetJSConsole( void )
		{
			nsxpcm::GetJSConsole( Main().Window(), &_JSConsoleWindow );
		}
		void LogAndPrompt( const str::string_ &Text )
		{
			nsxpcm::Log( Text );
			GetJSConsole();
		}
		void LogAndPrompt( const char *Text )
		{
			nsxpcm::Log( Text );
			GetJSConsole();
		}
	};

	typedef geckoo::steering_callback__ _steering_callback__;

	 class steering_callback__
	 : public _steering_callback__
	{
	private:
		xulftk::trunk___ *_Trunk;
	protected:
		virtual void GECKOOPreRegistration( void );
		virtual bso::bool__ GECKOORegister(
			nsIDOMWindow *Window,
			const str::string_ &Id );
		virtual void GECKOOPostRegistration( void );
		virtual void XULFUIPreRegistration( void ) = 0;
		virtual bso::bool__ XULFUIRegister( 
			nsIDOMWindow *Window,
			const str::string_ &Id ) = 0;
		virtual void XULFUIPostRegistration( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_steering_callback__::reset( P );
			_Trunk = NULL;
		}
		E_CVDTOR( steering_callback__ )
		void Init( xulftk::trunk___ &Trunk )
		{
			_steering_callback__::Init();
			_Trunk = &Trunk;
		}
	};

	typedef xulwdg::autocomplete_textbox__<xulftk::trunk___> _autocomplete_textbox__;

	class autocomplete_textbox__
	: public _autocomplete_textbox__
	{
	private:
		xulfbs::_ontextentered_callback__ _OnTextEntered;
		xulfbs::_ontextreverted_callback__ _OnTextReverted;
		void _SubInit( xulfbs::autocomplete_textbox_callback__ &Callback )
		{
			if ( HasAttribute( "autocompletesearch" ) )
				ERRu();

			SetAttribute( "autocompletesearch", GECKOF_AUTOCOMPLETE_COMPONENT_NAME );
			
			_OnTextEntered.Init( Callback );
			geckoo::AddPseudoEventHandler( GetNode(), "ontextentered", _OnTextEntered );

			_OnTextReverted.Init( Callback );
			geckoo::AddPseudoEventHandler( GetNode(), "ontextreverted", _OnTextReverted );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_autocomplete_textbox__::reset( P );
			_OnTextEntered.reset( P );
		}
		E_CDTOR( autocomplete_textbox__ );
		void Init(
			xulfbs::autocomplete_textbox_callback__ &Callback,
			xulftk::trunk___ &Trunk,
			const nsxpcm::supports__ &Supports )
		{
			reset();
			_autocomplete_textbox__::Init( Callback, Trunk, Supports );
			_SubInit( Callback );
		}
		NSXPCM_HANDLE_BOOLEAN_ATTRIBUTE( CompleteDefaultIndex, "completedefaultindex" );
		NSXPCM_HANDLE_BOOLEAN_ATTRIBUTE( History, "enablehistory" );
		NSXPCM_HANDLE_BOOLEAN_ATTRIBUTE( ForceComplete, "forcecomplete" );
		NSXPCM_HANDLE_BOOLEAN_ATTRIBUTE( HighlightNonMatches, "highlightnonmatches" );
		NSXPCM_HANDLE_BOOLEAN_ATTRIBUTE( IgnoreBlurWhileSearching, "ignoreblurwhilesearching" );
		NSXPCM_HANDLE_BOOLEAN_ATTRIBUTE( Comment, "showcommentcolumn" );
	};




}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
