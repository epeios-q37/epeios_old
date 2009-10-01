/*
	'frdkernl.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef FRDKERNL__INC
#define FRDKERNL__INC

#include "frdbcknd.h"

#include "frdtrnsnt.h"
#include "frdrgstry.h"

#include "csdsnc.h"
#include "csducl.h"
#include "xml.h"
#include "bkdacc.h"

#define FRDKERNL_DEFAULT_LANGUAGE	"en"

namespace frdkernl {
	using namespace frdbcknd;
	using namespace frdtrnsnt;

	// If modified, modify 'GetRawMessage_(...)' in 'frdkernel.cpp' according.
	enum message__ 
	{
		mSelectProjectFile,

		mUnableToOpenProjectFile_1,
		mMissingProjectTree,
		mMissingProjectName,

		mBadOrMissingBackendType,
		mNoRemoteHostServiceGiven,
		mNoBackendLocationGiven,
		mUnableToConnectToBackend_1,

		mMissingDatabaseName,
		mMissingDatabasePath,

		mNoDatabaseSelected,
		mBadDatabasePath,

		mDropStructureItemConfirmation,

		mDeleteTableConfirmation,
		mDeleteFieldConfirmation,
		mDeleteRecordConfirmation,

		mDropRecordConfirmation,

		mNoSuchRecordPosition,
		mBadRecordPosition,

		mNotImplementedYet,

		m_amount,
		m_Undefined
	};

	// What should be in the XML dump. Not used as is, but through 'xml_dump_flag__'.
	// Id modified, modify 'xml_dump_flag__' according.
	enum _xml_dump__ {
		_xdCurrentTable,	// Current selected table.
		_xdCurrentField,	// Current selected field.
		_xdCurrentRecord,	// Current selected record.
		_xdStructure,	// The structure of the database.
		_xdContent,	// The content (data of the record list or record).
		_xdDatabases,	// The available databases.
		_xd_amount,
		_xd_undefined
	};

#ifdef XDF
#	define FRDKERNL__XDF_BACKUP	XDF
#endif

#define XDF( name )	xdf##name = ( 1 << _xd##name )

	enum xml_dump_flag__ {
		XDF( CurrentTable ),
		XDF( CurrentField ),
		XDF( CurrentRecord ),
		xdfAllCurrent = xdfCurrentTable | xdfCurrentField | xdfCurrentRecord,
		XDF( Structure ),
		XDF( Content ),
		XDF( Databases ),
		xdfNone = 0,
		xdfAll = ( ( 1 << _xd_amount ) - 1 ),
	};

#ifdef FRDKERNL__XDF_BACKUP
#	define XDF FRDKERNL__XDF_BACKUP
#endif

	csducl::type__ GetBackendType( const frdrgstry::registry___ &Registry );

	typedef frdbcknd::backend___ _backend___;

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

#define FRDKERNL_TRANSIENT_USE( type, object )\
		const type &object( void ) const\
		{\
			return _Transient.object();\
		}\
		type &object( void )\
		{\
			return _Transient.object();\
		}\

		struct target__ {
			bso::bool__ Database;	// At 'true' if database opened, 'false' otherwise.
			table__ Table;
			field__ Field;
			record__ Record;
			record_position__ RecordPosition;
		void reset( bso::bool__ = true )
		{
			Database = false;
			Table = UNDEFINED_TABLE;
			Field = UNDEFINED_FIELD;
			Record = UNDEFINED_RECORD;
			RecordPosition = UNDEFINED_RECORD_POSITION;
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
			record_position__ RecordPosition,
			table__ Table )
		{
			Set( Record, RecordPosition, Table );
		}
		target__( table__ Table )
		{
			Set( Table );
		}
		void Init( void )
		{
			reset();
		}
		void Set(
			record__ Record,
			record_position__ RecordPosition,
			field__ Field,
			table__ Table,
			bso::bool__ Database )
		{
			if ( ( Record != UNDEFINED_RECORD ) && ( Table == UNDEFINED_TABLE ) )
				ERRu();

			if ( ( Field != UNDEFINED_FIELD ) && ( Table == UNDEFINED_TABLE ) )
				ERRu();

			if ( Table != UNDEFINED_TABLE && !Database )
				ERRu();

			this->Record = Record;
			this->RecordPosition = RecordPosition;
			this->Field = Field;
			this->Table = Table;
			this->Database = Database;
		}
		void Set(
			record__ Record,
			record_position__ RecordPosition,
			table__ Table )
		{
			Set( Record, RecordPosition, UNDEFINED_FIELD, Table, Database );
		}
		void Set(
			record__ Record,
			record_position__ RecordPosition )
		{
			Set( Record, RecordPosition, this->Table );
		}
		void Set(
			field__ Field,
			table__ Table )
		{
			Set( UNDEFINED_RECORD, UNDEFINED_RECORD_POSITION, Field, Table, Database );
		}
		void Set( field__ Field )
		{
			Set( Field, this->Table );
		}
		void Set( table__ Table )
		{
			Set( UNDEFINED_RECORD, UNDEFINED_RECORD_POSITION, UNDEFINED_FIELD, Table, Database );
		}
		void Set( bso::bool__ Database )
		{
			Set( UNDEFINED_RECORD, UNDEFINED_RECORD_POSITION, UNDEFINED_FIELD, UNDEFINED_TABLE, Database );
		}
	};

	class kernel___
	: private _backend___
	{
	private:
		str::string _Language;
		log_functions__ _LogFunctions;
		csducl::universal_client_core _ClientCore;
		rgstry::registry _GlobalRegistry;
		rgstry::overloaded_unique_registry___ _Registry;
		rgstry::nrow__ _LocalRegistryRoot;
		transient__ _Transient;
		records _Records;
		target__ _Target;
		bso::bool__ _ProjectIsOpen;
		void _DumpCurrent(
			int Flags,
			xml::writer_ &Writer );
		void _DumpAvailableDatabases( xml::writer_ &Writer );
		void _DumpStructure( xml::writer_ &Writer );
		void _DumpContent( xml::writer_ &Writer );
		bso::bool__ _Connect(
			const char *RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type );
		bso::bool__ _Connect(
			const str::string_ &RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type );
		void _CloseConnection( void )
		{
			if ( !IsConnected() )
				ERRu();

			_backend___::reset();
			_ClientCore.reset();
		}
		void _EndClosing( void )
		{
			_GlobalRegistry.Init();
			_Registry.Init( _GlobalRegistry, NONE);
			_LocalRegistryRoot = NONE;
			_Target.Init();
			_Records.Init();
			_Transient.Init();
			_ProjectIsOpen = false;
		}
	protected:
		virtual void FRDKERNLClose( void ) = 0;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				Close();

			_backend___::reset( P );
			_ClientCore.reset( P );
			_Registry.reset( P );
			_LocalRegistryRoot = NONE;
			_GlobalRegistry.reset( P );
			_Language.reset( P );
			_Target.reset( P );
			_Records.reset( P );
			_Transient.reset( P );
			_ProjectIsOpen = false;
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
			const str::string_ &Language,
			const char *LocalesFileName );
		bso::bool__ Connect(
			const char *RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type );
		bso::bool__ Connect(
			const str::string_ &RemoteHostServiceOrLocalLibraryPath,
			csducl::type__ Type );
		const str::string_ &BackendVersion( str::string_ &Version )
		{
			_backend___::Version( Version );

			return Version;
		}
		bso::bool__ OpenProject(
			const char *ProjectFile,
			const char *RootPath,
			str::string_ &ProjectName,
			str::string_ &Message );	// If 'false' returned, error is in 'Message'.
		bso::bool__ OpenProject(
			xtf::extended_text_iflow__ &Project,
			const char *RootPath,
			str::string_ &ProjectNAme,
			str::string_ &Message );	// If 'false' returned, error is in 'Message'.
		void Close( void )
		{
			if ( IsProjectOpened() || IsConnected() ) {
				FRDKERNLClose();
				_CloseConnection();
			}

			_EndClosing();
		}
		bso::bool__ IsProjectOpened( void )
		{
			return _ProjectIsOpen;
		}
		bso::bool__ IsConnected( void )
		{
			return _backend___::IsConnected();
		}
		void SetLocalRegistry(
			xtf::extended_text_iflow__ &Project,
			const str::string_ &Path );	/* To call after 'Init()'. 'Project' contains the 'XML' tree containing the user configuration.
											'Path' contains the path of the root subtree. */
		void DumpRegistry(
			rgstry::nrow__ Root,
			txf::text_oflow__ &OFlow ) const
		{
			_GlobalRegistry.Dump( Root, true, true, OFlow );
		}
		void DumpLocalRegistry( txf::text_oflow__ &OFlow ) const
		{
			DumpRegistry( _LocalRegistryRoot, OFlow );
		}
		const str::string_ &GetMessage(
			message__ Message,
			str::string_ &Translation );
		const char *GetMessage(
			message__ Message,
			STR_BUFFER___ &Buffer );
		FRDKERNL_TRANSIENT_USE( structure_management, StructureManagement );
		FRDKERNL_TRANSIENT_USE( database_identification, DatabaseIdentification );
		FRDKERNL_TRANSIENT_USE( database_selection, DatabaseSelection );
		FRDKERNL_TRANSIENT_USE( backend_selection, BackendSelection );
		FRDKERNL_TRANSIENT_USE( record_input, RecordInput );
		bso::bool__ GetRegistryValue(
			const char *Path,
			str::string_ &Value )
		{
			return _Registry.GetPathValue( str::string( Path ), Value );
		}
		bso::bool__ CreateDatabase(
			const str::string_ &Name,
			const str::string_ &Path,
			const str::string_ &Comment )
		{
			if ( _backend___::CreateDatabase( Path, Name, Comment ) ) {
				_Target.Set( UNDEFINED_TABLE );
				_Target.Set( true );
				return true;
			} else
				return false;
		}
		bso::bool__ OpenDatabase( const str::string_ &Path )
		{
			if ( _backend___::OpenDatabase( Path ) )
			{
				_Target.Set( true );
				return true;
			} else
				return false;

		}
		bso::bool__ CloseDatabase( void )
		{
			if ( !IsConnected() )
				ERRc();

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
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			return _backend___::CreateOrModifyTable( _Target.Table, Name, Comment );
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
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			if ( _Target.Table == UNDEFINED_TABLE )
				ERRu();

			return AddOrModifyField( _Target.Field, _Target.Table, Name, Comment );
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
				if ( Target().RecordPosition != UNDEFINED_RECORD_POSITION )
					_Records.Remove( Target().RecordPosition - 1 );
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
		bso::bool__ GoToRecordPosition( record_position__ Position )
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
			return GoToRecordPosition( 1 );
		}
		bso::bool__ GoToPreviousRecord( void )
		{
			return GoToRecordPosition( _Target.RecordPosition - 1 ); 
		}
		bso::bool__ GoToNextRecord( void )
		{
			return GoToRecordPosition( _Target.RecordPosition + 1 ); 
		}
		bso::bool__ GoToLastRecord( void )
		{
			return GoToRecordPosition( _Records.Amount() );
		}
		void DumpAsXML(
			int Flags,
			str::string_ &XML );
		bso::bool__ GetDatabase( str::string_ &Database )
		{
			return frdrgstry::GetPathValue( frdrgstry::paths::Parameters.Database.Path, _Registry, Database );
		}
		bso::bool__ GetCurrentTable( table__ &Table )
		{
			return frdrgstry::GetProfileIntegerValue( frdrgstry::paths::Profiles.CurrentTable, _Registry, **Table );
		}
		void SetCurrentTable( table__ Table )
		{
			frdrgstry::SetProfileIntegerValue( frdrgstry::paths::Profiles.CurrentTable, _Registry, **Table );
		}	
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