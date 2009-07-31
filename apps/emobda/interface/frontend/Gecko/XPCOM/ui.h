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

#include "../../mbdkernl.h"
#include "ui_base.h"
#include "ui_main.h"
#include "ui_dbs_f.h"
#include "ui_dbsdb.h"
#include "ui_bksdb.h"
#include "ui_struct.h"
#include "ui_lst_v.h"
#include "ui_rcd_f.h"
#include "ui_rcd_v.h"

namespace ui {
	using namespace ui_base;
	using mbdkernl::target__;
	using mbdkernl::table__;
	using mbdkernl::table_id__;
	using mbdkernl::field__;
	using mbdkernl::field_id__;
	using mbdkernl::record__;
	using mbdkernl::record_position__;

	typedef mbdkernl::message__ message__;	// To resolve ambiguity.
	typedef mbdkernl::kernel___ _kernel___;

	enum context__ {
		cSessionForm,
		cSessionView,
		cStructureView,
		cStructureItemView,
		cItemForm,
		cListView,
		cRecordForm,
		cRecordView,
		c_amount,
		c_Undefined
	};

	class ui___
	: public ui_base::bridge_functions__
	{
	public:
		ui_main::main__ Main;
		ui_dbsdb::database_selection__ DatabaseSelection;
		ui_bksdb::backend_selection__ BackendSelection;
		ui_dbs_f::database_form__ DatabaseForm;
		ui_struct::structure__ Structure;
		ui_lst_v::list_view__ ListView;
		ui_rcd_f::record_form__ RecordForm;
		ui_rcd_v::record_view__ RecordView;
		void _SwitchTo( context__ Context );
		_kernel___ *_Kernel;
		const _kernel___ &_K( void ) const
		{
			return *_Kernel;
		}
		_kernel___ &_K( void )
		{
			return *_Kernel;
		}
		virtual void UIExposeKernel( void ) = 0;	// Kernel used by UI becomes current kernel to use for coming UI part.
		virtual void UIRevokeKernel( void ) = 0;	// Kernel used by this UI do no more be availabe for coming UI part.
	protected:
		virtual const ui::ui___ &__UI( void ) const
		{
			return *this;
		}
		virtual ui::ui___ &__UI( void )
		{
			return *this;
		}
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		void Init( _kernel___ &Kernel )
		{
			// 'ui_...' type member are are initalized later.
			_Kernel = &Kernel;
		}
		const _kernel___ &K( void ) const
		{
			return _K();
		}
		_kernel___ &K( void )
		{
			return _K();
		}
		void Alert(
			nsIDOMWindow *Window,
			const char *Message )
		{
			nsxpcm::Alert( Window, Message );
		}
		void Alert(
			nsIDOMWindow *Window,
			const str::string_ &Message )
		{
			nsxpcm::Alert( Window, Message );
		}
		void Alert(
			nsIDOMWindow *Window,
			message__ Message )
		{
			Alert( Window, _K().GetMessage( Message ) );
		}
		void Alert( const char *Message )
		{
			Alert( Main.Window, Message );
		}
		void Alert( const str::string_ &Message )
		{
			Alert( Main.Window, Message );
		}
		void Alert( message__ Message )
		{
			Alert( Main.Window, Message );
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
		void SelectTable( table__ Table )
		{
			K().Target().Set( Table );

			FillTableMenu( true );
			ApplyQuery();
		}
		void SelectTable( ui_main::table_menu_item__ &MenuItem );
		bso::bool__ GetDatabaseIdentification(
			str::string_ &Name,
			str::string_ &Path,
			str::string_ &Comment )
		{
			bso::bool__ Validated = false;

			nsIDOMWindow *Window = NULL;
			UIExposeKernel();
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
		bso::bool__ GetDatabaseIdentification ( void );
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
			K().Target().reset();
			_SwitchTo( cStructureView );
		}
		bso::bool__ GetSelectedBackend(
			csducl::type__ &Type,
			str::string_ &RemoteHostServiceOrLocalLibraryPath )
		{
			bso::bool__ Validated = false;

			nsIDOMWindow *Window = NULL;
			UIExposeKernel();
			nsxpcm::GetWindowInternal( Main.Window )->Open( NS_LITERAL_STRING( "BackendSelectionDialogBox.xul" ),  NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar,modal" ), &Window );


			switch ( _K().BackendSelection().GetState() ) {
			case mbdtrnsnt::bssRemote:
				_K().BackendSelection().GetHostService( RemoteHostServiceOrLocalLibraryPath );
				Type = csducl::tShared;
				Validated = true;
				break;
			case mbdtrnsnt::bssLocal:
				Type = csducl::tLibrary;
				Validated = true;
				break;
			case mbdtrnsnt::bssCancelled:
				Validated = false;
				break;
			default:
				ERRc();
				break;
			}

			_K().ResetTransient();

			return Validated;
		}
		bso::bool__ GetSelectedDatabase( str::string_ &Path )
		{
			bso::bool__ Validated = false;

			nsIDOMWindow *Window = NULL;
			UIExposeKernel();
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
		bso::bool__ GetSelectedDatabase( void );
		void BrowseDatabase( void )
		{
		ERRProlog
			str::string Path;
		ERRBegin
			Path.Init();

			if ( GetSelectedDatabase( Path ) ) {
				if ( _K().OpenDatabase( Path ) ) {
					_SwitchTo( cSessionView );
				}
			}
		ERRErr
		ERREnd
		ERREpilog
		}
		void CloseDatabase( void )
		{
			_K().CloseDatabase();

			_SwitchTo( cSessionView );
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
		void ApplyQuery( void )
		{
			_K().ApplyQuery();
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
		void ApplySession( void )
		{
			_SwitchTo( cSessionView );
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
			target__ Target = _K().Target();

			if ( Target.Field != UNDEFINED_FIELD )
				DefineField();
			else if ( Target.Table != UNDEFINED_TABLE )
				DefineTable();
			else
				ERRc();
		}
		bso::bool__ GoToRecordNumber( record_position__ Position )
		{
			if ( !K().GoToRecordNumber( Position ) ) {
				Alert( mbdkernl::mUnableToReachRecord );
				return false;
			}

			_SwitchTo( cRecordView );

			return true;
		}
		bso::bool__ GoToFirstRecord( void )
		{
			if ( !K().GoToFirstRecord() ) {
				Alert( mbdkernl::mUnableToReachRecord );
				return false;
			}

			_SwitchTo( cRecordView );

			return true;
		}
		bso::bool__ GoToPreviousRecord( void )
		{
			if ( !K().GoToPreviousRecord() ) {
				Alert( mbdkernl::mUnableToReachRecord );
				return false;
			}

			_SwitchTo( cRecordView );

			return true;
		}
		bso::bool__ GoToNextRecord( void )
		{
			if ( !K().GoToNextRecord() ) {
				Alert( mbdkernl::mUnableToReachRecord );
				return false;
			}

			_SwitchTo( cRecordView );

			return true;
		}
		bso::bool__ GoToLastRecord( void )
		{
			if ( !K().GoToLastRecord() ) {
				Alert( mbdkernl::mUnableToReachRecord );
				return false;
			}

			_SwitchTo( cRecordView );

			return true;
		}
		void FillTableMenu( bso::bool__ ContextIsStandard );
		void FillListView( void );
		void FillRecordForm( void );
		void FillRecordView( void );
		void SelectRecord( void );
		void ApplyRecord( void );
		void ApplyStructureItem( void );
		void FillDatabaseSelectionList( void );
	};
}

#endif