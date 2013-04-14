/* DON'T MODIFY : computer-generated file ! */
/*
	API from : esketchbkd V0.1.0 (Build : Apr 14 2013 11:05:58 MSVC_x86)
	This file was generated using getbkdapi V0.2.4 (Build Apr 14 2013 11:21:18 (MSVC_x86)),
	and a XSL file (CVS features : $Id$).
*/

#ifndef ESKETCH__INC
# define ESKETCH__INC

# define ESKETCH_TARGET "esketch"
# define ESKETCH_API_VERSION "1"

# include "fblfrd.h"
# include "cpe.h"

namespace esketch {

	class statics___
	{
	private:
		fblfrd::object__ _ID;
		fblfrd::backend_access___ *_Backend;
		fblfrd::command__ _Commands[1];
	public:
		void reset( bso::bool__ = true )
		{
			_Backend = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( statics___ )
		void Init( fblfrd::backend_access___ &Backend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8_t__ Parameters[] = {
				0, 
			};

			_Backend = &Backend;

			_ID = FBLFRD_MASTER_OBJECT;
			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "SKTTest";;
			CommandDetail.Casts.Append( Parameters + 0, 1 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			_Backend->GetCommands( FBLFRD_MASTER_TYPE, CommandsDetails, Commands );
			Commands.Recall( 0, 1, _Commands );
		}
		fblovl::reply__ SKTTest( void ) const
		{
			Backend().PushHeader( _ID, Commands()[0] );

			Backend().EndOfInParameters();


			return Backend().Handle();
		}
		fblfrd::backend_access___ &Backend( void ) const
		{
			return *_Backend;
		}
		const fblfrd::command__ *Commands( void ) const
		{
			return _Commands;
		}
	};

	class skt_myobject_common__
	{
	private:
		fblfrd::id16__ _ID;
		fblfrd::backend_access___ *_Backend;
		fblfrd::command__ _Commands[1];
	public:
		void reset( bso::bool__ = true )
		{
			_Backend = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( skt_myobject_common__ )
		void Init( fblfrd::backend_access___ &Backend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8_t__ Parameters[] = {
				0, 
			};

			_Backend = &Backend;

			_ID = _Backend->GetType( str::string( "myobject" ) );

			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "Test";;
			CommandDetail.Casts.Append( Parameters + 0, 1 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			_Backend->GetCommands( _ID, CommandsDetails, Commands );
			Commands.Recall( 0, 1, _Commands );
		}
		fblfrd::object__ GetNewObject( void )
		{
			return _Backend->GetNewObject( _ID );
		}
		void RemoveObject( fblfrd::object__ Object )
		{
			_Backend->RemoveObject( Object );
		}
		fblfrd::backend_access___ &Backend( void ) const
		{
			return *_Backend;
		}
		const fblfrd::command__ *Commands( void ) const
		{
			return _Commands;
		}
	};

	class skt_myobject___
	{
	private:
		fblfrd::object__ _ID;
		skt_myobject_common__ *Common_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P == true )
				if ( ( Common_ != NULL ) && ( _ID != FBLFRD_UNDEFINED_OBJECT ) )
					Common_->RemoveObject( _ID );

			Common_ = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		skt_myobject___( void )
		{
			reset( false );
		}
		~skt_myobject___( void )
		{
			reset();
		}
		fblfrd::object__ ID( void ) const
		{
			return _ID;
		}
		void Init( skt_myobject_common__ &Common )
		{
			Common_ = &Common;

			_ID = Common_->GetNewObject();
		}
		fblovl::reply__ Test( void ) const
		{
			Common_->Backend().PushHeader( _ID, Common_->Commands()[0] );

			Common_->Backend().EndOfInParameters();


			return Common_->Backend().Handle();
		}
	};

}

#endif //#ifndef esketch__INC
