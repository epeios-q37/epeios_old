/*
	'txmtbl' library by Claude SIMON (csimon@epeios.org)
	Requires the 'txmtbl' header file ('txmtbl.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

#define TXMTBL__COMPILATION

#include "txmtbl.h"

class txmtbltutor
: public ttr_tutor
{
public:
	txmtbltutor( void )
	: ttr_tutor( TXMTBL_NAME )
	{
#ifdef TXMTBL_DBG
		Version = TXMTBL_VERSION "\b\bD $";
#else
		Version = TXMTBL_VERSION;
#endif
		Owner = TXMTBL_OWNER;
		Date = "$Date$";
	}
	virtual ~txmtbltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace txmtbl;

static inline bso::bool__ HandleEscape_(
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	escape__ Escape,
	bso::bool__ &EOX,
	bso::char__ &C )
{
	bso::bool__ Retry = false;

	if ( Flow.EOX() )
		ERRf();
		
	switch( C = Flow.Get() ) {
	case 'n':
		C = '\n';
		break;
	case 'r':
		C = '\r';
		break;
	case 's':
		C = Separator;
		break;
	case 't':
		C = '\t';
		break;
	case '\n':
		if ( !( EOX = Flow.EOX() ) ) {
			if ( Flow.View() == '\r' )
				Flow.Get();
			Retry = true;
		}
		break;
	case '\r':
		if ( !( EOX = Flow.EOX() ) ) {
			if ( Flow.View() == '\n' )
				Flow.Get();
			Retry = true;
		}
		break;
	case '\t':
		Retry = true;
		break;
	default:
		if ( C != Escape )
			ERRf();
		break;
	}
	
	return Retry;
}

static inline bso::bool__ IsNotEndOfCell_(
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	escape__ Escape,
	bso::bool__ &EOX,
	bso::char__ &C )
{
	bso::bool__ Loop = false;

	if ( !( EOX = Flow.EOX() ) )
		do {		
			if ( ( C = Flow.Get() ) == Escape )
				Loop = HandleEscape_( Flow, Separator, Escape, EOX, C );
			else
				Loop = false;
		} while ( Loop );
		
	return !EOX && ( C != Separator ) && ( C != '\n' ) && ( C != '\r' );
}

static inline txmtbl::delimiter GetDelimiter_( 
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	bso::bool__ EOX,
	bso::char__ C )
{
	if ( EOX )
		return txmtbl::dEOF;
	else if ( C == '\n' )
	{
		if ( !Flow.EOX() && ( Flow.View() == '\r' ) )
			Flow.Get();

		return txmtbl::dEOL;
	}
	else if ( C == '\r' )
	{
		if ( !Flow.EOX() && ( Flow.View() == '\n' ) )
			Flow.Get();

		return txmtbl::dEOL;
	}
	else if ( C == Separator )
		return txmtbl::dSeparator;
	else
	{
		ERRc();
		return txmtbl::dUnknow;
	}
}

txmtbl::delimiter txmtbl::GetCell(
	xtf::extended_text_iflow__ &Flow,
	cell_ &Cell,
	separator__ Separator,
	escape__ Escape )
{
	bso::char__ C = 0;
	bso::bool__ EOX = false;

	Cell.Init();

	Cell.Location( Flow.Column() );

	while( IsNotEndOfCell_( Flow, Separator, Escape, EOX, C ) ) 
		Cell.Append( C );
	
	return GetDelimiter_( Flow, Separator, EOX, C );
}

txmtbl::delimiter txmtbl::SkipCell(
	xtf::extended_text_iflow__ &Flow,
	separator__ Separator,
	escape__ Escape )
{
	bso::char__ C = 0;
	bso::bool__ EOX = false;

	while( IsNotEndOfCell_( Flow, Separator, Escape, EOX,C ) )
	{}	// To avoid a warning.

	return GetDelimiter_( Flow, Separator, EOX, C );
}

bso::bool__ txmtbl::GetLine(
	xtf::extended_text_iflow__ &Flow,
	line_ &Line,
	separator__ Separator,
	escape__ Escape )
{
ERRProlog
	cell Cell;
	bso::bool__ Loop;
	location__ Location;
ERRBegin
	Cell.Init();

	do {
		Location = Flow.Line();

		Loop = ( GetCell( Flow, Cell, Separator ) == txmtbl::dSeparator ) && !Flow.EOX();

		if ( Loop || Cell.Amount() || Line.Amount() )
			Line.Add( Cell, Location );

	}while ( Loop );
ERRErr
ERREnd
ERREpilog

	return !Flow.EOX();
}

void line_::Erase_( stack_ &Stack )
{
	while( Stack.Amount() != 0 )
		cells_::Remove( Stack.Pop() );
}

amount__ line_::RemoveEmptyCells( void )
{
	amount__ Amount = 0;
ERRProlog
	ctn::E_CMITEM( cell_ ) Cell;
	tym::row__ Current = First();
	stack Stack;
ERRBegin
	Cell.Init( *this );

	Stack.Init();

	while( Current != NONE )
	{
		if ( !Cell( Current ).Amount() )
		{
			Stack.Push( Current );
			Amount++;
		}

		Current = Next( Current );
	}
	
	Erase_( Stack );
ERRErr
ERREnd
ERREpilog
	return Amount;
}

tym::row__ line_::FirstNonEmptyCell( void ) const
{
	ctn::E_CMITEM( cell_ ) Cell;
	tym::row__ Current = First();

	Cell.Init( *this );

	while( ( Current != NONE ) && !Cell( Current ).Amount() )
		Current = Next( Current );

	return Current;
}

tym::row__ line_::LastNonEmptyCell( void ) const
{
	ctn::E_CMITEM( cell_ ) Cell;
	tym::row__ Current = Last();

	Cell.Init( *this );

	while( ( Current != NONE ) && !Cell( Current ).Amount() )
		Current = Previous( Current );

	return Current;
}

amount__ line_::RemoveHeadingEmptyCells( void )
{
	amount__ Amount = 0;
ERRProlog
	tym::row__ Current = FirstNonEmptyCell();
	stack Stack;
ERRBegin
	Stack.Init();

	if ( Current != NONE )
	{
		Current = Previous( Current );

		while( Current != NONE )
		{
			Stack.Push( Current );

			Amount++;

			Current = Previous( Current );
		}

		Erase_( Stack );
	}
	else if ( this->Amount() )
		RemoveAllCells();
ERRErr
ERREnd
ERREpilog
	return Amount;
}

amount__ line_::RemoveTailingEmptyCells( void )
{
	amount__ Amount = 0;
ERRProlog
	tym::row__ Current = LastNonEmptyCell();
	stack Stack;
ERRBegin

	Stack.Init();

	if ( Current != NONE )
	{
		Current = Next( Current );

		while( Current != NONE )
		{

			Stack.Push( Current );

			Amount++;

			Current = Next( Current );
		}

		Erase_( Stack );
	}
	else if ( this->Amount() )
		RemoveAllCells();
ERRErr
ERREnd
ERREpilog
	return Amount;
}

amount__ line_::RemoveCentralEmptyCells( void )
{
	amount__ Amount = 0;
ERRProlog
	ctn::E_CMITEM( cell_ ) Cell;
	tym::row__
		Current = FirstNonEmptyCell(),
		Last = LastNonEmptyCell();
	stack Stack;
ERRBegin
	Stack.Init();

	Cell.Init( *this );

	if ( Current != Last )
	{
		Current = Next( Current );

		while( Current != Last )
		{
			if ( !Cell( Current ).Amount() )
			{
				Stack.Push( Current );
				Amount++;
			}

			Current = Next( Current );
		}

		Erase_( Stack );
	}
	else if ( this->Amount() && ( Current == NONE ) )
		RemoveAllCells();
ERRErr
ERREnd
ERREpilog
	return Amount;
}

amount__ line_::RemoveCellsAt( tym::row__ Position )
{
	amount__ Amount = 0;
ERRProlog
	stack Stack;
ERRBegin
	Stack.Init();

	while( Position != NONE )
	{
		Stack.Push( Position );

		Amount++;

		Position = Next( Position );
	}

	Erase_( Stack );
ERRErr
ERREnd
ERREpilog
	return Amount;
}

static inline bool IsCommentary_(
	const cell_ &Cell,
	bso::char__ Marker )
{
	return Cell.Amount() && ( Cell( 0 ) == Marker );
}

amount__ line_::RemoveComment( bso::char__ Marker )
{
	tym::row__ Position = First();
	ctn::E_CMITEM( cell_ ) Cell;

	Cell.Init( *this );

	while( ( Position != NONE ) && !IsCommentary_( Cell( Position ), Marker ) )
		Position = Next( Position );

	if ( Position != NONE )
		return RemoveCellsAt( Position );
	else
		return 0;
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const line_ &Line )
{
	tym::row__ Current;
	ctn::E_CMITEM( cell_ ) Cell;

	Cell.Init( Line );

	Current = Line.First();

	if ( Current != NONE )
	{
		Flow << Cell( Current );
		Current = Line.Next( Current );
	}

	while( Current != NONE )
	{
		Flow << txf::tab << Cell( Current );
		Current = Line.Next( Current );
	}

	return Flow;
}

bso::bool__ txmtbl::GetFirstNonEmptyLine(
	xtf::extended_text_iflow__ &Flow,
	line_ &Line,
	separator__ Separator,
	escape__ Escape )
{
	if ( Flow.EOX() )
		return false;
	else
	{
		do
		{
			Line.Init();
			GetLine( Flow, Line, Separator, Escape );

			Line.RemoveEmptyCells();

		} while( !Line.Amount() && !Flow.EOX() );

		return Line.Amount() != 0;
	}
}


void txmtbl::GetTable(
	xtf::extended_text_iflow__ &Flow,
	table_ &Table,
	separator__ Separator,
	escape__ Escape )
{
ERRProlog
	line Line;
ERRBegin
	while( !Flow.EOX() )
	{
		Line.Init();

		Line.Location( Flow.Line() );

		GetLine( Flow, Line, Separator, Escape );

		Table.AddLine( Line );
	}
ERRErr
ERREnd
ERREpilog
}

void table_::Erase_( stack_ &Stack )
{
	while( Stack.Amount() != 0 )
		lines_::Remove( Stack.Pop() );
}

void table_::RemoveEmptyCells( void )
{
	tym::row__ Current = First();

	while( Current != NONE )
	{
		lines_::operator ()( Current ).RemoveEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveHeadingEmptyCells( void )
{
	tym::row__ Current = First();

	while( Current != NONE )
	{
		lines_::operator ()( Current ).RemoveHeadingEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveTailingEmptyCells( void )
{
	tym::row__ Current = First();

	while( Current != NONE )
	{
		lines_::operator ()( Current ).RemoveTailingEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveCentralEmptyCells( void )
{
	tym::row__ Current = First();

	while( Current != NONE )
	{
		lines_::operator()( Current ).RemoveCentralEmptyCells();

		Current = Next( Current );
	}

	lines_::Flush();
}

void table_::RemoveComments( bso::char__ Marker )
{
	tym::row__ Current = First();

	while( Current != NONE )
	{
		lines_::operator()( Current ).RemoveComment( Marker );

		Current = Next( Current );
	}

	lines_::Flush();
}

amount__ table_::RemoveEmptyLines( void )
{
	amount__ Amount = 0;
ERRProlog
	ctn::E_CITEM( line_ ) Line;
	tym::row__ Current = First();
	stack Stack;
ERRBegin
	Line.Init( *this );

	Stack.Init();

	while( Current != NONE )
	{
		if ( !Line( Current ).Amount() )
		{
			Stack.Push( Current );
			Amount++;
		}

		Current = Next( Current );
	}

	Erase_( Stack );
ERRErr
ERREnd
ERREpilog
	return Amount;

}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const table_ &Table )
{
	ctn::E_CITEM( line_ ) Line;
	tym::row__ Current;

	Line.Init( Table );

	Current = Table.First();

	if ( Current != NONE )
	{
		Flow << Line( Current );
		Current = Table.Next( Current );
	}

	while( Current != NONE )
	{
		Flow << txf::nl << Line( Current );
		Current = Table.Next( Current );
	}

	return Flow;
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class txmtblpersonnalization
: public txmtbltutor
{
public:
	txmtblpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~txmtblpersonnalization( void )
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

static txmtblpersonnalization Tutor;

ttr_tutor &TXMTBLTutor = Tutor;
