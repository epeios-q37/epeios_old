/*
	Header for the 'xulwdg' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULWDG__INC
#define XULWDG__INC

#define XULWDG_NAME		"XULWDG"

#define	XULWDG_VERSION	"$Revision$"

#define XULWDG_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULWDGTutor;

#if defined( XXX_DBG ) && !defined( XULWDG_NODBG )
#define XULWDG_DBG
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
//D XUL WiDGets 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "nsxpcm.h"

#if 0
// Sensitive widget (reacts to events).
#define XULWDG_SWIDGET( widget, name )\
	class name\
	: public widget\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
	}

// Unsensitive widget (reacts to no event).
#define XULWDG_UWIDGET( widget, name )\
	class name\
	: public widget\
	{}
#endif

#define XULWDG_ALL_WIDGETS( target )\
	typedef xulwdg::broadcaster__<target> broadcaster__;\
	typedef xulwdg::command__<target> command__;\
	typedef xulwdg::button__<target> button__;\
	typedef xulwdg::textbox__<target> textbox__;\
	typedef xulwdg::listbox__<target> listbox__;\
	typedef xulwdg::tree__<target> tree__;\
	typedef xulwdg::window__<target> window__;\


namespace xulwdg {

	template <typename target, typename widget> class widget__
	: public widget
	{
	private:
		target *_Target;
	public:
		void reset( bso::bool__ P = true )
		{
			widget::reset( P );
			_Target = NULL;
		}
		E_CVDTOR( widget__ );
		void Init(
			target &Target,
			nsISupports *Supports,
			nsIDOMWindow *Window )
		{
			_Target = &Target;
			widget::Init( Supports, Window );
		}
		void Init( target &Target,
			nsIDOMWindow *Window,
			const str::string_ &Id )
		{
			_Target = &Target;
			widget::Init( Window, Id );
		}
		void Init( target &Target,
			nsIDOMWindow *Window,
			const char *Id )
		{
			_Target = &Target;
			widget::Init( Window, Id );
		}
		const target &Target( void ) const
		{
			return *_Target;
		}
		target &Target( void )
		{
			return *_Target;
		}
	};

#	define XULWDG__WN( widget, name )\
	template <typename target> E_TTCLONE__( widget__< E_COVER2( target, nsxpcm::widget##__ )>, name##__ );

#	define XULWDG__W( widget )	XULWDG__WN( widget, widget )

	XULWDG__W( textbox );
	XULWDG__W( radio );
	XULWDG__W( radiogroup );
	XULWDG__W( button );
	XULWDG__W( listbox );
	XULWDG__W( tree );
	XULWDG__W( deck );
	XULWDG__W( broadcaster );
	XULWDG__W( command );
	XULWDG__W( menu );
	XULWDG__W( menu_item );
	XULWDG__W( panel );
	XULWDG__WN( widget, box );
//	XULWDG__W( document, nsxpcm::efClose );
	XULWDG__W( window );
	XULWDG__W( description );

	typedef nsxpcm::event_handler__ _event_handler__;

	template <typename target, int events> class event_handler__
	: public _event_handler__
	{
	private:
		target *_Target;
	public:
		void reset( bso::bool__ P = true )
		{
			_event_handler__::reset( P );
			_Target = NULL;
		}
		E_CVDTOR( event_handler__ );
		void Init( target &Target )
		{
			_Target = &Target;
			_event_handler__::Init();
		}
		void Add(
			nsISupports *Supports,
			int Events = events )
		{
			_event_handler__::Add( Supports, Events);
		}
		void Add(
			nsIDOMWindow *Window,
			const str::string_ &Id,
			int Events = events )
		{
			Add( nsxpcm::GetElementById( nsxpcm::GetDocument( Window ), Id ), Events );
		}
		void Add(
			nsIDOMWindow *Window,
			const char *Id,
			int Events = events )
		{
			Add( Window, str::string( Id ), Events );
		}
		const target &Target( void ) const
		{
			return *_Target;
		}
		target &Target( void )
		{
			return *_Target;
		}
	};

#	define XULWDG__EH( name, events )\
	template <typename target> E_TTCLONE__( event_handler__<E_COVER2( target, events )>, name##_eh__ );

	XULWDG__EH( regular, nsxpcm::efCommand );
	XULWDG__EH( radio, nsxpcm::efCommand );
	XULWDG__EH( radiogroup, nsxpcm::efCommand );
	XULWDG__EH( button, nsxpcm::efCommand );
	XULWDG__EH( listbox, nsxpcm::efCommand );
	XULWDG__EH( tree, nsxpcm::efSelect | nsxpcm::efDblClick );
	XULWDG__EH( command, nsxpcm::efCommand );
	XULWDG__EH( menu_item, nsxpcm::efCommand );
	XULWDG__EH( window, nsxpcm::efClose );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
