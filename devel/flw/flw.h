/*
	Header for the 'flw' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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


#include "err.h"
#include "tol.h"
#include "limits.h"

#ifdef CPE__UNIX
#	ifndef FLW_LET_SIGPIPE
#		define FLW__IGNORE_SIGPIPE
#	endif
#endif

namespace flw {
	//t Amount of data.
	typedef int			amount__;

	//t The max value for a amount.
	#define FLW_AMOUNT_MAX	INT_MAX

	//t Size (of a cache, for example).
	typedef int			size__;

	//t The max value for a size.
	#define FLW_SIZE_MAX	INT_MAX

	//t Type of a datum.
	typedef unsigned char		datum__;

	//c Base input flow.
	class iflow___
	{
	private:
		// The cache.
		datum__ *Cache_;
		// Size of the cache.
		size__ Size_;
		// Amount of data available in the cache.
		amount__ Available_;
		// Position of the available data.
		size__ Position_;
		// Data for the end of flow (EOFD).
		struct {
			// The data.
			const datum__ *Data;
			// Size of the data.
			size__ Size;
			// The length of the content of th flow.
			size__ Length;
			int
				//At true if we are currently handling end of file data.
				HandlingEOFD:		1,
				// At true if we have to handle the length of the content of the data (in other word, 'Length' is significatif'.
				HandleLength:	1,
				// At true if we have to generate an error if not all awaited data re red. Only significant if 'HandleLenght' at true.
				HandleToFew:	1;
		} EOFD_;
		/* Put up to 'Wanted' and a minimum of 'Minimum' bytes into 'Buffer'
		directly from device. */
		amount__ DirectGet_(
			amount__ Minimum,
			datum__ *Buffer,
			amount__ Wanted )
		{
			amount__ Amount = 0;

			if ( EOFD_.HandlingEOFD ) {
				Amount = HandleEOFD( Buffer, Wanted );

				if ( ( Amount < Minimum ) || !Amount )
					ERRf();
			} else if ( EOFD_.HandleLength ) {

				if ( EOFD_.Length ) {
					if ( Wanted > EOFD_.Length )
						Wanted = EOFD_.Length;

					Amount = FLWGet( Minimum, Buffer, Wanted );

					if ( Amount < Minimum )
						ERRf();	/* Because, if there is not enough data in the flow,
								'FLWGet' already calls the 'HandleEOFD' function. */

					EOFD_.Length -= Amount;
				}

				if ( !EOFD_.Length )
					EOFD_.HandlingEOFD = true;
			} else 
					Amount = FLWGet( Minimum, Buffer, Wanted );

			return Amount;
		}
#ifdef FLW_DBG
		// Test if there is a cache available.
		void Test_( void )
		{
			if ( ( Cache_ == NULL ) )
				ERRu();
				
			if ( Size_ == 0 )
				ERRu();
		}				
