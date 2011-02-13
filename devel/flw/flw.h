/*
	Header for the 'flw' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef FLW_CACHE_SIZE
#	define FLW__CACHE_SIZE	1024
#else
#	define FLW__CACHE_SIZE	FLW_CACHE_SIZE
#endif

#ifndef FLW_ICACHE_SIZE
#	define FLW__ICACHE_SIZE	FLW__CACHE_SIZE
#else
#	define FLW__ICACHE_SIZE	FLW_ICACHE_SIZE
#endif

#ifndef FLW_OCACHE_SIZE
#	define FLW__OCACHE_SIZE	FLW__CACHE_SIZE
#else
#	define FLW__OCACHE_SIZE	FLW_OCACHE_SIZE
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
	class iflow__	/* Bien que cette classe ai un destructeur, elle est suffix�e par '__', d'une part pour simplifier
					son utilisation (comme d�claration de param�tre d'une fonction) et, d'autre part,
					parce qu'elle ne sera jamais instanci�e telle quelle, mais toujours h�rit�e (bien que ce ne
					soit pas obligatoire d'un point de vue C++, car ce n'est pas une fonction abstraite).*/
	{
	private:
		fwf::iflow_functions___ *_Functions;
		// The cache.
		datum__ *_Cache;
		// Size of the cache.
		size__ _Size;
		// Amount of data available in the cache.
		size__ _Available;
		// Position of the available data.
		size__ _Position;
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
		/* Put up to 'Wanted' and a minimum of 'Minimum' bytes into 'Buffer'
		directly from device. */
		size__ _DirectRead(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted )
		{
			size__ Amount = 0;

			if ( EOFD_.HandlingEOFD ) {
				Amount = HandleEOFD( Buffer, Wanted );
			} else if ( EOFD_.HandleAmount ) {

				size__ ToRead = Wanted;

				if ( (size__)ToRead > ( _AmountMax - _Red ) )
					ToRead = (size__)( _AmountMax - _Red );

				// 'ToRead' ne peut �tre < 1, car on ne serait pas ici sinon.

				Amount = _Read( ( ToRead > Minimum ? Minimum : ToRead ), Buffer, ToRead );

				/* Si 'Wanted' est < 'Minimum', 'Amount' sera n�cessairement inf�rieur � 'Minimum', 
				bien qu'il puisse encore avoir des donn�es disponibles. Cela est voulu, car alors on 
				est dans le cas o� retourne les donn�es 'EOFD', car le nombre d'octets lus correspond
				� celui demand� ('_AmountMax' atteint). */

				if ( Amount < Minimum )
					Amount += HandleEOFD( Buffer, Wanted - Amount );
				else  if ( _Red == _AmountMax )
					EOFD_.HandlingEOFD = true;

			} else {
				Amount = _Read( Minimum, Buffer, Wanted );

				if ( (Amount < Minimum ) || ( Amount == 0 ) ) {
					Amount += HandleEOFD( Buffer, Wanted - Amount );
				}
			}

			if ( ( Amount < Minimum  ) && ( Amount != 0 ) )
				ERRf();

			return Amount;
		}
#ifdef FLW_DBG
		// Test if there is a cache available.
		void _Test( void )
		{
			if ( ( _Cache == NULL ) )
				ERRu();
				
			if ( _Size == 0 )
				ERRu();
		}				
#endif		
		// Fill the cache with a minimum of 'Minimum' bytes. The cache must be empty.
		void _FillCache( size__ Minimum )
		{
#ifdef FLW_DBG
			_Test();

			if ( _Available != 0 )				
				ERRc();
#endif
			_Available = _DirectRead( Minimum, _Cache, _Size );
			_Position = 0;
		}
		/* Read from cache up to 'Amount' bytes and place them to 'Buffer'.
		Return amount of data red. */
		size__ _ReadFromCache(
			size__ Amount,
			datum__ *Buffer )
		{
#ifdef FLW_DBG
			_Test();
#endif
			if ( _Available < Amount )
				Amount = _Available;
				
			if ( Amount != 0 ) {
				memcpy( Buffer, _Cache + _Position, (size_t)Amount );
				
				_Position += Amount;
				_Available -= Amount;
			}
			
			return Amount;
		}
		/* Put a minimum of 'Minimum' and up to 'Wanted' bytes in 'Buffer',
		directly or through the cache. Return amount of byte red.
		The cache must be empty. */
		size__ _ReadFromCacheOrDirectly(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted )
		{
#ifdef FLW_DBG
			_Test();

			if ( _Available != 0 )				
				ERRc();
#endif
			if ( _Size > Wanted ) {
				_FillCache( Minimum );
				return _ReadFromCache( Wanted, Buffer );
			} else
				return _DirectRead( Minimum, Buffer, Wanted );
		}
		/* Place up to 'Amount' bytes in 'Buffer' with a minimum of 'Minimum'.
		Return number of bytes red. */
		size__ _ReadUpTo(
			size__ Amount,
			datum__ *Buffer,
			size__ Minimum )
		{
			size__ Available = _ReadFromCache( Amount, Buffer );

			if ( ( Available < Minimum ) || ( Available == 0 ) )
				Available += _ReadFromCacheOrDirectly( Minimum - Available, Buffer + Available, Amount  - Available );

			return Available;
		}
		// Place 'Amount' bytes in 'Buffer'.
		void _Read(
			size__ Amount,
			datum__ *Buffer )
		{
			if ( _ReadUpTo( Amount, Buffer, Amount ) != Amount )
				ERRf();
		}
		// Generic read.
		size__ _LoopingRead(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted )
		{
			size__ PonctualAmount = _Functions->Read( Wanted, Buffer );
			size__ CumulativeAmount = PonctualAmount;

			while ( ( PonctualAmount != 0 ) && ( Minimum > CumulativeAmount ) ) {
				PonctualAmount = _Functions->Read( Wanted - CumulativeAmount, Buffer + CumulativeAmount );
				CumulativeAmount += PonctualAmount;
			}

			return CumulativeAmount;
		}
		size__ _Read(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted );
		void _Dismiss( void )
		{
			_Functions->Dismiss();
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
			_Available = _Position = 0;
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
			fwf::iflow_functions___ &Functions,
			datum__ *Cache,
			size__ Size,
			size__ AmountMax )
		{
			if ( _Red )
				Dismiss();

			_Functions = &Functions;
			_Cache = Cache;
			_Size = Size;
			_AmountMax = AmountMax;

			_Red = 0;
			_Available = _Position = 0;
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
			return _ReadUpTo( Amount, (datum__ *)Buffer, Minimum );
		}
		//f Place 'Amount' bytes in 'Buffer'.
		void Read(
			size__ Amount,
			void *Buffer )
		{
			_Read( Amount, (datum__ *)Buffer );
		}
		//f Get next byte.
		datum__ Get( void )
		{
			datum__ C;

			_Read( 1, &C );

			return C;
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

			if ( _Available > Amount )	// This means we have red too much data.
				ERRf();

			EOFD_.HandleAmount = true;
			EOFD_.HandleToFew = true;
			_AmountMax = Amount;

			_Red = _Available;

			if ( _Red == _AmountMax )
				EOFD_.HandlingEOFD = true;
		}
		//f Return the amount of data red since last 'Reset()'.
		size__ AmountRed( void ) const
		{
			return _Red - _Available;
		}
		void SetAmountMax( size__ AmountMax )
		{
			_AmountMax = AmountMax;
		}
		void Dismiss( void )
		{
			_Dismiss();
		}
/*		size__ ReadRelay(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted )
		{
			size__ Amount = 0;

			Amount = ReadUpTo( Wanted, Buffer );

			while ( Amount < Minimum )
				Amount += ReadUpTo( Wanted - Amount, Buffer + Amount );

			return Amount;
		}
*/		datum__ *GetCurrentCacheDatum( bso::bool__ MarkAsUsed )	/* Si 'AsUsed' � vrai, consid�re le 'datum' retourn� comme utilis�. */

		{
			if ( _Available == 0 )
				_FillCache( 1 );

			if ( _Available == 0 )
				ERRf();

			if ( MarkAsUsed ) {
				_Available--;
				return _Cache + _Position++;
			} else
				return _Cache + _Position;
		}
	};


	template <int CacheSize = FLW__ICACHE_SIZE> class standalone_iflow__
	: public iflow__
	{
	private:
		flw::datum__ _Cache[CacheSize];
	public:
		void Init( 
			fwf::iflow_functions___ &Functions,
			size__ AmountMax )
		{
			iflow__::Init( Functions, _Cache, sizeof( _Cache ), AmountMax );
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
	class oflow__	/* Bien que cette classe ai un destructeur, elle est suffix�e par '__', d'une part pour simplifier
					son utilisation (comme d�claration de param�tre d'une fonction) et, d'autre part,
					parce qu'elle ne sera jamais instanci�e telle quelle, mais toujours h�rit�e (bien que ce ne
					soit pas obligatoire d'un point de vue C++, car ce n'est pas une focntion abstraite).*/
	{
	private:
		fwf::oflow_functions___ *_Functions;
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
		size__ _LoopingWrite(
			const datum__ *Buffer,
			size__ Wanted,
			size__ Minimum )
		{
			size__ PonctualAmount = _Functions->Write( Buffer, Wanted );
			size__ CumulativeAmount = PonctualAmount;

			while ( ( PonctualAmount != 0 ) && ( Minimum > CumulativeAmount ) ) {
				PonctualAmount = _Functions->Write( Buffer + CumulativeAmount, Wanted - CumulativeAmount );
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
			_Functions->Commit();

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
			fwf::oflow_functions___ &Functions,
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
/*		size__ WriteRelay(
			const datum__ *Buffer,
			size__ Maximum )
		{
			return WriteUpTo( Buffer, Maximum );
		}
*/		datum__ *GetCurrentCacheDatum( bso::bool__ MarkAsUsed )	/* Si 'AsUsed' � vrai, consid�re le 'datum' retourn� comme utilis�. */
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
	};

	template <int CacheSize> class standalone_oflow__
	: public oflow__
	{
	private:
		flw::datum__ _Cache[CacheSize];
	public:
		void Init(
			fwf::oflow_functions___ &Functions,
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
			fwf::ioflow_functions___ &Functions,
			datum__ *ICache,
			size__ ISize,
			size__ ReadAmountMax,
			datum__ *OCache,
			size__ OSize,
			size__ WriteAmountMax )
		{
			iflow__::Init( Functions, ICache, ISize, ReadAmountMax );
			oflow__::Init( Functions, OCache, OSize, WriteAmountMax );

		}
		void Init(
			fwf::ioflow_functions___ &Functions,
			datum__ *Cache,
			size__ Size,
			size__ AmountMax )
		{
			iflow__::Init( Functions, Cache, Size / 2, AmountMax );
			oflow__::Init( Functions, Cache + Size / 2, Size / 2, AmountMax );
		}
	};

	template <int InCacheSize = FLW__ICACHE_SIZE, int OutCacheSize = FLW__OCACHE_SIZE> class standalone_ioflow__
	: public ioflow__
	{
	private:
		flw::datum__ _ICache[InCacheSize];
		flw::datum__ _OCache[OutCacheSize];
	public:
		void Init(
			fwf::ioflow_functions___ &Functions,
			size__ ReadAmountMax,
			size__ WriteAmountMax )
		{
			ioflow__::Init( Functions, _ICache, sizeof( _ICache ), ReadAmountMax, _OCache, sizeof( _OCache ), WriteAmountMax );
		}
		void Init(
			fwf::ioflow_functions___ &Functions,
			size__ AmountMax )
		{
			ioflow__::Init( Functions, _ICache, sizeof( _ICache ), AmountMax, _OCache, sizeof( _OCache ), AmountMax );
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
