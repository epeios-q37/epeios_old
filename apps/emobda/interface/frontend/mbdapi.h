/* DON'T MODIFY */
/* This file was generated using getbkdapi	V0.2.2, and a XSL filea
($RCSfile$ $Revision$ $Date$ $Author$) */
// $Id$	(CVS feature).
#ifndef emobda__INC
#define emobda__INC

#include "bkdacc.h"
#include "cpe.h"

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
	MANAGER_IncorrectTableName,
	MANAGER_UnableToCreateTable,
	MANAGER_NoSuchTable,
	MANAGER_IncorrectFieldName,
	MANAGER_NameAlreadyUsed,
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

	if ( !strcmp( Message, "MANAGER_IncorrectTableName" ) )
		return MANAGER_IncorrectTableName;

	if ( !strcmp( Message, "MANAGER_UnableToCreateTable" ) )
		return MANAGER_UnableToCreateTable;

	if ( !strcmp( Message, "MANAGER_NoSuchTable" ) )
		return MANAGER_NoSuchTable;

	if ( !strcmp( Message, "MANAGER_IncorrectFieldName" ) )
		return MANAGER_IncorrectFieldName;

	if ( !strcmp( Message, "MANAGER_NameAlreadyUsed" ) )
		return MANAGER_NameAlreadyUsed;

	ERRb();
	return (message__)0;	// To avoid a warning.
}

template <typename backend_access> class emobda_backend_access___
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

		if ( Messages.Amount() != 14 )
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


		if ( Message( Row ) != bkdacc::string( "MANAGER_IncorrectTableName" ) )
			ERRb();

		Row = Messages.Next( Row );


		if ( Message( Row ) != bkdacc::string( "MANAGER_UnableToCreateTable" ) )
			ERRb();

		Row = Messages.Next( Row );


		if ( Message( Row ) != bkdacc::string( "MANAGER_NoSuchTable" ) )
			ERRb();

		Row = Messages.Next( Row );


		if ( Message( Row ) != bkdacc::string( "MANAGER_IncorrectFieldName" ) )
			ERRb();

		Row = Messages.Next( Row );


		if ( Message( Row ) != bkdacc::string( "MANAGER_NameAlreadyUsed" ) )
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

		ID_ = BKDACC_MASTER_OBJECT;		CommandsDetails.Init();

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
	bkdacc::command__ Commands[6];
	void Init( bkdacc::backend_access___ &Backend )
	{
		bkdacc::commands_details CommandsDetails;
		bkdacc::command_detail CommandDetail;
		bkdacc::ids16 Commands;

		bkdacc::id8__ Parameters[] = {
			18,18,18,0, 
			0, 18,
			18,18,0, 14,
			0, 15,19,19,12,
			14,18,18,0, 14,
			0, 26,
		};

		this->Backend = &Backend;

		ID_ = this->Backend->GetType( str::string( "manager" ) );

		CommandsDetails.Init();

		CommandDetail.Init();
		CommandDetail.Name = "CreateDatabase";;
		CommandDetail.Casts.Append( Parameters + 0, 4 );
		CommandsDetails.Append( CommandDetail );

		CommandDetail.Init();
		CommandDetail.Name = "GetDatabaseInfos";;
		CommandDetail.Casts.Append( Parameters + 4, 2 );
		CommandsDetails.Append( CommandDetail );

		CommandDetail.Init();
		CommandDetail.Name = "CreateTable";;
		CommandDetail.Casts.Append( Parameters + 6, 4 );
		CommandsDetails.Append( CommandDetail );

		CommandDetail.Init();
		CommandDetail.Name = "GetTables";;
		CommandDetail.Casts.Append( Parameters + 10, 5 );
		CommandsDetails.Append( CommandDetail );

		CommandDetail.Init();
		CommandDetail.Name = "AddField";;
		CommandDetail.Casts.Append( Parameters + 15, 5 );
		CommandsDetails.Append( CommandDetail );

		CommandDetail.Init();
		CommandDetail.Name = "GetFields";;
		CommandDetail.Casts.Append( Parameters + 20, 2 );
		CommandsDetails.Append( CommandDetail );


		Commands.Init();
		this->Backend->GetCommands( ID_, CommandsDetails, Commands );
		Commands.Recall( 0, 6, this->Commands );
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
	bso::bool__ GetDatabaseInfos( 
		bkdacc::string_ &Out1 ) const
	{
		Common_->Backend->PushHeader( ID_, Common_->Commands[1] );

		Common_->Backend->EndOfInParameters();

		Common_->Backend->StringOut( Out1 );

		return Common_->Backend->Handle();
	}
	bso::bool__ CreateTable( 
		const bkdacc::string_ &In1,
		const bkdacc::string_ &In2,
		bkdacc::id32__ &Out1 ) const
	{
		Common_->Backend->PushHeader( ID_, Common_->Commands[2] );
		Common_->Backend->StringIn( In1 );
		Common_->Backend->StringIn( In2 );

		Common_->Backend->EndOfInParameters();

		Common_->Backend->Id32Out( Out1 );

		return Common_->Backend->Handle();
	}
	bso::bool__ GetTables( 
		bkdacc::ids32_ &Out1,
		bkdacc::strings_ &Out2,
		bkdacc::strings_ &Out3,
		bkdacc::ids16_ &Out4 ) const
	{
		Common_->Backend->PushHeader( ID_, Common_->Commands[3] );

		Common_->Backend->EndOfInParameters();

		Common_->Backend->Ids32Out( Out1 );
		Common_->Backend->StringsOut( Out2 );
		Common_->Backend->StringsOut( Out3 );
		Common_->Backend->Ids16Out( Out4 );

		return Common_->Backend->Handle();
	}
	bso::bool__ AddField( 
		const bkdacc::id32__ &In1,
		const bkdacc::string_ &In2,
		const bkdacc::string_ &In3,
		bkdacc::id32__ &Out1 ) const
	{
		Common_->Backend->PushHeader( ID_, Common_->Commands[4] );
		Common_->Backend->Id32In( In1 );
		Common_->Backend->StringIn( In2 );
		Common_->Backend->StringIn( In3 );

		Common_->Backend->EndOfInParameters();

		Common_->Backend->Id32Out( Out1 );

		return Common_->Backend->Handle();
	}
	bso::bool__ GetFields( 
		bkdacc::items32_ &Out1 ) const
	{
		Common_->Backend->PushHeader( ID_, Common_->Commands[5] );

		Common_->Backend->EndOfInParameters();

		Common_->Backend->Items32Out( Out1 );

		return Common_->Backend->Handle();
	}
};

#endif //#ifndef emobda__INC
