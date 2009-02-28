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

#define KERNEL_DEFAULT_LANGUAGE	lgg::lEnglish
namespace kernel {

	enum message__ 
	{
		mCancelInputConfirmation,
		m_amount,
		m_Undefined
	};

	BKDACC_T32( table );
#define UNDEFINED_TABLE	((kernel::table__)BKDACC_UNDEFINED_ID32 )

	BKDACC_T8( table_id );
#define UNDEFINED_TABLE_ID	((kernel::table_id__)BKDACC_UNDEFINED_ID8 )

	BKDACC_T32( field );
#define UNDEFINED_FIELD	((kernel::field__)BKDACC_UNDEFINED_ID32 )

	BKDACC_T8( field_id );
#define UNDEFINED_FIELD_ID	((kernel::field_id__)BKDACC_UNDEFINED_ID8 )

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
	  public _backend___
	{
	private:
		msg::buffer__ _MessageBuffer;
		lgg::language__ _Language;
		log_functions__ _LogFunctions;
		csducl::universal_client_core _ClientCore;
		void _DumpStructureAsXML(
			const target__ &Target,
			str::string_ &XML );
		target__ _Current;
		void _H( bso::bool__ Result )
		{
		ERRProlog
			STR_BUFFER___ Buffer;
			str::string Message;
		ERRBegin
			if ( !Result ) {
				Message.Init();
				Message.Append( GetRawMessage() );
				Message.Append( '\n' );
				Message.Append( GetI18Message() );

				nsxpcm::Alert( UI.Main.Window, Message.Convert( Buffer ) );
				ERRReturn;
			}
		ERRErr
		ERREnd
		ERREpilog
		}
	protected:
		virtual const kernel___ &K_( void ) const
		{
			return *this;
		}
		virtual kernel___ &K_( void )
		{
			return *this;
		}
	public:
		ui::ui__ UI;
		void reset( bso::bool__ P = true )
		{
			_backend___::reset( P );
			_ClientCore.reset( P );
			_Language = lgg::l_undefined;
			_Current.reset( P );
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

//			_ClientCore.Init( "192.168.5.10:1234", NULL, _LogFunctions, csducl::tShared );
			_ClientCore.Init( "localhost:1234", NULL, _LogFunctions, csducl::tShared );
			_backend___::Init( _ClientCore );
			UI.Init( *this );

			_Language = KERNEL_DEFAULT_LANGUAGE;	// A chnager.
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
		void CreateDatabase(
			const str::string_ &Location,
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			_H( Manager.CreateDatabase( Location, Name, Comment ) );
		}
		void OpenDatabase( const str::string_ &Location )
		{
			_H( Manager.OpenDatabase( Location ) );
		}
		table__ CreateTable(
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			table__ Table = UNDEFINED_TABLE;

			_H( Manager.CreateTable( Name, Comment, *Table ) );

			return Table;
		}
		table__ CreateTable( void )
		{
			table__ Table = UNDEFINED_TABLE;
		ERRProlog
			str::string Name, Comment;
		ERRBegin
			Name.Init();
			UI.Structure.NameTextbox.GetValue( Name );

			Comment.Init();
			UI.Structure.CommentTextbox.GetValue( Comment );

			Table = CreateTable( Name, Comment );
		ERRErr
		ERREnd
		ERREpilog
			return Table;
		}
		field__ AddField(
			table__ Table,
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			field__ Field = UNDEFINED_FIELD;

			_H( Manager.AddField( *Table, Name, Comment, *Field ) );

			return Field;
		}
		field__ AddField( void )
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

			Field = AddField( _Current.Table, Name, Comment );
		ERRErr
		ERREnd
		ERREpilog
			return Field;
		}
		void FillTableMenu( void );
		void FillStructureView( void );
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
		void DefineTable( void )
		{
			ui_struct::structure__ &UI = this->UI.Structure;

			UI.Broadcasters.ItemBrowsing.Disable();
			UI.Broadcasters.ItemEdition.Enable();

			UI.NameTextbox.SetValue( str::string( "" ) );
			UI.NameTextbox.Select();

			UI.CommentTextbox.SetValue( str::string( "" ) );
		}
		void DefineField( void )
		{
			ui_struct::structure__ &UI = this->UI.Structure;

			UI.Broadcasters.ItemBrowsing.Disable();
			UI.Broadcasters.ItemEdition.Enable();

			UI.NameTextbox.SetValue( str::string( "" ) );
			UI.NameTextbox.Select();

			UI.CommentTextbox.SetValue( str::string( "" ) );
		}
		void GetDatabaseInfos(
			str::string_ &Name,
			str::string_ &Comment )
		{
			_H( Manager.GetDatabaseInfos( Name, Comment ) );
		}
		void GetTables( tables_ &Tables )
		{
			_H( Manager.GetTables( _( Tables ) ) );
		}
		void GetTablesInfos(
			const tables_ &Tables,
			bkdacc::strings_ &Names,
			bkdacc::strings_ &Comments,
			table_ids_ &Ids )
		{
			_H( Manager.GetTablesInfos( _( Tables ), Names, Comments, _( Ids ) ) );
		}
		void GetTableInfo(
			table__ Table,
			str::string_ &Name,
			str::string_ &Comment,
			table_id__ &Id );
		void GetFields(
			table__ Table,
			fields_ &Fields )
		{
			_H( Manager.GetFields( *Table, _( Fields ) ) );
		}
		void GetFieldsInfos(
			const fields_ &Fields,
			bkdacc::strings_ &Names,
			bkdacc::strings_ &Comments,
			field_ids_ &Ids )
		{
			_H( Manager.GetFieldsInfos( _( Fields ), Names, Comments, _( Ids ) ) );
		}
		void GetFieldInfo(
			field__ field,
			str::string_ &Name,
			str::string_ &Comment,
			field_id__ &Id );
		void SetCurrent( target__ Current )
		{
			_Current = Current;
		}
		void SetCurrent();
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