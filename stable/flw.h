/*
	Header for the 'flw' library by Claude SIMON (csimon@epeios.org)
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

#ifndef FLW__INC
#define FLW__INC

#define FLW_NAME		"FLW"

#define	FLW_VERSION	"$Revision$"

#define FLW_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &FLWTutor;

#if defined( XXX_DBG ) && !defined( FLW_NODBG )
#define FLW_DBG
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
//D FLoW 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include <string.h>
#include "bso.h"
#include "cpe.h"

#ifndef FLW_CACHE_SIZE
#	define FLW__CACHE_SIZE	1024
#else
#	define FLW__CACHE_SIZE	FLW_CACHE_SIZE
#endif

#ifndef FLW_ICACHE_SIZE
#	define FLW__ICACHE_SIZE	FLW__CACHE_SIZE
#else
#	define FLW__ICACHE_SIZE	FLW_iCACHE_SIZE
#endif

#ifndef FLW_OCACHE_SIZE
#	define FLW__OCACHE_SIZE	FLW__CACHE_SIZE
#else
#	define FLW__OCACHE_SIZE	FLW_OCACHE_SIZE
#endif

#ifdef CPE__MT
#	include "mtx.h"
#	define FLW_NO_MUTEX	MTX_INVALID_HANDLER
	typedef mtx::mutex_handler__ mutex__;
#else
	typedef void *mutex__;
#	define FLW_NO_MUTEX	NULL
#endif

#ifdef CPE__UNIX
#	ifndef FLW_LET_SIGPIPE
#		define FLW__IGNORE_SIGPIPE
#	endif
#endif

namespace flw {
	//t Amount of data.
	typedef bso::msize__		size__;

	//d The max value for a amount.
#	define FLW_SIZE_MAX			BSO_MSIZE_MAX

	//t Size (of a cache, for example).
	typedef bso::bsize__		bsize__;

	//d The max value for a size.
#	define FLW_BSIZE_MAX		BSO_BSIZE_MAX

	//t Type of a datum.
	typedef unsigned char		datum__;

#ifdef CPE__MT
	inline void Test_( mutex__ Mutex )
	{
		if ( Mutex != FLW_NO_MUTEX )
			ERRc();
	}
#endif

	inline mutex__ Create_( void )
	{
#ifdef CPE__MT
		return mtx::Create( mtx::mOwned );
#else
		return FLW_NO_MUTEX;
#endif
	}


	inline void Delete_( mutex__ Mutex )
	{
#ifdef CPE__MT
		Test_( Mutex );
		mtx::Delete( Mutex );
#endif
	}

	inline void Lock_( mutex__ Mutex )
	{
#ifdef CPE__MT
		Test_( Mutex );
		mtx::Lock( Mutex );
#endif
	}

	inline void Unlock_( mutex__ Mutex )
	{
#ifdef CPE__MT
		Test_( Mutex );
		mtx::Unlock( Mutex );
#endif
	}

	inline bso::bool__ IsLocked_( mutex__ Mutex )
	{
#ifdef CPE__MT
		Test_( Mutex );
		return mtx::IsLocked( Mutex );
#endif
		return false;
	}

	inline bso::bool__ IsOwner_( mutex__ Mutex )
	{
#ifdef CPE__MT
		Test_( Mutex );
		return mtx::IsOwner( Mutex );
#endif
		return false;
	}

		//c Base input flow.
	class iflow_functions___
	{
	private:
		mutex__ _Mutex;	// Mutex pour protèger la ressource.
		void _Lock( void )
		{
			Lock_( _Mutex );
		}
		void _Unlock( void )
		{
			if ( IsLocked_( _Mutex ) ) {
#ifdef FLW_DBG
				if ( !IsOwner_( _Mutex ) )
					ERRu();
#endif
				Unlock_( _Mutex );
			}
		}
	protected:
		/* Place un minimum de 'Miimum' octets et jusqu'à 'Wanted' octets dans 'Buffer'. Retourne le nombre d'octets
		effectivement place dans 'Buffer'. Cette value peut être infèrieure à 'Minimum' uniquement si toutes les données 
		ont été lues du périphérique sous-jacent. */
		virtual bsize__ FLWRead(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted ) = 0;
		virtual void FLWDismiss( void )
		{}
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				Dismiss();

				if ( _Mutex != FLW_NO_MUTEX )
					Delete_( _Mutex );
			}

			_Mutex = FLW_NO_MUTEX;
		}
		iflow_functions___( void )
		{
			reset( false );
		}
		virtual ~iflow_functions___( void )
		{
			reset();
		}
		void Init( void )
		{
			reset();

			_Mutex = Create_();
		}
		void Dismiss( void )
		{
			FLWDismiss();

			_Unlock();
		}
		bsize__ Read(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted )
		{
#ifdef FLW_DBG
			if ( Minimum < 1 )
				ERRu();
#endif
			_Lock();

			return FLWRead( Minimum, Buffer, Wanted );
		}
	};

	//c Base input flow.
	class iflow__ // Bien qu'il y ai un destructeur, cette class est de la forme '__' car elle ne peut être instancié seule.
	{
	private:
		iflow_functions___ &_Functions;
		// The cache.
		datum__ *_Cache;
		// Size of the cache.
		bsize__ _Size;
		// Amount of data available in the cache.
		bsize__ _Available;
		// Position of the available data.
		bsize__ _Position;
		// Amount of data red since the last reset.
		size__ _Red;
		// Max amount of data alllowed between 2 reset.
		size__ _AmountMax;
		// Data for the end of flow (EOFD).
		struct {
			// The data.
			const datum__ *Data;
			// Size of the data.
			bsize__ Size;
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
		bsize__ _DirectRead(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted )
		{
#ifdef FLW_DBG
			if ( Minimum < 1 )
				ERRu();
#endif
			bsize__ Amount = 0;

			if ( EOFD_.HandlingEOFD ) {
				Amount = HandleEOFD( Buffer, Wanted );
			} else if ( EOFD_.HandleAmount ) {

				bsize__ ToRead = Wanted;

				if ( (size__)ToRead > ( _AmountMax - _Red ) )
					ToRead = (bsize__)( _AmountMax - _Red );

				// 'ToRead' ne peut être < 1, car on ne serait pas ici sinon.

				Amount = _Read( ( ToRead > Minimum ? Minimum : ToRead ), Buffer, ToRead );

				/* Si 'Wanted' est < 'Minimum', 'Amount' sera nécessairement inférieur à 'Minimum', 
				bien qu'il puisse encore avoir des données disponibles. Cela est voulu, car alors on 
				est dans le cas où retourne les données 'EOFD', car le nombre d'octets lus correspond
				à celui demandé ('_AmountMax' atteind). */

				if ( Amount < Minimum )
					Amount += HandleEOFD( Buffer, Wanted - Amount );
				else  if ( _Red == _AmountMax )
					EOFD_.HandlingEOFD = true;

			} else 
				Amount = _Read( Minimum, Buffer, Wanted );

			if ( Amount < Minimum  )
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
		void _FillCache( bsize__ Minimum )
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
		bsize__ _ReadFromCache(
			bsize__ Amount,
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
		bsize__ _ReadFromCacheOrDirectly(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted )
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
		bsize__ _ReadUpTo(
			bsize__ Amount,
			datum__ *Buffer,
			bsize__ Minimum )
		{
			bsize__ Available = _ReadFromCache( Amount, Buffer );

			if ( ( Available < Minimum ) || ( Available == 0 ) )
				Available += _ReadFromCacheOrDirectly( Minimum - Available, Buffer + Available, Amount  - Available );

			return Available;
		}
		// Place 'Amount' bytes in 'Buffer'.
		void _Read(
			bsize__ Amount,
			datum__ *Buffer )
		{
			_ReadUpTo( Amount, Buffer, Amount );
		}
		// Generic read.
		bsize__ _Read(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted );
		void _Dismiss( void )
		{
			_Functions.Dismiss();
		}
		/*f Handle EOFD. To call when no more data available in the medium.
		Return amount of data written. If 0 is returned, then there is no
		more end of flow data available, and an error should be launched. */
		bsize__ HandleEOFD(
			datum__ *Buffer,
			bsize__ Size )
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
			if ( P )
				Dismiss();

			_Red = 0;
			_Available = _Position = 0;
			EOFD_.Data = NULL;
			EOFD_.Size = 0;
			EOFD_.HandlingEOFD = EOFD_.HandleAmount = EOFD_.HandleToFew = false;
		}
		iflow__(
			iflow_functions___ &Functions,
			datum__ *Cache,
			bsize__ Size,
			size__ AmountMax )
		: _Functions( Functions )
		{
			reset( false );

			_Cache = Cache;
			_Size = Size;
			_AmountMax = AmountMax;
		}
		~iflow__( void )
		{
			reset();
		}
		/*f Place up to 'Amount' bytes in 'Buffer' with a minimum of 'Minimum'.
		Return amount of bytes red. */
		bsize__ ReadUpTo(
			bsize__ Amount,
			void *Buffer,
			bsize__ Minimum = 1 )
		{
			return _ReadUpTo( Amount, (datum__ *)Buffer, Minimum );
		}
		//f Place 'Amount' bytes in 'Buffer'.
		void Read(
			bsize__ Amount,
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
		/* 'Data' is a NUL terminated string to use as end of flow data.
		'Data' is NOT duplicated and should not be modified. */
		void EOFD( const char *Data )
		{
			size_t Length = strlen( Data );

			if ( Length > FLW_BSIZE_MAX )
				ERRl();

			EOFD_.Data = (const datum__ *)Data;
			EOFD_.Size = (bsize__)Length;
			EOFD_.HandleAmount = false;
		}
		/* 'Data' is a NUL terminated string to use as end of flow data.
		'Data' is NOT suplicated and should not be modified. This data will
		be put in the flow after having read 'Amount' bytes.*/
		void EOFD(
			const char *Data,
			bsize__ Amount )
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
		bsize__ ReadRelay(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted )
		{
			bsize__ Amount = 0;

			Amount = ReadUpTo( Wanted, Buffer );

			while ( Amount < Minimum )
				Amount += ReadUpTo( Wanted - Amount, Buffer + Amount );

			return Amount;
		}
	};


	class unsafe_iflow___	// Classe non thread-safe.
	: public iflow__
	{
	private:
		flw::datum__ _Cache[FLW__ICACHE_SIZE];
	public:
		unsafe_iflow___(
			iflow_functions___ &Functions,
			size__ AmountMax )
			: iflow__( Functions, _Cache, sizeof( _Cache ), AmountMax )
		{}
	};


#ifndef CPE__MT
	typedef unsafe_iflow___	iflow___;
#endif


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
	class oflow_functions___
	{
	private:
		mutex__ _Mutex;	// Mutex pour protèger la ressource.
		void _Lock( void )
		{
			Lock_( _Mutex );
		}
		void _Unlock( void )
		{
			if ( IsLocked_( _Mutex ) ) {
#ifdef FLW_DBG
				if ( !IsOwner_( _Mutex ) )
					ERRu();
#endif
				Unlock_( _Mutex );
			}
		}
	protected:
		virtual bsize__ FLWWrite(
			const datum__ *Buffer,
			bsize__ Wanted,
			bsize__ Minimum,
			bool Synchronization ) = 0;
		virtual void FLWSynchronize( void )
		{}
	public:
		void reset( bso::bool__ P = true ) 
		{
			if ( P ) {
				Synchronize();

				if ( _Mutex != FLW_NO_MUTEX )
					Delete_( _Mutex );
			}

			_Mutex = FLW_NO_MUTEX;
		}
		oflow_functions___( void )
		{
			reset( false );
		}
		virtual ~oflow_functions___( void )
		{
			reset();
		}
		void Init( void )
		{
			reset();

			_Mutex = Create_();
		}
		void Synchronize( void )
		{
			FLWSynchronize();

			_Unlock();
		}
		bsize__ Write(
			const datum__ *Buffer,
			bsize__ Wanted,
			bsize__ Minimum,
			bool Synchronization )
		{
			_Lock();
			return FLWWrite( Buffer, Wanted, Minimum, Synchronization );
		}
	};

	//c Basic output flow.
	class oflow__	// Althought it has a destructor, it's a '__' version because it has to be inherited to be used.
	{
	private:
		oflow_functions___ &_Functions;
		// The cache.
		datum__ *Cache_;
		// The size of the cache.
		bsize__ Size_;
		// The amount of bytes yet free.
		bsize__ Free_;
		// Amount of data written since the last synchronizing.
		size__ Written_;
		// Max amount of data between 2 synchronizing.
		size__ AmountMax_;
		// Put up to 'Wanted' and a minimum of 'Minimum' bytes from buffer directly into the device.
		bsize__ _DirectWrite(
			const datum__ *Buffer,
			bsize__ Wanted,
			bsize__ Minimum,
			bool Synchronization );
		void _DumpCache( bool Synchronization )
		{
			bsize__ Stayed = Size_ - Free_;
			
			if ( ( Stayed != 0 ) || Synchronization ) {
				_DirectWrite( Cache_, Stayed, Stayed, Synchronization );

				Free_ = Size_;
			}
		}
		bsize__ _WriteIntoCache(
			const datum__ *Buffer,
			bsize__ Amount )
		{
			if ( Free_ < Amount )
				Amount = Free_;
				
			memcpy( Cache_ + Size_ - Free_, Buffer, (size_t)Amount );
			
			Free_ -= Amount;
			
			return Amount;
		}
		/* Put up to 'Amount' bytes from 'Buffer' directly or through the cache.
		Return amount of bytes written. Cache MUST be EMPTY. */
		bsize__ _DirectWriteOrIntoCache(
			const datum__ *Buffer,
			bsize__ Amount )
		{
#ifdef FLW_DBG
			if ( Size_ != Free_ )
				ERRc();
#endif
			if ( Amount > Size_ )
				return _DirectWrite( Buffer, Amount, Amount, false );
			else
				return _WriteIntoCache( Buffer, Amount );
		}
		// Synchronization.
		void _Synchronize( void )
		{
			_DumpCache( true );

			_Functions.Synchronize();
		}
		// Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		bsize__ _WriteUpTo(
			const datum__ *Buffer,
			bsize__ Amount )
		{
			bsize__ AmountWritten = _WriteIntoCache( Buffer, Amount );

			if ( ( AmountWritten == 0 )  && ( Amount != 0 ) ) {
				_DumpCache( false );
				AmountWritten = _DirectWriteOrIntoCache( Buffer, Amount );
			}

			return AmountWritten;
		}
		// Put 'Amount' data from 'Buffer'.
		void _Write(
			const datum__ *Buffer,
			bsize__ Amount );
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				Synchronize();

			Written_ = 0;
		}
		oflow__(
			oflow_functions___ &Functions,
			datum__ *Cache,
			bsize__ Size,
			size__ AmountMax )
		: _Functions( Functions )
		{
			Cache_ = Cache;
			Size_ = Free_ = Size;
			AmountMax_ = AmountMax;

			reset( false );

		}
		~oflow__( void )
		{
			reset();
		}
		//f Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		bsize__ WriteUpTo(
			const void *Buffer,
			bsize__ Amount )
		{
			return _WriteUpTo( (datum__ *)Buffer, Amount );
		}
		//f Put 'Amount' data from 'Buffer'.
		void Write(
			const void *Buffer,
			bsize__ Amount )
		{
			_Write( (const datum__ *)Buffer, Amount );
		}
		//f Put 'C'.
		void Put( datum__ C )
		{
			_Write( &C, 1 );
		}
		//f Synchronization.
		void Synchronize( void )
		{
			_Synchronize();
		}
		//f Return the amount of data written since last 'Synchronize()'.
		size__ AmountWritten( void ) const
		{
			return Written_ + ( Size_ - Free_ );
		}
		void SetAmountMax( size__ AmountMax )
		{
			AmountMax_ = AmountMax;
		}
		bsize__ WriteRelay(
			const datum__ *Buffer,
			bsize__ Wanted,
			bsize__ Minimum,
			bool Synchronization )
		{
			bsize__ Amount = 0;

			Amount = WriteUpTo( Buffer, Wanted );

			while ( Amount < Minimum )
				Amount += WriteUpTo( Buffer + Amount, Wanted - Amount);

			return Amount;
		}
	};

	class unsafe_oflow___	// Classe non thread-safe.
	: public oflow__
	{
	private:
		flw::datum__ _Cache[FLW__OCACHE_SIZE];
	public:
		unsafe_oflow___(
			oflow_functions___ &Functions,
			size__ AmountMax )
			: oflow__( Functions, _Cache, sizeof( _Cache ), AmountMax )
		{}
	};


