/*
	Header for the 'xulfbs' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XULFBS__INC
#define XULFBS__INC

#define XULFBS_NAME		"XULFBS"

#define	XULFBS_VERSION	"$Revision$"

#define XULFBS_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XULFBSTutor;

#if defined( XXX_DBG ) && !defined( XULFBS_NODBG )
#define XULFBS_DBG
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
//D XUL Frontend BaSe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "xulwdg.h"

# include "geckoo.h"

// Predeclarations
namespace xulftk {
	class trunk___;
};

namespace xulfrd {
	class ui___;
}

namespace xulfbs {

	void _Report(
		xulftk::trunk___ &Trunk,
		const char *Message );

	typedef nsxpcm::ui_core__ _ui_core__;

	template <typename trunk> class ui_core__
	: public _ui_core__
	{
	private:
		trunk *_Trunk;
	public:
		void reset( bso::bool__ P = true )
		{
			_ui_core__::reset( P );
			_Trunk = NULL;
		}
		E_CVDTOR( ui_core__ );
		void Init(
			nsIDOMWindow *Window,
			trunk &Trunk )
		{
			_ui_core__::Init( Window );
			_Trunk = &Trunk;
		}
		trunk &Trunk( void ) const
		{
			if ( _Trunk == NULL )
				ERRc();

			return *_Trunk;
		}
	};

	typedef xulwdg::event_handler__<xulftk::trunk___> _event_handler__;

	class event_handler__
	: public _event_handler__
	{
	protected:
		virtual void NSXPCMOnErr( const char *Message )
		{
			_Report( Trunk(), Message );
		}
	};

	XULWDG_ALMOST_ALL_WIDGETS( xulftk::trunk___ )

# define XULFBS_EH( name )\
	class name\
	: public xulfbs::event_handler__\
	{\
	protected:\
		virtual void NSXPCMOnEvent( nsxpcm::event__ Event );\
	};

	typedef nsxpcm::autocomplete_textbox_callback__ _autocomplete_textbox_callback__;
	typedef geckoo::pseudo_event_callback__ _pseudo_event_callback__;

	class autocomplete_textbox_callback__
	: public _autocomplete_textbox_callback__
	{
	private:
		void NSXPCMGetValue(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Value )
		{
			XULFBSGetValue( Pattern, Index, Value );
		}
		void NSXPCMGetLabel(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Label )
		{
			XULFBSGetLabel( Pattern, Index, Label );
		}
		void NSXPCMGetComment(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Comment )
		{
			XULFBSGetComment( Pattern, Index, Comment );
		}
		bso::ulong__ NSXPCMGetMatchingCount( const str::string_ &Pattern )
		{
			return XULFBSGetMatchingCount( Pattern );
		}
	protected:
		virtual void XULFBSGetValue(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Value ) = 0;
		virtual void XULFBSGetLabel(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Label )
		{
			GetValue( Pattern, Index, Label );
		}
		virtual void XULFBSGetComment(
			const str::string_ &Pattern,
			bso::ulong__ Index,
			str::string_ &Comment )
		{
			ERRu();
		}
		virtual bso::ulong__ XULFBSGetMatchingCount( const str::string_ &Pattern ) = 0;
		virtual void XULFBSOnTextEntered( nsIDOMElement *Element ) = 0;
		virtual void XULFBSOnTextReverted( nsIDOMElement *Element ) = 0;
	public:
		void OnTextEntered( nsIDOMElement *Element )
		{
			XULFBSOnTextEntered( Element );
		}
		void OnTextReverted( nsIDOMElement *Element )
		{
			XULFBSOnTextReverted( Element );
		}
	};

	typedef geckoo::pseudo_event_callback__ _pseudo_event_callback__;

	class _autocomplete_event_callback__
	: public _pseudo_event_callback__
	{
	private:
		autocomplete_textbox_callback__ *_Callback;
	protected:
		autocomplete_textbox_callback__ &_C( void )
		{
			if ( _Callback == NULL )
				ERRc();

			return *_Callback;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_pseudo_event_callback__::reset( P );

			_Callback = NULL;
		}
		E_CVDTOR( _autocomplete_event_callback__ )
		void Init( autocomplete_textbox_callback__ &Callback )
		{
			reset();

			_pseudo_event_callback__::Init();
			
			_Callback = &Callback;
		}
	};

	class _ontextentered_callback__
	: public _autocomplete_event_callback__
	{
	protected:
		virtual void GECKOOHandle( nsIDOMElement *Element )
		{
			_C().OnTextEntered( Element );
		}
	};

	class _ontextreverted_callback__
	: public _autocomplete_event_callback__
	{
	protected:
		virtual void GECKOOHandle( nsIDOMElement *Element )
		{
			_C().OnTextReverted( Element );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
