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

#include "csdsnc.h"
#include "csducl.h"
#include "xml.h"
#include "msg.h"
#include "lgg.h"
#include "bkdacc.h"
#include "nsxpcm.h"

// #define ADDRESS	"192.168.5.10:1234"	// Portable.
// #define ADDRESS	"10.0.2.2:1234"		// Logiplus.
#define ADDRESS	"localhost:1234"	// Local

#define DEFAULT_LOCATION	"d:\\emobda\\test"


#define KERNEL_DEFAULT_LANGUAGE	lgg::lEnglish
namespace kernel {
	using namespace mbdbkd;

	E_ROW( krow__ );	// 'kernel row', see below.

	extern nsxpcm::repository< class kernel___, krow__> Repository;

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

	enum temporary_context__ {
		tcStructureManagement,
		tcDatabaseIdentification,
		tcDatabaseSelection,
		tcRecordInput,
		tc_amount,
		tc_Undefined
	};

	template <typename e, e amount> struct _state__
	{
	private:
		e _State;
		e _Undefined( void ) const
		{
			return (e)( amount + 1 );
		}
		int _Amount( void ) const
		{
			return amount;
		}
	public:
		void reset( bso::bool__ = true )
		{
			_State = _Undefined();
		}
		e GetState( void ) const
		{
			return _State;
		}
		void SetState( e State )
		{
			if ( ( _State > _Amount() ) && ( _State != _Undefined() ) )
				ERRu();

			_State = State;
		}
	};

	enum structure_management_state__
	{
		smsModification,
		smsCreation,
		smsDuplication,
		sms_amount,
		sms_Undefined,
	};

	typedef _state__<structure_management_state__, sms_amount> _sms__;

	struct structure_management
	: public _sms__
	{
		target__ Target;
		void reset( bso::bool__ P = true )
		{
			_sms__::reset( P );
		}

	};

	enum database_identification_state__ {
		disValidated,
		disCancelled,
		dis_amount,
		dis_Undefined
	};

	typedef _state__<database_identification_state__, dis_amount> _dis__;

	struct database_identification
	: public _dis__
	{
	private:
		str::string _Name, _Path, _Comment;
		void _SetState( database_identification_state__ State )
		{
			_dis__::SetState( State );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_dis__::reset( P );

			_Name.reset();
			_Path.reset();
			_Comment.reset();
		}
		void Set(
			const str::string_ &Name,
			const str::string_ &Path,
			const str::string_ &Comment )
		{
			if ( GetState() != dis_Undefined )
				ERRc();

			_SetState( disValidated );

			_Name.Init( Name );
			_Path.Init( Path );
			_Comment.Init( Comment );
		}
		void SetCancelledState( void )
		{
			if ( GetState() != dis_Undefined )
				ERRc();

			_SetState( disCancelled );
		}
		void Get(
			str::string_ &Name,
			str::string_ &Path,
			str::string_ &Comment ) const
		{
			if ( GetState() != disValidated )
				ERRc();

			Name = _Name;
			Path = _Path;
			Comment = _Comment;
		}
		void SetState( void )	// To avoid the use of the '_dis__' one, because it has not to be used directly.
		{
			ERRu();
		}
	};

	enum database_selection_state__ {
		dssValidated,
		dssCancelled,
		dss_amount,
		dss_Undefined,
	};

	typedef _state__<database_selection_state__, dss_amount> _dss__;

	class database_selection
	: public _dss__
	{
	private:
		str::string _Path;
		void _SetState( database_selection_state__ State )
		{
			_dss__::SetState( State );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_dss__::reset( P );

			_Path.reset( P );
		}
		void Set( const str::string_ &Path )
		{
			if ( GetState() != dss_Undefined )
				ERRc();

			_SetState( dssValidated );

			_Path.Init( Path );
		}
		void SetCancelledState( void )
		{
			if ( GetState() != dss_Undefined )
				ERRc();

			_SetState( dssCancelled );
		}
		void Get( str::string_ &Path ) const
		{
			if ( GetState() != dssValidated )
				ERRc();

			Path = _Path;
		}
		void SetState( void )	// To avoid the use of the '_dis__' one, because it has not to be used directly.
		{
			ERRu();
		}
	};

