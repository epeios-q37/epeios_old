/*
  'xmlval' library by Claude L. Simon (csimon@webmails.com)
  Requires the 'xmlval' header file ('xmlval.h').
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com).

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

#define XMLVAL__COMPILATION

#include "xmlval.h"

class xmlvaltutor
: public ttr_tutor
{
public:
	xmlvaltutor( void )
	: ttr_tutor( XMLVAL_NAME )
	{
#ifdef XMLVAL_DBG
		Version = XMLVAL_VERSION "\b\bD $";
#else
		Version = XMLVAL_VERSION;
#endif
		Owner = XMLVAL_OWNER;
		Date = "$Date$";
	}
	virtual ~xmlvaltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace xmlval;

value_row__ tagged_values_::Init( tag_row__ RootTag )
{
	value_row__ ValueRow = NONE;
ERRProlog
	tagged_value Value;
ERRBegin
	
	Value.Init( str::string( "" ), RootTag );
	
	ValueRow = basic_::Init( Value );
ERRErr
ERREnd
ERREpilog
	return ValueRow;
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xmlvalpersonnalization
: public xmlvaltutor
{
public:
	xmlvalpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xmlvalpersonnalization( void )
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

static xmlvalpersonnalization Tutor;

ttr_tutor &XMLVALTutor = Tutor;
