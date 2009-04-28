/*
	'kernel.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef KERNEL__INC
#define KERNEL__INC

#include "../../mbdbkd.h"
#include "ui.h"

#include "csdsnc.h"
#include "csducl.h"
#include "xml.h"
#include "msg.h"
#include "lgg.h"
#include "bkdacc.h"

// #define ADDRESS	"192.168.5.10:1234"	// Portable.
// #define ADDRESS	"10.0.2.2:1234"		// Logiplus.
#define ADDRESS	"localhost:1234"	// Local

#define DEFAULT_LOCATION	"d:\\emobda\\test"


#define KERNEL_DEFAULT_LANGUAGE	lgg::lEnglish
namespace kernel {
	using namespace mbdbkd;

	E_ROW( krow__ );	// 'kernel row', see below.

	extern nsxpcm::repository< class kernel___, krow__> Repository;

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

	enum message__ 
	{
		mMissingDatabaseName,
		mMissingDatabasePath,

		mNoDatabaseSelected,
		mBadDatabasePath,

		mDropStructureItemConfirmation,
		mDeleteTableConfirmation,
		mDeleteFieldConfirmation,
		mDeleteRecordConfirmation,
		mDropRecordConfirmation,
		m_amount,
		m_Undefined
	};

	enum temporary_context__ {
		tcStructureManagement,
		tcDatabaseIdentification,
		tcDatabaseSelection,
		tc_amount,
		tc_Undefined
	};

	enum structure_management_mode__
	{
		smm_Undefined,
		smmModification,
		smmCreation,
		smmDuplication,
		smm_amount,
	};

	enum database_identification_state__ {
		dis_Undefined,
		disValidated,
		disCancelled,
		dis_amount
	};

	enum database_selection_state__ {
		dss_Undefined,
		dssValidated,
		dssCancelled,
		dss_amount
	};


	struct temporary__ {
		temporary_context__ Context;
		union {
		private:
			int _Raw;
		public:
			structure_management_mode__ StructureManagementMode;
			database_identification_state__ DatabaseIdentificationState;
			database_selection_state__ DatabaseSelectionState;
			void reset( void )
			{
				_Raw = 0;
			}
		} Core;
		struct {
			str::string Name, Path, Comment;
			void reset( void )
			{
				Name.reset();
				Path.reset();
				Comment.reset();
			}
		} DatabaseIdentification;
		struct {
			str::string Path;
			void reset( void )
			{
				Path.reset();
			}
		} DatabaseSelection;
		void reset( void )
		{
			Context = tc_Undefined;
			Core.reset();
			DatabaseIdentification.reset();
			DatabaseSelection.reset();
		}
		temporary__( void )
		{
			reset();
		}
		void SetStructureManagementMode( structure_management_mode__ Mode )
		{
			switch ( Context ) {
			case tc_Undefined:
				Context = tcStructureManagement;
				break;
			case tcStructureManagement:
				break;
			default:
				ERRc();
				break;
			}

			if ( Core.StructureManagementMode != smm_Undefined )
				ERRc();

			Core.StructureManagementMode = Mode;
		}
		structure_management_mode__ GetStructureManagementMode( void ) const
		{
			if ( Context != tcStructureManagement )
				ERRc();

			return Core.StructureManagementMode;
		}
		void SetDatabaseIdentification(
			const str::string_ &Name,
			const str::string_ &Path,
			const str::string_ &Comment )
		{
			if ( Context != tc_Undefined )
				ERRc();

			Context = tcDatabaseIdentification;

			if ( Core.DatabaseIdentificationState != dis_Undefined )
				ERRc();

			Core.DatabaseIdentificationState = disValidated;

			DatabaseIdentification.Name.Init( Name );
			DatabaseIdentification.Path.Init( Path );
			DatabaseIdentification.Comment.Init( Comment );
		}
		void SetCancelledDatabaseIdentificationState( void )
		{
			if ( Context != tc_Undefined )
				ERRc();

			Context = tcDatabaseIdentification;

			if ( Core.DatabaseIdentificationState != dis_Undefined )
				ERRc();

			Core.DatabaseIdentificationState = disCancelled;
		}
		database_identification_state__ GetDatabaseIdentificationState( void ) const
		{
			if ( Context != tcDatabaseIdentification )
				ERRc();

			return Core.DatabaseIdentificationState;
		}
		void GetDatabaseIdentification(
			str::string_ &Name,
			str::string_ &Path,
			str::string_ &Comment ) const
		{
			if ( Context != tcDatabaseIdentification )
				ERRc();

			if ( Core.DatabaseIdentificationState != disValidated )
				ERRc();

			Name = DatabaseIdentification.Name;
			Path = DatabaseIdentification.Path;
			Comment = DatabaseIdentification.Comment;
		}

		void SetSelectedDatabase( const str::string_ &Path )
		{
			if ( Context != tc_Undefined )
				ERRc();

			Context = tcDatabaseSelection;

			if ( Core.DatabaseSelectionState != dss_Undefined )
				ERRc();

			Core.DatabaseSelectionState = dssValidated;

			DatabaseSelection.Path.Init( Path );
		}
		void SetCancelledDatabaseSelectionState( void )
		{
			if ( Context != tc_Undefined )
				ERRc();

			Context = tcDatabaseSelection;

			if ( Core.DatabaseSelectionState != dss_Undefined )
				ERRc();

			Core.DatabaseSelectionState = dssCancelled;
		}
		database_selection_state__ GetDatabaseSelectionState( void ) const
		{
			if ( Context != tcDatabaseSelection )
				ERRc();

			return Core.DatabaseSelectionState;
		}
		void GetSelectedDatabase( str::string_ &Path ) const
		{
			if ( Context != tcDatabaseSelection )
				ERRc();

			if ( Core.DatabaseSelectionState != dssValidated )
				ERRc();

			Path = DatabaseSelection.Path;
		}
	};

	struct target__ {
		table__ Table;
		field__ Field;
		record__ Record;
		void reset( bso::bool__ = true )
		{
			Table = UNDEFINED_TABLE;
			Field = UNDEFINED_FIELD;
			Record = UNDEFINED_RECORD;
		}
		target__( void )
		{
			reset( false );
		}
		target__(
			field__ Field,
			table__ Table )
		{
			Set( Field, Table );
		}
		target__(
			record__ Record,
			table__ Table )
		{
			Set( Record, Table );
		}
		target__( table__ Table )
		{
			Set( Table );
		}
		void Set(
			record__ Record,
			field__ Field,
			table__ Table )
		{
			if ( ( Record != UNDEFINED_RECORD ) && ( Table == UNDEFINED_TABLE ) )
				ERRu();

			if ( ( Field != UNDEFINED_FIELD ) && ( Table == UNDEFINED_TABLE ) )
				ERRu();

			this->Record = Record;
			this->Field = Field;
			this->Table = Table;
		}
		void Set(
			record__ Record,
			table__ Table )
		{
			Set( Record, UNDEFINED_FIELD, Table );
		}
		void Set( record__ Record )
		{
			Set( Record, this->Table );
		}
		void Set(
			field__ Field,
			table__ Table )
		{
			Set( UNDEFINED_RECORD, Field, Table );
		}
		void Set( field__ Field )
		{
			Set( Field, this->Table );
		}
		void Set( table__ Table )
		{
			Set( UNDEFINED_RECORD, UNDEFINED_FIELD, Table );
		}
	};



	typedef mbdbkd::backend___ _backend___;

	class log_functions__
	: public csdsnc::log_functions__
	{
	protected:
		virtual void CSDSNCLog(
			csdsnc::log__ Log,
			const void *Flow,
			epeios::size__ Amount )
		{}
	};

	class kernel___
	: public ui::bridge_functions__,
	  private _backend___
	{
	private:
		krow__ _KRow;
		msg::buffer__ _MessageBuffer;
		lgg::language__ _Language;
		log_functions__ _LogFunctions;
		csducl::universal_client_core _ClientCore;
		void _DumpCurrent( xml::writer_ &Writer );
		void _DumpAvailableDatabases( xml::writer_ &Writer );
		void _DumpStructure( xml::writer_ &Writer );
		void _DumpContent( xml::writer_ &Writer );
		void _DumpAsXML( str::string_ &XML );
		temporary__ _Temporary;
		target__ _Target;
		void _SwitchTo( context__ Context );
		table__ _CreateOrModifyTable( void )
		{
			table__ Table = UNDEFINED_TABLE;
		ERRProlog
			str::string Name, Comment;
		ERRBegin
			Name.Init();
			UI.Structure.NameTextbox.GetValue( Name );

			Comment.Init();
			UI.Structure.CommentTextbox.GetValue( Comment );

			Table = CreateOrModifyTable( _Target.Table, Name, Comment );
		ERRErr
		ERREnd
		ERREpilog
			return Table;
		}
		field__ _CreateOrModifyField( void )
		{
			field__ Field = UNDEFINED_FIELD;
		ERRProlog
			str::string Name, Comment;
		ERRBegin
			if ( _Target.Table == UNDEFINED_TABLE )
				ERRu();

			Name.Init();
			UI.Structure.NameTextbox.GetValue( Name );

			Comment.Init();
			UI.Structure.CommentTextbox.GetValue( Comment );

			Field = AddOrModifyField( _Target.Field, _Target.Table, Name, Comment );
		ERRErr
		ERREnd
		ERREpilog
			return Field;
		}
	protected:
		virtual const kernel___ &__K( void ) const
		{
			return *this;
		}
		virtual kernel___ &__K( void )
		{
			return *this;
		}
		virtual void __Report( const char *Message )
		{
			nsxpcm::Alert( UI.Main.Window, Message );
		}
	public:
		ui::ui__ UI;
		void reset( bso::bool__ P = true )
		{
			_KRow = NONE;
			_backend___::reset( P );
			_ClientCore.reset( P );
			_Language = lgg::l_undefined;
			_Target.reset( P );
			_Temporary.reset();
		}
		kernel___( void )
		{
			reset( false );
		}
		~kernel___( void )
		{
			reset();
		}
		void Init(
			void *UP,	// Not used.
			krow__ KRow )
		{
			reset();

			_KRow = KRow;

			_ClientCore.Init( ADDRESS, NULL, _LogFunctions, csducl::tShared );	// Logiplus.
			_backend___::Init( _ClientCore );
			UI.Init( *this );

			_Language = KERNEL_DEFAULT_LANGUAGE;	// A changer.
		}
		const char *GetMessage( message__ Message );
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
			Alert( Window, GetMessage( Message ) );
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
		void UpdateDecks( void );
		bso::bool__ GetDatabaseIdentification(
			str::string_ &Name,
			str::string_ &Path,
			str::string_ &Comment )
		{
			bso::bool__ Validated = false;

			nsIDOMWindow *Window = NULL;
			Repository.SetCurrentRow( _KRow );
			nsxpcm::GetWindowInternal( this->UI.Main.Window )->Open( NS_LITERAL_STRING( "DatabaseForm.xul" ),  NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar,modal" ), &Window );

			switch ( _Temporary.GetDatabaseIdentificationState() ) {
			case disValidated:
				_Temporary.GetDatabaseIdentification( Name, Path, Comment );
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
		void DefineDatabase( void )
		{
		ERRProlog
			str::string Name, Path, Comment;
		ERRBegin
			Name.Init();
			Path.Init();
			Comment.Init();

			if ( GetDatabaseIdentification( Name, Path, Comment ) ) {
				if ( CreateDatabase( Path, Name, Comment ) )
					_SwitchTo( cStructureView );
			}
		ERRErr
		ERREnd
		ERREpilog
		}
		bso::bool__ GetSelectedDatabase( str::string_ &Path )
		{
			bso::bool__ Validated = false;

			nsIDOMWindow *Window = NULL;
			Repository.SetCurrentRow( _KRow );
			nsxpcm::GetWindowInternal( this->UI.Main.Window )->Open( NS_LITERAL_STRING( "DatabaseSelectionDialogBox.xul" ),  NS_LITERAL_STRING( "_blank" ), NS_LITERAL_STRING( "chrome,extrachrome,menubar,resizable,scrollbars,status,toolbar,modal" ), &Window );

			switch ( _Temporary.GetDatabaseSelectionState() ) {
			case dssValidated:
				_Temporary.GetSelectedDatabase( Path );
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
				if ( OpenDatabase( Path ) )
					_SwitchTo( cStructureView );
			}
		ERRErr
		ERREnd
		ERREpilog
		}
		void BrowseStructureItem( void )
		{
			SelectStructureItem();
			_SwitchTo( cStructureItemView );
		}
		void DefineStructureItem( void )
		{
			_Temporary.SetStructureManagementMode( smmModification );

			if ( GetTarget().Field != UNDEFINED_FIELD )
				DefineField();
			else if ( GetTarget().Table != UNDEFINED_TABLE )
				DefineTable();
			else
				ERRc();

		}
		void ApplyStructureItem( void );
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
		void FillDatabaseSelectionList( void );
		void FillStructureView( void );
		void FillTableMenu( void );
		void FillRecordForm( void );
		void FillRecordView( void );
		void FillListView( void );
		void UpdateButtonsPanel( void );
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
		void SelectTable( table__ Table )
		{
			_Target = target__( Table );

			FillTableMenu();
			BrowseList();
			UI.Main.Broadcasters.TableSelected.Enable();
		}
		void SelectTable( ui_main::table_menu_item__ &MenuItem );
		void SelectRecord( void );
		void SelectStructureItem();
		void ApplyRecord( void );
		void SetStructureManagementMode( structure_management_mode__ Mode )
		{
			_Temporary.SetStructureManagementMode( Mode );
		}
		structure_management_mode__ GetStructureManagementMode( void ) const
		{
			return _Temporary.GetStructureManagementMode();
		}
		bso::bool__ GetDatabaseIdentification( void );	// Called from UI event. Fill '_Temporary' with content from dedicated interface object.
		void SetDatabaseIdentification(
			const str::string_ &Name,
			const str::string_ &Path,
			const str::string_ &Comment )
		{
			_Temporary.SetDatabaseIdentification( Name, Path, Comment );
		}
		void SetCancelledDatabaseIdentificationState( void )
		{
			_Temporary.SetCancelledDatabaseIdentificationState();
		}
		bso::bool__ GetSelectedDatabase( void );	// Called from UI event. Fill '_Temporary' with content from dedicated interface object.
		void SetSelectedDatabase( const str::string_ &Path )
		{
			_Temporary.SetSelectedDatabase( Path );
		}
		void SetCancelledDatabaseSelectionState( void )
		{
			_Temporary.SetCancelledDatabaseSelectionState();
		}
		E_RWDISCLOSE__( target__, Target );
	};

	inline bkdacc::id32__ _ExtractId32( const str::string_ &Value )
	{
		epeios::row__ Error = NONE;
		bkdacc::id32__ Id = Value.ToUL( &Error );

		if ( Error != NONE )
			ERRu();

		return Id;
	
	}

	inline bkdacc::id16__ _ExtractId16( const str::string_ &Value )
	{
		epeios::row__ Error = NONE;
		bkdacc::id16__ Id = Value.ToUS( &Error );

		if ( Error != NONE )
			ERRu();

		return Id;
	
	}

	inline table__ ExtractTable( const str::string_ &Value )
	{
		return _ExtractId32( Value );
	}

	inline field__ ExtractField( const str::string_ &Value )
	{
		return _ExtractId32( Value );
	}

	inline record__ ExtractRecord( const str::string_ &Value )
	{
		return _ExtractId16( Value );
	}
}

#endif