	enum record_input_state__ {
		risCreation,
		risModification,
		risDuplication,
		ris_amount,
		ris_Undefined,
	};

	typedef _state__<record_input_state__, ris_amount> _ris__;

	class record_input
	: public _ris__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			_ris__::reset( P );
		}
	};

#define KERNEL_TEMPORARY_DEF( type, object )\
	private:\
		type _##object;\
	public:\
		const type &object( void ) const\
		{\
		_Test( tc##object );\
			return _##object;\
		}\
		type &object( void )\
		{\
			_TestAndSet( tc##object );\
			return _##object;\
		}\


	class temporary__
	{
	private:
		temporary_context__ _Context;
		void _Test( temporary_context__ Context ) const
		{
			if ( _Context != Context )
				ERRu();
		}
		void _TestAndSet( temporary_context__ Context )
		{
			if ( _Context == tc_Undefined )
				_Context = Context;

			if ( _Context != Context )
				ERRu();
		}
	public:
		KERNEL_TEMPORARY_DEF( structure_management, StructureManagement );
		KERNEL_TEMPORARY_DEF( database_identification, DatabaseIdentification );
		KERNEL_TEMPORARY_DEF( database_selection, DatabaseSelection );
		KERNEL_TEMPORARY_DEF( record_input, RecordInput );
		void reset( bso::bool__ P = true )
		{
			_Context = tc_Undefined;

			_StructureManagement.reset( P );
			_DatabaseIdentification.reset( P );
			_DatabaseSelection.reset( P );
			_RecordInput.reset();
		}
		temporary__( void )
		{
			reset( false );
		}
		E_RODISCLOSE__( temporary_context__, Context );
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

#define KERNEL_TEMPORARY_USE( type, object )\
		const type &object( void ) const\
		{\
			return _Temporary.object();\
		}\
		type &object( void )\
		{\
			return _Temporary.object();\
		}\


	class kernel___
	: private _backend___
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
	public:
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
		KERNEL_TEMPORARY_USE( structure_management, StructureManagement );
		KERNEL_TEMPORARY_USE( database_identification, DatabaseIdentification );
		KERNEL_TEMPORARY_USE( database_selection, DatabaseSelection );
		KERNEL_TEMPORARY_USE( record_input, RecordInput );
		void UpdateDecks( void );
		bso::bool__ CreateDatabase(
			const str::string_ &Name,
			const str::string_ &Path,
			const str::string_ &Comment )
		{
			return _backend___::CreateDatabase( Path, Name, Comment );
		}
		bso::bool__ BrowseDatabase( const str::string_ &Path )
		{
			return OpenDatabase( Path );
		}
		bso::bool__ CloseDatabase( void )
		{
			return _backend___::CloseDatabase();
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
		void ApplyStructureItem( void );
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
		bso::bool__ RemoveRecord( void )
		{
			if ( DeleteRecord( GetTarget().Record, GetTarget().Table ) ) {
				Target().Set( UNDEFINED_RECORD );
				return true;
			} else
				return false;
		}
		void FillDatabaseSelectionList( void );
		void FillStructureView( void );
		void FillTableMenu( void );
		void FillRecordForm( void );
		void FillRecordView( void );
		void FillListView( void );
		void UpdateButtonsPanel( void );
		void SelectTable( table__ Table )
		{
			_Target = target__( Table );

			FillTableMenu();
			BrowseList();
		}
		void SelectRecord( void );
		bso::bool__ GetSelectedStructureItem( target__ &Target );
		void ApplyRecord( void );
		bso::bool__ GetDatabaseIdentification( void );	// Called from UI event. Fill '_Temporary' with content from dedicated interface object.
		bso::bool__ GetSelectedDatabase( void );	// Called from UI event. Fill '_Temporary' with content from dedicated interface object.
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