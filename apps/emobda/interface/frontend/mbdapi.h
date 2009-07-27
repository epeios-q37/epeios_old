/* DON'T MODIFY */
/* This file was generated using getbkdapi	V0.2.2, and a XSL filea
($RCSfile$ $Revision$ $Date$ $Author$) */
// $Id$	(CVS feature).
#ifndef emobda__INC
#define emobda__INC

#include "bkdacc.h"
#include "cpe.h"

namespace emobda {

	enum message__ {
		_UnknowObjectType,
		_UnknowObjectTypeName,
		_UnknowCommandNameOrDescription,
		_UnknowLanguage,
		_BackendError,
		MANAGER_OK,
		MANAGER_IncorrectLocation,
		MANAGER_IncorrectDatabaseName,
		MANAGER_UnableToCreateDatabase,
		MANAGER_UnableToOpenDatabase,
		MANAGER_NameAlreadyUsed,
		MANAGER_IncorrectTableName,
		MANAGER_UnableToCreateTable,
		MANAGER_NoTableGiven,
		MANAGER_UnknownTable,
		MANAGER_IncorrectFieldName,
		MANAGER_UnknownField,
		MANAGER_NoFieldGiven,
		MANAGER_FieldNotOwnedByTable,
		MANAGER_BadFieldRow,
		MANAGER_SameFieldTwice,
		MANAGER_UnknownRecord,
		MANAGER_NoRecordGiven,
	};

	inline message__ GetMessageCode( bkdacc::backend_access___ &Backend )
	{
		const char *Message = Backend.GetRawMessage();

		if ( !strcmp( Message, "_UnknowObjectType" ) )

			return _UnknowObjectType;

		if ( !strcmp( Message, "_UnknowObjectTypeName" ) )

			return _UnknowObjectTypeName;

		if ( !strcmp( Message, "_UnknowCommandNameOrDescription" ) )

			return _UnknowCommandNameOrDescription;

		if ( !strcmp( Message, "_UnknowLanguage" ) )

			return _UnknowLanguage;

		if ( !strcmp( Message, "_BackendError" ) )

			return _BackendError;

		if ( !strcmp( Message, "MANAGER_OK" ) )

			return MANAGER_OK;

		if ( !strcmp( Message, "MANAGER_IncorrectLocation" ) )

			return MANAGER_IncorrectLocation;

		if ( !strcmp( Message, "MANAGER_IncorrectDatabaseName" ) )

			return MANAGER_IncorrectDatabaseName;

		if ( !strcmp( Message, "MANAGER_UnableToCreateDatabase" ) )

			return MANAGER_UnableToCreateDatabase;

		if ( !strcmp( Message, "MANAGER_UnableToOpenDatabase" ) )

			return MANAGER_UnableToOpenDatabase;

		if ( !strcmp( Message, "MANAGER_NameAlreadyUsed" ) )

			return MANAGER_NameAlreadyUsed;

		if ( !strcmp( Message, "MANAGER_IncorrectTableName" ) )

			return MANAGER_IncorrectTableName;

		if ( !strcmp( Message, "MANAGER_UnableToCreateTable" ) )

			return MANAGER_UnableToCreateTable;

		if ( !strcmp( Message, "MANAGER_NoTableGiven" ) )

			return MANAGER_NoTableGiven;

		if ( !strcmp( Message, "MANAGER_UnknownTable" ) )

			return MANAGER_UnknownTable;

		if ( !strcmp( Message, "MANAGER_IncorrectFieldName" ) )

			return MANAGER_IncorrectFieldName;

		if ( !strcmp( Message, "MANAGER_UnknownField" ) )

			return MANAGER_UnknownField;

		if ( !strcmp( Message, "MANAGER_NoFieldGiven" ) )

			return MANAGER_NoFieldGiven;

		if ( !strcmp( Message, "MANAGER_FieldNotOwnedByTable" ) )

			return MANAGER_FieldNotOwnedByTable;

		if ( !strcmp( Message, "MANAGER_BadFieldRow" ) )

			return MANAGER_BadFieldRow;

		if ( !strcmp( Message, "MANAGER_SameFieldTwice" ) )

			return MANAGER_SameFieldTwice;

		if ( !strcmp( Message, "MANAGER_UnknownRecord" ) )

			return MANAGER_UnknownRecord;

		if ( !strcmp( Message, "MANAGER_NoRecordGiven" ) )

			return MANAGER_NoRecordGiven;

		ERRb();

		return (message__)0;	// To avoid a warning.
	}

