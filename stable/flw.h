/*
	Header for the 'flw' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef FLW__INC
#define FLW__INC

#define FLW_NAME		"FLW"

#define	FLW_VERSION	"$Revision$"

#define FLW_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &FLWTutor;

#if defined( XXX_DBG ) && !defined( FLW_NODBG )
#define FLW_DBG
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
//D FLoW 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include <string.h>
#include "bso.h"
#include "cpe.h"
#include "fwf.h"

// Non utilisé dabs cette bibliothèque, mais dans des bibliothèques appelantes.
#ifndef FLW_INPUT_CACHE_SIZE
#	define FLW__INPUT_CACHE_SIZE	FWF__DEFAULT_CACHE_SIZE
#else
#	define FLW__INPUT_CACHE_SIZE	FLW_INPUT_CACHE_SIZE
#endif

#ifndef FLW_OUTPUT_CACHE_SIZE
#	define FLW__OUTPUT_CACHE_SIZE	FWF__DEFAULT_CACHE_SIZE
#else
#	define FLW__OUTPUT_CACHE_SIZE	FLW_OUTPUT_CACHE_SIZE
#endif

#ifdef CPE__UNIX
#	ifndef FLW_LET_SIGPIPE
#		define FLW__IGNORE_SIGPIPE
#	endif
#endif

#define FLW_SIZE_MAX	FWF_SIZE_MAX

namespace flw {
	using fwf::datum__;
	using fwf::size__;

	//c Base input flow.
	class iflow__	/* Bien que cette classe ai un destructeur, elle est suffixée par '__', d'une part pour simplifier
					son utilisation (comme déclaration de paramètre d'une fonction) et, d'autre part,
					parce qu'elle ne sera jamais instanciée telle quelle, mais toujours héritée (bien que ce ne
					soit pas obligatoire d'un point de vue C++, car ce n'est pas une fonction abstraite).*/
	{
	private:
		fwf::iflow_functions_base___ *_Functions;
		// Amount of data red since the last reset.
		size__ _Red;
		// Max amount of data alllowed between 2 reset.
		size__ _AmountMax;
		// Data for the end of flow (EOFD).
		struct {
			// The data.
			const datum__ *Data;
			// Size of the data.
			size__ Size;
			int
				//At true if we are currently handling end of file data.
				HandlingEOFD:		1,
				// At true if we have to handle the amount of data (in other words, EOFD are handled after 'AmountMax_' data red).
				HandleAmount:	1,
				// At true if we have to generate an error if not all awaited data are red. Only significant if 'HandleAmount' at true.
				HandleToFew:	1;
		} EOFD_;
		fwf::iflow_functions_base___ &_F( void )
		{
			if ( _Functions == NULL )
				ERRu();

			return *_Functions;
		}
		/* Put up to 'Wanted' and a minimum of 'Minimum' bytes into 'Buffer'
		directly from device. */
		size__ _EOFAwareRead(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted,
			bso::bool__ Adjust,
			bso::bool__ &CacheIsEmpty )
		{
			size__ Amount = 0;

			if ( EOFD_.HandlingEOFD ) {
				Amount = HandleEOFD( Buffer, Wanted );
			} else if ( EOFD_.HandleAmount ) {

				size__ ToRead = Wanted;

				if ( (size__)ToRead > ( _AmountMax - _Red ) )
					ToRead = (size__)( _AmountMax - _Red );

				// 'ToRead' ne peut être < 1, car on ne serait pas ici sinon.

				Amount = _RawRead( ( ToRead > Minimum ? Minimum : ToRead ), Buffer, ToRead, Adjust, CacheIsEmpty );

				/* Si 'Wanted' est < 'Minimum', 'Amount' sera nécessairement inférieur à 'Minimum', 
				bien qu'il puisse encore avoir des données disponibles. Cela est voulu, car alors on 
				est dans le cas où retourne les données 'EOFD', car le nombre d'octets lus correspond
				à celui demandé ('_AmountMax' atteint). */

				if ( Amount < Minimum )
					Amount += HandleEOFD( Buffer, Wanted - Amount );
				else  if ( _Red == _AmountMax )
					EOFD_.HandlingEOFD = true;

			} else {
				Amount = _RawRead( Minimum, Buffer, Wanted, Adjust, CacheIsEmpty );

				if ( (Amount < Minimum ) || ( Amount == 0 ) ) {
					Amount += HandleEOFD( Buffer, Wanted - Amount );
				}
			}

			if ( ( Amount < Minimum  ) && ( Amount != 0 ) )
				ERRf();

			return Amount;
		}
		// Fill the cache with a minimum of 'Minimum' bytes. The cache must be empty.
		/* Read from cache up to 'Amount' bytes and place them to 'Buffer'.
		Return amount of data red. */
		size__ _ReadUpTo(
			size__ Amount,
			datum__ *Buffer,
			size__ Minimum,
			bso::bool__ Adjust,
			bso::bool__ &CacheIsEmpty )
		{
			return _EOFAwareRead( Minimum, Buffer, Amount, Adjust, CacheIsEmpty );
		}
		// Place 'Amount' bytes in 'Buffer'.
		void _Read(
			size__ Amount,
			datum__ *Buffer,
			bso::bool__ Adjust,
			bso::bool__ &CacheIsEmpty )
		{
			if ( _ReadUpTo( Amount, Buffer, Amount, Adjust, CacheIsEmpty ) != Amount )
				ERRf();
		}
		// Generic read.
		size__ _LoopingRawRead(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted,
			bso::bool__ Adjust,
			bso::bool__ &CacheIsEmpty )
		{
			size__ PonctualAmount = _F().Read( Wanted, Buffer, Adjust, CacheIsEmpty );
			size__ CumulativeAmount = PonctualAmount;

			while ( ( PonctualAmount != 0 ) && ( Minimum > CumulativeAmount ) ) {
				PonctualAmount = _F().Read( Wanted - CumulativeAmount, Buffer + CumulativeAmount, Adjust, CacheIsEmpty );
				CumulativeAmount += PonctualAmount;
			}

			return CumulativeAmount;
		}
		size__ _RawRead(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted,
			bso::bool__ Adjust,
			bso::bool__ &CacheIsEmpty );
		void _Dismiss( void )
		{
			_F().Dismiss();
			_Red = 0;
		}
		/*f Handle EOFD. To call when no more data available in the medium.
		Return amount of data written. If 0 is returned, then there is no
		more end of flow data available, and an error should be launched. */
		size__ HandleEOFD(
			datum__ *Buffer,
			size__ Size )
		{
			EOFD_.HandlingEOFD = true;

			if ( EOFD_.HandleToFew && EOFD_.HandleAmount && ( _Red < _AmountMax ) )
				ERRf();

			if ( Size > EOFD_.Size )
				Size = EOFD_.Size;
				
			if ( Size != 0 ) {
				memcpy( Buffer, EOFD_.Data, (size_t)Size );
				EOFD_.Size -= Size;
				EOFD_.Data += Size;
			}

			if ( EOFD_.Size == 0 )
				_Dismiss();
			
			return Size;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Red )
					Dismiss();
			}

			_Red = 0;
			EOFD_.Data = NULL;
			EOFD_.Size = 0;
			EOFD_.HandlingEOFD = EOFD_.HandleAmount = EOFD_.HandleToFew = false;
		}
		iflow__( void )
		{
			reset( false );
		}
		~iflow__( void )
		{
			reset();
		}
		void Init(
			fwf::iflow_functions_base___ &Functions,
			size__ AmountMax )
		{
			if ( _Red )
				Dismiss();

			_Functions = &Functions;
			_AmountMax = AmountMax;

			_Red = 0;
			EOFD_.Data = NULL;
			EOFD_.Size = 0;
			EOFD_.HandlingEOFD = EOFD_.HandleAmount = EOFD_.HandleToFew = false;
		}
		/*f Place up to 'Amount' bytes in 'Buffer' with a minimum of 'Minimum'.
		Return amount of bytes red. */
		size__ ReadUpTo(
			size__ Amount,
			void *Buffer,
			size__ Minimum = 1 )
		{
			bso::bool__ Dummy = false;

			return _ReadUpTo( Amount, (datum__ *)Buffer, Minimum, true, Dummy );
		}
		//f Place 'Amount' bytes in 'Buffer'.
		void Read(
			size__ Amount,
			void *Buffer )
		{
			bso::bool__ Dummy = false;

			_Read( Amount, (datum__ *)Buffer, true, Dummy );
		}
		datum__ View( void )
		{
			datum__ C;
			bso::bool__ Dummy = false;

			_Read( 1, &C, false, Dummy );

			return C;
		}
		//f Get next byte.
		datum__ Get( void )
		{
			datum__ C;
			bso::bool__ Dummy = false;

			_Read( 1, &C, true, Dummy );

			return C;
		}
		datum__ Get( bso::bool__ &CacheIsEmpty )
		{
			datum__ C;

			_Read( 1, &C, true, CacheIsEmpty );

			return C;
		}
		void Unget( datum__ Datum )
		{
			return _F().Unget( Datum );
		}
		//f Skip 'Amount' bytes.
		void Skip( size__ Amount = 1 )
		{
			while ( Amount-- )
				Get();
		}
		void EOFD(
			const void *Data,
			size__ Length )
		{
			if ( Length > FLW_SIZE_MAX )
				ERRl();

			EOFD_.Data = (const datum__ *)Data;
			EOFD_.Size = (size__)Length;
			EOFD_.HandleAmount = false;
		}
		/* 'Data' is a NUL terminated string to use as end of flow data.
		'Data' is NOT duplicated and should not be modified. */
		void EOFD( const char *Data )
		{
			size_t Length = strlen( Data );

			if ( Length > FWF_SIZE_MAX )
				ERRl();

			EOFD( (void *)Data, (size__)Length );
		}
		/* 'Data' is a NUL terminated string to use as end of flow data.
		'Data' is NOT suplicated and should not be modified. This data will
		be put in the flow after having read 'Amount' bytes.*/
		void EOFD(
			const char *Data,
			size__ Amount )
		{
			EOFD( Data );

			EOFD_.HandleAmount = true;
			EOFD_.HandleToFew = true;
			_AmountMax = Amount;
			_Red = 0;
		}
		//f Return the amount of data red since last 'Reset()'.
		size__ AmountRed( void ) const
		{
			return _Red;
		}
		void SetAmountMax( size__ AmountMax )
		{
			_AmountMax = AmountMax;
		}
		void Dismiss( void )
		{
			if ( _Functions != NULL )
				_Dismiss();
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Functions != NULL;
		}
		bso::bool__ IsLocked( void )
		{
#ifdef FLW_DBG
			if ( !IsInitialized() )
				ERRu();
#endif

			return _Functions->IsLocked();
		}
		bso::bool__ IFlowIsLocked( void )	// Facilite l'utilisation de 'ioflow__'
		{
			return IsLocked();
		}
	};


	// Uniquement pour avoir une symétrie par rapport à 'standalone_iflow__'.
	template <int Dummy = 0> class standalone_iflow__
	: public iflow__
	{
	public:
		standalone_iflow__( void )
		{
			if ( Dummy != 0 )	
				ERRu();	// 'Dummy' n'étant pas utilisé, rien ne sert de modifier sa valeur.
		}
	};


	//f Get 'StaticObject' from 'InputFlow'.
	template <class t> inline void Get(
		iflow__ &InputFlow,
		t &StaticObject )
	{
		InputFlow.Read( sizeof( t ), &StaticObject );
	}

	/*f Place, from 'IFlow', up to 'Maximum' character in 'Buffer' or until
	reading the 'NULL' character. False is returned, if not enough place to put
	the string with its final NULL character. Buffer contains then 'Maximum'
	characters of the string. Otherwise, it returns true. */
	bool GetString(
		iflow__ &IFlow,
		char *Buffer,
		size__ Maximum );


	//c Basic output flow.
	class oflow__	/* Bien que cette classe ai un destructeur, elle est suffixée par '__', d'une part pour simplifier
					son utilisation (comme déclaration de paramètre d'une fonction) et, d'autre part,
					parce qu'elle ne sera jamais instanciée telle quelle, mais toujours héritée (bien que ce ne
					soit pas obligatoire d'un point de vue C++, car ce n'est pas une focntion abstraite).*/
	{
	private:
		fwf::oflow_functions_base___ *_Functions;
		// The cache.
		datum__ *_Cache;
		// The size of the cache.
		size__ _Size;
		// The amount of bytes yet free.
		size__ _Free;
		// Amount of data written since the last synchronizing.
		size__ _Written;
		// Max amount of data between 2 synchronizing.
		size__ _AmountMax;
		fwf::oflow_functions_base___ &_F( void )
		{
			if ( _Functions == NULL )
				ERRu();

			return *_Functions;
		}
		size__ _LoopingWrite(
			const datum__ *Buffer,
			size__ Wanted,
			size__ Minimum )
		{
			size__ PonctualAmount = _F().Write( Buffer, Wanted );
			size__ CumulativeAmount = PonctualAmount;

			while ( ( PonctualAmount != 0 ) && ( Minimum > CumulativeAmount ) ) {
				PonctualAmount = _F().Write( Buffer + CumulativeAmount, Wanted - CumulativeAmount );
				CumulativeAmount += PonctualAmount;
			}

			return CumulativeAmount;
		}

		// Put up to 'Wanted' and a minimum of 'Minimum' bytes from buffer directly into the device.
		size__ _DirectWrite(
			const datum__ *Buffer,
			size__ Wanted,
			size__ Minimum );
		void _DumpCache( void )
		{
			size__ Stayed = _Size - _Free;
			
			if ( Stayed != 0 ) {
				_DirectWrite( _Cache, Stayed, Stayed );

				_Free = _Size;
			}
		}
		size__ _WriteIntoCache(
			const datum__ *Buffer,
			size__ Amount )
		{
			if ( _Free < Amount )
				Amount = _Free;
				
			memcpy( _Cache + _Size - _Free, Buffer, (size_t)Amount );
			
			_Free -= Amount;
			
			return Amount;
		}
		/* Put up to 'Amount' bytes from 'Buffer' directly or through the cache.
		Return amount of bytes written. Cache MUST be EMPTY. */
		size__ _DirectWriteOrIntoCache(
			const datum__ *Buffer,
			size__ Amount )
		{
#ifdef FLW_DBG
			if ( _Size != _Free )
				ERRc();
#endif
			if ( Amount > _Size )
				return _DirectWrite( Buffer, Amount, Amount );
			else
				return _WriteIntoCache( Buffer, Amount );
		}
		// Synchronization.
		void _Commit( void )
		{
			_DumpCache();
			_F().Commit();

			_Written = 0;
		}
		// Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		size__ _WriteUpTo(
			const datum__ *Buffer,
			size__ Amount )
		{
			size__ AmountWritten = _WriteIntoCache( Buffer, Amount );

			if ( ( AmountWritten == 0 )  && ( Amount != 0 ) ) {
				_DumpCache();
				AmountWritten = _DirectWriteOrIntoCache( Buffer, Amount );
			}

			return AmountWritten;
		}
		// Put 'Amount' data from 'Buffer'.
		void _Write(
			const datum__ *Buffer,
			size__ Amount );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Size != _Free )
					Commit();
			}

			_Functions = NULL;
			_Cache = NULL;
			_Size = _Free = 0;
			_AmountMax = 0;
			_Written = 0;
		}
		oflow__( void )
		{
			reset( false );

		}
		virtual ~oflow__( void )
		{
			reset();
		}
		void Init(
			fwf::oflow_functions_base___ &Functions,
			datum__ *Cache,
			size__ Size,
			size__ AmountMax )
		{
			if ( _Size != _Free )
				Commit();

			_Functions = &Functions;
			_Cache = Cache;
			_Size = _Free = Size;
			_AmountMax = AmountMax;

			_Written = 0;
		}
		//f Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		size__ WriteUpTo(
			const void *Buffer,
			size__ Amount )
		{
			return _WriteUpTo( (datum__ *)Buffer, Amount );
		}
		//f Put 'Amount' data from 'Buffer'.
		void Write(
			const void *Buffer,
			size__ Amount )
		{
			_Write( (const datum__ *)Buffer, Amount );
		}
		//f Put 'C'.
		void Put( datum__ C )
		{
			_Write( &C, 1 );
		}
		//f Synchronization.
		void Commit( void )
		{
			if ( _Functions != NULL )
				_Commit();
		}
		//f Return the amount of data written since last 'Synchronize()'.
		size__ AmountWritten( void ) const
		{
			return _Written + ( _Size - _Free );
		}
		void SetAmountMax( size__ AmountMax )
		{
			_AmountMax = AmountMax;
		}
