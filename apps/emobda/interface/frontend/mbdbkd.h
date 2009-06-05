/*
	'mbdbkd.h' by Claude SIMON (http://zeusw.org/).

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


#ifndef MBDBKD__INC
#define MBDBKD__INC

#include "mbdapi.h"
#include "bkduac.h"
#include "csducl.h"

namespace mbdbkd {

	BKDACC_T32( table );
#define UNDEFINED_TABLE		((mbdbkd::table__)BKDACC_UNDEFINED_ID32 )

	BKDACC_T8( table_id );
#define UNDEFINED_TABLE_ID	((mbdbkd::table_id__)BKDACC_UNDEFINED_ID8 )

	BKDACC_T32( field );
#define UNDEFINED_FIELD		((mbdbkd::field__)BKDACC_UNDEFINED_ID32 )

	BKDACC_T8( field_id );
#define UNDEFINED_FIELD_ID	((mbdbkd::field_id__)BKDACC_UNDEFINED_ID8 )

	BKDACC_T16( record );
#define UNDEFINED_RECORD	((mbdbkd::record__)BKDACC_UNDEFINED_ID16 )

	class backend___
	{
	private:
		emobda_backend_access___<bkduac::backend_universal_access___> _BackendAccess;
		mbd_manager_common__	_ManagerCommon;
		csducl::universal_client _Flow;
		bso::bool__ _H( bso::bool__ Result )
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

//				nsxpcm::Alert( UI.Main.Window, Message.Convert( Buffer ) );
				_Report( Message.Convert( Buffer ) );
				ERRReturn;
			}
		ERRErr
		ERREnd
		ERREpilog
			return Result;
		}
		void _Report( const char *Message )
		{
			__Report( Message );
		}
	protected:
		virtual void __Report( const char *Message ) =0;
	public:
		mbd_manager___ Manager;
		void reset( bso::bool__ P = true )
		{
			Manager.reset( P );
			// _ManagerCommon.reset( P );
			_BackendAccess.reset( P );
			_Flow.reset( P );
		}
		void Init( csducl::universal_client_core &ClientCore )
		{
			_Flow.Init( ClientCore );
			switch ( ClientCore.GetType() ) {
			case csducl::tLibrary:
				_BackendAccess.Init( _Flow, bkduac::tLocal );
				break;
			case csducl::tShared:
				_BackendAccess.Init( _Flow, bkduac::tRemote );
				break;
			default:
				ERRu();
				break;
			}
			_ManagerCommon.Init( _BackendAccess );
			Manager.Init( _ManagerCommon );
			
		}
		const char *GetRawMessage( void ) const
		{
			return _BackendAccess.GetRawMessage();
		}
		const char *GetI18Message( void ) const
		{
			return _BackendAccess.GetI18Message();
		}
		bso::bool__ CreateDatabase(
			const str::string_ &Location,
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			return _H( Manager.CreateDatabase( Location, Name, Comment ) );
		}
		bso::bool__ GetAvailableDatabases( bkdacc::strings_ &Databases )
		{
			return _H( Manager.GetAvailableDatabases( Databases ) );
		}
		bso::bool__ OpenDatabase( const str::string_ &Location )
		{
			return _H( Manager.OpenDatabase( Location ) );
		}
		table__ CreateOrModifyTable(
			table__ Table,
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			_H( Manager.CreateOrModifyTable( *Table, Name, Comment, *Table ) );

			return Table;
		}
		field__ AddOrModifyField(
			field__ Field,
			table__ Table,
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			_H( Manager.AddOrModifyField( *Field, *Table, Name, Comment, *Field ) );

			return Field;
		}
		bso::bool__ DeleteTable( table__ Table )
		{
			return _H( Manager.DeleteTable( *Table ) );
		}
		bso::bool__ DeleteField( field__ Field )
		{
			return _H( Manager.DeleteField( *Field ) );
		}
		bso::bool__ GetDatabaseInfos(
			str::string_ &Name,
			str::string_ &Comment )
		{
			return _H( Manager.GetDatabaseInfos( Name, Comment ) );
		}
		bso::bool__ GetTables( tables_ &Tables )
		{
			return _H( Manager.GetTables( _( Tables ) ) );
		}
		bso::bool__ GetTablesInfos(
			const tables_ &Tables,
			bkdacc::strings_ &Names,
			bkdacc::strings_ &Comments,
			table_ids_ &Ids )
		{
			return _H( Manager.GetTablesInfos( _( Tables ), Names, Comments, _( Ids ) ) );
		}
		void GetTableInfo(
			table__ Table,
			str::string_ &Name,
			str::string_ &Comment,
			table_id__ &Id );
		bso::bool__ GetFields(
			table__ Table,
			fields_ &Fields )
		{
			return _H( Manager.GetFields( *Table, _( Fields ) ) );
		}
		bso::bool__ GetFieldsInfos(
			const fields_ &Fields,
			bkdacc::strings_ &Names,
			bkdacc::strings_ &Comments,
			field_ids_ &Ids,
			tables_ &Tables )
		{
			return _H( Manager.GetFieldsInfos( _( Fields ), Names, Comments, _( Ids ), _( Tables ) ) );
		}
		void GetFieldInfo(
			field__ field,
			str::string_ &Name,
			str::string_ &Comment,
			field_id__ &Id,
			table__ &Table );
		record__ InsertRecord(
			const bkdacc::items32_ &Items,
			table__ Table )
		{
			record__ Record = UNDEFINED_RECORD;

			_H( Manager.InsertOrModifyRecord( *Record, *Table, Items, *Record ) );

			return Record;
		}
		bso::bool__ ModifyRecord(
			record__ Record,
			const bkdacc::items32_ &Items,
			table__ Table )
		{
			return _H( Manager.InsertOrModifyRecord( *Record, *Table, Items, *Record ) );
		}
		bso::bool__ DeleteRecord(
			record__ Record,
			table__ Table )
		{
			return _H( Manager.DeleteRecord( *Record, *Table ) );
		}
		bso::bool__ GetRecords(
			table__ Table,
			records_ &Records )
		{
			return _H( Manager.GetRecords( *Table, _( Records ) ) );
		}
		bso::bool__ GetRecordsData(
			table__ Table,
			fields_ &Fields,
			records_ &Records,
			bkdacc::xstrings_ &DataCluster )
		{
			return _H( Manager.GetRecordsData( *Table, _( Fields ), _( Records ), DataCluster ) );
		}
		bso::bool__ CloseDatabase( void )
		{
			return _H( Manager.CloseDatabase() );
		}
	};
}

#endif