/*
	'bom' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'bom' header file ('bom.h').
	Copyright (C) 20132004 Claude SIMON.

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

#define BOM__COMPILATION

#include "bom.h"

class bomtutor
: public ttr_tutor
{
public:
	bomtutor( void )
	: ttr_tutor( BOM_NAME )
	{
#ifdef BOM_DBG
		Version = BOM_VERSION "\b\bD $";
#else
		Version = BOM_VERSION;
#endif
		Owner = BOM_OWNER;
		Date = "$Date$";
	}
	virtual ~bomtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

using namespace bom;

struct bom__ {
	const char *Data;
	fdr::size__ Size;
};

#define M( m ) { m, sizeof( m ) - 1 }

// doit relèter l'ordre des déclarations dans 'byte_order_marker__'.
static bom__ BOMS_[bom_amount] =
{
	M( BOM_UTF_32_BE ),
	M( BOM_UTF_32_LE ),
	M( BOM_UTF_8 ),
	M( BOM_UTF_16_BE ),
	M( BOM_UTF_16_LE ),
};

static bso::bool__ Match_(
	const bom__ &Bom,
	const fdr::datum__ *Buffer,
	fdr::size__ &Size )	// Si retourne 'true', "Size' est modifié pour contenir la taille du 'BOM'.
{
	if ( ( Size >= Bom.Size ) && ( memcmp( Bom.Data, Buffer, Bom.Size ) == 0 ) ) {
		Size = Bom.Size;
		return true;
	} else
		return false;
}


byte_order_marker__ bom::DetectBOM(
	const fdr::datum__ *Buffer,
	fdr::size__ &Size )
{
	int Position = bom_First;

	while ( ( Position <= bom_Last ) && ( !Match_( BOMS_[Position], Buffer, Size ) ) )
		Position++;

	if ( Position > bom_Last)
		Position = bom_UnknownOrNone;

	return (byte_order_marker__)Position;
}



class bompersonnalization
: public bomtutor
{
public:
	bompersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~bompersonnalization( void )
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

static bompersonnalization Tutor;

ttr_tutor &BOMTutor = Tutor;
