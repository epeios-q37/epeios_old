/*
  'brkcmd' library by Claude L. Simon (csimon@webmails.com)
  Requires the 'brkcmd' header file ('brkcmd.h').
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com).

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

#define BRKCMD__COMPILATION

#include "brkcmd.h"

class brkcmdtutor
: public ttr_tutor
{
public:
	brkcmdtutor( void )
	: ttr_tutor( BRKCMD_NAME )
	{
#ifdef BRKCMD_DBG
		Version = BRKCMD_VERSION "\b\bD $";
#else
		Version = BRKCMD_VERSION;
#endif
		Owner = BRKCMD_OWNER;
		Date = "$Date$";
	}
	virtual ~brkcmdtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace brkcmd;

const char *brkcmd::CommandsNames[brkcmd::c_amount] = {
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
		cId16,		// The type of the new object.
	cEnd,
		cObject,	// The asked object.
	cEnd };
static const cast GetType[] = {
		cString,	// The name of the type.
	cEnd,
		cId16,		// The type asked.
	cEnd };
static const cast GetCommand[] = {
		cId16,		// The object type.
		cString,	// The command name.
		cIds8,	// The casts of the command.
	cEnd,
		cId16,	// The asked command.
	cEnd };
static const cast GetCommands[] = {
		cId16,		// The object type.
		cCommandsDetails,	// The wanted commands caracteristics.
	cEnd,
		cIds16,		// The asked commands.
	cEnd };
static const cast GetTypeAndCommands[] = {
		cString,	// The name of the type.
		cCommandsDetails,		// The wanted commands caracteristics.
	cEnd,
		cId16,		// The asked type.
		cIds16,		// The asked commands.
	cEnd };
static const cast RemoveObject[] = {
		cObject,	// The object to remove.
	cEnd,
	cEnd };
static const cast GetLanguagesIDAndName[] = {
	cEnd,
		cItems16,	// The language IDs and name.
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
		cItems16,	// The types ID and name.
	cEnd };
static const cast GetCommandsIDAndName[] = {
		cId16,		// Type of objects from which you want the commands name.
	cEnd,
		cItems16,	// The ids and names of the commands.
	cEnd };
static const cast GetParameters[] = {
		cId16,		// Type of objects from which you want the parameters.
		cId16,	// Command of objects from which you want the parameters.
	cEnd,
		cIds8,	// The parameters of the commands.
	cEnd };

const cast *brkcmd::CommandsParameters[brkcmd::c_amount] = {
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

class brkcmdpersonnalization
: public brkcmdtutor
{
public:
	brkcmdpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~brkcmdpersonnalization( void )
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

static brkcmdpersonnalization Tutor;

ttr_tutor &BRKCMDTutor = Tutor;
