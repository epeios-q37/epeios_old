/*
  Header for the 'txmtbl' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

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
//C Claude L. SIMON (simon@epeios.org)
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
	typedef tym::size__	amount__;

	//d The default cell separator character.
	#define TXMTBL_DEFAULT_CELL_SEPARATOR	'\t'
	
	//d The default escape character.
	#define TXMTBL_DEFAULT_ESCAPE_CHARACTER	'\\'

	//t A location (line or column).
	typedef xtf::location	location__;

	//t A separator char.
	typedef bso__char	separator__;

	//t A escape char.
	typedef bso__char	escape__;

	//c A cell.
	class cell_
	: public str::string_
	{
	public:
		struct s
		: public str::string_::s
		{
			location__ Location;
		} &S_;
		cell_( s &S )
		: S_( S ),
		  str::string_( S )
		{}
		void reset( bso__bool P = true )
		{
			string_::reset( P );
			S_.Location = 0;
		}
		void plug( mmm::multimemory_ &M )
		{
			string_::plug( M );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
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

	AUTO( cell )

	/*f Put in 'Cell' the next cell in flow 'Flow'. 'Separator' delimits the celles.
	Return the delimiter of the cell. */
	txmtbl::delimiter GetCell(
		xtf::extended_text_iflow___ &Flow,
		cell_ &Cell,
		separator__ Separator = TXMTBL_DEFAULT_CELL_SEPARATOR,
		escape__ Escape = TXMTBL_DEFAULT_ESCAPE_CHARACTER );

	/*f Skip next cell in flow 'Flow'. 'Separator' delimits the cells.
	Return the delimiter of the cell. */
	txmtbl::delimiter SkipCell(
		xtf::extended_text_iflow___ &Flow,
		separator__ Separator = TXMTBL_DEFAULT_CELL_SEPARATOR,
		escape__ Escape = TXMTBL_DEFAULT_ESCAPE_CHARACTER );

	typedef ctn::E_MCONTAINER_( cell_ ) cells_;
	typedef lst::E_LIST_ list_;


	//c A line of cells
	class line_
	: public list_,
	  public cells_
	{
	private:
		// Supprimer la cellule à la position 'Position'.
		void DeleteCell_( tym::row__ Position )
		{
			cells_::operator()( Position ).Allocate( 0 );
			cells_::Sync();

			list_::Remove( Position );
		}
	protected:
		virtual void LSTAllocate( tym::size__ Size )
		{
			cells_::Allocate( Size, aem::mFit );
		}
	public:
		struct s
		: public list_::s,
		  public cells_::s
		{
			location__ Location;
		} &S_;
		line_( s &S )
		: S_( S ),
		  lst::E_LIST_( S ),
		  ctn::E_MCONTAINER_( cell_ )( S )
		{}
		void reset( bool P = true )
		{
			list_::reset( P );
			cells_::reset( P );

			S_.Location = 0;
		}
		void plug( mmm::multimemory_ &M )
		{
			list_::plug( M );
			cells_::plug( M );
		}
		line_ &operator =( const line_ &L )
		{
			list_::operator =( L );
			cells_::operator =( L );

			S_.Location = L.S_.Location;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			list_::Init();
			cells_::Init();

			S_.Location = 0;
		}
		//f Write 'Cell' and 'Location' at 'Position'.
		void Write(
			const cell_ &Cell,
			location__ Location,
			tym::row__ Position )
		{
			cells_::Write( Cell, Position );
			cells_::Sync();

			S_.Location = Location;
		}
		//f Add 'Cell' and 'Location'. Return position where added.
		tym::row__ Add(
			const cell_ &Cell,
			location__ Location )
		{
			tym::row__ P = list_::CreateEntry();

			Write( Cell, Location, P );

			return P;
		}
		//f Return the position of the first non-empty cell.
		tym::row__ FirstNonEmptyCell( void ) const;
		//f Return the position of the last non-empty cell.
		tym::row__ LastNonEmptyCell( void ) const;
		//f Delete all empty cells. Retunr amount of cells deleted.
		amount__ DeleteEmptyCells( void );
		//f Delete all heading empty cells. Return amount of cells deleted.
		amount__ DeleteHeadingEmptyCells( void );
		//f Delete all tailing empty cells. Return amount of cells deleted.
		amount__ DeleteTailingEmptyCells( void );
		//f Delete all empty cells between the first and last non-empty cells. Return amount of cells deleted.
		amount__ DeleteCentralEmptyCells( void );
		//f Delete all cells from 'Position'. Return amount of cells deleted.
		amount__ DeleteCellsAt( tym::row__ Position );
		//f Delete all cells. Return amount of cells deleted.
		amount__ DeleteAllCells( void )
		{
			tym::row__ P = list_::First();

			if ( P != NONE )
				return DeleteCellsAt( P );
			else
				return 0;
		}
		/*f Delete the cells beginnig with 'Marker' and all following cells from the same line.
		Return amount of cell deleted.*/
		amount__ DeleteCommentary( bso__char Marker );
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
		NAV( list_:: )
	};

	AUTO( line )

	/*f Put in 'Line' current line in 'Flow' and return true, or false if there is no
	more line. Cells are separated by 'Separator'.*/
	bso__bool GetLine(
		xtf::extended_text_iflow___ &Flow,
		line_ &Line,
		separator__ Separator = TXMTBL_DEFAULT_CELL_SEPARATOR,
		escape__ Escape = TXMTBL_DEFAULT_ESCAPE_CHARACTER );

	/*f Put in 'Line' the first non-empty line in 'Flow' and return true, or false if there is no
	more line. Cells are separated by 'Separator'.*/
	bso__bool GetFirstNonEmptyLine(
		xtf::extended_text_iflow___ &Flow,
		line_ &Line,
		separator__ Separator = TXMTBL_DEFAULT_CELL_SEPARATOR,
		escape__ Escape = TXMTBL_DEFAULT_ESCAPE_CHARACTER );


	typedef ctn::E_CONTAINER_( line_ ) lines_;

	//c A table.
	class table_
	: public list_,
	  public lines_
	{
	private:
		// Delete line at position 'Position'.
		void DeleteLine_( tym::row__ Position )
		{
			lines_::operator ()(Position).DeleteAllCells();
			lines_::Sync();

			list_::Remove( Position );
		}
	protected:
		virtual void LSTAllocate( tym::size__ Size )
		{
			lines_::Allocate( Size );
		}
	public:
		struct s
		: public list_::s,
		  public lines_::s
		{};
		table_( s &S )
		: list_( S ),
		  lines_( S )
		{}
		void reset( bool P = true )
		{
			list_::reset( P );
			lines_::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			list_::plug( M );
			lines_::plug( M );
		}
		table_ &operator =( const table_ &T )
		{
			list_::operator =( T );
			lines_::operator =( T );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			list_::Init();
			lines_::Init();
		}
		//f Add 'Line'. Return position where added.
		tym::row__ AddLine(	const line_ &Line )
		{
			tym::row__ P = list_::CreateEntry();

			lines_::Write( Line, P );
			lines_::Sync();

			return P;
		}
		//f Delete the line at 'Position'.
		void DeleteLine( tym::row__ Position )
		{
			DeleteLine_( Position );
		}
		//f Delete all emty lines. Return amount of lines deleted.
		amount__ DeleteEmptyLines( void );
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
		NAV( list_:: )
	};

	AUTO( table )

	//f Put in 'Table' the line contained in 'Flow'. Cells are separated by 'Separator'.
	void GetTable(
		xtf::extended_text_iflow___ &Flow,
		table_ &Table,
		separator__ Separator = TXMTBL_DEFAULT_CELL_SEPARATOR,
		escape__ Escape = TXMTBL_DEFAULT_ESCAPE_CHARACTER );
}

txf::text_oflow___ &operator <<(
	txf::text_oflow___ &Flow,
	const txmtbl::line_ &Line );
	
txf::text_oflow___ &operator <<(
		txf::text_oflow___ &Flot,
		const txmtbl::table_ &Table );




/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
