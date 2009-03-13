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


#define KERNEL_DEFAULT_LANGUAGE	lgg::lEnglish
namespace kernel {
	using namespace mbdbkd;

	enum context__ {
		cSessionForm,
		cStructureView,
		cStructureItemView,
		cItemForm,
		cListView,
		cRecordForm,
		c_amount,
		c_Undefined
	};

	enum message__ 
	{
		mCancelInputConfirmation,
		m_amount,
		m_Undefined
	};

	enum temporary_mode__
	{
		tm_Undefined,
		tmModification,
		tmCreation
	};

	union temporary__ {
		int _Raw;
		temporary_mode__ Mode;
		void reset( void )
		{
			_Raw = 0;
		}

	};

	struct target__ {
		table__ Table;
		field__ Field;
		void reset( bso::bool__ = true )
		{
			Table = UNDEFINED_TABLE;
			Field = UNDEFINED_FIELD;
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
		target__( table__ Table )
		{
			Set( Table );
		}
		void Set(
			field__ Field,
			table__ Table )
		{
			if ( ( Field != UNDEFINED_FIELD ) && ( Table == UNDEFINED_TABLE ) )
				ERRu();

			this->Field = Field;
			this->Table = Table;
		}
		void Set( table__ Table )
		{
			Set( UNDEFINED_FIELD, Table );
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
		msg::buffer__ _MessageBuffer;
		lgg::language__ _Language;
		log_functions__ _LogFunctions;
		csducl::universal_client_core _ClientCore;
		void _DumpCurrent( xml::writer_ &Writer );
		void _DumpStructure( xml::writer_ &Writer );
		void _DumpAsXML( str::string_ &XML );
		temporary__ _Temporary;
		target__ _Current;
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

			Table = CreateOrModifyTable( _Current.Table, Name, Comment );
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
			if ( _Current.Table == UNDEFINED_TABLE )
				ERRu();

			Name.Init();
			UI.Structure.NameTextbox.GetValue( Name );

			Comment.Init();
			UI.Structure.CommentTextbox.GetValue( Comment );

			Field = AddOrModifyField( _Current.Field, _Current.Table, Name, Comment );
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
			_backend___::reset( P );
			_ClientCore.reset( P );
			_Language = lgg::l_undefined;
			_Current.reset( P );
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
		void Init( void )
		{
			reset();

			_ClientCore.Init( ADDRESS, NULL, _LogFunctions, csducl::tShared );	// Logiplus.
			_backend___::Init( _ClientCore );
			UI.Init( *this );

			_Language = KERNEL_DEFAULT_LANGUAGE;	// A changer.
		}
		const char *GetMessage( message__ Message );
		void Alert( const char *Message )
		{
			nsxpcm::Alert( UI.Main.Window, Message );
		}
		void Alert( const str::string_ &Message )
		{
			nsxpcm::Alert( UI.Main.Window, Message );
		}
		void Alert( message__ Message )
		{
			Alert( GetMessage( Message ) );
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
		void ApplyDatabase( void )
		{
			CreateDatabase( str::string( "h:\\temp\\emobda" ), str::string( "Ceci est le nom de la base de données !" ), str::string( "Ceci est le commentaire de la basqe de données !" ) );

			_SwitchTo( cStructureView );
		}
		void BrowseDatabase( void )
		{
			OpenDatabase( str::string( "h:\\temp\\emobda" ) );

			_SwitchTo( cStructureView );
		}
		void BrowseStructureItem( void )
		{
			SetCurrent();
			_SwitchTo( cStructureItemView );
		}
		void ApplyStructureItem( void );
		void DropStructureItem( void )
		{
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
		void FillStructureView( void );
		void FillTableMenu( void );
		void FillRecordForm( void );
		void FillListView( void );
		void UpdateUI( void )
		{
			FillStructureView();
			UI.Main.Broadcasters.DatabaseOpened.Disable();
/*
			UI.Structure.Broadcasters.StructureItemEdition.Disable();
			UI.Structure.Broadcasters.StructureItemCreation.Disable();
			UI.Structure.Broadcasters.StructureItemModification.Disable();
			UI.Structure.Broadcasters.StructureItemDeletion.Disable();
*/
		}
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

			if ( _Current.Table == UNDEFINED_TABLE ) {
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

			if ( _Current.Field == UNDEFINED_FIELD ) {
				UI.NameTextbox.SetValue( str::string( "" ) );

				UI.CommentTextbox.SetValue( str::string( "" ) );
			}

			UI.NameTextbox.Select();
		}
		void SelectTable( table__ Table )
		{
			_Current = target__( Table );

			FillTableMenu();

			BrowseList();
		}
		void SetCurrent( target__ Current )
		{
			_Current = Current;
		}
		void SetCurrent();
		void SetTemporaryMode( temporary_mode__ Mode )
		{
			if ( _Temporary.Mode != tm_Undefined )
				ERRc();

			_Temporary.Mode = Mode;
		}
		void ApplyRecord( void );
		E_RODISCLOSE__( target__, Current );
	};

	inline bkdacc::id32__ _ExtractRow( const str::string_ &Value )
	{
		epeios::row__ Error = NONE;
		bkdacc::id32__ Id = Value.ToUL( &Error );

		if ( Error != NONE )
			ERRu();

		return Id;
	
	}

	inline table__ ExtractTable( const str::string_ &Value )
	{
		return _ExtractRow( Value );
	}

	inline field__ ExtractField( const str::string_ &Value )
	{
		return _ExtractRow( Value );
	}
}

#endif