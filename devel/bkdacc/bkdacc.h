/*
	Header for the 'bkdacc' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#ifndef BKDACC__INC
#define BKDACC__INC

#define BKDACC_NAME		"BKDACC"

#define	BKDACC_VERSION	"$Revision$"

#define BKDACC_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &BKDACCTutor;

#if defined( XXX_DBG ) && !defined( BKDACC_NODBG )
#define BKDACC_DBG
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
//D BacKenD ACCess 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D BacKenD ACCess.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "bkdtpm.h"
#include "bkdcst.h"
#include "bkdcmd.h"
#include "bkdlcl.h"
#include "bso.h"
#include "str.h"
#include "ctn.h"

#define BKDACC_UNDEFINED_CAST		255
#define BKDACC_UNDEFINED_TYPE		65535
#define BKDACC_UNDEFINED_OBJECT		65535
#define BKDACC_UNDEFINED_COMMAND	65535

#define BKDACC_UNDEFINED_ID32		BKDTPM_UNDEFINED_ID32
#define BKDACC_UNDEFINED_ID16		BKDTPM_UNDEFINED_ID16
#define BKDACC_UNDEFINED_ID8		BKDTPM_UNDEFINED_ID8

#define	BKDACC_MASTER_OBJECT	BKDACC_UNDEFINED_OBJECT
#define BKDACC_MASTER_TYPE		BKDACC_UNDEFINED_TYPE
#define BKDACC_MASTER_COMMAND	BKDACC_UNDEFINED_COMMAND

#define BKDACC__T( name, type, bunch, container )\
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

#define BKDACC_T32( name )	BKDACC__T( name, bkdacc::id32__, bkdacc::ids32_, bkdacc::xids32_ )
#define BKDACC_T16( name )	BKDACC__T( name, bkdacc::id16__, bkdacc::ids16_, bkdacc::xids16_ )
#define BKDACC_T8( name )	BKDACC__T( name, bkdacc::id8__, bkdacc::ids8_, bkdacc::xids8_ )


namespace bkdacc {
	using namespace bkdtpm;
	typedef bkdtpm::id16__ command__;
	typedef bkdtpm::id16__ type__;

#define BKDACC_M( name, type )\
	void name##In( const bkdtpm::type &O )\
	{\
		_In( bkdcst::c##name, &O );\
	}\
	void name##Out( bkdtpm::type &O )\
	{\
		_Out( bkdcst::c##name, &O );\
	}\

	struct backend_access_functions__
	{
	protected:
		virtual void BKDACCIn(
			bkdcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow ) = 0;
		virtual void BKDACCOut(
			flw::ioflow__ &Flow,
			bkdcst::cast Cast,
			void *Pointer ) = 0;
		virtual bso::bool__ BKDACCHandle(
			bso::bool__ Success,
			flw::ioflow__ &Flow ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// A des fins de standadisation.
		}
		backend_access_functions__( void )
		{
			reset( false );
		}
		virtual ~backend_access_functions__( void )
		{
			reset();
		}
		void Init( void )
		{
			// A des fins de standadisation.
		}
		void In(
			bkdcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			BKDACCIn( Cast, Pointer, Flow );
		}
		void Out(
			flw::ioflow__ &Flow,
			bkdcst::cast Cast,
			void *Pointer )
		{
			BKDACCOut( Flow, Cast, Pointer );
		}
		bso::bool__ Handle(
			bso::bool__ Success,
			flw::ioflow__ &Flow )
		{
			return BKDACCHandle( Success, Flow );
		}
	};



	//c The backend access core.
	class backend_access___
	{
	private:
		backend_access_functions__ *_Functions;
//		virtual void _In(
		void _In(
			bkdcst::cast Cast,
			const void *Pointer )
		{
			_Functions->In( Cast, Pointer, *Channel_ );
		}
//		virtual void _Out(
		void _Out(
			bkdcst::cast Cast,
			void *Pointer )
		{
			_Functions->Out( *Channel_, Cast, Pointer );
		}
		bso::bool__ _Handle(
			bso::bool__ Success,
			flw::ioflow__ &Flow )
		{
			return _Functions->Handle( Success, Flow );
		}
		id16__ Commands_[bkdcmd::c_amount];
		char RawMessage_[100];
		char I18Message_[100];
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
		void Internal_( bkdcmd::command Command )
		{
			PushHeader( BKDACC_MASTER_OBJECT, Commands_[Command] );
		}
		void TestInput_( bkdcst::cast Cast )
		{
			Channel_->Put( Cast );
		}
		void _Handle( void )
		{
			if ( !Handle() )
				ERRu();
		}
		bso::bool__ _Send( void )
		{
			Channel_->Synchronize();

			if ( ( !flw::GetString( *Channel_, RawMessage_, sizeof( RawMessage_ ) ) ) )
				ERRl();

			if ( RawMessage_[0] == 0 )
				I18Message_[0] = 0;
			else if ( ( !flw::GetString( *Channel_, I18Message_, sizeof( I18Message_ ) ) ) )
				ERRl();

			return RawMessage_[0] == 0;
		}
		void _SendAndTest( void )
		{
			if ( !_Send() )
				ERRb();
		}
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Channel_ != NULL )
					Disconnect();
			}
				
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
		void Init(
			flw::ioflow__ &Channel,
			backend_access_functions__ &Functions )
		{
		ERRProlog
		ERRBegin
			reset();

			Channel_ = &Channel;
			_Functions = &Functions;

			RetrieveBackendCommands_();
		ERRErr
			Channel_ = NULL;	// Pour éviter toute future tentative de communication avec le backend.
		ERREnd
		ERREpilog
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
		BKDACC_M( Object, object__)
		BKDACC_M( Boolean, boolean__ )
		BKDACC_M( Booleans, booleans_ )
		BKDACC_M( SLong, slong__ )
		BKDACC_M( SLongs, slongs_ )
		BKDACC_M( ULong, ulong__ )
		BKDACC_M( ULongs, ulongs_ )
		BKDACC_M( Id8, id8__ )
		BKDACC_M( Ids8, ids8_ )
		BKDACC_M( XIds8, xids8_ )
		BKDACC_M( Id16, id16__ )
		BKDACC_M( Ids16, ids16_ )
		BKDACC_M( XIds16, xids16_ )
		BKDACC_M( Id32, id32__ )
		BKDACC_M( Ids32, ids32_ )
		BKDACC_M( XIds32, xids32_ )
		BKDACC_M( Char, char__ )
		BKDACC_M( String, string_ )
		BKDACC_M( Strings, strings_ )
		BKDACC_M( XStrings, xstrings_ )
		BKDACC_M( Byte, byte__ )
		BKDACC_M( Binary, binary_ )
		BKDACC_M( Binaries, binaries_ )
		BKDACC_M( Items8, items8_ )
		BKDACC_M( Items16, items16_ )
		BKDACC_M( Items32, items32_ )
		BKDACC_M( XItems8, xitems8_ )
		BKDACC_M( XItems16, xitems16_ )
		BKDACC_M( XItems32, xitems32_ )
		BKDACC_M( CommandsDetails, commands_details_ )
		BKDACC_M( ObjectsReferences, objects_references_ )
		void EndOfInParameters( void )
		{
			Channel_->Put( 0 );	// End of request
		}
		//f Send the request.
		bso::bool__ Handle( void )
		{
			bso::bool__ Success = _Handle( _Send(), *Channel_ );

			if ( Channel_->Get() != bkdcst::cEnd )
				ERRb();

			Channel_->Dismiss();

			return Success;
		}
		//f Return the explanation messag, if any.
		const char *GetRawMessage( void ) const
		{
			return RawMessage_;
		}
		const char *GetI18Message( void ) const
		{
			return I18Message_;
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
		bso::bool__ ThrowUError( void )
		{
			Internal_( bkdcmd::cThrowUError );

			EndOfInParameters();

			return Handle();	// NOTA : Always to 'true'.
		}
		//f Throw an intentional error, for testing purpose.
		bso::bool__ ThrowIError( void )
		{
			Internal_( bkdcmd::cThrowIError );

			EndOfInParameters();

			return  Handle();	// NOTA : Always to 'true'.
		}
		//f Return the id of a new object of type 'Type'.
		object__ GetNewObject( type__ Type )
		{
			object__ O;

			Internal_( bkdcmd::cGetNewObject );

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

			Internal_( bkdcmd::cGetType );

			StringIn( TypeName );

			EndOfInParameters();

			Id16Out( Type );

			_Handle();

			return Type;
		}
		//f Put in 'RawMessages' all the raw messages from backend.
		void GetRawMessages( strings_ &RawMessages )
		{
			Internal_( bkdcmd::cGetRawMessages );

			EndOfInParameters();

			StringsOut( RawMessages );

			_Handle();
		}
		//f Return the command of object type 'Type', named 'Name' with parameter 'Parameters'.
		command__ GetCommand(
			type__ Type,
			const string_ &Name,
			const ids8_ &Parameters )
		{
			command__ Command;

			Internal_( bkdcmd::cGetCommand );

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
			Internal_( bkdcmd::cGetCommands );

			Id16In( Type );
			CommandsDetailsIn( CommandDetails );

			EndOfInParameters();

			Ids16Out( Commands );

			_Handle();
		}
		//f Remove object 'Object'.
		void RemoveObject( object__ Object )
		{
			Internal_( bkdcmd::cRemoveObject );

			ObjectIn( Object );

			EndOfInParameters();

			_Handle();
		}
		//f Put in 'Name' and 'Version' the name and version of the backend.
		void AboutBackend(
			string_ &Name,
			string_ &Version)
		{
			Internal_( bkdcmd::cAboutBackend );

			EndOfInParameters();

			StringOut( Name );
			StringOut( Version );

			_Handle();
		}
		void AboutPublisher(
			string_ &Name,
			string_ &Version)
		{
			Internal_( bkdcmd::cAboutPublisher );

			EndOfInParameters();

			StringOut( Name );
			StringOut( Version );

			_Handle();
		}
		//f Disconnection.
		void Disconnect( void )
		{
			Internal_( bkdcmd::cDisconnect );

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
			Internal_( bkdcmd::cGetTypesIDAndPrefixAndName );

			EndOfInParameters();

			XItems16Out( TypeXItems );

			_Handle();
		}
		//f Put in 'Items' the commands name and id of an object of type 'Type'.
		void GetCommandsIDAndName(
			type__ Type,
			items16_ &CommandItems )
		{
			Internal_( bkdcmd::cGetCommandsIDAndName );

			Id16In( Type );

			EndOfInParameters();

			Items16Out( CommandItems );

			_Handle();
		}
   //f Return the current language. 	 
		const string_ &GetLanguage( string_ &Language ) 	 
        { 	 
			Internal_( bkdcmd::cGetLanguage ); 	 

			EndOfInParameters(); 	 

			StringOut( Language ); 	 

			_Handle(); 

			return Language;
        } 	 
         //f Set language to language of id 'Language'; 	 
		void SetLanguage( const string_ &Language ) 	 
		{ 	 
			Internal_( bkdcmd::cSetLanguage ); 	 

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
			Internal_( bkdcmd::cGetParameters );
			Id16In( Type );
			Id16In( Command );

			EndOfInParameters();

			Ids8Out( Parameters );

			_Handle();
		}
/*		void In(
			bkdcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			_In( Cast, Pointer, Flow );
		}
		void Out(
			flw::ioflow__ &Flow,
			bkdcst::cast Cast,
			void *Pointer )
		{
			_Out( Flow, Cast, Pointer );
		}
*/		bso::bool__ Handle(
			bso::bool__ Success,
			flw::ioflow__ &Flow )
		{
			return _Handle( Success, Flow );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
