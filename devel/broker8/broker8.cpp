/*
  'broker8' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'broker8' header file ('broker8.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define BROKER8__COMPILATION

#include "broker8.h"

class broker8tutor
: public ttr_tutor
{
public:
	broker8tutor( void )
	: ttr_tutor( BROKER8_NAME )
	{
#ifdef BROKER8_DBG
		Version = BROKER8_VERSION " (DBG)";
#else
		Version = BROKER8_VERSION;
#endif
		Owner = BROKER8_OWNER;
		Date = "$Date$";
	}
	virtual ~broker8tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

broker0::cast broker8__data::GetCast( bso__ubyte CastID ) const
{
	int i = 0;

	while ( ( i < broker0::c_amount ) && ( Casts_[i] != CastID ) )
		i++;

	if (i == broker0::c_amount )
		return broker0::cInvalid;
	else
		return (broker0::cast)i;
}


void broker8__core_::GetRemoteCasts_()
{
	int i;

	PutObject( -1 );	// Master object.
	PutCommand( -1 );	// Master command.

	for( i = 0; i < broker0::c_amount; i++ )
		flw::PutString( broker0::CastsNames[i], *Flow_ );

	Send();

	for( i = 0; i < broker0::c_amount; i++ )
		Data_->Casts_[i] = GetByte_();

	TestCompletion();
}

bso__ushort broker8__core_::GetRemoteDefaultCommand_()
{
	bso__ushort MC;

	PutObject( -1 );	// Master object.
	PutCommand( -1 );	// Master command.

	Send();

	MC = GetShort_();

	TestCompletion();

	return MC;
}

void broker8__core_::GetRemoteCommands_( bso__ushort DefaultCommand )
{
	PutObject( -1 );	// Master object.
	PutCommand( DefaultCommand );
	AddType( -1 );	//Master type.

	AddString( BROKER1CommandsNames[broker1::cGetCommands] );
	AddCasts( BROKER1CommandsParameters[broker1::cGetCommands] );

	Send();

	Data_->Commands_[broker1::cGetCommands] = GetCommand();

	TestCompletion();

	GetCommands( -1, BROKER1CommandsNames, BROKER1CommandsParameters, broker1::c_amount, Data_->Commands_ );
}

void broker8__core_::GetCasts( broker8_casts_ &Casts )
{
	broker8__cast Cast;

	TestCast_( broker0::cCasts );

	do {
		Cast = GetByte_();
		Casts.Add( Cast );
	} while ( Cast != Data_->GetCastID( broker0::cEnd ) );

	while ( ( Cast = GetByte_() ) != Data_->GetCastID( broker0::cEnd) )
		Casts.Add( Cast );
}



void broker8__core_::GetCommands(
	bso__ushort Type,
	const char *Names[],
	const broker0::cast *Casts[],
	bso__ushort Amount,
	bso__ushort *Commands )
{
	int i;

	Internal_( broker1::cGetCommands );

	AddType( Type );	//Master type.

	BeginMulti();

	for( i = 0; i < Amount; i++ ) {
		AddString( Names[i] );
		AddCasts( Casts[i] );
	}

	EndMulti();

	Send();

	GetMulti();

	for( i = 0; i < Amount; i++ )
		Commands[i] = GetCommand();

	if ( !IsEndMulti() )
		ERRu();

	TestCompletion();
}


void broker8__core_::AddCasts( const broker0::cast *Casts )
{
	int i = 0;

	PutCast_( broker0::cCasts );

	do {
		PutCast_( Casts[i] );
	} while( Casts[i++] != broker0::cEnd );

	do {
		PutCast_( Casts[i] );
	} while( Casts[i++] != broker0::cEnd );
}

void broker8__core_::GetLanguagesIDAndName( broker8_language_items_ &Items)
{
ERRProlog
	broker8_language_item Item;
ERRBegin

	Internal_( broker1::cGetLanguagesIDAndName );
	Send();

	Item.Init();

	GetMulti();

	while ( !IsEndMulti() ) {
		Item.Init();

		Item.ID( GetId16() );
		GetString( Item.Value );

		Items.Add( Item );
	}

	TestCompletion();
ERRErr
ERREnd
ERREpilog
}

void broker8__core_::GetTypesIDAndName( broker8_type_items_ &Items)
{
ERRProlog
	broker8_type_item Item;
ERRBegin

	Internal_(broker1::cGetTypesIDAndName );

	Send();

	Item.Init();

	GetMulti();

	while ( !IsEndMulti() ) {
		Item.Init();

		Item.ID( GetType() );
		GetString( Item.Value );

		Items.Add( Item );
	}

	TestCompletion();
ERRErr
ERREnd
ERREpilog
}

void broker8__core_::GetCommandsIDAndName(
	bso__ushort Type,
	broker8_command_items_ &Items )
{
ERRProlog
	broker8_command_item Item;
ERRBegin

	Internal_( broker1::cGetCommandsIDAndName );

	AddType( Type );
	Send();

	Item.Init();

	GetMulti();

	while ( !IsEndMulti() ) {
		Item.Init();

		Item.ID( GetCommand() );
		GetString( Item.Value );

		Items.Add( Item );
	}

	TestCompletion();
ERRErr
ERREnd
ERREpilog
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class broker8personnalization
: public broker8tutor
{
public:
	broker8personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~broker8personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static broker8personnalization Tutor;

ttr_tutor &BROKER8Tutor = Tutor;
