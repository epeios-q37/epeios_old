/*
	Header for the 'xulwdg' library by Claude SIMON (csimon at zeusw dot org)
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

namespace xulwdg {

	template <typename trunk> class _trunk_depot__
	{
	private:
		 trunk *_Trunk;
	public:
		void reset( bso::bool__ P = true )
		{
			_Trunk = NULL;
		}
		E_CVDTOR( _trunk_depot__ );
		void Init( trunk &Trunk )
		{
			_Trunk = &Trunk;
		}
		const trunk &Trunk( void ) const
		{
			return *_Trunk;
		}
		trunk &Trunk( void )
		{
			return *_Trunk;
		}
	};


	template <typename trunk, typename widget> class _widget__
	: public widget,
	  public _trunk_depot__<trunk>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			widget::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( _widget__ );
		void Init(
			trunk &Trunk,
			nsISupports *Supports,
			nsIDOMWindow *Window )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			widget::Init( Supports, Window );
		}
		void Init(
			trunk &Trunk,
			nsIDOMWindow *Window,
			const str::string_ &Id )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			widget::Init( Window, Id );
		}
		void Init(
			trunk &Trunk,
			nsIDOMWindow *Window,
			const char *Id )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			widget::Init( Window, Id );
		}
	};

# define XULWDG__WN( widget, name )\
	template <typename trunk> E_TTCLONE__( _widget__< E_COVER2( trunk, nsxpcm::widget##__ )>, name##__ );

# define XULWDG__W( widget )	XULWDG__WN( widget, widget )

	XULWDG__WN( widget, box );
	XULWDG__W( broadcaster );
	XULWDG__W( button );
	XULWDG__W( deck );
	XULWDG__W( description );
	XULWDG__W( listbox );
	XULWDG__W( menu );
	XULWDG__W( menuitem );
	XULWDG__W( menulist );
	XULWDG__W( radio );
	XULWDG__W( radiogroup );
	XULWDG__W( tabs );
	XULWDG__W( tabpanels );
	XULWDG__W( textbox );
	//	XULWDG__W( tree );	// D�finit explictiement ci-dessous � cause de 'Init(...)' de 'nsxpcm::tree__'.
	XULWDG__W( widget );
	XULWDG__W( window );

	typedef nsxpcm::tree__ _tree__;

	template <typename trunk> class tree__
	: public _tree__,
	  public _trunk_depot__<trunk>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_tree__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( tree__ );
		void Init(
			nsxpcm::tree_view_callback__ &Callback,
			trunk &Trunk,
			nsISupports *Supports,
			nsIDOMWindow *Window )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			_tree__::Init( Callback, Supports, Window );
		}
		void Init(
			nsxpcm::tree_view_callback__ &Callback,
			trunk &Trunk,
			nsIDOMWindow *Window,
			const str::string_ &Id )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			_tree__::Init( Callback, Window, Id );
		}
		void Init(
			nsxpcm::tree_view_callback__ &Callback,
			trunk &Trunk,
			nsIDOMWindow *Window,
			const char *Id )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			_tree__::Init( Callback, Window, Id );
		}
	};

	typedef nsxpcm::autocomplete_textbox__ _autocomplete_textbox__;

	template <typename trunk> class autocomplete_textbox__
	: public _autocomplete_textbox__,
	  public _trunk_depot__<trunk>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_autocomplete_textbox__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( autocomplete_textbox__ );
		void Init(
			nsxpcm::autocomplete_textbox_callback__ &Callback,
			trunk &Trunk,
			nsISupports *Supports,
			nsIDOMWindow *Window )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			_autocomplete_textbox__::Init( Callback, Supports, Window );
		}
		void Init(
			nsxpcm::autocomplete_textbox_callback__ &Callback,
			trunk &Trunk,
			nsIDOMWindow *Window,
			const str::string_ &Id )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			_autocomplete_textbox__::Init( Callback, Window, Id );
		}
		void Init(
			nsxpcm::autocomplete_textbox_callback__ &Callback,
			trunk &Trunk,
			nsIDOMWindow *Window,
			const char *Id )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			_autocomplete_textbox__::Init( Callback, Window, Id );
		}
	};

	typedef nsxpcm::event_handler__ _event_handler__;

	template <typename trunk> class event_handler__
	: public _event_handler__,
	  public _trunk_depot__<trunk>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_event_handler__::reset( P );
			_trunk_depot__<trunk>::reset( P );
		}
		E_CVDTOR( event_handler__ );
		void Init( trunk &Trunk )
		{
			_trunk_depot__<trunk>::Init( Trunk );
			_event_handler__::Init();
		}
		void Add(
			nsISupports *Supports,
			int Events )
		{
			_event_handler__::Add( Supports, Events);
		}
		void Add(
			nsIDOMWindow *Window,
			const str::string_ &Id,
			int Events )
		{
			Add( nsxpcm::GetElementById( nsxpcm::GetDocument( Window ), Id ), Events );
		}
		void Add(
			nsIDOMWindow *Window,
			const char *Id,
			int Events )
		{
			Add( Window, str::string( Id ), Events );
		}
	};

}

# define XULWDG__EH( name )\
	template <typename target> E_TTCLONE__( event_handler__<E_COVER2( target )>, name##_eh__ );

# define XULWDG_WIDGET( name, target )	typedef xulwdg::name##__<target> name##__;\

/* Manque 'autocomplete_textbox__', car surcharg� dans autre biblioth�que.*/
# define XULWDG_ALMOST_ALL_WIDGETS( target )\
	XULWDG_WIDGET( box, target )\
	XULWDG_WIDGET( broadcaster, target )\
	XULWDG_WIDGET( button, target )\
	XULWDG_WIDGET( deck, target )\
	XULWDG_WIDGET( description, target )\
	XULWDG_WIDGET( listbox, target )\
	XULWDG_WIDGET( menu, target )\
	XULWDG_WIDGET( menulist, target )\
	XULWDG_WIDGET( menuitem, target )\
	XULWDG_WIDGET( radio, target )\
	XULWDG_WIDGET( radiogroup, target )\
	XULWDG_WIDGET( tabs, target )\
	XULWDG_WIDGET( tabpanels, target )\
	XULWDG_WIDGET( textbox, target )\
	XULWDG_WIDGET( tree, target )\
	XULWDG_WIDGET( widget, target )\
	XULWDG_WIDGET( window, target )



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
