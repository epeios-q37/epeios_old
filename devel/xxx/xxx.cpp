/*
  '$xxx$' library by Claude L. Simon ($EMAIL$)
  Requires the '$xxx$' header file ('$xxx$.h').
  Copyright (C) 2000 $COPYRIGHT$.

  $ADDENDUM1$
  $ADDENDUM2$

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

//	$$Id$$

#define $XXX$__COMPILATION

#include "$xxx$.h"

class $xxx$tutor
: public ttr_tutor
{
public:
	$xxx$tutor( void )
	: ttr_tutor( $XXX$_NAME )
	{
#ifdef $XXX$_DBG
		Version = $XXX$_VERSION " (DBG)";
#else
		Version = $XXX$_VERSION;
#endif
		Owner = $XXX$_OWNER;
		Date = "$$Date$$";
	}
	virtual ~$xxx$tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$$BEGIN$$*/

using namespace $xxx$;



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class $xxx$personnalization
: public $xxx$tutor
{
public:
	$xxx$personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~$xxx$personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$$END$$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static $xxx$personnalization Tutor;

ttr_tutor &$XXX$Tutor = Tutor;
