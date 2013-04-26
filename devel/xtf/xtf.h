/*
	Header for the 'xtf' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
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

# include "err.h"
# include "bso.h"
# include "str.h"
# include "bom.h"
# include "utf.h"

//d The default cell separator.
#define XTF_DEFAULT_CELL_SEPARATOR	'\t'

// Predeclaration.
namespace lcl {
	class meaning_;
};

namespace xtf {
	//t type of position in a text (line or column).
	typedef bso::uint__ location__;

	enum error__ {
		eUnsupportedEncoding,
		eUnexpectedEncoding,
		eEncodingDiscrepancy,
		e_amount,
		e_Undefined,
		e_NoError
	};

	inline const char *GetLabel( error__ Error )
	{
		switch ( Error ) {
		case eUnsupportedEncoding:
			return XTF_NAME "_UnsupportedEncoding";
			break;
		case eUnexpectedEncoding:
			return XTF_NAME "_UnexpectedEncoding";
			break;
		case eEncodingDiscrepancy:
			return XTF_NAME "_EncodingDiscrepancy";
			break;
		default:
			ERRPrm();
			break;
		}

		return NULL;	// Pour éviter un 'Warning'.
	}

	struct utf__
	{
		fdr::datum__ Data[5];
		bso::u8__ Size;
		void reset( bso::bool__ = true )
		{
			Size = 0;
		}
		E_CDTOR( utf__ )
		void Init( void )
		{
			Size = 0;
		}
	};

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

	typedef bso::u8__ _amount__;

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
		utf::utf__ _UTFHandler;
		utf__ _UTF;
		error__ _Error;
		flw::iflow__ &_F( void ) const
		{
			if ( _Flow == NULL )
				ERRFwk();

			return *_Flow;
		}
		void _NewCharAdjust( void )
		{
			_Coord.Column++;
		}
		// Adjust counters.after reading a new line character.
		void _NewLineAdjust( void )
		{
			_Coord.Line++;
			_Coord.Column = 0;
		}
		bom::byte_order_marker__ _GetBOM( void )
		{
			fdr::datum__ BOMBuffer[BOM_SIZE_MAX];
			fdr::size__ Size = _F().View( sizeof( BOMBuffer ), BOMBuffer );
			bom::byte_order_marker__ BOM = bom::DetectBOM( BOMBuffer, Size );	// Si != 'bom::bom_UnknownOrNone', 'Size' contient au retour la taille du 'BOM'.

			if ( BOM != bom::bom_UnknownOrNone )
				_F().Skip( Size );

			return BOM;
		}
		utf::format__ _HandleFormat(
			utf::format__ ExpectedFormat,
			bom::byte_order_marker__ BOM )
		{
			switch ( ExpectedFormat ) {
			case utf::f_Guess:
				if ( BOM == bom::bomUTF_8 )
					ExpectedFormat = utf::fUTF_8;
				else if ( BOM != bom::bom_UnknownOrNone ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fANSI:
				if ( BOM != bom::bom_UnknownOrNone )
				{
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fUTF_8:
				if ( ( BOM != bom::bom_UnknownOrNone ) && ( BOM != bom::bomUTF_8 ) ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fUTF_16_BE:
				if ( BOM != bom::bomUTF_16_BE ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fUTF_16_LE:
				if ( BOM != bom::bomUTF_16_LE ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fUTF_32_BE:
				if ( BOM != bom::bomUTF_32_BE ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			case utf::fUTF_32_LE:
				if ( BOM != bom::bomUTF_32_LE ) {
					_Error = eUnexpectedEncoding;
					ExpectedFormat = utf::f_Undefined;
				}
				break;
			default:
				ERRPrm();	// Les autres formats ne sont pas accpétés et filtrés en amont.
				break;
			}

			return ExpectedFormat;
		}
		void _SetMeaning( lcl::meaning_ &Meaning );
		bso::bool__ _PrefetchUTF( void )
		{
			if ( _UTF.Size == 0 ) {
				_UTF.Size = _F().View(sizeof( _UTF.Data ), _UTF.Data );

				if ( _UTF.Size == 0 )
					return false;

				_UTF.Size = _UTFHandler.Handle( _UTF.Data, _UTF.Size );

				if ( _UTF.Size == 0 )
					return false;
			}

			return true;
		}
	public:
		void reset( bool P = true )
		{
			_Coord.reset( P );
			_Coord.Line = _Coord.Column = 1;
			_Flow = NULL;
			EOL_ = 0;
			_Error = e_Undefined;
		}
		E_CVDTOR( extended_text_iflow__ );
		extended_text_iflow__(
			flw::iflow__ &IFlow,
			utf::format__ Format,
			coord__ Coord = coord__( 1, 0 ) )
		{
			reset( false );

			Init( IFlow, Format, Coord );
		}
		//f Initialization with 'Flow'..
		bom::byte_order_marker__ Init(
			flw::iflow__ &IFlow,
			utf::format__ Format,
			coord__ Coord = coord__( 1, 0 ) )
		{
			_Coord.Init( Coord );
			_Flow = NULL;
			EOL_ = 0;
			_Flow = &IFlow;
			_Error = e_NoError;

			_UTF.Init();

			bom::byte_order_marker__ BOM = _GetBOM();

			if ( ( Format = _HandleFormat( Format, BOM ) ) != utf::f_Undefined )
				if ( !_UTFHandler.Init( Format ) )
					_Error = eUnsupportedEncoding;

			return BOM;
		}
		//f Extract and return next character in flow.
		flw::datum__ Get( utf__ &UTF )
		{
			if ( !_PrefetchUTF() )
				ERRDta();

			UTF = _UTF;

			_F().Skip( _UTF.Size );

			_UTF.Init();

			flw::datum__ C = _UTF.Data[0];

			if ( EOL_ == 0 ) {
				if ( ( C == '\n' ) || ( C == '\r' ) ) {
					EOL_ = (flw::datum__)C;
					_NewLineAdjust();
				} else {
					_NewCharAdjust();
				}
			} else if ( EOL_ == '\r' ) {
				if ( C == '\n' ) {
					EOL_ = 0;
				} else if ( C == '\r' ) {
					EOL_ = (flw::datum__)C;
					_NewLineAdjust();
				} else {
					EOL_ = 0;
					_NewCharAdjust();
				}
			} else if ( EOL_ == '\n' ) {
				if ( C == '\r' ) {
					EOL_ = 0;
				} else if ( C == '\n' ) {
					EOL_ = (flw::datum__)C;
					_NewLineAdjust();
				} else {
					EOL_ = 0;
					_NewCharAdjust();
				}
			} else
				ERRFwk();

			return C;
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
		flw::datum__ View(
			utf__ &UTF,
			bso::bool__ HandleNL = false )
		{
			if ( !_PrefetchUTF() )
				ERRDta();

			flw::datum__ C = _UTF.Data[0];

			if ( HandleNL && EOL_ ) {

				if ( ( ( EOL_ == '\r' ) && ( C == '\n' ) ) 
					 || ( EOL_ == '\n' && ( C == '\r' ) ) ) {

						EOL_ = 0;

						_F().Skip( _UTF.Size );
						
						if ( !_PrefetchUTF() )
							ERRDta();

						C = _UTF.Data[0];
				}
			}

			UTF = _UTF;

			return C;
		}
		//f True if at end of text.
		bso::bool__ EndOfFlow( error__ &Error )	// Si erreur, 'ErrorMeaning' est initialisé, sinon reste vide.
		{ 

			if ( _Error == e_NoError ) {
				if ( _UTF.Size != 0 )
					return false;

				if ( _F().EndOfFlow() )
					return true;

				if ( !_PrefetchUTF() ) {
					Error = _Error = eEncodingDiscrepancy; 
					return true;
				}

				return false;
			} else {
				Error = _Error;
				return true;
			}
		}
		//f Return the amount of data red.
		flw::size__ AmountRed( void ) const
		{
			return _F().AmountRed();
		}
		void Set( coord__ Coord )
		{
			_Coord = Coord;
		}
		flw::iflow__ &UndelyingFlow( void ) const
		{
			return _F();
		}
		bso::bool__ SetFormat( utf::format__ Format )
		{
			return _UTFHandler.SetFormat( Format );
		}
		utf::format__ Format( void ) const
		{
			return _UTFHandler.Format();
		}

	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
