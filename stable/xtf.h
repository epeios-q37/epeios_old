/*
	Header for the 'xtf' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef XTF__INC
#define XTF__INC

#define XTF_NAME		"XTF"

#define	XTF_VERSION	"$Revision$"

#define XTF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &XTFTutor;

#if defined( XXX_DBG ) && !defined( XTF_NODBG )
#define XTF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D eXtended Text Flow. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D eXtended Text Flow. Text flow with extended features.

#include "err.h"
#include "bso.h"
#include "str.h"

//d The default cell separator.
#define XTF_DEFAULT_CELL_SEPARATOR	'\t'

namespace xtf {
	//t type of position in a text (line or column).
	typedef bso::ulong__ location__;

	struct coord__ {
		location__ Line;
		location__ Column;
		void reset( bso::bool__ = false )
		{
			Line = Column = 0;
		}
		coord__( void )
		{
			reset( true );
		}
		coord__(
			location__ Line,
			location__ Column )
		{
			reset( true );

			this->Line = Line;
			this->Column = Column;
		}
		void Init( coord__ Coord = coord__() )
		{
			reset();

			*this = Coord;
		}
	};

	typedef bso::ubyte__ _amount__;

	//c To handle a text flow, with counting lines and columns.
	class extended_text_iflow__
	{
	private:
		// L'entree de base.
		flw::iflow__ *_Flow;
		// Corrdonnée du prochain caractère.
		coord__ _Coord;
		// '0' if no EOL char encountered, or the value of the EOL char ('\r' or '\n').
		bso::char__ EOL_;
		void NewCharAdjust_( void )
		{
			_Coord.Column++;
		}
		// Adjust counters.after reading a new line character.
		void NewLineAdjust_( void )
		{
			_Coord.Line++;
			_Coord.Column = 0;
		}
	public:
		void reset( bool P = true )
		{
			_Coord.reset( P );
			_Coord.Line = _Coord.Column = 1;
			_Flow = NULL;
			EOL_ = 0;
		}
		E_CVDTOR( extended_text_iflow__ );
		extended_text_iflow__(
			flw::iflow__ &IFlow,
			coord__ Coord = coord__( 1, 0 ) )
		{
			reset( false );

			Init( IFlow, Coord );
		}
		//f Initialization with 'Flow'..
		void Init(
			flw::iflow__ &IFlow,
			coord__ Coord = coord__( 1, 0 ) )
		{
			_Coord.Init( Coord );
			_Flow = NULL;
			EOL_ = 0;

			_Flow = &IFlow;
		}
		//f Extract and return next character in flow.
		flw::datum__ Get( void )
		{
			flw::datum__ C = _Flow->Get();

			if ( EOL_ == 0 ) {
				if ( ( C == '\n' ) || ( C == '\r' ) ) {
					EOL_ = (flw::datum__)C;
					NewLineAdjust_();
				} else {
					NewCharAdjust_();
				}
			} else if ( EOL_ == '\r' ) {
				if ( C == '\n' ) {
					EOL_ = 0;
				} else if ( C == '\r' ) {
					EOL_ = (flw::datum__)C;
					NewLineAdjust_();
				} else {
					EOL_ = 0;
					NewCharAdjust_();
				}
			} else if ( EOL_ == '\n' ) {
				if ( C == '\r' ) {
					EOL_ = 0;
				} else if ( C == '\n' ) {
					EOL_ = (flw::datum__)C;
					NewLineAdjust_();
				} else {
					EOL_ = 0;
					NewCharAdjust_();
				}
			} else
				ERRc();

			return C;
		}
		//f 'C' would be the next character returned by 'get()'.
		void Unget( flw::datum__ C )
		{
			if ( EOL_ )
				EOL_ = 0;

			if ( ( C != '\n' ) && ( C != '\r' ) )
				_Coord.Column--;
			else
			{
				if ( _Coord.Line )
					_Coord.Line--;

				_Coord.Column = 0;
			}

			_Flow->Unget( C );
		}
		//f NOTA : if '.Line' == 0; a '\n' or a '\r' was unget()'.
		const coord__ &Coord( void ) const
		{
			return _Coord;
		}
		/*f Put the rest of the current cell in 'Cell'. Return true if the cell is delimited by 'Separator',
		false otherwise (cell delimited by a new line, for example). */
		bso::bool__ GetCell(
			str::string_ &Cell,
			flw::datum__ Separator = XTF_DEFAULT_CELL_SEPARATOR );
		//f Put rest of the current line in 'Line'. Return true if line ended by a new line, false when EOF.
		bso::bool__ GetLine( str::string_ &Line )
		{
			return GetCell( Line, 0 );
		}
		//f Skip the current line.
		void SkipLine( void )
		{
			GetLine( *(str::string_ *)NULL );
		}
		//f Return the next character in the flow, but let it in the flow.
		flw::datum__ View( bso::bool__ HandleNL = false )
		{
			flw::datum__ C = _Flow->View();

			if ( HandleNL && EOL_ ) {

				if ( ( ( EOL_ == '\r' ) && ( C == '\n' ) ) 
					 || ( EOL_ == '\n' && ( C == '\r' ) ) ) {
						 EOL_ = 0;
						 _Flow->Get();
						 C = _Flow->View();
				}
			}

			return C;
		}
		//f True if at end of text.
		bso::bool__ EndOfFlow( void )
		{
			return _Flow->EndOfFlow();
		}
		//f Return the amount of data red.
		flw::size__ AmountRed( void ) const
		{
			return _Flow->AmountRed();
		}
		void Set( coord__ Coord )
		{
			_Coord = Coord;
		}
		flw::iflow__ &UndelyingFlow( void ) const
		{
			return *_Flow;
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
