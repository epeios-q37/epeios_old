/*
  'brkcst' library by Claude L. Simon (simon@epeios.org)
  Requires the 'brkcst' header file ('brkcst.h').
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org).

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
  along with this program; if not, go to http://www.fsf.org/
  or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define BRKCST__COMPILATION

#include "brkcst.h"

class brkcsttutor
: public ttr_tutor
{
public:
	brkcsttutor( void )
	: ttr_tutor( BRKCST_NAME )
	{
#ifdef BRKCST_DBG
		Version = BRKCST_VERSION "\b\bD $";
#else
		Version = BRKCST_VERSION;
#endif
		Owner = BRKCST_OWNER;
		Date = "$Date$";
	}
	virtual ~brkcsttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace brkcst;

// MUST reflect 'brkcst::cast'
const char *brkcst::CastsNames[c_amount] = {
	"_end_",
	"Object",
	"Boolean",
	"Id8",
	"Ids8",
	"Id16",
	"Ids16",
	"Id32", 
	"Ids32", 
	"Char",
	"String",
	"Strings",
	"Byte",
	"Binary",
	"Binaries",
	"Items8",
	"Items16",
	"Items32",
	"CommandsDetails",
	"ObjectsRefences"
};

cast brkcst::GetID( const char *CastName )
{
	int i = 0;

	while( ( i < c_amount ) && strcmp( CastName, CastsNames[i] ) )
		i++;

	if ( i >= c_amount )
		i = cInvalid;

	return (cast)i;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class brkcstpersonnalization
: public brkcsttutor
{
public:
	brkcstpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~brkcstpersonnalization( void )
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

static brkcstpersonnalization Tutor;

ttr_tutor &BRKCSTTutor = Tutor;
