/*
	'dbstbl' library by Claude SIMON (csimon@epeios.org)
	Requires the 'dbstbl' header file ('dbstbl.h').
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

#define DBSTBL__COMPILATION

#include "dbstbl.h"

class dbstbltutor
: public ttr_tutor
{
public:
	dbstbltutor( void )
	: ttr_tutor( DBSTBL_NAME )
	{
#ifdef DBSTBL_DBG
		Version = DBSTBL_VERSION "\b\bD $";
#else
		Version = DBSTBL_VERSION;
#endif
		Owner = DBSTBL_OWNER;
		Date = "$Date$";
	}
	virtual ~dbstbltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dbstbl;

void dbstbl::table_::_InsertInIndexes( row__ Row )
{
	irow__ IRow = Indexes.First();

	while ( IRow != NONE ) {
		_I( IRow ).Index( Row );

		IRow = Indexes.Next( IRow );
	}
}

void dbstbl::table_::_DeleteFromIndexes( row__ Row )
{
	irow__ IRow = Indexes.First();

	while ( IRow != NONE ) {
		_I( IRow ).Delete( Row );

		IRow = Indexes.Next( IRow );
	}
}

#ifdef DBSTBL__THREAD_SAFE

#define RO	const table_ &T = _RO();
#define RW	table_ &T = _RW();

#define RRO	_RRO();
#define RRW	_RRW();

irow__ dbstbl::thread_safe_table_::AddIndex(
	index_ &Index,
	bso::bool__ Reindex )
{
	irow__ Row = NONE;
ERRProlog
ERRBegin
	RW

	Row = T.AddIndex( Index, Reindex );
ERRErr
ERREnd
	RRW
ERREpilog
	return Row;
}

row__ dbstbl::thread_safe_table_::Store( const data_ &Data )
{
	row__ Row = NONE;
ERRProlog
ERRBegin
	RW

	T.Store( Data );
ERRErr
ERREnd
	RRW
ERREpilog
	return Row;
}

void dbstbl::thread_safe_table_::Store(
	const data_ &Data,
	row__ Row )
{
ERRProlog
ERRBegin
	RW

	T.Store( Data, Row );
ERRErr
ERREnd
	RRW
ERREpilog
}

void dbstbl::thread_safe_table_::Delete( row__ Row )
{
ERRProlog
ERRBegin
	RW

	T.Delete( Row );
ERRErr
ERREnd
	RRW
ERREpilog
}

row__ dbstbl::thread_safe_table_::Search(
	const data_ &Data,
	irow__ IRow,
	bso::sign__ &Sign )
{
	row__ Row = NONE;
ERRProlog
ERRBegin
	RO

	Row = T.Search( Data, IRow, Sign );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

row__ dbstbl::thread_safe_table_::Search(
	const data_ &Data,
	irow__ IRow )
{
	row__ Row = NONE;
ERRProlog
ERRBegin
	RO

	Row = T.Search( Data, IRow );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

row__ dbstbl::thread_safe_table_::First( irow__ IRow )
{
	row__ Row = NONE;
ERRProlog
ERRBegin
	RO

	Row = T.First( IRow );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

row__ dbstbl::thread_safe_table_::Last( irow__ IRow )
{
	row__ Row = NONE;
ERRProlog
ERRBegin
	RO

	Row = T.Last( IRow );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

row__ dbstbl::thread_safe_table_::Next( 
	irow__ IRow,
	row__ Row )
{
ERRProlog
ERRBegin
	RO

	Row = T.Next( IRow, Row );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

row__ dbstbl::thread_safe_table_::Previous( 
	irow__ IRow,
	row__ Row )
{
ERRProlog
ERRBegin
	RO

	Row = T.Previous( IRow, Row );
ERRErr
ERREnd
	RRO
ERREpilog
	return Row;
}

mdr::size__ dbstbl::thread_safe_table_::Amount( void )
{
	mdr::size__ Amount;
ERRProlog
ERRBegin
	RO

	Amount = T.Amount();
ERRErr
ERREnd
	RRO
ERREpilog
	return Amount;
}

#endif

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dbstblpersonnalization
: public dbstbltutor
{
public:
	dbstblpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dbstblpersonnalization( void )
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

static dbstblpersonnalization Tutor;

ttr_tutor &DBSTBLTutor = Tutor;
