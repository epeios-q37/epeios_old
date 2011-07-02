/*
	Header for the 'fblfrd' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef FBLFRD__INC
#define FBLFRD__INC

#define FBLFRD_NAME		"FBLFRD"

#define	FBLFRD_VERSION	"$Revision$"

#define FBLFRD_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLFRDTutor;

#if defined( XXX_DBG ) && !defined( FBLFRD_NODBG )
#define FBLFRD_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D Frontend/Backend Layout FrontEnD 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "fbltyp.h"
# include "fblcst.h"
# include "fblrpl.h"
# include "fblcmd.h"

# define FBLFRD_UNDEFINED_CAST		255
# define FBLFRD_UNDEFINED_TYPE		65535
# define FBLFRD_UNDEFINED_OBJECT		65535
# define FBLFRD_UNDEFINED_COMMAND	65535

# define FBLFRD_UNDEFINED_ID32		FBLTYP_UNDEFINED_ID32
# define FBLFRD_UNDEFINED_ID16		FBLTYP_UNDEFINED_ID16
# define FBLFRD_UNDEFINED_ID8		FBLTYP_UNDEFINED_ID8

# define	FBLFRD_MASTER_OBJECT	FBLFRD_UNDEFINED_OBJECT
# define FBLFRD_MASTER_TYPE		FBLFRD_UNDEFINED_TYPE
# define FBLFRD_MASTER_COMMAND	FBLFRD_UNDEFINED_COMMAND

# define FBLFRD__T( name, type, bunch, container )\
	E_TYPEDEF( type, name##__ );\
	typedef bch::E_BUNCH_( name##__ )	name##s_;\
	E_AUTO( name##s )\
	typedef ctn::E_XMCONTAINER_( name##s_ )	x##name##s_;\
	E_AUTO( x##name##s )\
	inline bunch &_( name##s_ &O )\
	{\
		return *(bunch *)&O;\
	}\
	inline const bunch &_( const name##s_ &O )\
	{\
		return *(bunch *)&O;\
	}\
	inline container &_( x##name##s_ &O )\
	{\
		return *(container *)&O;\
	}\
	inline const container &_( const x##name##s_ &O )\
	{\
		return *(container *)&O;\
	}

# define FBLFRD_T32( name )	FBLFRD__T( name, fblfrd::id32__, fblfrd::ids32_, fblfrd::xids32_ )
# define FBLFRD_T16( name )	FBLFRD__T( name, fblfrd::id16__, fblfrd::ids16_, fblfrd::xids16_ )
# define FBLFRD_T8( name )	FBLFRD__T( name, fblfrd::id8__, fblfrd::ids8_, fblfrd::xids8_ )

# define FBLFRD_M( name, type )\
	void name##In( const fbltyp::type &O )\
	{\
		_In( fblcst::c##name, &O );\
	}\
	void name##Out( fbltyp::type &O )\
	{\
		_Out( fblcst::c##name, &O );\
	}\



namespace fblfrd {
	using namespace fbltyp;
		typedef fbltyp::id16__ command__;
		typedef fbltyp::id16__ type__;


	struct parameters_handling_functions__
	{
	protected:
		virtual void FBLFRDIn(
			fblcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow ) = 0;
		virtual void FBLFRDOut(
			flw::ioflow__ &Flow,
			fblcst::cast Cast,
			void *Pointer ) = 0;
		virtual void FBLFRDPostProcess( flw::ioflow__ &Flow ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// A des fins de standadisation.
		}
		parameters_handling_functions__( void )
		{
			reset( false );
		}
		virtual ~parameters_handling_functions__( void )
		{
			reset();
		}
		void Init( void )
		{
			// A des fins de standadisation.
		}
		void In(
			fblcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			FBLFRDIn( Cast, Pointer, Flow );
		}
		void Out(
			flw::ioflow__ &Flow,
			fblcst::cast Cast,
			void *Pointer )
		{
			FBLFRDOut( Flow, Cast, Pointer );
		}
		void PostProcess( flw::ioflow__ &Flow )
		{
			FBLFRDPostProcess( Flow );
		}
	};

	class error_handling_functions__
	{
	protected:
		virtual void FBLFRDHandleError(
			fblrpl::reply__ Reply,
			const char *Message ) = 0;
	public:
		void reset ( bso::bool__ = true )
		{
			// Standardisation.
		}
		error_handling_functions__( void )
		{
			reset( false );
		}
		~error_handling_functions__( void )
		{
			reset();
		}
		void Init( void )
		{
			// Standardisation.
		}
		void HandleError(
			fblrpl::reply__ Reply,
			const char *Message )
		{
			FBLFRDHandleError( Reply, Message );
		}
	};

	//c The backend access core.
	class backend_access___
	{
	private:
		parameters_handling_functions__ *_ParametersHandlingFunctions;
		error_handling_functions__ *_ErrorHandlingFunctions;
		void _In(
			fblcst::cast Cast,
			const void *Pointer )
		{
			_ParametersHandlingFunctions->In( Cast, Pointer, *Channel_ );
		}
		void _Out(
			fblcst::cast Cast,
			void *Pointer )
		{
			_ParametersHandlingFunctions->Out( *Channel_, Cast, Pointer );
		}
		void _PostProcess( flw::ioflow__ &Flow )
		{
			_ParametersHandlingFunctions->PostProcess( Flow );
		}
		void _HandleError(
			fblrpl::reply__ Reply,
			const char *Message )
		{
			_ErrorHandlingFunctions->HandleError( Reply, Message );
		}
		id16__ Commands_[fblcmd::c_amount];
		char Message_[100];
		flw::ioflow__ *Channel_;
		bso::bool__ TestBackendCasts_( void );
		command__ GetBackendDefaultCommand_( void );
		void GetGetCommandsCommand_( command__ DefaultCommand );
		void GetBackendCommands_( void );
		void RetrieveBackendCommands_( void )
		{
				command__ DefaultCommand;

				if ( !TestBackendCasts_() )
					ERRb();

				DefaultCommand = GetBackendDefaultCommand_();

				GetGetCommandsCommand_( DefaultCommand );

				GetBackendCommands_();
		}
		void Internal_( fblcmd::command Command )
		{
			PushHeader( FBLFRD_MASTER_OBJECT, Commands_[Command] );
		}
		void TestInput_( fblcst::cast Cast )
		{
			Channel_->Put( Cast );
		}
		void _Handle( void )
		{
			if ( Handle() != fblrpl::rOK )
				ERRu();
		}
		fblrpl::reply__ _Send( void )
		{
			fblrpl::reply__ Reply = fblrpl::r_Undefined;

			Channel_->Commit();

			if ( ( Reply = (fblrpl::reply__)Channel_->Get() ) != fblrpl::rOK ) {
				if ( Reply >= fblrpl::r_amount )
					ERRb();

				if ( ( !flw::GetString( *Channel_, Message_, sizeof( Message_ ) ) ) )
					ERRl();

				_HandleError( Reply, Message_ );
			}

			return Reply;
		}
		void _SendAndTest( void )
		{
			if ( _Send() != fblrpl::rOK )
				ERRb();
		}
		bso::bool__ _TestCompatibility(
			const char *Language,
			const char *BackendLabel,
			const char *APIVersion,
			flw::ioflow__ &Flow,
			str::string_ &Message,
			str::string_ &URL );
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Channel_ != NULL )
					Disconnect();
			}
				
			_ParametersHandlingFunctions = NULL;
			_ErrorHandlingFunctions = NULL;
			Channel_ = NULL;
		}
		backend_access___( void )
		{
			reset( false );
		}
		virtual ~backend_access___( void )
		{
			reset( true );
		}
		//f Initialization with 'Channel' to parse/answer the request.
		bso::bool__ Init(
			const char *Language,
			const char *BackendLabel,
			const char *APIVersion,
			flw::ioflow__ &Channel,
			parameters_handling_functions__ &ParametersHandlingFunctions,
			error_handling_functions__ &ErrorHandlingFunctions,
			str::string_ &Message,
			str::string_ &URL )
		{
			bso::bool__ Success = true;
		ERRProlog
		ERRBegin
			reset();

			if ( !_TestCompatibility( Language, BackendLabel, APIVersion, Channel, Message, URL ) ) {
				Success = false;
				ERRReturn;
			}

			Channel_ = &Channel;
			_ParametersHandlingFunctions = &ParametersHandlingFunctions;
			_ErrorHandlingFunctions = &ErrorHandlingFunctions;

			RetrieveBackendCommands_();
		ERRErr
			Channel_ = NULL;	// Pour éviter toute future tentative de communication avec le backend.
		ERREnd
		ERREpilog
			return Success;
		}
		//f Add header with object 'Object' and command 'Command'.
		void PushHeader(
			object__ Object,
			command__ Command )
		{
			flw::Put( Object, *Channel_ );
			flw::Put( Command, *Channel_ );
		}
		//f Put 'Object'.
		FBLFRD_M( Object, object__)
		FBLFRD_M( Boolean, boolean__ )
		FBLFRD_M( Booleans, booleans_ )
		FBLFRD_M( SLong, slong__ )
		FBLFRD_M( SLongs, slongs_ )
		FBLFRD_M( ULong, ulong__ )
		FBLFRD_M( ULongs, ulongs_ )
		FBLFRD_M( Id8, id8__ )
		FBLFRD_M( Ids8, ids8_ )
		FBLFRD_M( XIds8, xids8_ )
		FBLFRD_M( Id16, id16__ )
		FBLFRD_M( Ids16, ids16_ )
		FBLFRD_M( XIds16, xids16_ )
		FBLFRD_M( Id32, id32__ )
		FBLFRD_M( Ids32, ids32_ )
		FBLFRD_M( XIds32, xids32_ )
		FBLFRD_M( Char, char__ )
		FBLFRD_M( String, string_ )
		FBLFRD_M( Strings, strings_ )
		FBLFRD_M( XStrings, xstrings_ )
		FBLFRD_M( Byte, byte__ )
		FBLFRD_M( Binary, binary_ )
		FBLFRD_M( Binaries, binaries_ )
		FBLFRD_M( Items8, items8_ )
		FBLFRD_M( Items16, items16_ )
		FBLFRD_M( Items32, items32_ )
		FBLFRD_M( XItems8, xitems8_ )
		FBLFRD_M( XItems16, xitems16_ )
		FBLFRD_M( XItems32, xitems32_ )
		FBLFRD_M( CommandsDetails, commands_details_ )
		FBLFRD_M( ObjectsReferences, objects_references_ )
		void EndOfInParameters( void )
		{
			Channel_->Put( 0 );	// End of request
		}
		//f Send the request.
		fblrpl::reply__ Handle( void )
		{
			fblrpl::reply__  Reply = _Send();

			if ( Reply == fblrpl::rOK )
				_PostProcess( *Channel_ );

			if ( Channel_->Get() != fblcst::cEnd )
				ERRb();

			Channel_->Dismiss();

			return Reply;
		}
		//f Return the explanation messag, if any.
		const char *GetMessage( void ) const
		{
			return Message_;
		}
		//f Return the channel used to handle the request as input flow.
		flw::iflow__ &Input( void )
		{
			return *Channel_;
		}
		//f Return the channel used to handle the request as ouput flow.
		flw::oflow__ &Output( void )
		{
			return *Channel_;
		}
		//f Throw an user error, for testing purpose.
		fblrpl::reply__ ThrowUError( void )
		{
			Internal_( fblcmd::cThrowUError );

			EndOfInParameters();

			return Handle();	// NOTA : Always to 'true'.
		}
		//f Throw an intentional error, for testing purpose.
		fblrpl::reply__ ThrowIError( void )
		{
			Internal_( fblcmd::cThrowIError );

			EndOfInParameters();

			return  Handle();	// NOTA : Always to 'true'.
		}
		// Génère une erreur utilisateur retournant le messahe 'Message'.
		fblrpl::reply__ ThrowUserDefinedError( const string_ &Message )
		{
			Internal_( fblcmd::cThrowUserDefinedError );

			StringIn( Message );

			EndOfInParameters();

			return  Handle();	// NOTA : Always to 'true'.
		}
		//f Return the id of a new object of type 'Type'.
		object__ GetNewObject( type__ Type )
		{
			object__ O;

			Internal_( fblcmd::cGetNewObject );

			Id16In( Type );

			EndOfInParameters();

			ObjectOut( O );

			_Handle();

			return O;
		}
		//f Return the type id. of type 'TypeName'.
		type__ GetType( const string_ &TypeName )
		{
			type__ Type;

			Internal_( fblcmd::cGetType );

			StringIn( TypeName );

			EndOfInParameters();

			Id16Out( Type );

			_Handle();

			return Type;
		}
#if 0
		//f Put in 'RawMessages' all the raw messages from backend.
		void GetRawMessages( strings_ &RawMessages )
		{
			Internal_( fblcmd::cGetRawMessages );

			EndOfInParameters();

			StringsOut( RawMessages );

			_Handle();
		}
#endif
		//f Return the command of object type 'Type', named 'Name' with parameter 'Parameters'.
		command__ GetCommand(
			type__ Type,
			const string_ &Name,
			const ids8_ &Parameters )
		{
			command__ Command;

			Internal_( fblcmd::cGetCommand );

			Id16In( Type );
			StringIn( Name );
			Ids8In( Parameters );

			EndOfInParameters();

			Id16Out( Command );

			_Handle();

			return Command;
		}
		/*f Put in 'Commands' commands from object of type 'Type',
		whith names 'Names' and casts 'Casts'. */
		void GetCommands(
			type__ Type,
			const commands_details_ &CommandDetails,
			ids16_ &Commands )
		{
			Internal_( fblcmd::cGetCommands );

			Id16In( Type );
			CommandsDetailsIn( CommandDetails );

			EndOfInParameters();

			Ids16Out( Commands );

			_Handle();
		}
		//f Remove object 'Object'.
		void RemoveObject( object__ Object )
		{
			Internal_( fblcmd::cRemoveObject );

			ObjectIn( Object );

			EndOfInParameters();

			_Handle();
		}
		void About(
			string_ &ProtocolVersion,
			string_ &BackendLabel,
			string_ &APIVersion,
			string_ &Backend,
			string_ &Publisher)
		{
			Internal_( fblcmd::cAbout );

			EndOfInParameters();

			StringOut( ProtocolVersion );
			StringOut( BackendLabel );
			StringOut( APIVersion );
			StringOut( Backend );
			StringOut( Publisher );

			_Handle();
		}
		//f Disconnection.
		void Disconnect( void )
		{
			Internal_( fblcmd::cDisconnect );

			EndOfInParameters();

			_Handle();
			
			Channel_ = NULL;
		}
		bso::bool__ IsConnected( void ) const
		{
			return Channel_ != NULL;
		}
		//f Put in 'TypeXItems' the types prefix, name and id.
		void GetTypesIDAndPrefixAndName( xitems16_ &TypeXItems )
		{
			Internal_( fblcmd::cGetTypesIDAndPrefixAndName );

			EndOfInParameters();

			XItems16Out( TypeXItems );

			_Handle();
		}
		//f Put in 'Items' the commands name and id of an object of type 'Type'.
		void GetCommandsIDAndName(
			type__ Type,
			items16_ &CommandItems )
		{
			Internal_( fblcmd::cGetCommandsIDAndName );

			Id16In( Type );

			EndOfInParameters();

			Items16Out( CommandItems );

			_Handle();
		}
   //f Return the current language. 	 
		const string_ &GetLanguage( string_ &Language ) 	 
        { 	 
			Internal_( fblcmd::cGetLanguage ); 	 

			EndOfInParameters(); 	 

			StringOut( Language ); 	 

			_Handle(); 

			return Language;
        } 	 
         //f Set language to language of id 'Language'; 	 
		void SetLanguage( const string_ &Language ) 	 
		{ 	 
			Internal_( fblcmd::cSetLanguage ); 	 

			StringIn( Language ); 	 

			EndOfInParameters(); 	 

			_Handle(); 	 
		}
  		//f Put in 'Parameters' parameters of command 'Command' from object type 'Type'.
		void GetParameters(
			type__ Type,
			command__ Command,
			ids8_ &Parameters )
		{
			Internal_( fblcmd::cGetParameters );
			Id16In( Type );
			Id16In( Command );

			EndOfInParameters();

			Ids8Out( Parameters );

			_Handle();
		}
/*		void In(
			fblcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			_In( Cast, Pointer, Flow );
		}
		void Out(
			flw::ioflow__ &Flow,
			fblcst::cast Cast,
			void *Pointer )
		{
			_Out( Flow, Cast, Pointer );
		}
		*/
/*		fblrpl::reply__ Handle(
			fblrpl::reply__ Reply,
			flw::ioflow__ &Flow )
		{
			return _Handle( Reply, Flow );
		}
*/
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
