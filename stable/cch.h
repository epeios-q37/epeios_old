/*
	Header for the 'cch' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2001, 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef CCH__INC
#define CCH__INC

#define CCH_NAME		"CCH"

#define	CCH_VERSION	"$Revision$"

#define CCH_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &CCHTutor;

#if defined( XXX_DBG ) && !defined( CCH_NODBG )
#define CCH_DBG
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
//D CaCHe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bch.h"
#include "ctn.h"

#ifdef CCH_USE_SMA_HEAP
#	define CCH__USE_SMA_HEAP
#endif

#ifdef CCH__USE_SMA_HEAP
#	include "sma.h"
#endif

#ifdef CCH__USE_SMA_HEAP
#	ifdef CCH_SMA_HEAP_SIZE
#		define CCH__SMA_HEAP_SIZE	CCH_SMA_HEAP_SIZE
#	else
#		define CCH__SMA_HEAP_SIZE	1000000
#	endif
#endif

namespace cch {
	//t Size of a cache.
	typedef bso::size__ csize__;

	using bso::size__;

	//t Position in the buffer.
	typedef bso::size__ position__;

	#define CCH_POSITION_MAX	EPEIOS_SIZE_MAX
	#define CCH_ROW_MAX		EPEIOS_ROW_MAX

	//t Amount of data in a buffer.
	typedef csize__ amount__;

	template <class type__, typename r> class const_bunch_caller__
	{
	protected:
		virtual const bch::E_BUNCHt_( type__, r ) &CCHGetBunch( void ) = 0;
	};

	template <class type__, typename r> class volatile_bunch_caller__
	{
	protected:
		virtual bch::E_BUNCHt_( type__, r ) &CCHGetBunch( void ) = 0;
	};

	//e How the cache is filled for the first time.
	enum first_cache_justification__ {
		fcjLeft,
		fcjRight,
		fcjCentered,
		fcj_amount,
		fcj_Undefined
	};

	// The core of a cache of static objects of type 'type'.
	template <class type__, typename r, class bunch_caller__> class const_bunch_cache___
	: public bunch_caller__
	{
	protected:
		// The buffer which acts as cache.
		type__ *Cache_;
		// At true if 'Cache' was created internally.
		bso::bool__ Internal_;
		// Size of 'Cache'.
		csize__ Size_;
		// Amount of data in 'Cache'
		amount__ Amount_;
		// Position of the first data of the cache in the bunch.
		epeios::row_t__ Position_;
		bso::bool__ IsInsideCache_(
			r Position,
			epeios::size__ Amount )
		{
			return ( *Position >= Position_ )
				     && ( ( *Position + Amount ) <= ( Position_ + Amount_ ) );
		}
		void ReadFromCache_(
			r Position,
			epeios::size__ Amount,
			type__ *Buffer )
		{
#ifdef CCH_DBG
			if ( !IsInsideCache_( Position, Amount ) )
				ERRc();
#endif
			memcpy( Buffer, *Position - Position_ + Cache_, Amount * sizeof( type__ ) );
		}
		epeios::size__ BunchAmount_( void )
		{
			return bunch_caller__::CCHGetBunch().Amount();
		}
		void ReadDirectlyFromBunch_(
			r Position,
			epeios::size__ Amount,
			type__ *Buffer )
		{
			bunch_caller__::CCHGetBunch().Recall( Position, Amount, Buffer );
		}
		void FillCache_(
			r Position,
			epeios::size__ Amount )
		{
			epeios::size__ BunchAmount = BunchAmount_();

#ifdef CCH_DBG
			if ( Amount > Size_ )
				ERRc();
#endif
			if ( BunchAmount <= Size_ ) {
				Position_ = 0;
				Amount_ = BunchAmount;
			} else {
				Amount_ = Size_;

				if ( Position_ == NONE ) {
					if ( ( ( Size_ - Amount ) / 2 ) > *Position )
						Position_ = 0;
					else
						Position_ = *Position - ( Size_ - Amount ) / 2;
				} else if ( *Position < Position_ ) {
					if ( ( *Position + Amount ) < Size_ )
						Position_ = 0;
					else
						Position_ = *Position + Amount - Size_;
				} else {
					Position_ = *Position;
				}

				if ( ( Position_ + Size_ ) > BunchAmount )
					Position_ = BunchAmount - Size_;
			}

			if ( Amount_ != 0 )
				ReadDirectlyFromBunch_( Position_, Amount_, Cache_ );
			else
				Position_ = NONE;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( Internal_ )
#ifdef CCH__USE_SMA_HEAP
					Heap.Free( Cache_ );
#else
					if ( Cache_ != NULL )
						free( Cache_ );
#endif
			}

			Cache_ = NULL;
			Internal_ = false;
			Size_ = 0;
			Amount_ = 0;

			Position_ = NONE;

		}
		const_bunch_cache___( void )
		{
			reset( false );
		}
		~const_bunch_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with 'Buffer' of size 'Size'.
		void Init(
			type__ *Buffer,
			epeios::size__ Size,
			first_cache_justification__ FirstCacheJustification )
		{
			reset();

			Cache_ = Buffer;
			Size_ = Size;

			switch( FirstCacheJustification ) {
			case fcjLeft:
				Position_ = 0;
				break;
			case fcjRight:
				Position_ = CCH_ROW_MAX;
				break;
			case fcjCentered:
				Position_ = NONE;
				break;
			default:
				ERRu();
				break;
			}
		}
		//f Initialisation and creation of a buffer of size 'Size'.
		void Init(
			csize__ Size,
			first_cache_justification__ FirstCacheJustification )
		{
#ifdef CCH__USE_SMA_HEAP
			type__ *Buffer = (type__ *)Heap.Allocate( Size * sizeof( type__ ) );
#else
			type__ *Buffer = (type__ *)malloc( Size * sizeof( type__ ) );
#endif

			if ( Buffer == NULL )
				ERRa();

			Init( Buffer, Size, FirstCacheJustification );

			Internal_ = true;
		}
	};

	// The core of a cache of static objects of type 'type'.
	template <class type__, typename r, class bunch_caller__> class volatile_bunch_cache___
	: public const_bunch_cache___< type__, r, bunch_caller__>
	{
	private:
		position__ 
			First_,	// Position of the first datum written.
			Last_;	// Position of the last datum written.
	protected:
		void Allocate_( epeios::size__ Size )
		{
			bunch_caller__::CCHGetBunch().Allocate( Size );
		}
		void WriteIntoCache_(
			const type__ *Buffer,
			epeios::size__ Amount,
			r Position )
		{
			position__ First = *Position - First_;
			position__ Last = First + Amount - 1;
#ifdef CCH_DBG
			if ( !IsInsideCache_( Position, Amount ) )
				ERRc();
#endif
			if ( First < First_ )
				First_ = First;

			if ( Last > Last_ )
				Last_ = Last;

			memcpy( const_bunch_cache___< type__, r, bunch_caller__>::Cache_ + First, Buffer, Amount * sizeof( type__ ) );
		}
		void WriteDirectlyIntoBunch_(
			const type__ *Buffer,
			epeios::size__ Amount,
			r Position )
		{
			bunch_caller__::CCHGetBunch().Store( Buffer, Amount, Position );
		}
		void DumpCache_( bso::bool__ Adjust )
		{
			if ( Amount_ )
				if ( Last_ > First_ ) {
					if ( Adjust )
						Allocate_( Position_ + Amount_ );
#ifdef CCH_DBG
					if ( ( Position_ + Amount_ ) > BunchAmount_() )
						ERRc();
#endif
					WriteDirectlyIntoBunch_( Cache_ + First_, Last_ - First_ + 1, Position_ );
				}

			Amount_ = 0;
			Position_ = NONE;
			Last_ = 0;
			First_ = CCH_POSITION_MAX;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			const_bunch_cache___< type__, r, bunch_caller__>::reset( P );
			Last_ = 0;
			First_ = CCH_POSITION_MAX;
		}
		volatile_bunch_cache___( void )
		{
			reset( false );
		}
		~volatile_bunch_cache___( void )
		{
			reset( true );
		}
	};

	//c A read-only cache of static object of type 't'.
	template <class type__, typename r> class core_read_only_cache___
	: public const_bunch_cache___<type__, r, const_bunch_caller__<type__,r> >
	{
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Synchronize();
			}

			const_bunch_cache___<type__, r, const_bunch_caller__<type__,r> >::reset( P );
		}
		core_read_only_cache___( void )
		{
			reset( false );
		}
		~core_read_only_cache___( void )
		{
			reset( true );
		}
		void Read(
			r Position,
			epeios::size__ Amount,
			type__ *Buffer )
		{
			if ( IsInsideCache_( Position, Amount ) )
				ReadFromCache_( Position, Amount, Buffer );
			else {
				Amount_ = 0;
				Position_ = NONE;

				if ( Amount > Size_ ) {
					ReadDirectlyFromBunch_( Position, Amount, Buffer );
				} else {
					FillCache_( Position, Amount );
					ReadFromCache_( Position, Amount, Buffer );
				}
			}
		}
		//f Return data at 'Position'.
		type__ Get( r Position )
		{
			type__ Data;

			Read( Position, 1, &Data );

			return Data;
		}
		/*f Synchronize the cache with the bunch. To call if you want to do
		an operation directly on the underlying bunch. */
		void Synchronize( void )
		{
			Amount_ = 0;
			Position_ = NONE;
		}
	};

	//c A read-write cache of static object of type 't'.
	template <class type__, typename r> class core_read_write_cache___
	: public volatile_bunch_cache___<type__, r, volatile_bunch_caller__<type__, r> >
	{
	protected:
		// At true if you append data.
		bso::bool__ AppendMode_;
	public:
		void reset( bso::bool__ P = true )
		{
			volatile_bunch_cache___<type__, r, volatile_bunch_caller__<type__, r> >::reset( P );

			AppendMode_ = false;
		}
		core_read_write_cache___( void )
		{
			reset( false );
		}
		~core_read_write_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with bunch 'Bunch', end 'Buffer' of size 'Size'.
		void Init(
			type__ *Buffer,
			epeios::size__ Size,
			first_cache_justification__ FirstCacheJustification )
		{
			volatile_bunch_cache___<type__, r, volatile_bunch_caller__<type__, r> >::Init( Buffer, Size, FirstCacheJustification );
		}
		//f Initialisation and creation of a buffer of size 'Size'.
		void Init(
			csize__ Size,
			first_cache_justification__ FirstCacheJustification )
		{
			volatile_bunch_cache___<type__, r, volatile_bunch_caller__<type__, r> >::Init( Size, FirstCacheJustification );
		}
		//f Put 'Amount' data at 'Position' in 'Buffer'.
		void Read(
			r Position,
			epeios::size__ Amount,
			type__ *Buffer )
		{
			if ( IsInsideCache_( Position, Amount ) )
				ReadFromCache_( Position, Amount, Buffer );
			else {
				DumpCache_( AppendMode_ );
				AppendMode_ = false;

				if ( Amount > Size_ ) {
					ReadDirectlyFromBunch_( Position, Amount, Buffer );
				} else {
					FillCache_( Position, Amount );
					ReadFromCache_( Position, Amount, Buffer );
				}
			}
		}
		//f Return data at 'Position'.
		type__ Get( r Position )
		{
			type__ Data;

			Read( Position, 1, &Data );

			return Data;
		}
		//f Put 'Amount' data from 'Buffer' at 'Position'.
		void Write(
			const type__ *Buffer,
			epeios::size__ Amount,
			r Position )
		{
			if ( IsInsideCache_( Position, Amount ) )
				WriteIntoCache_( Buffer, Amount, Position );
			else {
				DumpCache_( false );

				if ( Amount > Size_ ) {
					WriteDirectlyIntoBunch_( Buffer, Amount, Position );
				} else {
					FillCache_( Position, Amount );
					WriteIntoCache_( Buffer, Amount, Position );
				}
			}
		}
		//f Put 'Data' at 'Position'.
		void Put(
			type__ Data,
			r Position )
		{
			Write( &Data, 1, Position );
		}
		//f Append 'Data'.
		r Append( type__ Data )
		{
			r Position;

			if ( AppendMode_ && ( Amount_ == Size_ ) )
			{
				Synchronize();
				AppendMode_ = false;
			}

			if ( !AppendMode_ ) {
				Synchronize();
				Position_ = BunchAmount_();
				Amount_ = 0;
				AppendMode_ = true;
			}

			WriteIntoCache_( &Data, 1, Position = Position_ + Amount_++ );

			return Position;
		}
		/*f Return the amount of data contained in the underlying bunch, 
		considering data put in the cache. */
		epeios::size__ Amount( void )
		{
			if ( AppendMode_ )
				return Position_ + Amount_;
			else
				return BunchAmount_();
		}

		//f Synchronize the content of the cache and the content of the bunch.
		void Synchronize( void )
		{
			DumpCache_( AppendMode_ );
			AppendMode_ = false;
		}
	};


	//c A read-only cache of static object of type 't'.
	template <class type__, typename r> class read_only_cache___
	: public core_read_only_cache___<type__, r>
	{
	private:
		const bch::E_BUNCHt_( type__, r ) *Bunch_;
	protected:
		virtual const bch::E_BUNCHt_( type__, r ) &CCHGetBunch( void )
		{
			return *Bunch_;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			core_read_only_cache___<type__, r>::reset( P );

			Bunch_ = NULL;
		}
		read_only_cache___( void )
		{
			reset( false );
		}
		~read_only_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with bunch 'Bunch', end 'Buffer' of size 'Size'.
		void Init(
			const bch::bunch_<type__, r> &Bunch,
			type__ *Buffer,
			epeios::size__ Size,
			first_cache_justification__ FirstCacheJustification )
		{
			core_read_only_cache___<type__, r>::Init( Buffer, Size, FirstCacheJustification );

			Bunch_ = &Bunch;
		}
		//f Initialisation and creation of a buffer of size 'Size'.
		void Init( 
			const bch::bunch_<type__, r> &Bunch,
			csize__ Size,
			first_cache_justification__ FirstCacheJustification )
		{
			core_read_only_cache___<type__, r>::Init( Size, FirstCacheJustification );

			Bunch_ = &Bunch;
		}
	};


	//c A read-only cache of static object of type 't'.
	template <class type__, typename r> class read_write_cache___
	: public core_read_write_cache___<type__, r>
	{
	private:
		bch::E_BUNCHt_( type__, r ) *Bunch_;
	protected:
		virtual bch::E_BUNCHt_( type__, r ) &CCHGetBunch( void )
		{
			return *Bunch_;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			core_read_write_cache___<type__, r>::reset( P );

			Bunch_ = NULL;
		}
		read_write_cache___( void )
		{
			reset( false );
		}
		~read_write_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with bunch 'Bunch', end 'Buffer' of size 'Size'.
		void Init(
			bch::bunch_<type__, r> &Bunch,
			type__ *Buffer,
			epeios::size__ Size,
			first_cache_justification__ FirstCacheJustification )
		{
			core_read_write_cache___<type__, r>::Init( Buffer, Size, FirstCacheJustification );

			Bunch_ = &Bunch;
		}
		//f Initialisation with bunch 'Bunch', and creation of a buffer of size 'Size'.
		void Init(
			bch::bunch_<type__, r> &Bunch,
			csize__ Size,
			first_cache_justification__ FirstCacheJustification )
		{
			core_read_write_cache___<type__, r>::Init( Size, FirstCacheJustification );

			Bunch_ = &Bunch;
		}
	};

	//c A read-only cache of object 't' using an item (in 'CTN4 library meaning).
	template <class type__, typename rb, typename rc> class item_read_only_cache___
	: public core_read_only_cache___<type__, rb>
	{
	private:
		ctn::E_CMITEMt( bch::E_BUNCHt_( type__, rb ), rc ) *Item_;
		rc PositionInContainer_;
	protected:
		virtual const bch::E_BUNCHt_( type__, rb ) &CCHGetBunch( void )
		{
			return Item_->operator()( PositionInContainer_ );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Synchronize();
			}

			core_read_only_cache___<type__, rb>::reset( P );

			PositionInContainer_ = NONE;
		}
		item_read_only_cache___( void )
		{
			reset( false );
		}
		~item_read_only_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with bunch 'Bunch', end 'Buffer' of size 'Size'.
		void Init(
			ctn::E_CMITEMt( bch::E_BUNCHt_( type__, rb ), rc ) &Item,
			rc PositionInContainer,
			csize__ Size,
			first_cache_justification__ FirstCacheJustification )
		{
			core_read_only_cache___<type__, rb>::Init( Size, FirstCacheJustification );

			PositionInContainer_ = PositionInContainer;
			Item_ = &Item;
		}
	};

	//c A read-only cache of static object of type 't'.
	template <class type__, typename rb, typename rc> class item_read_write_cache___
	: public core_read_write_cache___<type__, rb>
	{
	private:
		ctn::E_MITEMt( bch::E_BUNCHt_( type__, rb ), rc ) *Item_;
		rc PositionInContainer_;
	protected:
		virtual bch::E_BUNCHt_( type__, rb ) &CCHGetBunch( void )
		{
			return Item_->operator()( PositionInContainer_ );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Synchronize();
			}

			Item_ = NULL;
			core_read_write_cache___<type__, rb>::reset( P );
		}
		item_read_write_cache___( void )
		{
			reset( false );
		}
		~item_read_write_cache___( void )
		{
			reset( true );
		}
		//f Initialisation with bunch 'Bunch', end 'Buffer' of size 'Size'.
		void Init(
			ctn::E_MITEMt( bch::E_BUNCHt_( type__, rb ), rc ) &Item,
			rc PositionInContainer,
			csize__ Size,
			first_cache_justification__ FirstCacheJustification )
		{
			core_read_write_cache___<type__, rb>::Init( Size, FirstCacheJustification );

			PositionInContainer_ = PositionInContainer;
			Item_ = &Item;
		}
	};

	template <class type__,
		      typename item, typename item_cache,
			  typename rb, typename rc>
			  class read_only_caches___
	{
	protected:
		bch::E_BUNCHt( item_cache *, rc ) Caches_;
		read_only_cache___<item_cache *, rc> Cache_;
		item Item_;
		csize__ CacheSize_;
		first_cache_justification__ FirstCacheJustification_;
		void _Fill(
			rc First,
			rc Last )
		{
			Caches_.Store( NULL, First, *Last - *First + 1 );
		}
		void Erase_(
			rc First,
			rc Last )
		{
			epeios::row_t__ &Current = *First;

			for( ;Current <= *Last; Current++ )
				if ( Caches_( Current ) != NULL ) {
#if	1
					delete Caches_( Current );
#else
					Caches_( Current )->reset();
					free( Caches_( Current ) );
#endif
					Caches_.Store( NULL, Current );
				}
		}
		item_cache &GetCache_( rc P )
		{
#ifdef CCH_DBG
			if ( *P >= Caches_.Amount() )
				ERRu();
#endif
			if ( Cache_.Get( P ) == NULL ) {
				Cache_.Synchronize();
				item_cache *IC;
#if 1				
				if ( ( IC = new item_cache ) == NULL )
					ERRa();
#else
				if ( ( IC = (item_cache *)malloc( sizeof( item_cache ) ) ) == NULL )
					ERRa();

				IC->reset( false );
#endif

				IC->Init( Item_, P, CacheSize_, FirstCacheJustification_ );
				Caches_.Store( IC, P );
			}

			return *Cache_.Get( P );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				Synchronize();
				if ( Caches_.Amount() )
					Erase_( Caches_.First(), Caches_.Last() );
			}

			Item_.reset( P );

			Caches_.reset( P );
			CacheSize_ = 0;
			FirstCacheJustification_ = fcj_Undefined;

		}
		read_only_caches___( void )
		{
			reset( false );
		}
		~read_only_caches___( void )
		{
			reset( true );
		}
		/*f Initialization with 'Container' and, for e&ch item,
		creation of a buffer of size 'Size'. */
		void Init( 
			const ctn::E_MCONTAINERt_( bch::E_BUNCHt_( type__, rb ), rc ) &Container,
			csize__ Size,
			csize__ IntermediateSize,
			first_cache_justification__ FirstCacheJustification )
		{
			reset();

			CacheSize_ = Size;

			FirstCacheJustification_ = FirstCacheJustification;

			Item_.Init( Container );

			Caches_.Init();

			Allocate( Container.Amount() );

			Cache_.Init( Caches_, IntermediateSize, fcjCentered );
		}
		//f Put 'Amount' data at 'Position' in 'Buffer'.
		void Read(
			rc PositionInContainer,
			rb PositionInBunch,
			epeios::size__ Amount,
			type__ *Buffer )
		{
#ifdef CCH_DBG
			if ( *PositionInContainer >= Caches_.Amount() )
				ERRu();
#endif
			GetCache_( PositionInContainer ).Read( PositionInBunch, Amount, Buffer );

		}
		//f Return data at 'Position'.
		type__ Get(
			rc PositionInContainer,
			rb PositionInBunch )
		{
			type__ Data;

			Read( PositionInContainer, PositionInBunch, 1, &Data );

			return Data;
		}
		//f Allocate room for 'Amount' items.
		void Allocate( epeios::size__ Amount )
		{
			epeios::size__ OldAmount = Caches_.Amount();

			Cache_.Synchronize();

			if ( OldAmount > Amount ) {
				Erase_( OldAmount - 1, Amount );
				Caches_.Allocate( Amount );
			} else {
				Caches_.Allocate( Amount );
				_Fill( OldAmount, Amount - 1 );
			}
		}
		/*f Return the amount of data contained in the bunch correspondinf to 'Position', 
		considering data put in the cache. */
		epeios::size__ Amount( rc Position )
		{
			return GetCache_( Position ).Amount();
		}
		//f Synchronizing with the underlying container.
		void Synchronize( void )
		{
			item_cache *IC;

			rc R = Caches_.First();

			while( R != NONE ) {
				IC = &GetCache_( R );

				if ( IC != NULL )
					IC->Synchronize();

				R = Caches_.Next( R );
			}

			Item_.Flush();
		}
	};

	template <class type__, typename rb, typename rc> class read_write_caches___
	: public read_only_caches___<type__,ctn::E_MITEMt( bch::E_BUNCHt_( type__, rb ), rc ),item_read_write_cache___<type__, rb,rc>,rb,rc>
	{
	public:
		void reset( bso::bool__ P = true )
		{
			read_only_caches___<type__,ctn::E_MITEMt( bch::E_BUNCHt_( type__, rb ), rc ),item_read_write_cache___<type__, rb,rc>,rb,rc>::reset( P );	
		}
		read_write_caches___( void )
		{
			reset( false );
		}
		~read_write_caches___( void )
		{
			reset( true );
		}
		/*f Initialization with 'Container' and, for each item,
		creation of a buffer of size 'Size'. */
		void Init( 
			ctn::E_MCONTAINERt_( bch::E_BUNCHt_( type__, rb ), rc ) &Container,
			csize__ Size,
			csize__ IntermediateSize,
			first_cache_justification__ FirstCacheJustification )
		{
			reset();

			CacheSize_ = Size;

			FirstCacheJustification_ = FirstCacheJustification;

			Item_.Init( Container );

			Caches_.Init();

			Allocate( Container.Amount() );

			Cache_.Init( Caches_, IntermediateSize, fcjCentered );
		}
		//f Put 'Amount' data at 'Position' in 'Buffer'.
		void Write(
			type__ *Buffer,
			epeios::size__ Amount,
			rc PositionInContainer,
			rb PositionInBunch )
		{
			GetCache_( PositionInContainer ).Write( Buffer, Amount, PositionInBunch );

		}
		//f Return data at 'Position'.
		void Put(
			type__ Data,
			rc PositionInContainer,
			rb PositionInBunch )
		{
			Write( &Data, 1, PositionInContainer, PositionInBunch );
		}
		//f Append 'Data' for container 'P"ositionInContainer'.
		rb Append(
			type__ Data,
			rc PositionInContainer )
		{
			return GetCache_( PositionInContainer ).Append( Data );
		}
	};