# if 0
/*		size__ WriteRelay(
			const datum__ *Buffer,
			size__ Maximum )
		{
			return WriteUpTo( Buffer, Maximum );
		}
		datum__ *GetCurrentCacheDatum( bso::bool__ MarkAsUsed )	/* Si 'AsUsed' à vrai, considère le 'datum' retourné comme utilisé. */
		{
			if ( _Free == 0 )
				_DumpCache();

			if ( _Free == 0 )
				ERRf();

			if ( MarkAsUsed )
				return _Cache + _Size - _Free--;
			else
				return _Cache + _Size - _Free;
		}
#endif
		size__ GetCacheSize( void) const
		{
			return _Size;
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Functions != NULL;
		}
		bso::bool__ IsLocked( void )
		{
#ifdef FLW_DBG
			if ( !IsInitialized() )
				ERRu();
#endif

			return _Functions->IsLocked();
		}
		bso::bool__ OFlowIsLocked( void )	// Facilite l'utilisation de 'ioflow__'
		{
			return IsLocked();
		}
	};

	template <int CacheSize = FLW__OUTPUT_CACHE_SIZE> class standalone_oflow__
	: public oflow__
	{
	private:
		flw::datum__ _Cache[CacheSize];
	public:
		void Init(
			fwf::oflow_functions_base___ &Functions,
			size__ AmountMax )
		{
			oflow__::Init( Functions, _Cache, sizeof( _Cache ), AmountMax );
		}
	};


	//f Write to 'OutputFlow' 'StaticObject'.
	template <class t> inline void Put(
		const t &StaticObjet,
		oflow__ &OutputFlow )
	{
		OutputFlow.Write( &StaticObjet, sizeof( t ) );
	}

	//f Write to 'OutputFlow' the 'NULL' terminated String 'String' WITH the 'NULL' character.
	inline void PutString(
		const char *String,
		oflow__ &OutputFlow )
	{
		size_t Length = strlen( String );

		if ( Length >= FLW_SIZE_MAX )
			ERRl();

		OutputFlow.Write( String, (size__)( Length + 1 ) );
	}


	//c Basic input/output flow.
	class ioflow__
	: public iflow__,
	  public oflow__
	{
	public:
		void reset( bso::bool__ P = true )
		{
			iflow__::reset( P );
			oflow__::reset( P );
		}
		ioflow__( void )
		{
			reset( false );
		}
		void SetAmountMax(
			size__ ReadAmountMax,
			size__ WriteAmountMax )
		{
			iflow__::SetAmountMax( ReadAmountMax );
			oflow__::SetAmountMax( WriteAmountMax );
		}
		void SetAmountMax( size__ AmountMax )
		{
			SetAmountMax( AmountMax, AmountMax );
		}
		void Init(
			fwf::ioflow_functions_base___ &Functions,
			size__ ReadAmountMax,
			datum__ *OCache,
			size__ OSize,
			size__ WriteAmountMax )
		{
			iflow__::Init( Functions, ReadAmountMax );
			oflow__::Init( Functions, OCache, OSize, WriteAmountMax );

		}
		void Init(
			fwf::ioflow_functions_base___ &Functions,
			datum__ *Cache,
			size__ Size,
			size__ AmountMax )
		{
			iflow__::Init( Functions, AmountMax );
			oflow__::Init( Functions, Cache + Size / 2, Size / 2, AmountMax );
		}
	};

	template <int OutCacheSize = FLW__OUTPUT_CACHE_SIZE> class standalone_ioflow__
	: public ioflow__
	{
	private:
		flw::datum__ _OutputCache[OutCacheSize];
	public:
		void Init(
			fwf::ioflow_functions_base___ &Functions,
			size__ ReadAmountMax,
			size__ WriteAmountMax )
		{
			ioflow__::Init( Functions, ReadAmountMax, _OutputCache, sizeof( _OutputCache ), WriteAmountMax );
		}
		void Init(
			fwf::ioflow_functions_base___ &Functions,
			size__ AmountMax )
		{
			ioflow__::Init( Functions, AmountMax, _OutputCache, sizeof( _OutputCache ), AmountMax );
		}
	};

	// Copie 'Amount' octets de 'IFlow' dans 'OFlow'.
	void Copy(
		iflow__ &IFlow,
		size__ Amount,
		oflow__ &OFlow );

}

inline flw::oflow__ &operator <<(
	flw::oflow__ &OFlow,
	const char *String )
{
	size_t Length = strlen( String );

	if ( Length > FLW_SIZE_MAX )
		ERRl();

	OFlow.Write( String, (flw::size__)Length );

	return OFlow;
}

inline flw::oflow__ &operator <<(
	flw::oflow__ &OFlow,
	char Character )
{
	OFlow.Put( (flw::datum__)Character );

	return OFlow;
}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
