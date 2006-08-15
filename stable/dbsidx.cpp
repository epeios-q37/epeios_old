/*
	'dbsidx' library by Claude SIMON (csimon@epeios.org)
	Requires the 'dbsidx' header file ('dbsidx.h').
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

#define DBSIDX__COMPILATION

#include "dbsidx.h"

class dbsidxtutor
: public ttr_tutor
{
public:
	dbsidxtutor( void )
	: ttr_tutor( DBSIDX_NAME )
	{
#ifdef DBSIDX_DBG
		Version = DBSIDX_VERSION "\b\bD $";
#else
		Version = DBSIDX_VERSION;
#endif
		Owner = DBSIDX_OWNER;
		Date = "$Date$";
	}
	virtual ~dbsidxtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dbsidx;

bso::sign__ dbsidx::index_::_Search(
	const data_ &Data,
	row__ &Row ) const
{
	bso::sign__ Result = 0;
ERRProlog
	data DataToCompare;
	idxbtq::E_ISEEKERt__( row__ ) Seeker;
ERRBegin
	if ( _S.Root == NONE ) {
		Row = NONE;
		ERRReturn;
	}

	Seeker.Init( BaseIndex, _S.Root );

	Row = Seeker.GetCurrent();

	while ( Row != NONE ) {
		DataToCompare.Init();

		_Retrieve( Row, DataToCompare );

		switch ( Result = _S.Sort->Compare( Data, DataToCompare ) ) {
		case 0:
		case 1:
			Row = Seeker.SearchGreater();
			break;
		case -1:
			Row = Seeker.SearchLesser();
			break;
		default:
			ERRc();
			break;
		}
	}

	Row = Seeker.GetCurrent();
ERRErr
ERREnd
ERREpilog
	return Result;
}

void dbsidx::index_::Index( dbsdct::row__ Row )
{
ERRProlog
	data Data;
	row__ TargetRow = NONE;
ERRBegin
	if ( _S.Root == NONE ) {
		_S.Root = Row;
		ERRReturn;
	}

	Data.Init();

	_Retrieve( Row, Data );

	switch ( _Search( Data, TargetRow ) ) {
	case 1:
	case 0:
		BaseIndex.MarkAsGreater( Row, TargetRow );
		break;
	case -1:
		BaseIndex.MarkAsLesser( Row, TargetRow );
		break;
	default:
		ERRc();
		break;
	}

#ifdef DBSIDX_DBG
	if ( TargetRow == NONE )
		ERRc();
#endif


ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dbsidxpersonnalization
: public dbsidxtutor
{
public:
	dbsidxpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dbsidxpersonnalization( void )
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

static dbsidxpersonnalization Tutor;

ttr_tutor &DBSIDXTutor = Tutor;
