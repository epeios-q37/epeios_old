/*
  'broker1' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'broker1' header file ('broker1.h').
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

#define BROKER1__COMPILATION

#include "broker1.h"

class broker1tutor
: public ttr_tutor
{
public:
	broker1tutor( void )
	: ttr_tutor( BROKER1_NAME )
	{
#ifdef BROKER1_DBG
		Version = BROKER1_VERSION " (DBG)";
#else
		Version = BROKER1_VERSION;
#endif
		Owner = BROKER1_OWNER;
		Date = "$Date$";
	}
	virtual ~broker1tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

const char *BROKER1CommandsNames[broker1::c_amount] = {
	"_SetErrorBreakingStatus$1",
	"_GetErrorBreakingStatus$1",
	"_GetNewObject$1",
	"_GetType$1",
	"_GetCommand$1",
	"_GetCommands$1",
	"_GetTypeAndCommands$1",
	"_RemoveObject$1",
	"_GetLanguagesIDAndName$1",
	"_GetVersion$1",
	"_Disconnect$1",
	"_GetTypesIDAndName$1",
	"_GetCommandsIDAndName$1",
	"_GetParameters$1",
};

static const broker0::cast SetErrorBreakingStatus[] = {
	broker0::cBoolean,	// Boolean of the new error modus.
	broker0::cEnd,
	broker0::cBoolean,	// Boolean of the old error modus.
	broker0::cEnd };
static const broker0::cast GetErrorBreakingStatus[] = {
	broker0::cEnd,
	broker0::cBoolean,	// Boolean of the old error modus.
	broker0::cEnd };
static const broker0::cast GetNewObject[] = {
	broker0::cType,		// The type of the new object.
	broker0::cEnd,
	broker0::cObject,	// The asked object.
	broker0::cEnd };
static const broker0::cast GetType[] = {
	broker0::cString,	// The name of the type.
	broker0::cEnd,
	broker0::cType,		// The type asked.
	broker0::cEnd };
static const broker0::cast GetCommand[] = {
	broker0::cType,		// The object type.
	broker0::cString,	// The command name.
	broker0::cCasts,	// The casts of the command.
	broker0::cEnd,
	broker0::cCommand,	// The asked command.
	broker0::cEnd };
static const broker0::cast GetCommands[] = {
	broker0::cType,		// The object type.
	broker0::cMulti,	// The name and casts of the asked commands.
	broker0::cEnd,
	broker0::cMulti,	// The asked commands.
	broker0::cEnd };
static const broker0::cast GetTypeAndCommands[] = {
	broker0::cString,	// The name of the type.
	broker0::cMulti,	// Name and casts of the commands.
	broker0::cEnd,
	broker0::cType,		// The asked type.
	broker0::cMulti,	// The asked commands.
	broker0::cEnd };
static const broker0::cast RemoveObject[] = {
	broker0::cObject,	// The object to remove.
	broker0::cEnd,
	broker0::cEnd };
static const broker0::cast GetLanguagesIDAndName[] = {
	broker0::cEnd,
	broker0::cMulti,	// The language IDs ('Id.16') and name ('String').
	broker0::cEnd };
static const broker0::cast GetVersion[] = {
	broker0::cEnd,
	broker0::cString,	// String containing the version.
	broker0::cEnd };
static const broker0::cast Disconnect[] = {
	broker0::cEnd,
	broker0::cEnd };
static const broker0::cast GetTypesIDAndName[] = {
	broker0::cEnd,
	broker0::cMulti,	// The types ID and name ('String').
	broker0::cEnd };
static const broker0::cast GetCommandsIDAndName[] = {
	broker0::cType,		// Type of objects from which you want the commands name.
	broker0::cEnd,
	broker0::cMulti,	// The names of the commands.
	broker0::cEnd };
static const broker0::cast GetParameters[] = {
	broker0::cType,		// Type of objects from which you want the parameters.
	broker0::cCommand,	// Command of objects from which you want the parameters.
	broker0::cEnd,
	broker0::cCasts,	// The parameters of the commands.
	broker0::cEnd };

const broker0::cast *BROKER1CommandsParameters[broker1::c_amount] = {
	SetErrorBreakingStatus,
	GetErrorBreakingStatus,
	GetNewObject,
	GetType,
	GetCommand,
	GetCommands,
	GetTypeAndCommands,
	RemoveObject,
	GetLanguagesIDAndName,
	GetVersion,
	Disconnect,
	GetTypesIDAndName,
	GetCommandsIDAndName,
	GetParameters,
};



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class broker1personnalization
: public broker1tutor
{
public:
	broker1personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~broker1personnalization( void )
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

static broker1personnalization Tutor;

ttr_tutor &BROKER1Tutor = Tutor;
