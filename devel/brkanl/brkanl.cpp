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

using namespace frtend;

namespace {

	void HandleCommands_(
		const commands_items_ &Items,
		type__ Type,
		frontend___ &Frontend,
		commands_ &Commands )
	{
	ERRProlog
		CMITEM( command_item_ ) Item;
		POSITION__ P =NONE;
		command Command;
	ERRBegin
		Item.Init( Items );

		P = Items.First();

		while( P != NONE ) {
			Command.Init();
			Command.Identification.Value = Item( P ).Value;
			Command.Identification.ID( Item( P ).ID() );
			Frontend.GetParameters( Type, Item( P ).ID(), Command.Parameters );
			Commands.Add( Command );
			P = Items.Next( P );
		}
	ERRErr
	ERREnd
	ERREpilog
	}

	void GetCommands_(
		type__ Type,
		frtend::frontend___ &Frontend,
		commands_ &Commands )
	{
	ERRProlog
		commands_items Items;
	ERRBegin
		Items.Init();

		Frontend.GetCommandsIDAndName( Type, Items );

		HandleCommands_( Items, Type, Frontend, Commands );
	ERRErr
	ERREnd
	ERREpilog
	}

	void HandleTypes_(
		const types_items_ &Items,
		frtend::frontend___ &Frontend,
		types_ &Types )
	{
	ERRProlog
		CMITEM( type_item_ ) Item;
		POSITION__ P = NONE;
		type Type;
	ERRBegin
		Item.Init( Items );

		P = Items.First();

		while( P != NONE ) {
			Type.Init();

			Type.Identification.Value = Item( P ).Value;
			Type.Identification.ID( Item( P ).ID() );

			GetCommands_( Item( P ).ID(), Frontend, Type.Commands );

			Types.Add( Type );
			P = Items.Next( P );
		}
	ERRErr
	ERREnd
	ERREpilog
	}

	void PutMasterType_( types_items_ &Items )
	{
	ERRProlog
		type_item Item;
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
	frtend::frontend___ &Frontend,
	types_ &Types )
{
ERRProlog
	types_items Items;
ERRBegin
	Items.Init();
	
	PutMasterType_( Items );
	
	Frontend.GetTypesIDAndName( Items );
	
	HandleTypes_( Items, Frontend, Types );
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
