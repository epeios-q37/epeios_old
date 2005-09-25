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

#ifdef CPE__MT
#	include "mtx.h"
#	define FLW_NO_MUTEX	MTX_INVALID_HANDLER
#else
#	define FLW_NO_MUTEX	(flw::mutex__)NULL
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
	typedef mtx::mutex_handler__ mutex__;
#else
	typedef void *	mutex__;
#endif

	inline void _Lock( mutex__ Mutex ) {
#ifdef CPE__MT
		if ( Mutex != FLW_NO_MUTEX )
			mtx::Lock( Mutex );
#endif
	}

	inline void _Unlock( mutex__ Mutex ) {
#ifdef CPE__MT
		if ( Mutex != FLW_NO_MUTEX )
			mtx::Unlock( Mutex );
#endif
	}



	//c Base input flow.
	class iflow__ // Althought it has a destructor, it's a '--' version because it has to be inherited to be used.
	{
	private:
		// The cache.
		datum__ *Cache_;
		// Size of the cache.
		bsize__ Size_;
		// Amount of data available in the cache.
		bsize__ Available_;
		// Position of the available data.
		bsize__ Position_;
		// Amount of data red since the last reset.
		size__ Red_;
		// Max amount of data alllowed between 2 reset.
		size__ AmountMax_;
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
		struct {
			mutex__ Mutex;	// Mutex pour protèger la ressource.
			bso::bool__ Owner;	// Si à 'true', alors on est l'actuel posseseur du 'flow'.
		} _ThreadHandling;	// Gestion du multitâche.
		void _TestAndLock( void )
		{
			if ( !_ThreadHandling.Owner ) {
				_Lock( _ThreadHandling.Mutex );
				_ThreadHandling.Owner = true;
			}
		}
		void _Unlock( void )
		{
#ifdef FLW_DBG
			if ( !_ThreadHandling.Owner )
				ERRu();
#endif
			if ( _ThreadHandling.Mutex != FLW_NO_MUTEX ) {
				_ThreadHandling.Owner = false;
				flw::_Unlock( _ThreadHandling.Mutex );
			}
		}
		/* Put up to 'Wanted' and a minimum of 'Minimum' bytes into 'Buffer'
		directly from device. */
		bsize__ _DirectRead(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted )
		{
			bsize__ Amount = 0;

			if ( EOFD_.HandlingEOFD ) {
				Amount = HandleEOFD( Buffer, Wanted );

				if ( ( Amount < Minimum ) || !Amount )
					ERRf();
			} else if ( EOFD_.HandleAmount ) {

				if ( (size__)Wanted > ( AmountMax_ - Red_ ) )
					Wanted = (bsize__)( AmountMax_ - Red_ );

				Amount = _Read( Minimum, Buffer, Wanted );

				if ( Amount < Minimum )
					ERRf();	/* Because, if there is not enough data in the flow,
							'FLWRead' already calls the 'HandleEOFD' function. */

				if ( Red_ == AmountMax_ )
					EOFD_.HandlingEOFD = true;

			} else 
				Amount = _Read( Minimum, Buffer, Wanted );

			return Amount;
		}
#ifdef FLW_DBG
		// Test if there is a cache available.
		void _Test( void )
		{
			if ( ( Cache_ == NULL ) )
				ERRu();
				
			if ( Size_ == 0 )
				ERRu();
		}				
#endif		
		// Fill the cache with a minimum of 'Minimum' bytes. The cache must be empty.
		void _FillCache( bsize__ Minimum )
		{
#ifdef FLW_DBG
			_Test();

			if ( Available_ != 0 )				
				ERRc();
#endif
			Available_ = _DirectRead( Minimum, Cache_, Size_ );
			Position_ = 0;
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
			if ( Available_ < Amount )
				Amount = Available_;
				
			if ( Amount != 0 ) {
				memcpy( Buffer, Cache_ + Position_, (size_t)Amount );
				
				Position_ += Amount;
				Available_ -= Amount;
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

			if ( Available_ != 0 )				
				ERRc();
#endif
			if ( Size_ > Wanted ) {
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
			bsize__ Available = _ReadFromCache( Amount, Buffer );

			if ( Amount > Available )
				Available += _ReadFromCacheOrDirectly( Amount - Available, Buffer + Available, Amount - Available );
		}
		// Generic read.
		bsize__ _Read(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted );
		void _Dismiss( void )
		{
#ifdef FLW_DBG
			if ( Available_ != 0 )
				ERRf();
#endif
			if ( _ThreadHandling.Owner ) {
				Red_ = 0;

				FLWDismiss();

				_Unlock();
			}
		}
	protected:
		/*v Called to place up to 'Wanted' bytes in 'Buffer', and not less than
		'Minimum'. Returns how many bytes red. If 'Minimum' == 0, then the function
		must return	as fast as possible if there is no data to read. */
		virtual bsize__ FLWRead(
			bsize__ Minimum,
			datum__ *Buffer,
			bsize__ Wanted ) = 0;
		virtual void FLWDismiss( void )
		{}
		/*f Handle EOFD. To call when no more data available in the medium.
		Return amount of data written. If 0 is returned, then there is no
		more end of flow data available, and an error should be launched. */
		bsize__ HandleEOFD(
			datum__ *Buffer,
			bsize__ Size )
		{
			EOFD_.HandlingEOFD = true;

			if ( EOFD_.HandleToFew && EOFD_.HandleAmount && ( Red_ < AmountMax_ ) )
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
		void reset( bso::bool__ = true ) 
		{
			Cache_ = 0;
			Size_ = 0;
			Red_ = 0;
			AmountMax_ = 0;
			Available_ = Position_ = 0;
			EOFD_.Data = NULL;
			EOFD_.Size = 0;
			EOFD_.HandlingEOFD = EOFD_.HandleAmount = EOFD_.HandleToFew = false;

			_ThreadHandling.Mutex = FLW_NO_MUTEX;

			_ThreadHandling.Owner = true;
		}
		iflow__( void )
		{
			reset( false );
		}
		virtual ~iflow__( void )
		{
			reset();
		}
		void Init(
			datum__ *Cache,
			bsize__ Size,
			size__ AmountMax,
#ifdef CPE__MT
			mutex__ Mutex )
#else
			mutex__ Mutex = FLW_NO_MUTEX )
#endif
		{
			reset();

			Cache_ = Cache;
			Size_ = Size;
			AmountMax_ = AmountMax;

			_ThreadHandling.Mutex = Mutex;

			if ( Mutex == FLW_NO_MUTEX )
				_ThreadHandling.Owner = true;
			else
				_ThreadHandling.Owner = false;
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

			if ( Available_ > Amount )	// This means we have red too much data.
				ERRf();

			EOFD_.HandleAmount = true;
			EOFD_.HandleToFew = true;
			AmountMax_ = Amount;

			Red_ = Available_;

			if ( Red_ == AmountMax_ )
				EOFD_.HandlingEOFD = true;
		}
		//f Return the amount of data red since last 'Reset()'.
		size__ AmountRed( void ) const
		{
			return Red_ - Available_;
		}
		void SetAmountMax( size__ AmountMax )
		{
			AmountMax_ = AmountMax;
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
	class oflow__	// Althought it has a destructor, it's a '__' version because it has to be inherited to be used.
	{
	private:
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
		struct {
			mutex__ Mutex;	// Mutex pour protèger la ressource.
			bso::bool__ Owning;	// Si à 'true', alors on est l'actuel posseseur du 'flow'.
		} _ThreadHandling;	// Gestion du multitâche.
		void _TestAndLock( void )
		{
			if ( !_ThreadHandling.Owning ) {
				_Lock( _ThreadHandling.Mutex );
				_ThreadHandling.Owning = true;
			}
		}
		void _Unlock( void )
		{
#ifdef FLW_DBG
			if ( !_ThreadHandling.Owning )
				ERRu();
#endif
			if ( _ThreadHandling.Mutex != FLW_NO_MUTEX ) {
				_ThreadHandling.Owning = false;
				flw::_Unlock( _ThreadHandling.Mutex );
			}
		}
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

			FLWSynchronizing();

			_Unlock();
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
	protected:
		virtual bsize__ FLWWrite(
			const datum__ *Buffer,
			bsize__ Wanted,
			bsize__ Minimum,
			bool Synchronization ) = 0;
		virtual void FLWSynchronizing( void )
		{}
	public:
		void reset( bso::bool__ = true )
		{
			Cache_ = NULL;
			Size_ = Free_ = 0;
			AmountMax_ = 0;
			Written_ = 0;

			_ThreadHandling.Mutex = FLW_NO_MUTEX;

			_ThreadHandling.Owning = true;
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
			datum__ *Cache,
			bsize__ Size,
			size__ AmountMax,
#ifdef CPE__MT
			mutex__ Mutex )
#else
			mutex__ Mutex = FLW_NO_MUTEX )
#endif
		{
			reset();

			Cache_ = Cache;
			Size_ = Free_ = Size;
			AmountMax_ = AmountMax;

			_ThreadHandling.Mutex = Mutex;

			if ( Mutex == FLW_NO_MUTEX )
				_ThreadHandling.Owning = true;
			else
				_ThreadHandling.Owning = false;
		}
/*
		virtual ~oflow__( void )
		{
			if ( Size_ != Free_ )
				_Synchronize();
		}
*/
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

	//c Basic input/output flow.
	class ioflow__// Althought it has a destructor, it's a '__' version because it has to be inherited to be used.
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
		~ioflow__( void )
		{
			reset();
		}
		void Init(
			datum__ *ICache,
			bsize__ ISize,
			size__ ReadAmountMax,
			datum__ *OCache,
			bsize__ OSize,
			size__ WriteAmountMax,
#ifdef CPE__MT
			mutex__ IMutex,
			mutex__ OMutex )
#else
			mutex__ IMutex = FLW_NO_MUTEX,
			mutex__ OMutex = FLW_NO_MUTEX )
#endif
		{
			iflow__::Init( ICache, ISize, ReadAmountMax, IMutex );
			oflow__::Init( OCache, OSize, WriteAmountMax, OMutex );

		}
		void Init(
			datum__ *Cache,
			bsize__ Size,
			size__ AmountMax,
#ifdef CPE__MT
			mutex__ IMutex,
			mutex__ OMutex )
#else
			mutex__ IMutex = FLW_NO_MUTEX,
			mutex__ OMutex = FLW_NO_MUTEX )
#endif
		{
			iflow__::Init( Cache, Size / 2, AmountMax, IMutex );
			oflow__::Init( Cache + Size / 2, Size / 2, AmountMax, OMutex );
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
