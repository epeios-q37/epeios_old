/*
	Header for the 'xulfui' library by Claude SIMON (csimon at zeusw dot org)
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

# include "geckoo.h"

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

	typedef geckoo::user_callback__ _user_callback__;

	class pseudo_event_callback__
	{
	protected:
		virtual void XULFUIHandle( nsIDOMElement *Element ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( pseudo_event_callback__);
		void Init( void )
		{
			reset();
		}
		void Handle( nsIDOMElement *Element )
		{
			XULFUIHandle( Element );
		}
	};

	void Add(
		const char *Id,
		pseudo_event_callback__ &Callback );

	bso::bool__ Launch(
		const char *Id,
		nsIDOMElement *Element );

	void Remove( const char *Id );

	 class user_callback__
	 : public _user_callback__
	{
	private:
		xulftk::trunk___ *_Trunk;
	protected:
		virtual bso::bool__ GECKOORegister(
			nsIDOMWindow *Window,
			const str::string_ &Id );
		virtual bso::bool__ GECKOOHandlePseudoEvent(
			nsIDOMElement *Element,
			const char *Parameter )
		{
			return Launch( Parameter, Element );
		}
		virtual bso::bool__ XULFUIRegister(
			nsIDOMWindow *Window,
			const str::string_ &Id ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			_user_callback__::reset( P );
			_Trunk = NULL;
		}
		E_CVDTOR( user_callback__ )
		void Init( xulftk::trunk___ &Trunk )
		{
			_user_callback__::Init();
			_Trunk = &Trunk;
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
