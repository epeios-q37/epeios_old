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

using namespace broker1;

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

static const cast SetErrorBreakingStatus[] = {
	cBoolean,	// Boolean of the new error modus.
	cEnd,
	cBoolean,	// Boolean of the old error modus.
	cEnd };
static const cast GetErrorBreakingStatus[] = {
	cEnd,
	cBoolean,	// Boolean of the old error modus.
	cEnd };
static const cast GetNewObject[] = {
	cType,		// The type of the new object.
	cEnd,
	cObject,	// The asked object.
	cEnd };
static const cast GetType[] = {
	cString,	// The name of the type.
	cEnd,
	cType,		// The type asked.
	cEnd };
static const cast GetCommand[] = {
	cType,		// The object type.
	cString,	// The command name.
	cCasts,	// The casts of the command.
	cEnd,
	cCommand,	// The asked command.
	cEnd };
static const cast GetCommands[] = {
	cType,		// The object type.
	cArray,	// The name and casts of the asked commands.
	cEnd,
	cArray,	// The asked commands.
	cEnd };
static const cast GetTypeAndCommands[] = {
	cString,	// The name of the type.
	cArray,	// Name and casts of the commands.
	cEnd,
	cType,		// The asked type.
	cArray,	// The asked commands.
	cEnd };
static const cast RemoveObject[] = {
	cObject,	// The object to remove.
	cEnd,
	cEnd };
static const cast GetLanguagesIDAndName[] = {
	cEnd,
	cArray,	// The language IDs ('Id.16') and name ('String').
	cEnd };
static const cast GetVersion[] = {
	cEnd,
	cString,	// String containing the version.
	cEnd };
static const cast Disconnect[] = {
	cEnd,
	cEnd };
static const cast GetTypesIDAndName[] = {
	cEnd,
	cArray,	// The types ID and name ('String').
	cEnd };
static const cast GetCommandsIDAndName[] = {
	cType,		// Type of objects from which you want the commands name.
	cEnd,
	cArray,	// The names of the commands.
	cEnd };
static const cast GetParameters[] = {
	cType,		// Type of objects from which you want the parameters.
	cCommand,	// Command of objects from which you want the parameters.
	cEnd,
	cCasts,	// The parameters of the commands.
	cEnd };

const cast *BROKER1CommandsParameters[broker1::c_amount] = {
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
