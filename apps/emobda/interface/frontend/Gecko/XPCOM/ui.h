/*
	'ui.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef UI__INC
#define UI__INC

#include "kernel.h"
#include "ui_base.h"
#include "ui_main.h"
#include "ui_dbcdb.h"
#include "ui_dbsdb.h"
#include "ui_struct.h"
#include "ui_lst_v.h"
#include "ui_rcd_f.h"
#include "ui_rcd_v.h"

namespace ui {
	using namespace ui_base;
	using namespace kernel;
	using mbdtrnsnt::target__;

	typedef kernel::message__ message__;	// To resolve ambiguity.

	enum context__ {
		cSessionForm,
		cStructureView,
		cStructureItemView,
		cItemForm,
		cListView,
		cRecordForm,
		cRecordView,
		c_amount,
		c_Undefined
	};

	template <typename widget> void Register(
		kernel::kernel___ &Kernel,
		widget &Widget,
		nsIDOMDocument *Document,
		const char *Id,
		int Events )
	{
		Widget.Init( Kernel );
		nsxpcm::Register( Widget, Document, Id, Events );
	}

	template <typename widget> void Register(
		kernel::kernel___ &Kernel,
		widget &Widget,
		nsISupports *Supports,
		int Events )
	{
		Widget.Init( Kernel );
		nsxpcm::Register( Widget, Supports, Events );
	}

	inline void Register(
		kernel::kernel___ &Kernel,
		window__ &Window,
		nsIDOMWindow *Element )
	{
		Register( Kernel, Window, Element, nsxpcm::efClose );
	}

	inline void Register(
		kernel::kernel___ &Kernel,
		broadcaster__ &Broadcaster,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Kernel, Broadcaster, Document, Id, nsxpcm::efNone );
	}

	inline void Register(
		kernel::kernel___ &Kernel,
		command__ &Command,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Kernel, Command, Document, Id, nsxpcm::efCommand );
	}

	inline void Register(
		kernel::kernel___ &Kernel,
		tree__ &Tree,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Kernel, Tree, Document, Id, nsxpcm::efSelect | nsxpcm::efDblClick );
	}

	inline void Register(
		kernel::kernel___ &Kernel,
		deck__ &Deck,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Kernel, Deck, Document, Id, nsxpcm::efNone );
	}

	inline void Register(
		kernel::kernel___ &Kernel,
		textbox__ &Textbox,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Kernel, Textbox, Document, Id, nsxpcm::efNone );
	}

	inline void Register(
		kernel::kernel___ &Kernel,
		button__ &Button,
		nsIDOMDocument *Document,
		const char *Id )
	{
		Register( Kernel, Button, Document, Id, nsxpcm::efCommand );
	}

	class ui___ {
	private:
		ui_main::main__ Main;
		ui_dbcdb::database_creation__ DatabaseCreation;
		ui_dbsdb::database_selection__ DatabaseSelection;
		ui_struct::structure__ Structure;
		ui_lst_v::list_view__ ListView;
		ui_rcd_f::record_form__ RecordForm;
		ui_rcd_v::record_view__ RecordView;
		void _SwitchTo( context__ Context );
		kernel::kernel___ *_Kernel;
		const kernel::kernel___ &_K( void ) const
		{
			return *_Kernel;
		}
		kernel::kernel___ &_K( void )
		{
			return *_Kernel;
		}
	public:
		void Init(
			bridge_functions__ &Functions,
			kernel::kernel___ &Kernel )
		{
			Main.Init();
			Structure.Init( Functions );
			_Kernel = &Kernel;
		}
		void Alert( const char *Message )
		{
			_K().Alert( Main.Window, Message );
		}
		void Alert( const str::string_ &Message )
		{
			_K().Alert( Main.Window, Message );
		}
		void Alert( message__ Message )
		{
			_K().Alert( Main.Window, Message );
		}
		bso::bool__ Confirm( const char *Message )
		{
			return nsxpcm::Confirm( Main.Window, Message );
		}
		bso::bool__ Confirm( const str::string_ &Message )
		{
			return nsxpcm::Confirm( Main.Window, Message );
		}
		bso::bool__ Confirm( message__ Message )
		{
			return Confirm( _K().GetMessage( Message ) );
		}
		table__ CreateOrModifyTable( void )
		{
			table__ Table = UNDEFINED_TABLE;
		ERRProlog
			str::string Name, Comment;
		ERRBegin
			Name.Init();
			Structure.NameTextbox.GetValue( Name );

			Comment.Init();
			Structure.CommentTextbox.GetValue( Comment );

			Table = _K().CreateOrModifyTable( Name, Comment );
		ERRErr
		ERREnd
		ERREpilog
			return Table;
		}
		field__ CreateOrModifyField( void )
		{
			field__ Field = UNDEFINED_FIELD;
		ERRProlog
			str::string Name, Comment;
		ERRBegin
			Name.Init();
			Structure.NameTextbox.GetValue( Name );

			Comment.Init();
			Structure.CommentTextbox.GetValue( Comment );

			Field = _K().CreateOrModifyField( Name, Comment );
		ERRErr
		ERREnd
		ERREpilog
			return Field;
		}
		void SelectTable( ui_main::table_menu_item__ &MenuItem );
		bso::bool__ GetDatabaseIdentification(
			str::string_ &Name,
			str::string_ &Path,
			str::string_ &Comment )
		{
			bso::bool__ Validated = false;

			nsIDOMWindow *Window = NULL;
			_K().Expose();
			nsxpcm::GetWindowInternal(Main.Window )->Open( NS_LITERAL_STRING( "DatabaseForm.xul" ),  NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar,modal" ), &Window );

			switch ( _K().DatabaseIdentification().GetState() ) {
			case mbdtrnsnt::disValidated:
				_K().DatabaseIdentification().Get( Name, Path, Comment );
				Validated = true;
				break;
			case mbdtrnsnt::disCancelled:
				Validated = false;
				break;
			default:
				ERRc();
				break;
			}

			_K().ResetTransient();

			return Validated;
		}
		void CreateDatabase( void )
		{
		ERRProlog
			str::string Name, Path, Comment;
		ERRBegin
			Name.Init();
			Path.Init();
			Comment.Init();

			if ( GetDatabaseIdentification( Name, Path, Comment ) ) {
				if ( _K().CreateDatabase( Path, Name, Comment ) ) {
					_SwitchTo( cStructureView );
				}
			}
		ERRErr
		ERREnd
		ERREpilog
		}
		void DefineDatabase( void )
		{
			_SwitchTo( cStructureView );
		}
		bso::bool__ GetSelectedDatabase( str::string_ &Path )
		{
			bso::bool__ Validated = false;

			nsIDOMWindow *Window = NULL;
			_K().Expose();
			nsxpcm::GetWindowInternal( Main.Window )->Open( NS_LITERAL_STRING( "DatabaseSelectionDialogBox.xul" ),  NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar,modal" ), &Window );

			switch ( _K().DatabaseSelection().GetState() ) {
			case mbdtrnsnt::dssValidated:
				_K().DatabaseSelection().Get( Path );
				Validated = true;
				break;
			case mbdtrnsnt::dssCancelled:
				Validated = false;
				break;
			default:
				ERRc();
				break;
			}

			_K().ResetTransient();

			return Validated;
		}
		void BrowseDatabase( void )
		{
		ERRProlog
			str::string Path;
		ERRBegin
			Path.Init();

			if ( GetSelectedDatabase( Path ) ) {
				if ( _K().OpenDatabase( Path ) ) {
					_SwitchTo( cStructureView );
				}
			}
		ERRErr
		ERREnd
		ERREpilog
		}
		void CloseDatabase( void )
		{
			_K().CloseDatabase();

			_SwitchTo( cSessionForm );
		}
		void BrowseStructureItem( void )
		{
			_SwitchTo( cStructureItemView );
		}
		void DeleteTable( void )
		{
			if ( _K().DeleteTable() )
				_SwitchTo( cStructureView );
		}
		void DeleteField( void )
		{
			if ( _K().DeleteField() )
				_SwitchTo( cStructureView );
		}
		void DropStructureItem( void )
		{
			_K().ResetTransient();
			_SwitchTo( cStructureView );
		}
		void BrowseList( void )
		{
			_SwitchTo( cListView );
		}
		void DefineRecord( void )
		{
			_SwitchTo( cRecordForm );
		}
		void BrowseRecord( void )
		{
			_SwitchTo( cRecordView );
		}
		void DropRecord( void )
		{
			_K().ResetTransient();
			_SwitchTo( cRecordView );
		}
		void RemoveRecord( void )
		{
			if ( _K().RemoveRecord() )
				_SwitchTo( cListView );
		}
		void DefineSession( void )
		{
			_SwitchTo( cSessionForm );
		}
		void DefineTable( void )
		{
			ui_struct::structure__ &UI = Structure;

			UI.Broadcasters.ItemBrowsing.Disable();
			UI.Broadcasters.ItemEdition.Enable();

			if ( _K().Target().Table == UNDEFINED_TABLE ) {
				UI.NameTextbox.SetValue( str::string( "" ) );

				UI.CommentTextbox.SetValue( str::string( "" ) );
			}

			UI.NameTextbox.Select();
		}
		void DefineField( void )
		{
			ui_struct::structure__ &UI = Structure;

			UI.Broadcasters.ItemBrowsing.Disable();
			UI.Broadcasters.ItemEdition.Enable();

			if ( _K().Target().Field == UNDEFINED_FIELD ) {
				UI.NameTextbox.SetValue( str::string( "" ) );

				UI.CommentTextbox.SetValue( str::string( "" ) );
			}

			UI.NameTextbox.Select();
		}
		void DefineStructureItem( void )
		{
			target__ Target = _K().StructureManagement().Target;

			if ( Target.Field != UNDEFINED_FIELD )
				DefineField();
			else if ( Target.Table != UNDEFINED_TABLE )
				DefineTable();
			else
				ERRc();
		}
		bso::bool__ GetSelectedStructureItem( target__ &Target );
		void FillTableMenu( void );
		void FillStructureView( void );
		void UpdateDecks( void );
		void FillListView( void );
		void FillRecordForm( void );
		void FillRecordView( void );
	};
}

#endif