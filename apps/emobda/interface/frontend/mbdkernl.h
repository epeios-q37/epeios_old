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

#ifndef MBDKERNL__INC
#define MBDKERNL__INC

#include "mbdbkd.h"

#include "mbdtrnsnt.h"

#include "csdsnc.h"
#include "csducl.h"
#include "xml.h"
#include "msg.h"
#include "lgg.h"
#include "bkdacc.h"


// #define ADDRESS	"192.168.5.10:1234"	// Portable.
// #define ADDRESS	"10.0.2.2:1234"		// Logiplus.
// #define MBDKERNL_TEST_ADDRESS	"localhost:1234"	// Local

#define MBDKERNL_TEST_LOCATION	"d:\\emobda\\test"

#define MBDKERNL_DEFAULT_LANGUAGE	lgg::lEnglish

namespace mbdkernl {
	using namespace mbdbkd;
	using namespace mbdtrnsnt;

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

		mUnableToReachRecord,
		m_amount,
		m_Undefined
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

#define MBDKERNL_TRANSIENT_USE( type, object )\
		const type &object( void ) const\
		{\
			return _Transient.object();\
		}\
		type &object( void )\
		{\
			return _Transient.object();\
		}\


	class kernel___
	: private _backend___
	{
	private:
		msg::buffer__ _MessageBuffer;
		lgg::language__ _Language;
		log_functions__ _LogFunctions;
		csducl::universal_client_core _ClientCore;
		void _DumpCurrent( xml::writer_ &Writer );
		void _DumpAvailableDatabases( xml::writer_ &Writer );
		void _DumpStructure( xml::writer_ &Writer );
		void _DumpContent( xml::writer_ &Writer );
		transient__ _Transient;
		records _Records;
		target__ _Target;
	public:
		void reset( bso::bool__ P = true )
		{
			_backend___::reset( P );
			_ClientCore.reset( P );
			_Language = lgg::l_undefined;
			_Target.reset( P );
			_Records.reset( P );
			_Transient.reset( P );
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
			csducl::type__ Type,
			const char *RemoteHostServiceOrLocalLibraryPath )
		{
			reset();

			_ClientCore.Init( RemoteHostServiceOrLocalLibraryPath, NULL, _LogFunctions, Type );
			_backend___::Init( _ClientCore );

			_Language = MBDKERNL_DEFAULT_LANGUAGE;	// A changer.

			_Records.Init();
		}
		const char *GetMessage( message__ Message );
		MBDKERNL_TRANSIENT_USE( structure_management, StructureManagement );
		MBDKERNL_TRANSIENT_USE( database_identification, DatabaseIdentification );
		MBDKERNL_TRANSIENT_USE( database_selection, DatabaseSelection );
		MBDKERNL_TRANSIENT_USE( backend_selection, BackendSelection );
		MBDKERNL_TRANSIENT_USE( record_input, RecordInput );
		bso::bool__ CreateDatabase(
			const str::string_ &Name,
			const str::string_ &Path,
			const str::string_ &Comment )
		{
			if ( _backend___::CreateDatabase( Path, Name, Comment ) ) {
				_Target.Set( UNDEFINED_TABLE );
				return true;
			} else
				return false;
		}
		bso::bool__ OpenDatabase( const str::string_ &Path )
		{
			if ( _backend___::OpenDatabase( Path ) )
			{
				_Target.Set( UNDEFINED_TABLE );
				return true;
			} else
				return false;

		}
		bso::bool__ CloseDatabase( void )
		{
			if ( _backend___::CloseDatabase() ) {
				_Target.reset();
				return true;
			} else
				return false;
		}
		bso::bool__ GetDatabaseInfos(
			str::string_ &Name,
			str::string_ &Comment )
		{
			return _backend___::GetDatabaseInfos( Name, Comment );
		}
		table__ CreateOrModifyTable(
			const str::string_ Name,
			const str::string_ &Comment )
		{
			return _backend___::CreateOrModifyTable( StructureManagement().Target.Table, Name, Comment );
		}
		bso::bool__ GetTableInfos(
			table__ Table,
			str::string_ &Name,
			str::string_ &Comment,
			table_id__ Id )
		{
			return _backend___::GetTableInfos( Table, Name, Comment, Id );
		}
		bso::bool__ DeleteTable( void )
		{
			if ( GetTarget().Table == UNDEFINED_TABLE )
				ERRc();

			if ( _backend___::DeleteTable( GetTarget().Table ) ) {
				Target().Set( UNDEFINED_TABLE );
				return true;
			} else
				return false;
		}
		field__ CreateOrModifyField(
			const str::string_ Name,
			const str::string_ &Comment )
		{
			if ( StructureManagement().Target.Table == UNDEFINED_TABLE )
				ERRu();

			return AddOrModifyField( StructureManagement().Target.Field, StructureManagement().Target.Table, Name, Comment );
		}
		bso::bool__ GetFieldInfos(
			field__ Field,
			str::string_ &Name,
			str::string_ &Comment,
			field_id__ &Id,
			table__ &Table )
		{
			return _backend___::GetFieldInfos( Field, Name, Comment, Id, Table );
		}
		bso::bool__ DeleteField( void )
		{
			if ( GetTarget().Field == UNDEFINED_FIELD )
				ERRc();

			if ( _backend___::DeleteField( GetTarget().Field ) ) {
				Target().Set( UNDEFINED_FIELD );
				return true;
			} else
				return false;
		}
		record__ InsertRecord(
			const bkdacc::items32_ &Data,
			table__ Table )
		{
			return _backend___::InsertRecord( Data, Table );
		}
		bso::bool__ InsertRecord( const bkdacc::items32_ &Data )
		{
			record__ Record = InsertRecord( Data, GetTarget().Table );

			if ( Record != UNDEFINED_RECORD ) {
				_Records.Append( Record );
				Target().Set( Record, _Records.Amount() );
			} else {
				Target().Set( UNDEFINED_RECORD, UNDEFINED_RECORD_POSITION );
			}
			
			return Record != UNDEFINED_RECORD;
		}
		bso::bool__ ModifyRecord( 
			record__ Record,
			const bkdacc::items32_ &Data,
			table__ Table )
		{
			return _backend___::ModifyRecord( Record, Data, Table );
		}
		bso::bool__ ModifyRecord( const bkdacc::items32_ &Data )
		{
			return ModifyRecord( GetTarget().Record, Data, GetTarget(). Table );
		}
		bso::bool__ RemoveRecord( void )
		{
			if ( DeleteRecord( GetTarget().Record, GetTarget().Table ) ) {
				Target().Set( UNDEFINED_RECORD, UNDEFINED_RECORD_POSITION );
				return true;
			} else
				return false;
		}
		void ApplyQuery( void );
		void ResetTransient( void )
		{
			_Transient.reset();
		}
		context__ GetTransientContext( void ) const
		{
			return _Transient.Context();
		}
		E_RWDISCLOSE__( target__, Target );
		bso::size__ GetRecordsAmount( void ) const
		{
			return _Records.Amount();
		}
		bso::bool__ GoToRecordNumber( record_position__ Position )
		{
			if ( Position == 0 )
				return false;

			if ( Position > _Records.Amount() )
				return false;

			_Target.Set( _Records( Position - 1 ), Position );

			return true;
		}
		bso::bool__ GoToFirstRecord( void )
		{
			return GoToRecordNumber( 1 );
		}
		bso::bool__ GoToPreviousRecord( void )
		{
			return GoToRecordNumber( _Target.RecordPosition - 1 ); 
		}
		bso::bool__ GoToNextRecord( void )
		{
			return GoToRecordNumber( _Target.RecordPosition + 1 ); 
		}
		bso::bool__ GoToLastRecord( void )
		{
			return GoToRecordNumber( _Records.Amount() );
		}
		void DumpAsXML( str::string_ &XML );
	};

	inline bkdacc::id32__ _ExtractId32( const str::string_ &Value )
	{
		return Value.ToUL();
	}

	inline bkdacc::id16__ _ExtractId16( const str::string_ &Value )
	{
		return Value.ToUS();
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

	inline record_position__ ExtractRecordPosition( const str::string_ &Value )
	{
		return Value.ToUL();
	}
}

#endif