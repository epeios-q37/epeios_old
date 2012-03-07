/*
	'ui_ssn_frm.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eDemo' software.

    'eDemo' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eDemo' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef UI_SSN_VEW__INC
# define UI_SSN_VEW__INC

# include "nsxpcm.h"
# include "xulfsv.h"
# include "xulfui.h"
# include "ui_base.h"
# include "ui_grid.h"

namespace ui_ssn_vew {
	typedef xulfsv::session_view__ _session_view__;

	UI_BASE_EH( get_directory_eh__ );

	typedef ui_base::textbox__ _textbox__;
	typedef xulfui::pseudo_event_callback__ _pseudo_event_callback__;

	class autocomplete_textbox_ontextentered_callback__
	: public _pseudo_event_callback__
	{
	private:
		class autocomplete_textbox__ *_Textbox;
	protected:
		virtual void XULFUIHandle( nsIDOMElement *Element );
	public:
		void reset( bso::bool__ P = true )
		{
			_pseudo_event_callback__::reset( P );
			_Textbox = NULL;
		}
		E_CVDTOR( autocomplete_textbox_ontextentered_callback__ );
		void Init( class autocomplete_textbox__ &Textbox )
		{
			_pseudo_event_callback__::Init();
			_Textbox = &Textbox;
		}
	};

	typedef nsxpcm::autocomplete_textbox_callback__ _autocomplete_textbox_callback__;

	class autocomplete_textbox_callback__
	: public _autocomplete_textbox_callback__
	{
	protected:
		virtual void NSXPCMGetLabel(
			bso::ulong__ Index,
			str::string_ &Label )
		{
			XULFUIGetLabel( Index, Label );
		}
		virtual void NSXPCMGetComment(
			bso::ulong__ Index,
			str::string_ &Comment )
		{
			XULFUIGetComment( Index, Comment );
		}
		virtual bso::ulong__ NSXPCMGetMatchingCount( void )
		{
			return XULFUIGetMatchingCount();
		}
		virtual void XULFUIGetLabel(
			bso::ulong__ Index,
			str::string_ &Label ) = 0;
		virtual void XULFUIGetComment(
			bso::ulong__ Index,
			str::string_ &Comment )
		{
			ERRu();
		}
		virtual bso::ulong__ XULFUIGetMatchingCount( void ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CVDTOR( autocomplete_textbox_callback__ )
		void Init( void )
		{
			reset();
		}
		void GetLabel(
			bso::ulong__ Index,
			str::string_ &Label )
		{
			XULFUIGetLabel( Index, Label );
		}
		void GetComment(
			bso::ulong__ Index,
			str::string_ &Comment )
		{
			XULFUIGetComment( Index, Comment );
		}
		bso::ulong__ GetMatchingCount( void )
		{
			return XULFUIGetMatchingCount();
		}
	};

	class autocomplete_textbox__
	: public _textbox__
	{
	private:
		nsCOMPtr<nsxpcm::autocomplete_result___> _AutoCompleteResult;
		void _SubInit(
			const str::string_ &SearchParam,
			autocomplete_textbox_callback__ &Callback )
		{
			nsxpcm::CreateInstance( NSXPCM_AUTOCOMPLETE_RESULT_CONTRACTID, _AutoCompleteResult );
			_AutoCompleteResult->Init( Callback );
			nsxpcm::Add( SearchParam, _AutoCompleteResult );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_textbox__::reset( P );
		}
		E_CDTOR( autocomplete_textbox__ );
		void Init(
			const str::string_ &SearchParam,
			autocomplete_textbox_callback__ &Callback,
			trunk::trunk___ &Trunk,
			nsISupports *Supports,
			nsIDOMWindow *Window )
		{
			reset();
			_SubInit( SearchParam, Callback );
			_textbox__::Init( Trunk, Supports, Window );
		}
		void Init(
			const str::string_ &SearchParam,
			autocomplete_textbox_callback__ &Callback,
			trunk::trunk___ &Trunk,
			nsIDOMWindow *Window,
			const str::string_ &Id )
		{
			reset();
			_SubInit( SearchParam, Callback );
			_textbox__::Init( Trunk, Window, Id );
		}
		void Init(
			const str::string_ &SearchParam,
			autocomplete_textbox_callback__ &Callback,
			trunk::trunk___ &Trunk,
			nsIDOMWindow *Window,
			const char *Id )
		{
			reset();
			_SubInit( SearchParam, Callback );
			_textbox__::Init( Trunk, Window, Id );
		}
	};

	struct session_view__
	: public _session_view__
	{
	private:
		trunk::trunk___ *_Trunk;
	public:
		ui_grid::grid__ Grid;
		struct broadcasters__
		: public _session_view__::broadcasters__
		{} Broadcasters;
		struct event_handlers__
		: public _session_view__::event_handlers__
		{
			get_directory_eh__ ehGetDirectory;
		} EventHandlers;
		struct widgets__
		: public _session_view__::widgets__
		{
			ui_base::textbox__
				txbDirectorySymbolicName,
				txbDirectory;
			autocomplete_textbox__ txbAutocomplete;
		} Widgets;
		session_view__( void )
		: _session_view__( Broadcasters, EventHandlers, Widgets )
		{}
		void Init(
			nsIDOMWindow *Window,
			trunk::trunk___ &Trunk );
		void Update( void )
		{
			_session_view__::Update();
			Grid.Update();
		}
	};

	void RegisterSessionViewUI(
		trunk::trunk___ &Trunk,
		nsIDOMWindow *Window );
}

#endif