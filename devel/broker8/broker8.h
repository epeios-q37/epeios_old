/*
  Header for the 'broker8' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef BROKER8__INC
#define BROKER8__INC

#define BROKER8_NAME		"BROKER8"

#define	BROKER8_VERSION	"$Revision$"	

#define BROKER8_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BROKER8Tutor;

#if defined( XXX_DBG ) && !defined( BROKER8_NODBG )
#define BROKER8_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

//D BROKER Part 8. Obslolete.

#error Oboslete. Use 'FRTEND' library instead.

#include "broker1.h"
#include "bso.h"
#include "str.h"
#include "ctn.h"

#define BROKER8_UNDEFINED_CAST		255
#define BROKER8_UNDEFINED_TYPE		65535
#define BROKER8_UNDEFINED_OBJECT	65535


//c An item with id type 'id'. Very often used by frontends.
template <class id> class broker8_item_
{
public:
	struct s
	{
		str_string_::s Value;
		id ID;
	} &S_;
	str_string_ Value;
	broker8_item_( s &S )
	: S_( S ),
	  Value( S.Value )
	{}
	void reset( bool P = true )
	{
		Value.reset( P );
	}
	void plug( mdr_basic_memory_driver_ &MD )
	{
		Value.plug( MD );
	}
	void plug( mmm_multimemory_ &M )
	{
		Value.plug( M );
	}
	broker8_item_ &operator =( const broker8_item_ &I )
	{
		Value = I.Value;
		S_.ID = I.S_.ID;

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		Value.Init();
	}
	//f Return the value of the ID.
	id ID( void ) const
	{
		return S_.ID;
	}
	//f 'ID' becomes the ID.
	void ID( id ID ) 
	{
		S_.ID = ID;
	}
};

//m Create a item definitions named 'name' with id type 'id'.
#define BROKER8_ITEM( id, name )\
	typedef broker8_item_<id>	name##_;\
	AUTO( name )

//m Create a items definitions named 'name' with item type 'item'.
#define BROKER8_ITEMS( item, name )\
	typedef XMCONTAINER_( item##_ )	name##_;\
	AUTO( name )

BROKER8_ITEM( bso__ushort, broker8_language_item )
BROKER8_ITEMS( broker8_language_item, broker8_language_items )

BROKER8_ITEM( bso__ushort, broker8_type_item )
BROKER8_ITEMS( broker8_type_item, broker8_type_items )

BROKER8_ITEM( bso__ushort, broker8_command_item )
BROKER8_ITEMS( broker8_command_item, broker8_command_items )

typedef bso__ubyte				broker8__cast;
typedef SET_( broker8__cast )	broker8_casts_;
typedef SET( broker8__cast )	broker8_casts;

struct broker8__data {
	bso__ubyte Casts_[broker0::c_amount];
	bso__ushort Commands_[broker1::c_amount];
	broker8__data( void )
	{
		Casts_[broker0::cEnd] = BROKER8_UNDEFINED_CAST;
	}
	bso__bool OK( void ) const
	{
		return Casts_[broker0::cEnd] != BROKER8_UNDEFINED_CAST;
	}
	bso__ubyte GetCastID( broker0::cast Cast ) const
	{
		return Casts_[Cast];
	}
	bso__ushort GetCommandID( broker1::command Command ) const
	{
		return Commands_[Command];
	}
	broker0::cast GetCast( bso__ubyte CastID ) const;
};

//c The frontend core.
class broker8__core_
{
private:
	broker8__data *Data_;
	bso__ubyte RawCast_;
	char Message_[200];
	flw::ioflow___ *Flow_;
	void PutCast_( broker0::cast Cast )
	{
		PutByte_( Data_->GetCastID( Cast ) );
	}
	bso__ubyte GetCast_( void )
	{
		return Flow_->Get();
	}
	void GetRemoteCasts_( void );
	bso__ushort GetRemoteDefaultCommand_( void );
	void GetRemoteCommands_( bso__ushort DefaultCommand );
	void PutByte_( bso__ubyte V )
	{
		flw::Put( V, *Flow_ );
	}
	void PutShort_( bso__ushort V )
	{
		flw::Put( V, *Flow_ );
	}
	void PutLong_( bso__ulong V )
	{
		flw::Put( V, *Flow_ );
	}
	bso__ubyte GetByte_( void )
	{
		bso__ubyte V;

		flw::Get( *Flow_, V );

		return V;
	}
	bso__ushort GetShort_( void )
	{
		bso__ushort V;

		flw::Get( *Flow_, V );

		return V;
	}
	bso__ulong GetLong_( void )
	{
		bso__ulong V;

		flw::Get( *Flow_, V );

		return V;
	}
	void TestCast_( broker0::cast Cast )
	{
		if ( Data_->GetCastID( Cast ) != ( RawCast_ == BROKER8_UNDEFINED_CAST ? GetCast_() : RawCast_ ) )
				ERRu();

		RawCast_ = BROKER8_UNDEFINED_CAST;
	}
	void Internal_( broker1::command Command )
	{
		PutObject( -1 );
		PutCommand( Data_->GetCommandID( Command ) );
	}
public:
	broker8__core_( void )
	{
		Flow_ = NULL;
		Data_ = NULL;
		RawCast_ = BROKER8_UNDEFINED_CAST;
	}
	//f Initialization with flow 'Flow' and data 'Data'.
	void Init(
		flw::ioflow___ &Flow,
		broker8__data &Data )
	{
		RawCast_ = BROKER8_UNDEFINED_CAST;
		Flow_ = &Flow;

		Data_ = &Data;

		if ( !Data_->OK() ) {
			bso__ushort DefaultCommand;

			Data_->Casts_[broker0::cEnd] = 0;
		
			GetRemoteCasts_();

			DefaultCommand = GetRemoteDefaultCommand_();

			GetRemoteCommands_( DefaultCommand );
		}
	}
	//f Return the last explanation message.
	const char *Message( void )
	{
		return Message_;
	}
	//f Return the cast of id 'ID'.
	broker0::cast GetCast( bso__ubyte ID ) const
	{
		return Data_->GetCast( ID );
	}
	//f Put the request object.
	void PutObject( bso__ushort Object )
	{
		PutShort_( Object );
	}
	//f Put the request command.
	void PutCommand( bso__ushort Command )
	{
		PutShort_( Command );
	}
	//f add object 'Object'.
	void AddObject( bso__ushort Object )
	{
		PutCast_( broker0::cObject );
		PutShort_( Object );
	}
	//f Add type 'Type'.
	void AddType( bso__ushort Type )
	{
		PutCast_( broker0::cType );
		PutShort_( Type );
	}
	//f Add casts 'Casts'.
	void AddCasts( const broker0::cast *Casts );
	//f Add command 'Command'.
	void AddCommand( bso__ushort Command )
	{
		PutCast_( broker0::cCommand );
		PutShort_( Command );
	}
	//f Add boolean 'Boolean'.
	void AddBoolean( bso__bool Boolean )
	{
		PutCast_( broker0::cBoolean );
		PutByte_( Boolean );
	}
	//f Add 8 bits identifier 'Id'.
	void AddId8( bso__ubyte Id )
	{
		PutCast_( broker0::cId8 );
		PutByte_( Id );
	}
	//f Add 16 bits identifier 'Id'.
	void AddId16( bso__ushort Id )
	{
		PutCast_( broker0::cId16 );
		PutShort_( Id );
	}
	//f Add 32 bits identifier 'Id'.
	void AddId32( bso__ulong Id )
	{
		PutCast_( broker0::cId32 );
		PutLong_( Id );
	}
	//f add character 'C'.
	void AddChar( char C )
	{
		PutCast_( broker0::cChar );
		PutByte_( C );
	}
	//f Add string 'String'.
	void AddString( const char *String )
	{
		PutCast_( broker0::cString );
		flw::PutString( String, *Flow_ );
	}
	//f Add string 'String'.
	void AddString( const str_string_ &String )
	{
		PutCast_( broker0::cString );
		*Flow_ << String;
	}
	//f Add byte 'Byte'
	void AddByte( bso__ubyte Byte )
	{
		PutCast_( broker0::cByte );
		PutByte_( Byte );
	}
	//f Signalize the beginning of a multi-session.
	void BeginMulti( void )
	{
		PutCast_( broker0::cMulti );
	}
	//f Signalize the end of a multi-session.
	void EndMulti( void )
	{
		PutCast_( broker0::cEnd );
	}
	//f Send the request.
	bso__bool Send( err::handle ERRHandle = err::hUsual )
	{
		Flow_->Put( 0 );	// End of request

		Flow_->Synchronize();

		if ( ( !flw::GetString( *Flow_, Message_, sizeof( Message_ ) ) ) )
			ERRl();

		if ( Message_[0] != 0 ) {
			if( ERRHandle == err::hUsual )
				ERRb();
			return true;
		} else
			return false;

	}
	//f Get object.
	bso__ushort GetObject( void )
	{
		TestCast_( broker0::cObject );
		return GetShort_();
	}
	//f Get type;
	bso__ushort GetType( void )
	{
		TestCast_( broker0::cType );
		return GetShort_();
	}
	//f Return casts in 'Casts'.
	void GetCasts( broker8_casts_ &Casts );
	//f Return a command.
	bso__ushort GetCommand( void )
	{
		TestCast_( broker0::cCommand );
		return GetShort_();
	}
	//f return a boolean
	bso__bool GetBoolean( void )
	{
		TestCast_( broker0::cBoolean );
		return GetByte_() != false;
	}
	//f return a 8 bits identifier.
	bso__ubyte GetId8( void )
	{
		TestCast_( broker0::cId8 );
		return GetByte_();
	}
	//f Return a 16 bits identifier..
	bso__ushort GetId16( void )
	{
		TestCast_( broker0::cId16 );
		return GetShort_();
	}
	//f Return a 32 bits identifier.
	bso__ulong GetId32( void )
	{
		TestCast_( broker0::cId32 );
		return GetLong_();
	}
	//f Return a character. 
	bso__char GetChar( void )
	{
		TestCast_( broker0::cChar );
		return GetByte_();
	}
	//f Get a string and place it in 'String'.
	void GetString( str_string_ &String )
	{
		TestCast_( broker0::cString );
		*Flow_ >> String;
	}
	//f Get a byte.
	bso__ubyte GetByte( void )
	{
		TestCast_( broker0::cByte );
		return GetByte_();
	}
	//f Return true if end of a multi-session, false otherwise.
	bso__bool IsEndMulti( void )
	{
		if ( ( RawCast_ = GetCast_() ) == Data_->GetCastID( broker0::cEnd ) )
		{
			RawCast_ = BROKER8_UNDEFINED_CAST;
			return true;
		}
		else
			return false;
	}
	//f Signalize that a multi-session is expected.
	void GetMulti( void )
	{
		if ( GetCast_() != Data_->GetCastID( broker0::cMulti ) )
			ERRu();
	}
	//f Verify if all the reponse handled.
	void TestCompletion( void )
	{
		if ( GetCast_() != Data_->GetCastID( broker0::cEnd ) )
			ERRu();
	}
	//f Set error breaking status to 'B'. Return previous error breaking status.
	bso__bool SetErrorBreakingStatus( bso__bool B )
	{
		bso__bool R;

		Internal_( broker1::cSetErrorBreakingStatus );

		AddBoolean( B );

		Send();

		R = GetBoolean();

		TestCompletion();

		return R;
	}
	//f Get error breaking status.
	bso__bool GetErrorBreakingStatus( bool )
	{
		bso__bool R;

		Internal_( broker1::cGetErrorBreakingStatus );

		Send();

		R = GetBoolean();

		TestCompletion();

		return R;
	}
	//f Return the id of a new object of type 'Type'.
	bso__ushort GetNewObject( bso__ushort Type )
	{
		bso__ushort O;

		Internal_( broker1::cGetNewObject );

		AddType( Type );

		Send();

		O = GetObject();

		TestCompletion();

		return O;
	}
	//f Return the type id. of type 'Type'.
	bso__ushort GetType( const char *Type )
	{
		bso__ushort T;

		Internal_( broker1::cGetType );

		AddString( Type );

		Send();

		T = GetType();

		TestCompletion();

		return T;
	}
	//f Return the command of object type 'Type', named 'Name' with parameter 'Parameters'.
	bso__ushort GetCommand(
		bso__ushort Type,
		const char *Name,
		broker0::cast *Parameters )
	{
		bso__ushort C;

		Internal_( broker1::cGetCommand );

		AddType( Type );
		AddString( Name );
		AddCasts( Parameters );

		Send();

		C = GetCommand();

		TestCompletion();

		return C;
	}
	/*f Put in 'Commands' the 'Amount' commands from object of type 'Type',
	whith names 'Names' and casts 'Casts'. */
	void GetCommands(
		bso__ushort Type,
		const char *Names[],
		const broker0::cast *Casts[],
		bso__ushort Amount,
		bso__ushort *Commands );
	//f Remove object 'Object'.
	void RemoveObject( bso__ushort Object )
	{
		Internal_( broker1::cRemoveObject );

		AddObject( Object );

		Send();

		TestCompletion();
	}
	//f Put in 'String' the version of the broker.
	void GetVersion( str_string_ &Version )
	{
		Internal_( broker1::cGetVersion );

		Send();

		GetString( Version );

		TestCompletion();
	}
	//f Disconnection.
	void Disconnect( void )
	{
		Internal_( broker1::cDisconnect );

		Send();

		TestCompletion();
	}
	//f Put in 'Items' the language names and ids.
	void GetLanguagesIDAndName( broker8_language_items_ &Items);
	//f Put in 'Items' the types name and id.
	void GetTypesIDAndName( broker8_type_items_ &Items);
	//f Put in 'Items' the commands name and id of an object of type 'Type'.
	void GetCommandsIDAndName(
		bso__ushort Type,
		broker8_command_items_ &Items );
	void GetParameters(
		bso__ushort Type,
		bso__ushort Command,
		broker8_casts_ &Casts )
	{
		Internal_( broker1::cGetParameters );
		AddType( Type );
		AddCommand( Command );

		Send();

		GetCasts( Casts );

		TestCompletion();
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