	template <typename backend_access> class backend_access___
	: public backend_access
	{
	protected:
		virtual void BKDACCTestMessages( void )
		{
		ERRProlog
			bkdacc::strings Messages;
			ctn::E_CMITEM( bkdacc::string_ ) Message;
			epeios::row__ Row = NONE;
		ERRBegin
			Messages.Init();

			backend_access::GetRawMessages( Messages );

			if ( Messages.Amount() != 23 )
				ERRb();

			Message.Init( Messages );

#ifdef CPE__USE_VC_WORKAROUND
#	pragma inline_depth( 0 )	// Sinon VC fait n'importe quoi en configuration 'Release'.
#endif

			Row = Messages.First();

#ifdef CPE__USE_VC_WORKAROUND
#	pragma inline_depth()
#endif

			if ( Message( Row ) != bkdacc::string( "_UnknowObjectType" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "_UnknowObjectTypeName" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "_UnknowCommandNameOrDescription" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "_UnknowLanguage" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "_BackendError" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_OK" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_IncorrectLocation" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_IncorrectDatabaseName" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_UnableToCreateDatabase" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_UnableToOpenDatabase" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_NameAlreadyUsed" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_IncorrectTableName" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_UnableToCreateTable" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_NoTableGiven" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_UnknownTable" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_IncorrectFieldName" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_UnknownField" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_NoFieldGiven" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_FieldNotOwnedByTable" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_BadFieldRow" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_SameFieldTwice" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_UnknownRecord" ) )
				ERRb();

			Row = Messages.Next( Row );


			if ( Message( Row ) != bkdacc::string( "MANAGER_NoRecordGiven" ) )
				ERRb();

			Row = Messages.Next( Row );


		ERRErr
		ERREnd
		ERREpilog
		}
	};

	class statics___
	{
	private:
		bkdacc::object__ ID_;
	public:
		bkdacc::backend_access___ *Backend;
		bkdacc::command__ Commands[1];
		void Init( bkdacc::backend_access___ &Backend )
		{
			bkdacc::commands_details CommandsDetails;
			bkdacc::command_detail CommandDetail;
			bkdacc::ids16 Commands;

			bkdacc::id8__ Parameters[] = {
				0, 
			};

			this->Backend = &Backend;

			ID_ = BKDACC_MASTER_OBJECT;			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "MBDTest";;
			CommandDetail.Casts.Append( Parameters + 0, 1 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Backend->GetCommands( BKDACC_MASTER_TYPE, CommandsDetails, Commands );
			Commands.Recall( 0, 1, this->Commands );
		}
		bso::bool__ MBDTest( void ) const
		{
			Backend->PushHeader( ID_, Commands[0] );

			Backend->EndOfInParameters();


			return Backend->Handle();
		}
	};

	class mbd_manager_common__
	{
	private:
		bkdacc::id16__ ID_;
	public:
		bkdacc::backend_access___ *Backend;
		bkdacc::command__ Commands[18];
		void Init( bkdacc::backend_access___ &Backend )
		{
			bkdacc::commands_details CommandsDetails;
			bkdacc::command_detail CommandDetail;
			bkdacc::ids16 Commands;

			bkdacc::id8__ Parameters[] = {
				18,18,18,0, 
				0, 19,
				18,0, 
				0, 
				0, 18,18,
				14,18,18,0, 14,
				14,0, 
				0, 15,
				15,0, 19,19,9,
				14,0, 15,
				15,0, 19,19,9,15,
				14,14,18,18,0, 14,
				14,0, 
				0, 26,
				11,14,26,0, 11,
				11,14,0, 
				14,0, 12,
				14,15,12,0, 20,
			};

			this->Backend = &Backend;

			ID_ = this->Backend->GetType( str::string( "manager" ) );

			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "CreateDatabase";;
			CommandDetail.Casts.Append( Parameters + 0, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetAvailableDatabases";;
			CommandDetail.Casts.Append( Parameters + 4, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OpenDatabase";;
			CommandDetail.Casts.Append( Parameters + 6, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "CloseDatabase";;
			CommandDetail.Casts.Append( Parameters + 8, 1 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetDatabaseInfos";;
			CommandDetail.Casts.Append( Parameters + 9, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "CreateOrModifyTable";;
			CommandDetail.Casts.Append( Parameters + 12, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "DeleteTable";;
			CommandDetail.Casts.Append( Parameters + 17, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetTables";;
			CommandDetail.Casts.Append( Parameters + 19, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetTablesInfos";;
			CommandDetail.Casts.Append( Parameters + 21, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetFields";;
			CommandDetail.Casts.Append( Parameters + 26, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetFieldsInfos";;
			CommandDetail.Casts.Append( Parameters + 29, 6 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "AddOrModifyField";;
			CommandDetail.Casts.Append( Parameters + 35, 6 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "DeleteField";;
			CommandDetail.Casts.Append( Parameters + 41, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetFields";;
			CommandDetail.Casts.Append( Parameters + 43, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "InsertOrModifyRecord";;
			CommandDetail.Casts.Append( Parameters + 45, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "DeleteRecord";;
			CommandDetail.Casts.Append( Parameters + 50, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetRecords";;
			CommandDetail.Casts.Append( Parameters + 53, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetRecordsData";;
			CommandDetail.Casts.Append( Parameters + 56, 5 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Backend->GetCommands( ID_, CommandsDetails, Commands );
			Commands.Recall( 0, 18, this->Commands );
		}
		bkdacc::object__ GetNewObject( void )
		{
			return Backend->GetNewObject( ID_ );
		}
		void RemoveObject( bkdacc::object__ Object)
		{
			Backend->RemoveObject( Object );
		}
	};

	class mbd_manager___
	{
	private:
		bkdacc::object__ ID_;
		mbd_manager_common__ *Common_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P == true )
				if ( ( Common_ != NULL ) && ( ID_ != BKDACC_UNDEFINED_OBJECT ) )
					Common_->RemoveObject( ID_ );

			Common_ = NULL;
			ID_ = BKDACC_UNDEFINED_OBJECT;
		}
		mbd_manager___( void )
		{
			reset( false );
		}
		~mbd_manager___( void )
		{
			reset();
		}
		bkdacc::object__ ID( void ) const
		{
			return ID_;
		}
		void Init( mbd_manager_common__ &Common )
		{
			Common_ = &Common;

			ID_ = Common_->GetNewObject();
		}
		bso::bool__ CreateDatabase( 
			const bkdacc::string_ &In1,
			const bkdacc::string_ &In2,
			const bkdacc::string_ &In3 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[0] );
			Common_->Backend->StringIn( In1 );
			Common_->Backend->StringIn( In2 );
			Common_->Backend->StringIn( In3 );

			Common_->Backend->EndOfInParameters();


			return Common_->Backend->Handle();
		}
		bso::bool__ GetAvailableDatabases( 
			bkdacc::strings_ &Out1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[1] );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->StringsOut( Out1 );

			return Common_->Backend->Handle();
		}
		bso::bool__ OpenDatabase( 
			const bkdacc::string_ &In1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[2] );
			Common_->Backend->StringIn( In1 );

			Common_->Backend->EndOfInParameters();


			return Common_->Backend->Handle();
		}
		bso::bool__ CloseDatabase( void ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[3] );

			Common_->Backend->EndOfInParameters();


			return Common_->Backend->Handle();
		}
		bso::bool__ GetDatabaseInfos( 
			bkdacc::string_ &Out1,
			bkdacc::string_ &Out2 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[4] );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->StringOut( Out1 );
			Common_->Backend->StringOut( Out2 );

			return Common_->Backend->Handle();
		}
		bso::bool__ CreateOrModifyTable( 
			const bkdacc::id32__ &In1,
			const bkdacc::string_ &In2,
			const bkdacc::string_ &In3,
			bkdacc::id32__ &Out1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[5] );
			Common_->Backend->Id32In( In1 );
			Common_->Backend->StringIn( In2 );
			Common_->Backend->StringIn( In3 );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->Id32Out( Out1 );

			return Common_->Backend->Handle();
		}
		bso::bool__ DeleteTable( 
			const bkdacc::id32__ &In1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[6] );
			Common_->Backend->Id32In( In1 );

			Common_->Backend->EndOfInParameters();


			return Common_->Backend->Handle();
		}
		bso::bool__ GetTables( 
			bkdacc::ids32_ &Out1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[7] );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->Ids32Out( Out1 );

			return Common_->Backend->Handle();
		}
		bso::bool__ GetTablesInfos( 
			const bkdacc::ids32_ &In1,
			bkdacc::strings_ &Out1,
			bkdacc::strings_ &Out2,
			bkdacc::ids8_ &Out3 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[8] );
			Common_->Backend->Ids32In( In1 );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->StringsOut( Out1 );
			Common_->Backend->StringsOut( Out2 );
			Common_->Backend->Ids8Out( Out3 );

			return Common_->Backend->Handle();
		}
		bso::bool__ GetFields( 
			const bkdacc::id32__ &In1,
			bkdacc::ids32_ &Out1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[9] );
			Common_->Backend->Id32In( In1 );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->Ids32Out( Out1 );

			return Common_->Backend->Handle();
		}
		bso::bool__ GetFieldsInfos( 
			const bkdacc::ids32_ &In1,
			bkdacc::strings_ &Out1,
			bkdacc::strings_ &Out2,
			bkdacc::ids8_ &Out3,
			bkdacc::ids32_ &Out4 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[10] );
			Common_->Backend->Ids32In( In1 );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->StringsOut( Out1 );
			Common_->Backend->StringsOut( Out2 );
			Common_->Backend->Ids8Out( Out3 );
			Common_->Backend->Ids32Out( Out4 );

			return Common_->Backend->Handle();
		}
		bso::bool__ AddOrModifyField( 
			const bkdacc::id32__ &In1,
			const bkdacc::id32__ &In2,
			const bkdacc::string_ &In3,
			const bkdacc::string_ &In4,
			bkdacc::id32__ &Out1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[11] );
			Common_->Backend->Id32In( In1 );
			Common_->Backend->Id32In( In2 );
			Common_->Backend->StringIn( In3 );
			Common_->Backend->StringIn( In4 );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->Id32Out( Out1 );

			return Common_->Backend->Handle();
		}
		bso::bool__ DeleteField( 
			const bkdacc::id32__ &In1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[12] );
			Common_->Backend->Id32In( In1 );

			Common_->Backend->EndOfInParameters();


			return Common_->Backend->Handle();
		}
		bso::bool__ GetFields( 
			bkdacc::items32_ &Out1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[13] );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->Items32Out( Out1 );

			return Common_->Backend->Handle();
		}
		bso::bool__ InsertOrModifyRecord( 
			const bkdacc::id16__ &In1,
			const bkdacc::id32__ &In2,
			const bkdacc::items32_ &In3,
			bkdacc::id16__ &Out1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[14] );
			Common_->Backend->Id16In( In1 );
			Common_->Backend->Id32In( In2 );
			Common_->Backend->Items32In( In3 );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->Id16Out( Out1 );

			return Common_->Backend->Handle();
		}
		bso::bool__ DeleteRecord( 
			const bkdacc::id16__ &In1,
			const bkdacc::id32__ &In2 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[15] );
			Common_->Backend->Id16In( In1 );
			Common_->Backend->Id32In( In2 );

			Common_->Backend->EndOfInParameters();


			return Common_->Backend->Handle();
		}
		bso::bool__ GetRecords( 
			const bkdacc::id32__ &In1,
			bkdacc::ids16_ &Out1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[16] );
			Common_->Backend->Id32In( In1 );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->Ids16Out( Out1 );

			return Common_->Backend->Handle();
		}
		bso::bool__ GetRecordsData( 
			const bkdacc::id32__ &In1,
			const bkdacc::ids32_ &In2,
			const bkdacc::ids16_ &In3,
			bkdacc::xstrings_ &Out1 ) const
		{
			Common_->Backend->PushHeader( ID_, Common_->Commands[17] );
			Common_->Backend->Id32In( In1 );
			Common_->Backend->Ids32In( In2 );
			Common_->Backend->Ids16In( In3 );

			Common_->Backend->EndOfInParameters();

			Common_->Backend->XStringsOut( Out1 );

			return Common_->Backend->Handle();
		}
	};

}

#endif //#ifndef emobda__INC