#ifndef CPE__MT
	typedef unsafe_iflow___	iflow___;
#endif



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

		if ( Length >= FLW_BSIZE_MAX )
			ERRl();

		OutputFlow.Write( String, (bsize__)( Length + 1 ) );
	}

	class ioflow_functions___
	: public iflow_functions___,
	  public oflow_functions___
	{
	public:
		void reset( bso::bool__ P = true )
		{
			iflow_functions___::reset( P );
			oflow_functions___::reset( P );
		}
		void Init( void )
		{
			iflow_functions___::Init();
			oflow_functions___::Init();;
		}
	};

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
		ioflow__(
			ioflow_functions___ &Functions,
			datum__ *ICache,
			bsize__ ISize,
			size__ ReadAmountMax,
			datum__ *OCache,
			bsize__ OSize,
			size__ WriteAmountMax )
			: iflow__( Functions, ICache, ISize, ReadAmountMax ),
			  oflow__( Functions, OCache, OSize, WriteAmountMax )
		{
			reset( false );
		}
		ioflow__(
			ioflow_functions___ &Functions,
			datum__ *Cache,
			bsize__ Size,
			size__ AmountMax )
			: iflow__( Functions, Cache, Size / 2, AmountMax ),
			  oflow__( Functions, Cache + Size / 2, Size / 2, AmountMax )
		{
			reset();
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
	};


	class unsafe_ioflow___	// Classe non thread-safe.
	: public ioflow__
	{
	private:
		flw::datum__ _ICache[FLW__ICACHE_SIZE];
		flw::datum__ _OCache[FLW__OCACHE_SIZE];
	public:
		unsafe_ioflow___(
			ioflow_functions___ &Functions,
			size__ ReadAmountMax,
			size__ WriteAmountMax )
			: ioflow__( Functions, _ICache, sizeof( _ICache ), ReadAmountMax, _OCache, sizeof( _OCache ), WriteAmountMax )
		{}
		unsafe_ioflow___(
			ioflow_functions___ &Functions,
			size__ AmountMax )
			: ioflow__( Functions, _ICache, sizeof( _ICache ), AmountMax, _OCache, sizeof( _OCache ), AmountMax )
		{}
	};


#ifndef CPE__MT
	typedef unsafe_ioflow___	ioflow___;
#endif

}

inline flw::oflow__ &operator <<(
	flw::oflow__ &OFlow,
	const char *String )
{
	size_t Length = strlen( String );

	if ( Length > FLW_BSIZE_MAX )
		ERRl();

	OFlow.Write( String, (flw::bsize__)Length );

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