#ifdef CCH__USE_SMA_HEAP
	extern sma::memory_heap___ Heap;
#endif

}

#define E_RO_CACHEt___( type, r )	read_only_cache___<type, r>

#define E_RO_CACHE___( type )	E_RO_CACHEt___( type, epeios::row__ )

#define E_RW_CACHEt___( type, r )	read_write_cache___<type, r>

#define E_RW_CACHE___( type )	E_RW_CACHEt___( type, epeios::row__ )

#define E_RO_CACHEt___( type, r )	read_only_cache___<type, r>

#define E_RO_CACHE___( type )	E_RO_CACHEt___( type, epeios::row__ )

#define E_RW_CACHEt___( type, r )	read_write_cache___<type, r>

#define E_RW_CACHE___( type )	E_RW_CACHEt___( type, epeios::row__ )

#define E_RO_CACHESt___( type, rb, rc )	read_only_caches___<type, ctn::E_CMITEMt( bch::E_BUNCHt_( type, rb ), rc ), cch::item_read_only_cache___<type,rb,rc>, rb, rc>

#define E_RO_CACHES___( type, rb )	E_RO_CACHESt___( type, rb, epeios::row__ )

#define E_RW_CACHESt___( type, rb, rc )	read_write_caches___<type, rb, rc>

#define E_RW_CACHES___( type, rb ) E_RW_CACHESt___( type, rb, epeios::row__ )

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