#endif		
		// Fill the cache with a minimum of 'Minimum' bytes. The cache must be empty.
		void FillCache_( amount__ Minimum )
		{
#ifdef FLW_DBG
			Test_();

			if ( Available_ != 0 )				
				ERRc();
#endif
			Available_ = DirectGet_( Minimum, Cache_, Size_ );
			Position_ = 0;
		}
		/* Read from cache up to 'Amount' bytes and place them to 'Buffer'.
		Return amount of data red. */
		amount__ ReadFromCache_(
			amount__ Amount,
			datum__ *Buffer )
		{
#ifdef FLW_DBG
			Test_();
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
		amount__ ReadFromCacheOrDirectly_(
			amount__ Minimum,
			datum__ *Buffer,
			amount__ Wanted )
		{
#ifdef FLW_DBG
			Test_();

			if ( Available_ != 0 )				
				ERRc();
#endif
			if ( Size_ > Wanted ) {
				FillCache_( Minimum );
				return ReadFromCache_( Wanted, Buffer );
			} else
				return DirectGet_( Minimum, Buffer, Wanted );
		}
		/* Place up to 'Amount' bytes in 'Buffer' with a minimum of 'Minimum'.
		Return number of bytes red. */
		amount__ GetUpTo_(
			amount__ Amount,
			datum__ *Buffer,
			amount__ Minimum )
		{
			amount__ Available = ReadFromCache_( Amount, Buffer );

			if ( ( Available < Minimum ) || ( Available == 0 ) )
				Available += ReadFromCacheOrDirectly_( Minimum - Available, Buffer + Available, Amount  - Available );

			return Available;
		}
		//f Place 'Amount' bytes in 'Buffer'.
		void Get_(
			amount__ Amount,
			datum__ *Buffer )
		{
			amount__ Available = ReadFromCache_( Amount, Buffer );

			if ( Amount > Available )
				Available += ReadFromCacheOrDirectly_( Amount - Available, Buffer + Available, Amount - Available );
		}
	protected:
		/*v Called to place up to 'Wanted' bytes in 'Buffer', and not less than
		'Minimum'. Returns how many bytes red. If 'Minimum' == 0, then the function
		must return	as fast as possible if there is no data to read. */
		virtual amount__ FLWGet(
			amount__ Minimum,
			datum__ *Buffer,
			amount__ Wanted ) = 0;
		/*f Handle EOFD. To call when no more data available in the medium.
		Return amount of data written. If 0 is returned, then there is no
		more end of flow data available, and an error should be launched. */
		size__ HandleEOFD(
			datum__ *Buffer,
			size__ Size )
		{
			EOFD_.HandlingEOFD = true;

			if ( EOFD_.HandleToFew && EOFD_.Length )
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
		void reset( bool = true )
		{
			Cache_ = NULL;
			Available_ = Position_ = Size_ = 0;
			EOFD_.Data = NULL;
			EOFD_.Size = EOFD_.Length = 0;
			EOFD_.HandlingEOFD = EOFD_.HandleLength = EOFD_.HandleToFew = false;
		}
		iflow___( void )
		{
			reset( false );
		}
		virtual ~iflow___( void )
		{
			reset( true );
		}
		//f Initialization with 'Cache' of size 'Size' (>=2) as cache.
		void Init(
			datum__ *Cache,
			size__ Size )
		{
			reset();
			
			Cache_ = Cache;
			Size_ = Size;
			EOFD_.Data = NULL;
			EOFD_.Size = 0;
		}
		/*f Place up to 'Amount' bytes in 'Buffer' with a minimum of 'Minimum'.
		Return amount of bytes red. */
		amount__ GetUpTo(
			amount__ Amount,
			void *Buffer,
			amount__ Minimum = 1 )
		{
			return GetUpTo_( Amount, (datum__ *)Buffer, Minimum );
		}
		//f Place 'Amount' bytes in 'Buffer'.
		void Get(
			amount__ Amount,
			void *Buffer )
		{
			Get_( Amount, (datum__ *)Buffer );
		}
		//f Get next byte.
		datum__ Get( void )
		{
			datum__ C;

			Get_( 1, &C );

			return C;
		}
		//f Skip 'Amount' bytes.
		void Skip( amount__ Amount )
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
			EOFD_.HandleLength = false;
		}
		/* 'Data' is a NUL terminated string to use as end of flow data.
		'Data' is NOT suplicated and should not be modified. This data will
		be put in the flow after having read 'Length' bytes.*/
		void EOFD(
			const char *Data,
			size__ Length )
		{
			EOFD( Data );

			if ( Available_ > Length )	// This means we have red too much data.
				Available_ = Length;

			EOFD_.HandleLength = true;
			EOFD_.HandleToFew = true;
			EOFD_.Length = Length - Available_;	// In case il we have already red data.

			if ( !Length )
				EOFD_.HandlingEOFD = true;
		}
		
	};

	//f Get 'StaticObject' from 'InputFlow'.
	template <class t> inline void Get(
		iflow___ &InputFlow,
		t &StaticObject )
	{
		InputFlow.Get( sizeof( t ), &StaticObject );
	}

	/*f Place, from 'IFlow', up to 'Maximum' character in 'Buffer' or until
	reading the 'NULL' character. False is returned, if not enough place to put
	the string with its final NULL character. Buffer contains then 'Maximum'
	characters of the string. Otherwise, it returns true. */
	bool GetString(
		iflow___ &IFlow,
		char *Buffer,
		amount__ Maximum );


	//c Basic output flow.
	class oflow___
	{
	private:
		// The cache.
		datum__ *Cache_;
		// The size of the cache.
		size__ Size_;
		// The amount of bytes yet free.
		amount__ Free_;
		// Put up to 'Wanted' and a minimum of 'Minimum' bytes from buffer directly into the device.
		amount__ DirectPut_(
			const datum__ *Buffer,
			amount__ Wanted,
			amount__ Minimum,
			bool Synchronization )
		{
			return FLWPut( Buffer, Wanted, Minimum, Synchronization );
		}
		/* Try to dump the cache. Return true if succeed, false otherwise.
		'Synchronization' is at true if a synchonization is requested. */
		bool DumpCache_( bool Synchronization )
		{
			amount__ Stayed;
			amount__ AmountWritten;
			
			if ( ( Stayed = Size_ - Free_ ) != 0 ) {
				AmountWritten = DirectPut_( Cache_, Stayed, Stayed, Synchronization );
				
				if ( AmountWritten < Stayed )
					memmove( Cache_, Cache_ + AmountWritten, (size_t)( Stayed - AmountWritten ) );
					
				Free_ += AmountWritten;
			}
			
			return Free_ == Size_;
		}
		/* Force the dumping of the cache. 'Synchronization' at true
		if synchronization required. */
		void ForceDumpingOfCache_( bool Synchronization );				
		/* Write up to 'Amount' bytes from 'Buffer' into the cache.
		Return amount of bytes written. */
		amount__ WriteIntoCache_(
			const datum__ *Buffer,
			amount__ Amount )
		{
			if ( Free_ < Amount )
				Amount = Free_;
				
			memcpy( Cache_ + Size_ - Free_, Buffer, (size_t)Amount );
			
			Free_ -= Amount;
			
			return Amount;
		}
		/* Put up to 'Amount' bytes from 'Buffer' directly or through the cache.
		Return amount of bytes written. Cache MUST be EMPTY. */
		amount__ WriteIntoCacheOrDirectly_(
			const datum__ *Buffer,
			amount__ Amount )
		{
#ifdef FLW_DBG
			if ( Size_ != Free_ )
				ERRc();
#endif
			if ( Amount > Size_ )
				return DirectPut_( Buffer, Amount - 1, Amount - 1, false );
				// The '-1' is to be sure that at least 1 byte is in the cache when a 'Synchronize' occurs.
			else
				return WriteIntoCache_( Buffer, Amount );
		}
		// Force writing of 'Amount( bytes of 'Buffer'.
		void ForceWriting_(
			const datum__ *Data,
			amount__ Amount );
		// Synchronization.
		void Synchronize_( void )
		{
			if ( !DumpCache_( true ) )
				ForceDumpingOfCache_( true );
		}
		// Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		amount__ PutUpTo_(
			const datum__ *Buffer,
			amount__ Amount )
		{
			amount__ AmountWritten = WriteIntoCache_( Buffer, Amount );

			if ( ( AmountWritten == 0 )  && ( Amount != 0 ) )
			{
				if ( DumpCache_( false ) )
					AmountWritten = WriteIntoCacheOrDirectly_( Buffer, Amount );
				else
					AmountWritten = WriteIntoCache_( Buffer, Amount );
			}

			return AmountWritten;
		}
		// Put 'Amount' data from 'Buffer'.
		void Put_(
			const datum__ *Buffer,
			amount__ Amount )
		{
			amount__ AmountWritten = PutUpTo( Buffer, Amount );

			if ( AmountWritten < Amount )
				ForceWriting_( Buffer + AmountWritten, Amount - AmountWritten );
		}
	protected:
		/*v Called to put up to 'Wanted' and a minimum of 'Minimum' bytes from
		'Buffer'. If 'Synchronization' is true, then this fonction is called
		due to a synchronization operation. Return the amount of data written.
		If 'Minimum' == 0, then the function must return as fast as
		possible if no data can be written. */
		virtual amount__ FLWPut(
			const datum__ *Buffer,
			amount__ Wanted,
			amount__ Minimum,
			bool Synchronization ) = 0;
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Size_ != Free_ )
					Synchronize_();
			}
			
			Cache_ = NULL;
			
			Free_ = Size_ = 0;
		}
		oflow___( void )
		{
			reset( false );
		}
		virtual ~oflow___( void )
		{
			reset( true );
		}
		//f Initialization with 'Cache' of size 'Size' as cache.
		void Init(
			datum__ *Cache,
			size__ Size )
		{
			reset();
			
			Cache_ = Cache;
			Size_ = Free_ = Size;
		}
		//f Put up to 'Amount' bytes from 'Buffer'. Return number of bytes written.
		amount__ PutUpTo(
			const void *Buffer,
			amount__ Amount )
		{
			return PutUpTo_( (datum__ *)Buffer, Amount );
		}
		//f Put 'Amount' data from 'Buffer'.
		void Put(
			const void *Buffer,
			amount__ Amount )
		{
			Put_( (const datum__ *)Buffer, Amount );
		}
		//f Put 'C'.
		void Put( datum__ C )
		{
			Put_( &C, 1 );
		}
		//f Synchronization.
		void Synchronize( void )
		{
			Synchronize_();
		}
	};

	//f Write to 'OutputFlow' 'StaticObject'.
	template <class t> inline void Put(
		const t &StaticObjet,
		oflow___ &OutputFlow )
	{
		OutputFlow.Put( &StaticObjet, sizeof( t ) );
	}

	//f Write to 'OutputFlow' the 'NULL' terminated String 'String' WITH the 'NULL' character.
	inline void PutString(
		const char *String,
		oflow___ &OutputFlow )
	{
		size_t Length = strlen( String );

		if ( Length >= FLW_AMOUNT_MAX )
			ERRl();

		OutputFlow.Put( String, (amount__)( Length + 1 ) );
	}

	//c Basic input/output flow.
	class ioflow___
	: public iflow___,
	  public oflow___
	{
	public:
		void reset( bool P = true )
		{
			iflow___::reset( P );
			oflow___::reset( P );
		}
		ioflow___( void )
		{
			reset( false );
		}
		virtual ~ioflow___( void )
		{
			reset( true );
		}
		/*f Initialization with 'ICache' of size 'ISize' and 'OCache' of size
		'OSize' as, respectively, cache and size of this cache for the input flow
		and output flow. */
		void Init(
			datum__ *ICache,
			size__ ISize,
			datum__ *OCache,
			size__ OSize )
		{
			iflow___::Init( ICache, ISize );
			oflow___::Init( OCache, OSize );
		}
		//f Initialisation with cache 'Cache' of size 'Size'.
		void Init(
			datum__ *Cache,
			size__ Size )
		{
			Init( Cache, Size / 2, Cache + Size / 2, Size / 2 );
		}
	};
}

inline flw::oflow___ &operator <<(
	flw::oflow___ &OFlow,
	const char *String )
{
	size_t Length = strlen( String );

	if ( Length > FLW_AMOUNT_MAX )
		ERRl();

	OFlow.Put( String, (flw::amount__)Length );

	return OFlow;
}

inline flw::oflow___ &operator <<(
	flw::oflow___ &OFlow,
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
