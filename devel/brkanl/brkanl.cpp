/*
  'brkanl' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'brkanl' header file ('brkanl.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

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
  along with this program; if not, go to http://www.fsf.org or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define BRKANL__COMPILATION

#include "brkanl.h"

class brkanltutor
: public ttr_tutor
{
public:
	brkanltutor( void )
	: ttr_tutor( BRKANL_NAME )
	{
#ifdef BRKANL_DBG
		Version = BRKANL_VERSION " (DBG)";
#else
		Version = BRKANL_VERSION;
#endif
		Owner = BRKANL_OWNER;
		Date = "$Date$";
	}
	virtual ~brkanltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace brkanl;

namespace {

	void HandleCommands_(
		const broker8_command_items_ &Items,
		bso__ushort Type,
		broker8__core_ &Broker,
		broker_commands_ &Commands )
	{
	ERRProlog
		CMITEM( broker8_language_item_ ) Item;
		POSITION__ P =NONE;
		broker_command Command;
	ERRBegin
		Item.Init( Items );

		P = Items.First();

		while( P != NONE ) {
			Command.Init();
			Command.Identification.Value = Item( P ).Value;
			Command.Identification.ID( Item( P ).ID() );
			Broker.GetParameters( Type, Item( P ).ID(), Command.Parameters );
			Commands.Add( Command );
			P = Items.Next( P );
		}
	ERRErr
	ERREnd
	ERREpilog
	}

	void GetCommands_(
		bso__ushort Type,
		broker8__core_ &Broker,
		broker_commands_ &Commands )
	{
	ERRProlog
		broker8_command_items Items;
	ERRBegin
		Items.Init();

		Broker.GetCommandsIDAndName( Type, Items );

		HandleCommands_( Items, Type, Broker, Commands );
	ERRErr
	ERREnd
	ERREpilog
	}

	void HandleTypes_(
		const broker8_type_items_ &Items,
		broker8__core_ &Broker,
		broker_types_ &Types )
	{
	ERRProlog
		CMITEM( broker8_type_item_ ) Item;
		POSITION__ P = NONE;
		broker_type Type;
	ERRBegin
		Item.Init( Items );

		P = Items.First();

		while( P != NONE ) {
			Type.Init();

			Type.Identification.Value = Item( P ).Value;
			Type.Identification.ID( Item( P ).ID() );

			GetCommands_( Item( P ).ID(), Broker, Type.Commands );

			Types.Add( Type );
			P = Items.Next( P );
		}
	ERRErr
	ERREnd
	ERREpilog
	}

	void PutMasterType_( broker8_type_items_ &Items )
	{
	ERRProlog
		broker8_type_item Item;
	ERRBegin
		Item.Init();

		Item.Value = "_MASTER_";
		Item.ID( -1 );

		Items.Add( Item );
	ERRErr
	ERREnd
	ERREpilog
	}
}

void brkanl::Analyze(
	broker8__core_ &Broker,
	broker_types_ &Types )
{
ERRProlog
	broker8_type_items Items;
ERRBegin
	Items.Init();
	
	PutMasterType_( Items );
	
	Broker.GetTypesIDAndName( Items );
	
	HandleTypes_( Items, Broker, Types );
ERRErr
ERREnd
ERREpilog
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class brkanlpersonnalization
: public brkanltutor
{
public:
	brkanlpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~brkanlpersonnalization( void )
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

static brkanlpersonnalization Tutor;

ttr_tutor &BRKANLTutor = Tutor;
