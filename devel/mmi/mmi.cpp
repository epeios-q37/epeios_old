/*
  'mmi' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'mmi' header file ('mmi.h').
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

#define MMI__COMPILATION

#include "mmi.h"

class mmitutor
: public ttr_tutor
{
public:
	mmitutor( void )
	: ttr_tutor( MMI_NAME )
	{
#ifdef MMI_DBG
		Version = MMI_VERSION " (DBG)";
#else
		Version = MMI_VERSION;
#endif
		Owner = MMI_OWNER;
		Date = "$Date$";
	}
	virtual ~mmitutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#define capacite				uym__size
#define multimemoire_indexee	mmi_indexed_multimemory_

void multimemoire_indexee::AllouerPlus_(
	capacite CapaciteCourante,
	capacite CapaciteDemandee )
{
	mmi__descripteur D;

	D.Descripteur = 0;
	D.Capacite = 0;

	Descripteurs.Allocate( CapaciteDemandee );

	while ( CapaciteDemandee-- > CapaciteCourante )
		Descripteurs.Write( D, CapaciteDemandee );
}

void multimemoire_indexee::AllouerMoins_(
	capacite CapaciteCourante,
	capacite CapaciteDemandee )
{
	while ( CapaciteCourante-- > CapaciteDemandee )
		Multimemoire.Free( Descripteurs.Read( CapaciteCourante ).Descripteur );

	Descripteurs.Allocate( CapaciteDemandee );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class mmipersonnalization
: public mmitutor
{
public:
	mmipersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mmipersonnalization( void )
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

static mmipersonnalization Tutor;

ttr_tutor &MMITutor = Tutor;
