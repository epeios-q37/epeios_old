/*
	'flf' library by Claude SIMON (csimon@epeios.org)
	Requires the 'flf' header file ('flf.h').
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

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

#define FLF__COMPILATION

#include "flf.h"

class flftutor
: public ttr_tutor
{
public:
	flftutor( void )
	: ttr_tutor( FLF_NAME )
	{
#ifdef FLF_DBG
		Version = FLF_VERSION "\b\bD $";
#else
		Version = FLF_VERSION;
#endif
		Owner = FLF_OWNER;
		Date = "$Date$";
	}
	virtual ~flftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace flf;

static FILE *cin = stdin;
static FILE *cout = stdout;
static FILE *cerr = stderr;

file_oflow__ flf::coutF( ::cout );
file_oflow__ flf::cerrF( ::cerr );
file_iflow__ flf::cinF( ::cin );
txf::text_oflow___ flf::cout;
txf::text_oflow___ flf::cerr;
txf::text_iflow___ flf::cin;


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class flfpersonnalization
: public flftutor
{
public:
	flfpersonnalization( void )
	{
		cinF.Init();
		coutF.Init();
		cerrF.Init();

		flf::cin.Init( cinF );
		flf::cout.Init( coutF );
		flf::cerr.Init( cerrF );
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~flfpersonnalization( void )
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

static flfpersonnalization Tutor;

ttr_tutor &FLFTutor = Tutor;
