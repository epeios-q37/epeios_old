/*
  Header for the 'txmtbl' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef TXMTBL__INC
#define TXMTBL__INC

#define TXMTBL_NAME		"TXMTBL"

#define	TXMTBL_VERSION	"$Revision$"	

#define TXMTBL_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &TXMTBLTutor;

#if defined( XXX_DBG ) && !defined( TXMTBL_NODBG )
#define TXMTBL_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

//D TeXt Manager TaBLe. Table like text flow manager.

#include "err.h"
#include "flw.h"
#include "str.h"
#include "lst.h"
#include "ctn.h"
#include "xtf.h"

struct txmtbl
{
	//e The different delimiter.
	enum delimiter
	{
		//i End of line.
		dEOL,
		//i Separator
		dSeparator,
		//i End of flow
		dEOF,
		//i Unknow delimiter.
		dUnknow
	};
};

//t An amount of cell or of line.
typedef SIZE__	txmtbl__amount;

//d The default cell separator.
#define TXMTBL_DEFAULT_CELL_SEPARATOR	'\t'

//t A location (line or column).
typedef xtf::location	txmtbl__location;

//t A separator.
typedef bso__char	txmtbl__separator;

//c A cell.
class txmtbl_cell_
: public str_string_
{
public:
	struct s
	: public str_string_::s
	{
		txmtbl__location Location;
	} &S_;
	txmtbl_cell_( s &S )
	: S_( S ),
	  str_string_( S )
	{}
	void reset( bso__bool P = true )
	{
		str_string_::reset( P );
		S_.Location = 0;
	}
	void plug( mmm_multimemory_ &M )
	{
		str_string_::plug( M );
	}
	void plug( MEMORY_DRIVER_ &MD )
	{
		str_string_::plug( MD );
	}
	txmtbl_cell_ &operator =( const txmtbl_cell_ &C )
	{
		str_string_::operator =( C );

		S_.Location = C.S_.Location;

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		str_string_::Init();

		S_.Location = 0;
	}
	//f 'Location' becomes the location.
	void Location( txmtbl__location Location )
	{
		S_.Location = Location;
	}
	//f Return location of the cell.
	txmtbl__location Location( void ) const
	{
		return S_.Location;
	}
};

AUTO( txmtbl_cell )

/*f Put in 'Cell' the next cell in flow 'Flow'. 'Separator' delimits the celles.
Return the delimiter of the cell. */
txmtbl::delimiter TXMTBLGetCell(
	xtf::extended_text_iflow___ &Flow,
	txmtbl_cell_ &Cell,
	txmtbl__separator Separator = TXMTBL_DEFAULT_CELL_SEPARATOR );

/*f Skip next cell in flow 'Flow'. 'Separator' delimits the cells.
Return the delimiter of the cell. */
txmtbl::delimiter TXMTBLSkipCell(
	xtf::extended_text_iflow___ &Flow,
	txmtbl__separator Separator = TXMTBL_DEFAULT_CELL_SEPARATOR );


//c A line of cells
class txmtbl_line_
: public LIST_,
  public MCONTAINER_( txmtbl_cell_ )
{
private:
	// Supprimer la cellule à la position 'Position'.
	void DeleteCell_( POSITION__ Position )
	{
		MCONTAINER_( txmtbl_cell_ )::operator()( Position ).Allocate( 0 );
		MCONTAINER_( txmtbl_cell_ )::Sync();

		LIST_::Remove( Position );
	}
protected:
	virtual void LSTAllocate( SIZE__ Size )
	{
		MCONTAINER_( txmtbl_cell_ )::Allocate( Size );
	}
public:
	struct s
	: public ::LIST_::s,
	  public MCONTAINER_( txmtbl_cell_ )::s
	{
		txmtbl__location Location;
	} &S_;
	txmtbl_line_( s &S )
	: S_( S ),
	  LIST_( S ),
	  MCONTAINER_( txmtbl_cell_ )( S )
	{}
	void reset( bool P = true )
	{
		LIST_::reset( P );
		MCONTAINER_( txmtbl_cell_ )::reset( P );

		S_.Location = 0;
	}
	void plug( mmm_multimemory_ &M )
	{
		LIST_::plug( M );
		MCONTAINER_( txmtbl_cell_ )::plug( M );
	}
	txmtbl_line_ &operator =( const txmtbl_line_ &L )
	{
		LIST_::operator =( L );
		MCONTAINER_( txmtbl_cell_ )::operator =( L );

		S_.Location = L.S_.Location;

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		LIST_::Init();
		MCONTAINER_( txmtbl_cell_ )::Init();

		S_.Location = 0;
	}
	//f Write 'Cell' and 'Location' at 'Position'.
	void Write(
		const txmtbl_cell_ &Cell,
		txmtbl__location Location,
		POSITION__ Position )
	{
		MCONTAINER_( txmtbl_cell_ )::Write( Cell, Position );
		MCONTAINER_( txmtbl_cell_ )::Sync();

		S_.Location = Location;
	}
	//f Add 'Cell' and 'Location'. Return position where added.
	POSITION__ Add(
		const txmtbl_cell_ &Cell,
		txmtbl__location Location )
	{
		POSITION__ P = LIST_::CreateEntry();

		Write( Cell, Location, P );

		return P;
	}
	//f Return the position of the first non-empty cell.
	POSITION__ FirstNonEmptyCell( void ) const;
	//f Return the position of the last non-empty cell.
	POSITION__ LastNonEmptyCell( void ) const;
	//f Delete all empty cells. Retunr amount of cells deleted.
	txmtbl__amount DeleteEmptyCells( void );
	//f Delete all heading empty cells. Return amount of cells deleted.
	txmtbl__amount DeleteHeadingEmptyCells( void );
	//f Delete all tailing empty cells. Return amount of cells deleted.
	txmtbl__amount DeleteTailingEmptyCells( void );
	//f Delete all empty cells between the first and last non-empty cells. Return amount of cells deleted.
	txmtbl__amount DeleteCentralEmptyCells( void );
	//f Delete all cells from 'Position'. Return amount of cells deleted.
	txmtbl__amount DeleteCellsAt( POSITION__ Position );
	//f Delete all cells. Return amount of cells deleted.
	txmtbl__amount DeleteAllCells( void )
	{
		POSITION__ P = LIST_::First();

		if ( P != NONE )
			return DeleteCellsAt( P );
		else
			return 0;
	}
	/*f Delete the cells beginnig with 'Marker' and all following cells from the same line.
	Return amount of cell deleted.*/
	txmtbl__amount DeleteCommentary( bso__char Marker );
	//f 'Location' becomes the location.
	void Location( txmtbl__location Location )
	{
		S_.Location = Location;
	}
	//f Return location of the line.
	txmtbl__location Location( void ) const
	{
		return S_.Location;
	}
	FNLPA( LIST_:: )
};

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &Flow,
	const txmtbl_line_ &Line );

