/*
	Header for the 'cdgb64' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

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

#ifndef CDGB64__INC
#define CDGB64__INC

#define CDGB64_NAME		"CDGB64"

#define	CDGB64_VERSION	"$Revision$"

#define CDGB64_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &CDGB64Tutor;

#if defined( XXX_DBG ) && !defined( CDGB64_NODBG )
#define CDGB64_DBG
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
//D CoDinG Base 64 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "fwf.h"
#include "flw.h"

namespace cdgb64 {

	extern bso::raw__ *Encode;

	inline bso::raw__ Encode_( flw::datum__ Value )
	{
#ifdef CDGB64_DBG
		if ( Value > 63 )
			ERRc();
#endif
		return Encode[Value];
	}

	inline void Encode_(
		const flw::datum__ *Data,
		flw::datum__ *Result )
	{
		Result[0] = Encode_( Data[0]  & 0x3f );
		Result[1] = Encode_( ( ( Data[0] >> 6 ) | ( ( Data[1] << 2 ) & 0x3f ) ) );
		Result[2] = Encode_( ( ( Data[1] >> 4 ) | ( ( Data[2] << 4 ) & 0x3f ) ) );
		Result[3] = Encode_( Data[2] >> 2 );
	}

	typedef fwf::oflow_functions___<> _oflow_functions___;

	class encoding_oflow_functions___
	: public _oflow_functions___
	{
	private:
		fwf::datum__ _Cache[4];
		bso::ubyte__ _Amount;
		flw::oflow__ *_Flow;
	protected:
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Buffer,
			fwf::size__ Maximum )
		{
			fwf::size__ Amount = 0;
			flw::datum__ Item[4];

			switch ( _Amount ) {
			case 0:
				while ( Maximum >= 3 ) {
					Encode_( Buffer, Item );

					_Flow->Write( &Item, 4 );

					Maximum -= 3;
					Buffer += 3;
					Amount += 3;
				}

				switch ( Maximum ) {
				case 2:
					_Cache[1] = Buffer[1];
				case 1:
					_Cache[0] = Buffer[0];
					_Amount = (bso::ubyte__)Maximum;
					Amount += Maximum;
				case 0:
					break;
				default:
					ERRc();
					break;
				}
				break;
			case 1:
				switch( Maximum ) {
				case 0:
					ERRc();
					break;
				case 1:
					_Amount++;
					_Cache[1] = *Buffer;
					Amount = 1;
					break;
				default:
					Item[0] = _Cache[0];
					Item[1] = Buffer[0];
					Item[2] = Buffer[1];

					_Flow->Write( &Item, 4 );

					Amount = 2;

					break;
				};
				break;
			case 2:
				switch( Maximum ) {
				case 0:
					ERRc();
					break;
				default:
					Item[0] = _Cache[0];
					Item[1] = _Cache[2];
					Item[2] = *Buffer;

					_Flow->Write( &Item, 4 );

					Amount = 1;

					break;
				};
				break;
			default:
				ERRc();
				break;
			}

			return Amount;
		}
		virtual void FWFCommit( void )
		{
			flw::datum__ Result[4];

			if ( _Flow == NULL )
				return;

			Encode_( _Cache, Result );

			switch ( _Amount ) {
			case 0:
				return;
				break;
			case 1:
				_Flow->Write( &Result, 2 );
				break;
			case 2:
				_Flow->Write( &Result, 3 );
				break;
			default:
				break;
			}

			_Flow->Put( '=' );

			_Amount = 0;

			_Flow->Commit();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				Commit();

			_oflow_functions___::reset( P );
			_Amount = 0;
			_Flow = NULL;
		}
		encoding_oflow_functions___( void )
		{
			reset( false );
		}
		~encoding_oflow_functions___( void )
		{
			reset();
		}
		void Init(
			flw::oflow__ &Flow,
			fwf::thread_safety__ ThreadSafety = fwf::ts_Default )
		{
			Commit();

			_Amount = 0;
			_Flow = &Flow;

			_oflow_functions___::Init( ThreadSafety );
		}
	};

	class encoding_oflow___
	: public flw::standalone_oflow__<>
	{
	private:
		encoding_oflow_functions___ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			flw::standalone_oflow__<>::reset( P );
			_Functions.reset( P );
		}
		encoding_oflow___( void )
		{
			reset( false );
		}
		~encoding_oflow___( void )
		{
			reset();
		}
		void Init(
			flw::oflow__ &Flow,
			flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			_Functions.Init( Flow, fwf::tsDisabled );
			flw::standalone_oflow__<>::Init( _Functions, AmountMax  );
		}
	};

	extern bso::raw__ *Decode;

	inline bso::raw__ Decode_( bso::raw__ Value )
	{
#ifdef CDGB64_DBG
		if ( Decode[Value] == '@' )
			ERRc();
#endif
		return Decode[Value];
	}

	inline void Decode_(
		const fwf::datum__ *Source,	// Séquence complète de 4 octets en b64, sans '='.
		fwf::datum__ *Target )	// Doit avoir la place suffisante pour recevoir la séquence de 3 octets résultante.
	{
		Target[0] = Decode_( Source[0] ) | ( Decode_( Source[1] ) << 6 );
		Target[1] = ( Decode_( Source[1] ) >> 2 ) | ( Decode_( Source[2] ) << 4 );
		Target[2] = ( Decode_( Source[2] ) >> 4 ) | ( Decode_( Source[3] ) << 2 );
	}

	typedef fwf::iflow_functions___<> _iflow_functions____;

	inline void Decode_(
		fwf::datum__ *Data,
		fwf::size__ Amount )
	{
		const fwf::datum__ *Source = Data;
		fwf::datum__ *Target = Data;
#ifdef CDGB64_DBG
		if ( Amount & 3 )	// Si pas un multiple de 4.
			ERRc();
#endif
		while ( Amount >= 4 ) {
			Decode_( Source, Target );
			Amount -= 4;
			Source += 4;
			Target += 3;
		}
	}

	inline void DecodePartial_(
		const flw::datum__ *Source,
		flw::size__ Amount,
		fwf::datum__ *Target )
	{
		memmove( Target, Source, 4 );

		switch ( Amount ) {
		case 0:
			ERRc();
			break;
		case 1:
			ERRf();
			break;
		case 2:
			Target[2] = 'A';
		case 3:
			Target[3] = 'A';
			break;
		default:
			ERRc();
			break;
		}

		Decode_( Target, 4 );
	}

	class decoding_iflow_functions___
	: public _iflow_functions____
	{
	private:
		flw::iflow__ *_Flow;
	protected:
		virtual fwf::size__ FWFRead(
			fwf::size__ Maximum,
			fwf::datum__ *Buffer )
		{
			bso::size__
				TotalAmount = 0,
				PonctualAmount = 0;

#ifdef CDGB64_DBG
			if ( Maximum < 4 )
				ERRc();
#endif
			Maximum &= ~3UL;	// On veut un multiple de 4.

			TotalAmount = PonctualAmount = _Flow->ReadUpTo( Maximum, Buffer );

			while ( ( PonctualAmount != 0 ) && ( Buffer[TotalAmount-1] != '=' ) && ( TotalAmount & 3 ) )
				TotalAmount += PonctualAmount = _Flow->ReadUpTo( 4 - ( TotalAmount & 3 ), Buffer + TotalAmount );
				// On essaye d'avoir un nombre d'octets multiple de 4.


			if ( TotalAmount != 0 ) {
				if ( Buffer[TotalAmount-1] == '=' )
					TotalAmount--;

				if ( TotalAmount >= 4 )
					Decode_( Buffer, TotalAmount & ~3UL );

				if ( TotalAmount & 3 ) {
#ifdef CDGB64_DBG
					if ( Buffer[TotalAmount] != '=' )
						ERRc();
#endif
					DecodePartial_( Buffer + ( TotalAmount & ~3UL ), TotalAmount & 3, Buffer + 3 * ( TotalAmount >> 2 ) );
				}
			}

			return 3 * ( TotalAmount >> 2 ) + ( ( TotalAmount & 3 ) > 1 ? ( TotalAmount & 3 ) - 1 : 0 ); 
		}
		virtual void FWFDismiss( void )
		{
#ifdef CDGB64_DBG
			if ( _Flow == NULL )
				ERRu();
#endif
			_Flow->Dismiss();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow_functions____::reset( P );
			_Flow = NULL;
		}
		decoding_iflow_functions___( void )
		{
			reset( false );
		}
		~decoding_iflow_functions___( void )
		{
			reset();
		}
		void Init(
			flw::iflow__ &Flow,
			fwf::thread_safety__ ThreadSafety = fwf::ts_Default )
		{
			_Flow = &Flow;
			_iflow_functions____::Init( ThreadSafety );
		}
	};

	class decoding_iflow___
	: public flw::standalone_iflow__<>
	{
	private:
		decoding_iflow_functions___ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			flw::standalone_iflow__<>::reset( P );
			_Functions.reset( P );
		}
		void Init(
			flw::iflow__ &Flow,
			flw::size__ AmountMax = FLW_SIZE_MAX )
		{
			_Functions.Init( Flow );
			flw::standalone_iflow__<>::Init( _Functions, AmountMax  );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
