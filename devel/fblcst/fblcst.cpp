/*
	'fblcst' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'fblcst' header file ('fblcst.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#define FBLCST__COMPILATION

#include "fblcst.h"

class fblcsttutor
: public ttr_tutor
{
public:
	fblcsttutor( void )
	: ttr_tutor( FBLCST_NAME )
	{
#ifdef FBLCST_DBG
		Version = FBLCST_VERSION "\b\bD $";
#else
		Version = FBLCST_VERSION;
#endif
		Owner = FBLCST_OWNER;
		Date = "$Date$";
	}
	virtual ~fblcsttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace fblcst;

// MUST reflect 'bkdcst::cast'
const char *fblcst::CastsNames[c_amount] = {
	"_end_",
	"Object",
	"Boolean",
	"Booleans",
	"SLong",
	"SLongs",
	"ULong",
	"ULongs",
	"Id8",
	"Ids8",
	"XIds8",
	"Id16",
	"Ids16",
	"XIds16",
	"Id32", 
	"Ids32", 
	"XIds32", 
	"Id",
	"Ids",
	"XIds", 
	"Char",
	"String",
	"Strings",
	"XStrings",
	"Byte",
	"Binary",
	"Binaries",
	"Items8",
	"Items16",
	"Items32",
	"XItems8",
	"XItems16",
	"XItems32",
	"CommandsDetails",
	"ObjectsReferences"
};

cast fblcst::GetID( const char *CastName )
{
	int i = 0;

	while( ( i < c_amount ) && strcmp( CastName, CastsNames[i] ) )
		i++;

	if ( i >= c_amount )
		i = c_Unknow;

	return (cast)i;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class fblcstpersonnalization
: public fblcsttutor
{
public:
	fblcstpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fblcstpersonnalization( void )
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

static fblcstpersonnalization Tutor;

ttr_tutor &FBLCSTTutor = Tutor;
