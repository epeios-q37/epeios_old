/*
	'lst' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'lst' header file ('lst.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#define LST__COMPILATION

#include "lst.h"

class lsttutor
: public ttr_tutor
{
public:
	lsttutor( void )
	: ttr_tutor( LST_NAME )
	{
#ifdef LST_DBG
		Version = LST_VERSION "\b\bD $";
#else
		Version = LST_VERSION;
#endif
		Owner = LST_OWNER;
		Date = "$Date$";
	}
	virtual ~lsttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/
#include "flf.h"

static inline void Save_(
	epeios::row__ Row,
	flw::oflow__ &Flow )
{
	dtfptb::PutULong( *Row, Flow );
}

static void Save_(
	const stk::E_BSTACK_( epeios::row__ ) &Bunch,
	flw::oflow__ &Flow )
{
	stk::row__ Row = Bunch.First();

	while ( Row != NONE ) {
		Save_( Bunch( Row ), Flow );

		Row = Bunch.Next( Row );
	}
}

epeios::row__ lst::WriteToFile_(
	const store_ &Store,
	const char *FileName,
	time_t ReferenceTimeStamp )
{
	epeios::row__ Row;
ERRProlog
	flf::file_oflow___ Flow;
ERRBegin
	Flow.Init( FileName );

	if ( ( ReferenceTimeStamp == 0 )
		|| ( !fil::FileExists( FileName ) )
		|| ( fil::GetFileLastModificationTime( FileName ) <= ReferenceTimeStamp ) )
			Save_( Store.Released, Flow );

	while ( fil::GetFileLastModificationTime( FileName ) <= ReferenceTimeStamp ) {
		tol::Clock( true );	// Permet d'attendre une unité de temps.
		fil::TouchFile( FileName );
	}

	Row = Store.GetFirstAvailable();
ERRErr
ERREnd
ERREpilog
	return Row;
}

static inline void Load_(
	flw::iflow__ &Flow,
	epeios::row__ &Row )
{
	Row = dtfptb::GetULong( Flow );
}
	
static void Load_(
	flw::iflow__ &Flow,
	epeios::size__ Amount,
	stk::E_BSTACK_( epeios::row__ ) &Stack )
{
	epeios::row__ Row;

	while ( Amount-- ) {
		Load_( Flow, Row );
		Stack.Append( Row );
	}
}

uym::state__ lst::ReadFromFile_(
	const char *FileName,
	time_t ReferenceTimeStamp,
	store_ &Store )
{
	uym::state__ State = uym::s_Undefined;
ERRProlog
	flf::file_iflow___ Flow;
ERRBegin

	State = Test_( FileName, ReferenceTimeStamp );

	if ( uym::IsError( State ) )
		ERRReturn;

	if ( State == uym::sAbsent )
		ERRReturn;

	if ( Flow.Init( FileName, err::hUserDefined ) != fil::sSuccess ) {
		State = uym::sInconsistent;
		ERRReturn;
	}

	Load_( Flow, fil::GetFileSize( FileName ) / sizeof( epeios::row__ ), Store.Released );

	State = uym::sExists;
ERRErr
ERREnd
ERREpilog
	return State;
}

// Retourne l'élément succédant à 'Element', ou LST_INEXISTANT si inexistant.
epeios::row_t__ lst::Successeur_(
	epeios::row_t__ Element,
	epeios::size__ Amount,
	const store_ &Libres )
{
	while( ( ++Element < Amount ) && Libres.IsAvailable( Element ) ) {};

	if ( Element >= Amount )
		return NONE;
	else
		return Element;
}

// Retourne l'élément précédent 'Element', ou LST_INEXISTANT si inexistant.
epeios::row_t__ lst::Predecesseur_(
	epeios::row_t__ Element,
	const store_ &Libres )
{
	bso::bool__ Trouve = false;

	while( ( Element > 0 ) && !( Trouve = !Libres.IsAvailable( --Element ) ) ) {};

	if ( Trouve )
		return Element;
	else
		return NONE;
}

void lst::MarkAsReleased_(
	epeios::row_t__ First,
	epeios::row_t__ Last,
	store_ &Store )
{
	while ( First < Last )
		Store.RestorationRelease( Last-- );

	Store.RestorationRelease( First );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class lstpersonnalization
: public lsttutor
{
public:
	lstpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lstpersonnalization( void )
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

static lstpersonnalization Tutor;

ttr_tutor &LSTTutor = Tutor;
