/*
  'txmtbl' library by Claude L. Simon (simon@epeios.org)
  Requires the 'txmtbl' header file ('txmtbl.h').
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

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

static inline bso__bool IsNotEndOfCell_(
	xtf::extended_text_iflow___ &Flow,
	separator__ Separator,
	bso__bool &EOX,
	bso__char &C )
{
	return !( EOX = Flow.EOX() ) && ( ( C = Flow.Get() ) != Separator ) && ( C != '\n' ) && ( C != '\r' );
}

static inline txmtbl::delimiter GetDelimiter_( 
	xtf::extended_text_iflow___ &Flow,
	separator__ Separator,
	bso__bool EOX,
	bso__char C )
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
	xtf::extended_text_iflow___ &Flow,
	cell_ &Cell,
	separator__ Separator )
{
	bso__char C = 0;
	bso__bool EOX = false;

	Cell.Init();

	Cell.Location( Flow.Column() );

	while( IsNotEndOfCell_( Flow, Separator,EOX,C ) ) 
		Cell.Add( C );
	
	return GetDelimiter_( Flow, Separator, EOX, C );
}

txmtbl::delimiter txmtbl::SkipCell(
	xtf::extended_text_iflow___ &Flow,
	separator__ Separator )
{
	bso__char C = 0;
	bso__bool EOX = false;

	while( IsNotEndOfCell_( Flow, Separator,EOX,C ) );

	return GetDelimiter_( Flow, Separator, EOX, C );
}

bso__bool txmtbl::GetLine(
	xtf::extended_text_iflow___ &Flow,
	line_ &Line,
	separator__ Separator )
{
ERRProlog
	cell Cell;
	bso__bool Loop;
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

amount__ line_::DeleteEmptyCells( void )
{
	ctn::E_CMITEM( cell_ ) Cell;
	tym::row__ Temp, Current = First();
	amount__ Amount = 0;

	Cell.Init( *this );

	while( Current != NONE )
	{
		Temp = Current;
		Current = Next( Current );

		if ( !Cell( Temp ).Amount() )
		{
			DeleteCell_( Temp );
			Amount++;
		}
	}

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

amount__ line_::DeleteHeadingEmptyCells( void )
{
	tym::row__ Current = FirstNonEmptyCell(), Temp;
	amount__ Amount = 0;

	if ( Current != NONE )
	{
		Current = Previous( Current );

		while( Current != NONE )
		{
			Temp = Current;

			Current = Previous( Current );

			DeleteCell_( Temp );

			Amount++;
		}
	}
	else if ( this->Amount() )
		DeleteAllCells();

	return Amount;
}

amount__ line_::DeleteTailingEmptyCells( void )
{
	tym::row__ Current = LastNonEmptyCell(), Temp;
	amount__ Amount = 0;

	if ( Current != NONE )
	{
		Current = Next( Current );

		while( Current != NONE )
		{
			Temp = Current;

			Current = Next( Current );

			DeleteCell_( Temp );

			Amount++;
		}
	}
	else if ( this->Amount() )
		DeleteAllCells();

	return Amount;
}

amount__ line_::DeleteCentralEmptyCells( void )
{
	ctn::E_CMITEM( cell_ ) Cell;
	tym::row__
		Current = FirstNonEmptyCell(),
		Last = LastNonEmptyCell(),
		Temp;
	amount__ Amount = 0;

	Cell.Init( *this );

	if ( Current != Last )
	{
		Current = Next( Current );

		while( Current != Last )
		{
			Temp = Current;

			Current = Next( Current );

			if ( !Cell( Temp ).Amount() )
			{
				DeleteCell_( Temp );
				Amount++;
			}
		}
	}
	else if ( this->Amount() && ( Current == NONE ) )
		DeleteAllCells();

	return Amount;
}

amount__ line_::DeleteCellsAt( tym::row__ Position )
{
	tym::row__ Temp;
	amount__ Amount = 0;

	while( Position != NONE )
	{
		Temp = Position;

		Position = Next( Position );

		DeleteCell_( Temp );

		Amount++;
	}

	return Amount;
}

static inline bool IsCommentary_(
	const cell_ &Cell,
	bso__char Marker )
{
	return Cell.Amount() && ( Cell( 0 ) == Marker );
}

amount__ line_::DeleteCommentary( bso__char Marker )
{
	tym::row__ Position = First();
	ctn::E_CMITEM( cell_ ) Cell;

	Cell.Init( *this );

	while( ( Position != NONE ) && !IsCommentary_( Cell( Position ), Marker ) )
		Position = Next( Position );

	if ( Position != NONE )
		return DeleteCellsAt( Position );
	else
		return 0;
}

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &Flow,
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

bso__bool txmtbl::GetFirstNonEmptyLine(
	xtf::extended_text_iflow___ &Flow,
	line_ &Line,
	separator__ Separator )
{
	if ( Flow.EOX() )
		return false;
	else
	{
		do
		{
			Line.Init();
			GetLine( Flow, Line );

			Line.DeleteEmptyCells();

		} while( !Line.Amount() && !Flow.EOX() );

		return Line.Amount() != 0;
	}
}


void txmtbl::GetTable(
	xtf::extended_text_iflow___ &Flow,
	table_ &Table,
	separator__ Separator )
{
ERRProlog
	line Line;
ERRBegin
	while( !Flow.EOX() )
	{
		Line.Init();

		Line.Location( Flow.Line() );

		GetLine( Flow, Line, Separator );

		Table.AddLine( Line );
	}
ERRErr
ERREnd
ERREpilog
}

void table_::DeleteEmptyCells( void )
{
	tym::row__ Current = First();

	while( Current != NONE )
	{
		lines_::operator ()( Current ).DeleteEmptyCells();

		Current = Next( Current );
	}

	lines_::Sync();
}

void table_::DeleteHeadingEmptyCells( void )
{
	tym::row__ Current = First();

	while( Current != NONE )
	{
		lines_::operator ()( Current ).DeleteHeadingEmptyCells();

		Current = Next( Current );
	}

	lines_::Sync();
}

void table_::DeleteTailingEmptyCells( void )
{
	tym::row__ Current = First();

	while( Current != NONE )
	{
		lines_::operator ()( Current ).DeleteTailingEmptyCells();

		Current = Next( Current );
	}

	lines_::Sync();
}

void table_::DeleteCentralEmptyCells( void )
{
	tym::row__ Current = First();

	while( Current != NONE )
	{
		lines_::operator()( Current ).DeleteCentralEmptyCells();

		Current = Next( Current );
	}

	lines_::Sync();
}

void table_::DeleteCommentaries( bso__char Marker )
{
	tym::row__ Current = First();

	while( Current != NONE )
	{
		lines_::operator()( Current ).DeleteCommentary( Marker );

		Current = Next( Current );
	}

	lines_::Sync();
}

amount__ table_::DeleteEmptyLines( void )
{
	ctn::E_CITEM( line_ ) Line;
	tym::row__ Current = First(), Temp;
	amount__ Amount = 0;

	Line.Init( *this );

	while( Current != NONE )
	{
		Temp = Current;

		Current = Next( Current );

		if ( !Line( Temp).Amount() )
		{
			DeleteLine_( Temp );
			Amount++;
		}
	}

	return Amount;

}

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &Flow,
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
