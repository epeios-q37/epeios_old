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

#ifdef CPE__UNIX
#	ifndef FLW_LET_SIGPIPE
#		define FLW__IGNORE_SIGPIPE
#	endif
#endif

namespace flw {
	//t Amount of data.
	typedef bso::ulong__		amount__;

	//d The max value for a amount.
#	define FLW_AMOUNT_MAX	BSO_ULONG_MAX

	//t Size (of a cache, for example).
	typedef size_t			size__;

	//d The max value for a size.
#	define FLW_SIZE_MAX	INT_MAX

	//t Type of a datum.
	typedef unsigned char		datum__;

	//c Base input flow.
	class iflow__ // Althought it has a destructor, it's a '--' version because it has to be inherited to be used.
	{
	private:
		// The cache.
		datum__ *Cache_;
		// Size of the cache.
		size__ Size_;
		// Amount of data available in the cache.
		size__ Available_;
		// Position of the available data.
		size__ Position_;
		// Amount of data red since the last reset.
		amount__ Red_;
		// Max amount of data alllowed between 2 reset.
		amount__ AmountMax_;
		// Data for the end of flow (EOFD).
		struct {
			// The data.
			const datum__ *Data;
			// Size of the data.
			size__ Size;
			int
				//At true if we are currently handling end of file data.
				HandlingEOFD:		1,
				// At true if we have to handle the amount of data (in other word, EOFD are handled after 'AmountMax_' data red.
				HandleAmount:	1,
				// At true if we have to generate an error if not all awaited data re red. Only significant if 'HandleLenght' at true.
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