AUTO( txmtbl_line )

/*f Put in 'Line' current line in 'Flow' and return true, or false if there is no
more line. Cells are separated by 'Separator'.*/
bso__bool TXMTBLGetLine(
	xtf::extended_text_iflow___ &Flow,
	txmtbl_line_ &Line,
	txmtbl__separator Separator = TXMTBL_DEFAULT_CELL_SEPARATOR );

/*f Put in 'Line' the first non-empty line in 'Flow' and return true, or false if there is no
more line. Cells are separated by 'Separator'.*/
bso__bool TXMTBLGetFirstNonEmptyLine(
	xtf::extended_text_iflow___ &Flow,
	txmtbl_line_ &Line,
	txmtbl__separator Separator = TXMTBL_DEFAULT_CELL_SEPARATOR );



//c A table.
class txmtbl_table_
: public LIST_,
  public CONTAINER_( txmtbl_line_ )
{
private:
	// Delete line at position 'Position'.
	void DeleteLine_( POSITION__ Position )
	{
		CONTAINER_( txmtbl_line_ )::operator ()(Position).DeleteAllCells();
		CONTAINER_( txmtbl_line_ )::Sync();

		LIST_::Remove( Position );
	}
protected:
	virtual void LSTAllocate( SIZE__ Size )
	{
		CONTAINER_( txmtbl_line_ )::Allocate( Size );
	}
public:
	struct s
	: public ::LIST_::s,
	  public CONTAINER_( txmtbl_line_ )::s
	{};
	txmtbl_table_( s &S )
	: LIST_( S ),
	  CONTAINER_( txmtbl_line_ )( S )
	{}
	void reset( bool P = true )
	{
		LIST_::reset( P );
		CONTAINER_( txmtbl_line_ )::reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		LIST_::plug( M );
		CONTAINER_( txmtbl_line_ )::plug( M );
	}
	txmtbl_table_ &operator =( const txmtbl_table_ &T )
	{
		LIST_::operator =( T );
		CONTAINER_( txmtbl_line_ )::operator =( T );

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		LIST_::Init();
		CONTAINER_( txmtbl_line_ )::Init();
	}
	//f Add 'Line'. Return position where added.
	POSITION__ AddLine(	const txmtbl_line_ &Line )
	{
		POSITION__ P = LIST_::CreateEntry();

		CONTAINER_( txmtbl_line_ )::Write( Line, P );
		CONTAINER_( txmtbl_line_ )::Sync();

		return P;
	}
	//f Delete the line at 'Position'.
	void DeleteLine( POSITION__ Position )
	{
		DeleteLine_( Position );
	}
	//f Delete all emty lines. Return amount of lines deleted.
	txmtbl__amount DeleteEmptyLines( void );
	//f Delete all empty cells from all lines.
	void DeleteEmptyCells( void );
	//f Delete all heading cells from all the lines.
	void DeleteHeadingEmptyCells( void );
	//f Delete all tailing cells from all lines.
	void DeleteTailingEmptyCells( void );
	//f Delete all empty cells between the first and last non-empty cells from all the lines.
	void DeleteCentralEmptyCells( void );
	//f Delete, for each line, the cells beginning with 'Marker' and all following cells.
	void DeleteCommentaries( bso__char Marker );
	FNLPA( LIST_:: )
};

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &Flot,
	const txmtbl_table_ &Table );

AUTO( txmtbl_table )

//f Put in 'Table' the line contained in 'Flow'. Cells are separated by 'Separator'.
void TXMTBLGetTable(
	xtf::extended_text_iflow___ &Flow,
	txmtbl_table_ &Table,
	txmtbl__separator Separator = TXMTBL_DEFAULT_CELL_SEPARATOR );

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
