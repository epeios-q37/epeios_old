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
	typedef xulwdg::broadcast__<target> broadcast__;\
	typedef xulwdg::command__<target> command__;\
	typedef xulwdg::button__<target> button__;\
	typedef xulwdg::textbox__<target> textbox__;\
	typedef xulwdg::listbox__<target> listbox__;\
	typedef xulwdg::tree__<target> tree__;\
	typedef xulwdg::window__<target> window__;\


namespace xulwdg {

	template <typename target, typename widget, int events> class _generic__
	: public widget
	{
	private:
		target *_Target;
		void _ErrHandling( void )
		{
			if ( ERRMajor == err::itn ) {
				if ( ERRMinor != err::iAbort ) {
					err::buffer__ Buffer;
					XULWDGReport( err::Message( Buffer ) ); 
				}
			} else {
				err::buffer__ Buffer;
				XULWDGReport( err::Message( Buffer ) ); 
			}
			ERRRst()
		}
	protected:
		virtual void XULWDGReport( const char *Message ) = 0;
		virtual void XULWDGOnEvent( nsxpcm::event__ Event )
		{
			ERRc();	// Se produit pour les éléments dont on a défini une sensibilité à un évènement, sans mettre en place de gestionnaire.
		}
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event )
		{
		ERRProlog
		ERRBegin
			XULWDGOnEvent( Event );
		ERRErr
			_ErrHandling();
		ERREnd
		ERREpilog
		}
	public:
		void reset( bso::bool__ P = true )
		{
			widget::reset( P );
			_Target = NULL;
		}
		E_CVDTOR( _generic__ );
		void Init(
			target &Target,
			nsISupports *Supports,
			nsIDOMWindow *Window )
		{
			_Target = &Target;
			widget::Init( Supports, Window, events );
		}
		void Init( target &Target,
			nsIDOMWindow *Window,
			const str::string_ &Id )
		{
			_Target = &Target;
			widget::Init( Window, Id, events );
		}
		void Init( target &Target,
			nsIDOMWindow *Window,
			const char *Id )
		{
			_Target = &Target;
			widget::Init( Window, Id, events );
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

#	define XULWDG__WN( widget, name, events )\
	template <typename target> E_TTCLONE__( E_COVER2( _generic__< E_COVER2( target, nsxpcm::widget##__ ), events> ), name##__ );

#	define XULWDG__W( widget, events )	XULWDG__WN( widget, widget, events )

	XULWDG__W( textbox, nsxpcm::ef_None );
	XULWDG__W( radio, nsxpcm::efCommand );
	XULWDG__W( button, nsxpcm::efCommand );
	XULWDG__W( listbox, nsxpcm::efCommand );
	XULWDG__W( tree, nsxpcm::efSelect | nsxpcm::efDblClick );
	XULWDG__W( deck, nsxpcm::ef_None );
	XULWDG__W( broadcast, nsxpcm::ef_None );
	XULWDG__W( command, nsxpcm::efCommand );
	XULWDG__W( menu, nsxpcm::ef_None );
	XULWDG__W( menu_item, nsxpcm::efCommand );
	XULWDG__W( panel, nsxpcm::ef_None );
	XULWDG__WN( widget, box, nsxpcm::ef_None );
//	XULWDG__W( document, nsxpcm::efClose );
	XULWDG__W( window, nsxpcm::efClose );
	XULWDG__W( description, nsxpcm::ef_None );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