				if ( ( Amount < Minimum ) || !Amount )
					ERRf();
			} else if ( EOFD_.HandleAmount ) {

				if ( Wanted > ( AmountMax_ - Red_ ) )
					Wanted = AmountMax_ - Red_;

				Amount = _Read( Minimum, Buffer, Wanted );

				if ( Amount < Minimum )
					ERRf();	/* Because, if there is not enough data in the flow,
							'FLWGet' already calls the 'HandleEOFD' function. */

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
		void _FillCache( size__ Minimum )
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
		size__ _ReadFromCache(
			size__ Amount,
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
		size__ _ReadFromCacheOrDirectly(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted )
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
			size__ Available = _ReadFromCache( Amount, Buffer );

			if ( Amount > Available )
				Available += _ReadFromCacheOrDirectly( Amount - Available, Buffer + Available, Amount - Available );
		}
		// Generic read.
		size__ _Read(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted );
	protected:
		/*v Called to place up to 'Wanted' bytes in 'Buffer', and not less than
		'Minimum'. Returns how many bytes red. If 'Minimum' == 0, then the function
		must return	as fast as possible if there is no data to read. */
		virtual size__ FLWRead(
			size__ Minimum,
			datum__ *Buffer,
			size__ Wanted ) = 0;
		/*f Handle EOFD. To call when no more data available in the medium.
		Return amount of data written. If 0 is returned, then there is no
		more end of flow data available, and an error should be launched. */
		size__ HandleEOFD(
			datum__ *Buffer,
			size__ Size )
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
			
			return Size;
		}
	public:
		iflow__(
			datum__ *Cache,
			size__ Size,
			amount__ AmountMax )
		{
			Cache_ = Cache;
			Size_ = Size;
			Red_ = 0;
			AmountMax_ = AmountMax;
			Available_ = Position_ = 0;
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
		void Skip( amount__ Amount = 1 )
		{
			while ( Amount-- )
				Get();
		}
		/* 'Data' is a NUL terminated string to use as end of flow data.
		'Data' is NOT duplicated and should not be modified. */
		void EOFD( const char *Data )
		{
			size_t Length = strlen( Data );

			if ( Length > FLW_SIZE_MAX )
				ERRl();

			EOFD_.Data = (const datum__ *)Data;
			EOFD_.Size = (size__)Length;
			EOFD_.HandleAmount = false;
		}
		//f Set the counter of red data to 0.
		void Reset( void )
		{
			Red_ = Available_;

			if ( Red_ == AmountMax_ )
				EOFD_.HandlingEOFD = true;
		}
		/* 'Data' is a NUL terminated string to use as end of flow data.
		'Data' is NOT suplicated and should not be modified. This data will
		be put in the flow after having read 'Amount' bytes.*/
		void EOFD(
			const char *Data,
			amount__ Amount )
		{
			EOFD( Data );

			if ( Available_ > Amount )	// This means we have red too much data.
				Available_ = Amount;

			EOFD_.HandleAmount = true;
			EOFD_.HandleToFew = true;
			AmountMax_ = Amount;
			Reset();
		}
		//f Return the amount of data red since last 'Reset()'.
		amount__ AmountRed( void ) const
		{
			return Red_ - Available_;
		}
		void SetAmountMax( amount__ AmountMax )
		{
			AmountMax_ = AmountMax;
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
		size__ Size_;
		// The amount of bytes yet free.
		size__ Free_;
		// Amount of data written since the last synchronizing.
		amount__ Written_;
		// Max amount of data between 2 synchronizing.
		amount__ AmountMax_;
		// Put up to 'Wanted' and a minimum of 'Minimum' bytes from buffer directly into the device.
		size__ _DirectWrite(
			const datum__ *Buffer,
			size__ Wanted,
			size__ Minimum,
			bool Synchronization );
		void _DumpCache( bool Synchronization )
		{
			size__ Stayed = Size_ - Free_;
			
			if ( ( Stayed != 0 ) || Synchronization ) {
				_DirectWrite( Cache_, Stayed, Stayed, Synchronization );

				Free_ = Size_;
			}
		}
		size__ _WriteIntoCache(
			const datum__ *Buffer,
			size__ Amount )
		{
			if ( Free_ < Amount )
				Amount = Free_;
				
			memcpy( Cache_ + Size_ - Free_, Buffer, (size_t)Amount );
			
			Free_ -= Amount;
			
			return Amount;
		}
		/* Put up to 'Amount' bytes from 'Buffer' directly or through the cache.
		Return amount of bytes written. Cache MUST be EMPTY. */
		size__ _DirectWriteOrIntoCache(
			const datum__ *Buffer,
			size__ Amount )
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
		}
		// Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		size__ _WriteUpTo(
			const datum__ *Buffer,
			size__ Amount )
		{
			size__ AmountWritten = _WriteIntoCache( Buffer, Amount );

			if ( ( AmountWritten == 0 )  && ( Amount != 0 ) ) {
				_DumpCache( false );
				AmountWritten = _DirectWriteOrIntoCache( Buffer, Amount );
			}

			return AmountWritten;
		}
		// Put 'Amount' data from 'Buffer'.
		void _Write(
			const datum__ *Buffer,
			size__ Amount );
	protected:
		virtual size__ FLWWrite(
			const datum__ *Buffer,
			size__ Wanted,
			size__ Minimum,
			bool Synchronization ) = 0;
		virtual void FLWSynchronizing( int )	// Obsolete ; rajout du paramètre 'int( pour détecter ceux qui l'utilisent.
		{}
	public:
		oflow__(
			datum__ *Cache,
			size__ Size,
			amount__ AmountMax )
		{
			Cache_ = Cache;
			Size_ = Free_ = Size;
			AmountMax_ = AmountMax;
			Written_ = 0;
		}
		virtual ~oflow__( void )
		{
			if ( Size_ != Free_ )
				_Synchronize();
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
		void Synchronize( void )
		{
			_Synchronize();
		}
		//f Return the amount of data written since last 'Synchronize()'.
		amount__ AmountWritten( void ) const
		{
			return Written_ + ( Size_ - Free_ );
		}
		void SetAmountMax( amount__ AmountMax )
		{
			AmountMax_ = AmountMax;
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

		if ( Length >= FLW_AMOUNT_MAX )
			ERRl();

		OutputFlow.Write( String, (size__)( Length + 1 ) );
	}

	//c Basic input/output flow.
	class ioflow__// Althought it has a destructor, it's a '__' version because it has to be inherited to be used.
	: public iflow__,
	  public oflow__
	{
	protected:
		virtual void FLWSynchronizing( void )
		{
			iflow__::Reset();
		}
	public:
		ioflow__(
			datum__ *ICache,
			size__ ISize,
			amount__ ReadAmountMax,
			datum__ *OCache,
			size__ OSize,
			amount__ WriteAmountMax )
			: iflow__( ICache, ISize, ReadAmountMax ),
			  oflow__( OCache, OSize, WriteAmountMax )
		{}
		ioflow__(
			datum__ *Cache,
			size__ Size,
			amount__ AmountMax )
			: iflow__( Cache, Size / 2, AmountMax ),
			  oflow__( Cache + Size / 2, Size / 2, AmountMax )
		{}
		void SetAmountMax(
			amount__ ReadAmountMax,
			amount__ WriteAmountMax )
		{
			iflow__::SetAmountMax( ReadAmountMax );
			oflow__::SetAmountMax( WriteAmountMax );
		}
		void SetAmountMax( amount__ AmountMax )
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

	if ( Length > FLW_AMOUNT_MAX )
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
