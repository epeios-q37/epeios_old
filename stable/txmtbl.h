/*
	Header for the 'txmtbl' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

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

#ifndef TXMTBL__INC
#define TXMTBL__INC

#define TXMTBL_NAME		"TXMTBL"

#define	TXMTBL_VERSION	"$Revision$"

#define TXMTBL_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &TXMTBLTutor;

#if defined( XXX_DBG ) && !defined( TXMTBL_NODBG )
#define TXMTBL_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D TeXt Manager TabBLe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D TeXt Manager TaBLe. Table like text flow manager.

#include "err.h"
#include "flw.h"
#include "str.h"
#include "ctn.h"
#include "xtf.h"
#include "stk.h"

namespace txmtbl {

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

	//t An amount of cell or of line.
	typedef epeios::size__	amount__;

	//d The default cell separator character.
	#define TXMTBL_DEFAULT_CELL_SEPARATOR	'\t'
	
	//d The default escape character.
	#define TXMTBL_DEFAULT_ESCAPE_CHARACTER	'\\'

	//t A location (line or column).
	typedef xtf::location__	location__;

	//t A separator char.
	typedef bso::char__	separator__;

	//t A escape char.
	typedef bso::char__	escape__;

	using str::string_;

	//c A cell.
	class cell_
	: public string_
	{
	public:
		struct s
		: public string_::s
		{
			location__ Location;
		} &S_;
		cell_( s &S )
		: S_( S ),
		  str::string_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			string_::reset( P );
			S_.Location = 0;
		}
		void plug( mmm::multimemory_ &M )
		{
			string_::plug( M );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			string_::plug( MD );
		}
		cell_ &operator =( const cell_ &C )
		{
			string_::operator =( C );

			S_.Location = C.S_.Location;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			string_::Init();

			S_.Location = 0;
		}
		//f 'Location' becomes the location.
		void Location( location__ Location )
		{
			S_.Location = Location;
		}
		//f Return location of the cell.
		location__ Location( void ) const
		{
			return S_.Location;
		}
	};

	E_AUTO( cell )

	/*f Put in 'Cell' the next cell in flow 'Flow'. 'Separator' delimits the celles.
	Return the delimiter of the cell. */
	txmtbl::delimiter GetCell(
		xtf::extended_text_iflow__ &Flow,
		cell_ &Cell,
		separator__ Separator = TXMTBL_DEFAULT_CELL_SEPARATOR,
		escape__ Escape = TXMTBL_DEFAULT_ESCAPE_CHARACTER );

	/*f Skip next cell in flow 'Flow'. 'Separator' delimits the cells.
	Return the delimiter of the cell. */
	txmtbl::delimiter SkipCell(
		xtf::extended_text_iflow__ &Flow,
		separator__ Separator = TXMTBL_DEFAULT_CELL_SEPARATOR,
		escape__ Escape = TXMTBL_DEFAULT_ESCAPE_CHARACTER );

	typedef stk::E_BSTACK_( epeios::row__ ) stack_;
	typedef stk::E_BSTACK( epeios::row__ ) stack;

	typedef ctn::E_MCONTAINER_( cell_ ) cells_;

	//c A line of cells
	class line_
	: public cells_
	{
	private:
		void Erase_( stack_ &Stack );
	public:
		struct s
		: public cells_::s
		{
			location__ Location;
		} &S_;
		line_( s &S )
		: S_( S ),
		  ctn::E_MCONTAINER_( cell_ )( S )
		{}
		void reset( bool P = true )
		{
			cells_::reset( P );

			S_.Location = 0;
		}
		void plug( mmm::multimemory_ &M )
		{
			cells_::plug( M );
		}
		line_ &operator =( const line_ &L )
		{
			cells_::operator =( L );

			S_.Location = L.S_.Location;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			cells_::Init();

			S_.Location = 0;
		}
		//f Write 'Cell' and 'Location' at 'Position'.
		void Write(
			const cell_ &Cell,
			location__ Location,
			epeios::row__ Position )
		{
			cells_::Store( Cell, Position );
			cells_::Flush();

			S_.Location = Location;
		}
		//f Add 'Cell' and 'Location'. Return position where added.
		epeios::row__ Add(
			const cell_ &Cell,
			location__ Location )
		{
			epeios::row__ P = cells_::New();

			Write( Cell, Location, P );

			return P;
		}
		//f Return the position of the first non-empty cell.
		epeios::row__ FirstNonEmptyCell( void ) const;
		//f Return the position of the last non-empty cell.
		epeios::row__ LastNonEmptyCell( void ) const;
		//f Remove all empty cells. Retunr amount of cells removed.
		amount__ RemoveEmptyCells( void );
		//f Remove all heading empty cells. Return amount of cells removed.
		amount__ RemoveHeadingEmptyCells( void );
		//f Remove all tailing empty cells. Return amount of cells removed.
		amount__ RemoveTailingEmptyCells( void );
		//f Remove all empty cells between the first and last non-empty cells. Return amount of cells removed.
		amount__ RemoveCentralEmptyCells( void );
		//f Remove all cells from 'Position'. Return amount of cells removed.
		amount__ RemoveCellsAt( epeios::row__ Position );
		//f Remove all cells. Return amount of cells removed.
		amount__ RemoveAllCells( void )
		{
			epeios::row__ P = cells_::First();

			if ( P != NONE )
				return RemoveCellsAt( P );
			else
				return 0;
		}
		/*f Remove the cells beginnig with 'Marker' and all following cells from the same line.
		Return amount of cell removed.*/
		amount__ RemoveComment( bso::char__ Marker );
		//f Remove all non-significant cells (empty and comment).
		void Purge( bso::char__ CommentMarker )
		{
			RemoveComment( CommentMarker );
			RemoveEmptyCells();
		}
		//f 'Location' becomes the location.
		void Location( location__ Location )
		{
			S_.Location = Location;
		}
		//f Return location of the line.
		location__ Location( void ) const
		{
			return S_.Location;
		}
		/*f Return the unique cell in 'Cell'. Undefined result if there is no
		or more than one cell. */
		bso::bool__ GetUniqueCell( cell_ &Cell ) const
		{
			if ( Amount() != 1 )
				return false;

			Recall( First(), Cell );

			return true;
		}
		E_XNAV( cells_:: )
	};

	E_AUTO( line )

	/*f Put in 'Line' current line in 'Flow' and return true, or false if there is no
	more line. Cells are separated by 'Separator'.*/
	bso::bool__ GetLine(
		xtf::extended_text_iflow__ &Flow,
		line_ &Line,
		separator__ Separator = TXMTBL_DEFAULT_CELL_SEPARATOR,
		escape__ Escape = TXMTBL_DEFAULT_ESCAPE_CHARACTER );

	/*f Put in 'Line' the first non-empty line in 'Flow' and return true, or false if there is no
	more line. Cells are separated by 'Separator'.*/
	bso::bool__ GetFirstNonEmptyLine(
		xtf::extended_text_iflow__ &Flow,
		line_ &Line,
		separator__ Separator = TXMTBL_DEFAULT_CELL_SEPARATOR,
		escape__ Escape = TXMTBL_DEFAULT_ESCAPE_CHARACTER );


	typedef ctn::E_CONTAINER_( line_ ) lines_;

	//c A table.
	class table_
	: public lines_
	{
	private:
		void Erase_( stack_ &Stack );
	public:
		struct s
		: public lines_::s
		{};
		table_( s &S )
		: lines_( S )
		{}
		void reset( bool P = true )
		{
			lines_::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			lines_::plug( M );
		}
		table_ &operator =( const table_ &T )
		{
			lines_::operator =( T );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			lines_::Init();
		}
		//f Add 'Line'. Return position where added.
		epeios::row__ AddLine(	const line_ &Line )
		{
			epeios::row__ P = lines_::New();

			lines_::Store( Line, P );
			lines_::Flush();

			return P;
		}
		//f Remove the line at 'Position'.
		void RemoveLine( epeios::row__ Position )
		{
			lines_::Remove( Position );
		}
		//f Remove all emty lines. Return amount of lines removed.
		amount__ RemoveEmptyLines( void );
		//f Remove all empty cells from all lines.
		void RemoveEmptyCells( void );
		//f Remove all heading cells from all the lines.
		void RemoveHeadingEmptyCells( void );
		//f Remove all tailing cells from all lines.
		void RemoveTailingEmptyCells( void );
		//f Remove all empty cells between the first and last non-empty cells from all the lines.
		void RemoveCentralEmptyCells( void );
		//f Remove, for each line, the cells beginning with 'Marker' and all following cells.
		void RemoveComments( bso::char__ Marker );
		//f Remove all non-significant cells (empty and comment).
		void Purge( bso::char__ CommentMarker )
		{
			RemoveComments( CommentMarker );
			RemoveEmptyCells();
			RemoveEmptyLines();
		}
		/*f Return the unique cell in line 'Row' in 'Cell'. Undefined result if there is no
		or more than one cell. */
		bso::bool__ GetUniqueCell(
			epeios::row__ Row,
			cell_ &Cell ) const
		{
			ctn::E_CITEM( line_ ) Line;

			Line.Init( *this );

			return Line( Row ).GetUniqueCell( Cell );
		}
		location__ Location( epeios::row__ Row )
		{
			ctn::E_CITEM( line_ ) Line;

			Line.Init( *this );

			return Line( Row ).Location();
		}

		E_XNAV( lines_:: )
	};

	E_AUTO( table )

	//f Put in 'Table' the line contained in 'Flow'. Cells are separated by 'Separator'.
	void GetTable(
		xtf::extended_text_iflow__ &Flow,
		table_ &Table,
		separator__ Separator = TXMTBL_DEFAULT_CELL_SEPARATOR,
		escape__ Escape = TXMTBL_DEFAULT_ESCAPE_CHARACTER );
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &Flow,
	const txmtbl::line_ &Line );
	
txf::text_oflow__ &operator <<(
		txf::text_oflow__ &Flot,
		const txmtbl::table_ &Table );




/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
