/*
	Header for the 'utf' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20132004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef UTF__INC
#define UTF__INC

#define UTF_NAME		"UTF"

#define	UTF_VERSION	"$Revision$"

#define UTF_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &UTFTutor;

#if defined( XXX_DBG ) && !defined( UTF_NODBG )
#define UTF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D UCS transformation format 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "tol.h"
# include "bom.h"

namespace utf {

	class ansi__
	{
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CDTOR( ansi__ )
		void Init( void )
		{
			// Standardisation.
		}
		fdr::size__ Handle(
			const fdr::datum__ *Datum,
			fdr::size__ Size )
		{
			if ( Size == 0 )
				ERRFwk();

			return 1;
		}
	};

	inline bso::u8__ FindLast0_( fdr::datum__ C )
	{
		bso::u8__ Pos = 8;

		while ( Pos-- && ( ( C & ( 1 << Pos ) ) != 0 ) );

		return Pos + 1;
	}

	inline bso::u8__ CountUTF8ContinuationByte_(
		const fdr::datum__ *Datum,
		fdr::size__ Size )
	{
		bso::u8__ Amount = 0;

		while ( Size-- && ( ( Datum[Amount] & 0xC0 ) == 0x80 ) )
			Amount++;

		return Amount;
	}

	class utf8__ 
	{
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		E_CDTOR( utf8__ )
		void Init( void )
		{
			// Standardisation.
		}
		fdr::size__ Handle(
			const fdr::datum__ *Datum,
			fdr::size__ Size )
		{
			bso::u8__ LeadingByteLast0Position = 0;

			if ( Size == 0 )
				ERRFwk();	// Erreur.

			LeadingByteLast0Position = FindLast0_( Datum[0] );

			switch ( LeadingByteLast0Position ) {
			case 8:
				return 1;
				break;
			case 7:
				return 0;
				break;
			case 6:
			case 5:
			case 4:
			{
				bso::u8__ ContinuationByteCount = CountUTF8ContinuationByte_( Datum + 1, Size - 1 );

				if ( ( 7 - LeadingByteLast0Position ) != ContinuationByteCount  ) 
					return 0;
				else
					return ContinuationByteCount + 1;
				break;
			}
			case 3:
			case 2:
			case 1:
			case 0:
				return 0;
				break;
			default:
				ERRFwk();
				break;
			}
		}
	};

	enum format__ {
		fANSI,
		fUTF_8,
		fUTF_16_LE,
		fUTF_16_BE,
		fUTF_32_LE,
		fUTF_32_BE,
		f_amount,
		f_Undefined,
		f_Guess,	// Tenter de deviner si c'est de l'ANSI ou de l'UTF-8.
		f_Default = f_Guess
	};

	class utf__ {
	private:
		format__ _Format;
		ansi__ _ANSI;
		utf8__ _UTF8;
	public:
		void reset( bso::bool__ P = true )
		{
			_Format = f_Undefined;
			_ANSI.reset( P );
			_UTF8.reset( P );
		}
		E_CDTOR( utf__ );
		bso::bool__ Init( format__ Format )	// Retourne 'false' si format non supporté, 'true' sinon.
		{
			_Format = Format;

			switch( _Format ) {
			case f_Guess:
				_UTF8.Init();
			case fANSI:
				_ANSI.Init();
				break;
			case fUTF_8:
				_UTF8.Init();
				break;
			case fUTF_16_LE:
			case fUTF_16_BE:
			case fUTF_32_LE:
			case fUTF_32_BE:
				return false;
				break;
			default:
				ERRPrm();
				break;
			}

			return true;
		}
		bso::size__ Handle(
			const fdr::datum__ *Datum,
			bso::size__ Size )	// 5, sauf si EOF !
		{
			if ( Size == 0 )
				ERRFwk();

			switch( _Format ) {
			case f_Guess:
			{
				bso::u8__ UTFSize = _UTF8.Handle( Datum, Size );

				if ( UTFSize > 1 )
					if ( Size > UTFSize )
						switch ( Datum[UTFSize] >> 6 ) {
						case 0:
						case 1:
							break;
						case 2:
							UTFSize = 0;
							break;
						case 3:
							if ( FindLast0_( Datum[UTFSize] ) < 4 )
								UTFSize = 0;
							break;
						default:
							ERRFwk();
							break;
						}

				if ( UTFSize  == 0 ) {
					_Format = fANSI;
					_ANSI.Init();
					UTFSize = _ANSI.Handle( Datum, Size );
				} else if ( UTFSize > 1 )
					_Format = fUTF_8;

				return UTFSize;
				break;
			}
			case fANSI:
				return _ANSI.Handle( Datum, Size );
				break;
			case fUTF_8:
				return _UTF8.Handle( Datum, Size );
				break;
			default:
				ERRPrm();
				break;
			}

			return 0;	// Pour éviter iun 'warning'.
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
