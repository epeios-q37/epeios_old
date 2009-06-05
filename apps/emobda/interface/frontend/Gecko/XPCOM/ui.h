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

#include "ui_main.h"
#include "ui_dbcdb.h"
#include "ui_dbsdb.h"
#include "ui_struct.h"
#include "ui_lst_v.h"
#include "ui_rcd_f.h"
#include "ui_rcd_v.h"

namespace ui {
	using ui_base::bridge_functions__;

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

	struct ui___ {
	private:
		ui_main::main__ Main;
		ui_dbcdb::database_creation__ DatabaseCreation;
		ui_dbsdb::database_selection__ DatabaseSelection;
		ui_struct::structure__ Structure;
		ui_lst_v::list_view__ ListView;
		ui_rcd_f::record_form__ RecordForm;
		ui_rcd_v::record_view__ RecordView;
		void _SwitchTo( context__ Context );
		kernel::kernel__ *_Kernel;
		const kernel::kernel__ &_K( void ) const
		{
			return *Kernel;
		}
		kernel::kernel__ &_K( void )
		{
			return *Kernel;
		}
	public:
		void Init(
			bridge_functions__ &Functions,
			kernel::keral__ &Kernel )
		{
			Main.Init();
			Structure.Init( Functions );
			_Kernel = &Kernel;
		}
		void Alert( const char *Message )
		{
			Alert( UI.Main.Window, Message );
		}
		void Alert( const str::string_ &Message )
		{
			Alert( UI.Main.Window, Message );
		}
		void Alert( message__ Message )
		{
			Alert( UI.Main.Window, Message );
		}
		bso::bool__ Confirm( const char *Message )
		{
			return nsxpcm::Confirm( UI.Main.Window, Message );
		}
		bso::bool__ Confirm( const str::string_ &Message )
		{
			return nsxpcm::Confirm( UI.Main.Window, Message );
		}
		bso::bool__ Confirm( message__ Message )
		{
			return Confirm( GetMessage( Message ) );
		}
		table__ CreateOrModifyTable( void )
		{
			table__ Table = UNDEFINED_TABLE;
		ERRProlog
			str::string Name, Comment;
		ERRBegin
			Name.Init();
			UI.Structure.NameTextbox.GetValue( Name );

			Comment.Init();
			UI.Structure.CommentTextbox.GetValue( Comment );

			Table = CreateOrModifyTable( StructureManagement().Target.Table, Name, Comment );
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
			if ( StructureManagement().Target.Table == UNDEFINED_TABLE )
				ERRu();

			Name.Init();
			UI.Structure.NameTextbox.GetValue( Name );

			Comment.Init();
			UI.Structure.CommentTextbox.GetValue( Comment );

			Field = AddOrModifyField( StructureManagement().Target.Field, StructureManagement().Target.Table, Name, Comment );
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
			Repository.SetCurrentRow( _KRow );
			nsxpcm::GetWindowInternal( this->UI.Main.Window )->Open( NS_LITERAL_STRING( "DatabaseForm.xul" ),  NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar,modal" ), &Window );

			switch ( DatabaseIdentification().GetState() ) {
			case disValidated:
				DatabaseIdentification().Get( Name, Path, Comment );
				Validated = true;
				break;
			case disCancelled:
				Validated = false;
				break;
			default:
				ERRc();
				break;
			}

			_Temporary.reset();

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
				if ( _backend___::CreateDatabase( Path, Name, Comment ) ) {
					_Target.Set( UNDEFINED_TABLE );
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
			Repository.SetCurrentRow( _KRow );
			nsxpcm::GetWindowInternal( this->UI.Main.Window )->Open( NS_LITERAL_STRING( "DatabaseSelectionDialogBox.xul" ),  NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar,modal" ), &Window );

			switch ( DatabaseSelection().GetState() ) {
			case dssValidated:
				DatabaseSelection().Get( Path );
				Validated = true;
				break;
			case dssCancelled:
				Validated = false;
				break;
			default:
				ERRc();
				break;
			}

			_Temporary.reset();

			return Validated;
		}
		void BrowseDatabase( void )
		{
		ERRProlog
			str::string Path;
		ERRBegin
			Path.Init();

			if ( GetSelectedDatabase( Path ) ) {
				if ( OpenDatabase( Path ) ) {
					_Target.Set( UNDEFINED_TABLE );
					_SwitchTo( cStructureView );
				}
			}
		ERRErr
		ERREnd
		ERREpilog
		}
		void CloseDatabase( void )
		{
			_backend___::CloseDatabase();

			_SwitchTo( cSessionForm );
		}
		void BrowseStructureItem( void )
		{
			_SwitchTo( cStructureItemView );
		}
		void DeleteTable( void )
		{
			if ( GetTarget().Table == UNDEFINED_TABLE )
				ERRc();

			_backend___::DeleteTable( GetTarget().Table );
			Target().Set( UNDEFINED_TABLE );

			_SwitchTo( cStructureView );
		}
		void DeleteField( void )
		{
			if ( GetTarget().Field == UNDEFINED_FIELD )
				ERRc();

			_backend___::DeleteField( GetTarget().Field );
			Target().Set( UNDEFINED_FIELD );

			_SwitchTo( cStructureView );
		}
		void DropStructureItem( void )
		{
			_Temporary.reset();
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
			_Temporary.reset();
			_SwitchTo( cRecordView );
		}
		void RemoveRecord( void )
		{
			DeleteRecord( GetTarget().Record, GetTarget().Table );
			Target().Set( UNDEFINED_RECORD );
			_SwitchTo( cListView );
		}
		void DefineSession( void )
		{
			_SwitchTo( cSessionForm );
		}
		void DefineTable( void )
		{
			ui_struct::structure__ &UI = this->UI.Structure;

			UI.Broadcasters.ItemBrowsing.Disable();
			UI.Broadcasters.ItemEdition.Enable();

			if ( _Target.Table == UNDEFINED_TABLE ) {
				UI.NameTextbox.SetValue( str::string( "" ) );

				UI.CommentTextbox.SetValue( str::string( "" ) );
			}

			UI.NameTextbox.Select();
		}
		void DefineField( void )
		{
			ui_struct::structure__ &UI = this->UI.Structure;

			UI.Broadcasters.ItemBrowsing.Disable();
			UI.Broadcasters.ItemEdition.Enable();

			if ( _Target.Field == UNDEFINED_FIELD ) {
				UI.NameTextbox.SetValue( str::string( "" ) );

				UI.CommentTextbox.SetValue( str::string( "" ) );
			}

			UI.NameTextbox.Select();
		}
				void DefineStructureItem( void )
		{

			target__ Target;

			if ( GetSelectedStructureItem( Target ) ) {
				if ( Target.Field != UNDEFINED_FIELD )
					DefineField();
				else if ( Target.Table != UNDEFINED_TABLE )
					DefineTable();
				else
					ERRc();
			} else
				ERRc();

		}
	};
}

#endif