/*
  'ttr' library by Claude L. Simon (simon@epeios.org)
  Requires the 'ttr' header file ('ttr.h').
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org).

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
  along with this program; if not, go to http://www.fsf.org/
  or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define TTR__COMPILATION

#include "ttr.h"

class ttrtutor
: public ttr_tutor
{
public:
	ttrtutor( void )
	: ttr_tutor( TTR_NAME )
	{
#ifdef TTR_DBG
		Version = TTR_VERSION "\b\b\b (DBG)";
#else
		Version = TTR_VERSION;
#endif
		Owner = TTR_OWNER;
		Date = "$Date$";
	}
	virtual ~ttrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "stf.h"

ttr_chief TTR;
char ttr_tutor::Adv_[TTR_ADV_SIZE_MAX+1];
char ttr_tutor::ChiefReady_ = false;

void ttr_chief::Advertise( void )
{
	ttr_tutor *Current = First_;

	while( Current )
	{
		fout << Current->Advertise();
		Current = Current->Next();
	}
}

void ttr_chief::Sort_( ttr_tutor *Tutor )
{
	ttr_tutor **Index = &First_;

	while ( *Index && ( strcmp( (*Index)->Name, Tutor->Name ) < 0 ) )
		Index = &((*Index)->Next_);

	Tutor->Next_ = *Index;

	*Index = Tutor;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class ttrpersonnalization
: public ttrtutor
{
public:
	ttrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ttrpersonnalization( void )
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

static ttrpersonnalization Tutor;

ttr_tutor &TTRTutor = Tutor;
