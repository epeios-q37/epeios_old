/*
	Header for the 'bch' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2001-2004 Claude SIMON (csimon@epeios.org).

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

#ifndef BCH__INC
#define BCH__INC

#define BCH_NAME		"BCH"

#define	BCH_VERSION	"$Revision$"

#define BCH_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &BCHTutor;

#if defined( XXX_DBG ) && !defined( BCH_NODBG )
#define BCH_DBG
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
//D BunCH 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "tym.h"
#include "aem.h"
#include "epeios.h"

namespace bch {

	//c The kernel of a bunch. For internal use only. The 'sh' template parameter is to simplify the use of the 'STR' library.
	template <class type, class mmr, class mng, typename row, typename sh> class _bunch
	: public mmr,
	  public mng
	{
	private:
		/* Pousse (d�calge vers la fin de l'ensemble) les objets � la position
		'Position' de 'Quantite' positions */
		void Pousser_(
			epeios::row_t__ Position,
			epeios::size__ Quantite )
		{
			Allouer_( mng::Amount() + Quantite, aem::mDefault );
			mmr::Store( *this, mng::Amount() - Position - Quantite, Position, Position + Quantite);
		}
		// Insere � 'PosDest' 'Quantite' objets situ� � partir de 'PosSource' de 'Source'.
		void Inserer_(
			const _bunch &Source,
			epeios::size__ Quantite,
			epeios::row_t__ PosSource,
			epeios::row_t__ PosDest )
		{
			Pousser_( PosDest, Quantite );
			mmr::Store( Source, Quantite, PosSource, PosDest );
		}
		// Insere 'Quantite' objets de 'Objets' � 'Position'.
		void Inserer_(
			const type *Objets,
			epeios::size__ Quantite,
			epeios::row_t__ Position )
		{
			Pousser_( Position, Quantite );
			mmr::Store( Objets, Quantite, Position );
		}
		// Allocation de la place n�cessaire � 'Taille' objets.
		void Allouer_(
			epeios::size__ Taille,
			aem::mode Mode )
		{
			if ( mng::AmountToAllocate( Taille, Mode ) )
				mmr::Allocate( Taille );
		}
		// allocate if the set not big enough.
		void AllocateIfNecessary_( epeios::size__ Quantity )
		{
			if ( Quantity > mng::Amount() )
				Allouer_( Quantity, aem::mDefault );
		}
	public:
		struct s
		: public mmr::s,
		  public mng::s
		{};
		_bunch( s &S )
		: mmr( S ),
		  mng( S )
		{}
		void reset( bso::bool__ P )
		{
			mmr::reset( P );
			mng::reset( P );
		}
		_bunch &operator =( const _bunch &SC )
		{
			mng::operator =( SC );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			mmr::Init();
			mng::Init();
		}
		//f Initialization with 'Seed' containing 'Size' objects.
		void Init(
			const type *Seed,
			epeios::size__ Size )
		{
			this->Init();
			
			StoreAndAdjust( Seed, Size );
		}
		//f Allocate 'Size' objects. Extent is forced to 'Size' when 'Mode' = 'mFit'.
		void Allocate(
			epeios::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			Allouer_( Size, Mode );
		}
		//f Allocate 'Size' objects and fill newly created object with 'Object'. Extent is forced to 'Size' when 'Mode' = 'mFit'.
		void Allocate(
			epeios::size__ Size,
			const type &Object,
			aem::mode Mode = aem::mDefault )
		{
			epeios::size__ PreviousSize = mng::Amount();

			Allouer_( Size, Mode );

			if ( Size > PreviousSize )
				Store( Object, PreviousSize, Size - PreviousSize );
		}
		/*f Store at 'Offset' 'Amount' objects from 'Bunch' beginnig at 'Row'.
		Adjust the size of the bunch. */
		void StoreAndAdjust(
			const _bunch &Bunch,
			epeios::size__ Amount,
			row Row = 0,
			row Offset = 0 )
		{
			Allocate( Amount + *Offset );

			mmr::Store( Bunch, Amount, Row, Offset );
		}
		/*f Store at 'Offset' the content of 'Bunch' from position 'Row' to the end.
		Adjust the size of the bunch. */
		void StoreAndAdjust(
			const _bunch &Bunch,
			row Row,
			row Offset = 0 )
		{
			StoreAndAdjust( Bunch, Bunch.Amount() - Row, Row, Offset );
		}
		//f Store at 'Offset' 'Amount' objects from 'Buffer'.
		void StoreAndAdjust(
			const type *Buffer,
			epeios::size__ Amount,
			row Offset = 0 )
		{
			Allocate( Amount + *Offset );

			mmr::Store( Buffer, Amount, Offset );
		}
		//f Store at 'Offset' objects from 'Buffer'.
		void StoreAndAdjust(
			const type *Buffer,
			row Offset = 0 )
		{
			Allocate( sh::SizeOf( Buffer ) + *Offset );

			mmr::Store( Buffer, sh::SizeOf( Buffer ), Offset );
		}
		// Remplit tout le conteneur avec 'object' � partir de la position 'offset'.
		void Set(
			const type &Object,
			row Offset = 0 )
		{
			Store( Object, Offset, Amount() - *Offset );
		}
		//f Append 'Amount' object from 'Buffer'. Return the position where the objects are put.
		row Append(
			const type *Buffer,
			epeios::size__ Amount )
		{
			row Position = this->Amount();

			StoreAndAdjust( Buffer, Amount, Position );

			return Position;
		}
		//f Append 'Buffer'. Return the position where the objects are put.
		row Append( const type *Buffer )
		{
			row Position = this->Amount();

			StoreAndAdjust( Buffer, sh::SizeOf( Buffer ), Position );

			return Position;
		}
		//f Append the object 'Object'. Return the position where the object is put.
		row Append( const type &Object )
		{
			return Append( &Object, 1 );
		}
		//f Append 'Amount' 'Object's. Return the position where appended.
		void Append(
			const type &Object,
			epeios::size__ Amount )
		{
			StoreAndAdjust( Object, this->Amount(), Amount );
		}
		//f Append 'Amount' objects at 'Position' from 'Bunch'. Return the position where the object are put.
		row Append(
			const _bunch &Bunch,
			epeios::size__ Amount,
			row Position = 0 )
		{
			row Offset = this->Amount();

			StoreAndAdjust( Bunch, Amount, Position, Offset );

			return Position;
		}
		// Ajoute le contenu de 'Bunch' de la position 'First' � la position 'Last', toute deux incluses.
		row Append(
			const _bunch &Bunch,
			row First,
			row Last )
		{
#ifdef BCH_DBG
			if ( *First > *Last )
				ERRu();
#endif
			return Append( Bunch, *Last - *First + 1, *First );
		}
		//f Append all the objects from 'Bunch' beginning at 'Position'. Return the position where the objects are put.
		row Append(
			const _bunch &Bunch,
			row Position = 0 )
		{
			return Append( Bunch, Bunch.Amount() - *Position, Position );
		}
		//f Remove 'Amount' objects from the end of the bunch.
		void Truncate( epeios::size__ Amount = 1 )
		{
#ifdef BCH_DBG
			if ( Amount > this->Amount() )
				ERRu();
#endif
			Allouer_( this->Amount() - Amount, aem::mDefault );
		}
		//f Remove objects all objects beginning at 'Row'.
		void Truncate( row Row )
		{
#ifdef BCH_DBG
			if ( !Exists( Row ) )
				ERRu();
#endif
			Truncate ( mng::Amount() - *Row );
		}
		//f Remove all objects but 'Amount()' objects from 'Row'. The size of the bunch is readjusted.
		void Crop(
			epeios::size__ Amount,
			row Row = 0 )
		{
			Truncate( this->Amount() - ( *Row + Amount ) );

			Remove( 0, *Row );
		}
		//f Remove all objects but objects between 'First' and 'Last' included.
		void Crop(
			row First,
			row Last )
		{
#ifdef BCH_DBG
			if ( *Last < *First )
				ERRu();
#endif
			Crop( *Last - *First + 1, First );
		}
		//f Insert at 'RowDest' 'Amount' objects from 'Source' at 'RowSource'.
		void Insert(
			const _bunch &Source,
			epeios::size__ Amount,
			row RowSource,
			row RowDest )
		{
			Inserer_( Source, Amount, RowSource, RowDest );
		}
		//f Insert 'Bunch' at 'Row'.
		void Insert(
			const _bunch &Bunch,
			row Row )
		{
			Inserer_( Bunch, Bunch.Amount(), 0, *Row );
		}
		//f Insert 'Object' at 'Row'
		void Insert(
			const type &Object,
			row Row )
		{
			Inserer_( &Object, 1, *Row );
		}
		//f Insert at 'Row' 'Amount' objects from 'Source'.
		void Insert(
			const type *Source,
			epeios::size__ Amount,
			row Row )
		{
			Inserer_( Source, Amount, *Row );
		}
		//f Insert at 'Row' 'Source'.
		void Insert(
			const type *Source,
			row Row )
		{
			Inserer_( Source, sh::SizeOf( Source ), *Row );
		}
		//f Remove 'Amount' objects at row 'Row'. The size of the set is reduced.
		void Remove(
			row Row,
			epeios::size__ Amount = 1)
		{
			mmr::Store( *this, this->Amount() - ( Amount + *Row ), *Row + Amount, Row );

			Allouer_( this->Amount() - Amount, aem::mDefault );
		}
		//f Return the row of the first of 'Amount' new object.
		row New( epeios::size__ Amount = 1 )
		{
			epeios::row_t__ P = this->Amount();

			Allouer_( P + Amount, aem::mDefault );

			return P;
		}
		//f Return row of 'Object' between 'Begin' (included) and 'End' (excluded), or 'NONE' if not found.
		row Search(
			const type &Object,
			row Begin,
			row End ) const
		{
			return mmr::Search( Object, Begin, End );
		}
		//f Return row of 'Object' beginning at 'Begin' (included), or 'NONE' if not found.
		row Search(
			const type &Object,
			row Begin = 0 ) const
		{
			return Search( Object, Begin, mng::Amount() );
		}
		//f Store 'Count' 'Object' at 'Row'. Adjust the size of the bunch.
		void StoreAndAdjust(
			const type &Object,
			row Row,
			epeios::size__ Count )
		{
			Allocate( *Row + Count );

			mmr::Store( Object, Row, Count );
		}
		//f Return reference to memory.
		mmr &Memory( void )
		{
			return *this;
		}
		//f Return reference to memory.
		const mmr &Memory( void ) const
		{
			return *this;
		}
	};


	/*c The core set of static object of type 'type'. Internal use only. */
	template <class type, typename row, typename aem, typename sh> class _bunch_
	: public _bunch<type, tym::E_MEMORYt_( type, row ), aem, row, sh >
	{
	public:
		struct s
		: public _bunch<type, tym::E_MEMORYt_( type, row ), aem, row, sh >::s
		{};
		_bunch_( s &S )
		: _bunch<type, tym::E_MEMORYt_( type, row ), aem, row, sh >( S )
		{};
		void reset( bool P = true )
		{
			_bunch<type, tym::E_MEMORYt_( type, row ), aem, row, sh >::reset( P );
			_bunch<type, tym::E_MEMORYt_( type, row ), aem, row, sh >::Memory().reset( P );
		}
		_bunch_ &operator =( const _bunch_ &Op )
		{
			_bunch<type, tym::E_MEMORYt_( type, row ), aem, row , sh>::operator =( Op );

			Allocate( Op.Amount() );

			_bunch<type, tym::E_MEMORYt_( type, row ), aem, row, sh >::Memory().Store( Op, Op.Amount() );

			return *this;
		}
		void write( flw::oflow__ &OFlow ) const
		{
			flw::Put( aem::Amount(), OFlow );
			tym::E_MEMORYt_( type, row )::Memory().write( 0, _bunch<type, tym::E_MEMORYt_( type, row ), aem, row, sh >::Amount(), OFlow );
		}
		void read( flw::iflow__ &IFlow )
		{
			epeios::size__ Amount;

			flw::Get( IFlow, Amount );
			_bunch<type, tym::E_MEMORYt_( type, row ), aem, row, sh >::Allocate( Amount );
			tym::E_MEMORYt_( type, row )::Memory().read( IFlow, 0, _bunch<type, tym::E_MEMORYt_( type, row ), aem, row, sh >::Amount() );

		}
		//f Adjust the extent to amount.
		void Adjust( void )
		{
			if ( _bunch<type, tym::E_MEMORYt_( type, row ), aem, row, sh >::Force( aem::Amount() ) )
				tym::E_MEMORYt_( type, row )::Memory().Allocate( aem::Amount() );
		}
	};

	class _dummy_size_handler
	{
	public:
		static epeios::size__ SizeOf( void * )
		{
			ERRu();
			return 0;	// To avoid a warning.
		}
	};


	/*c A bunch of static object of type 'type'. Use 'E_BUNCH_( type )' rather then directly this class. */
	template <class type, typename row, typename sh = _dummy_size_handler> class bunch_
	: public _bunch_<type, row, aem::amount_extent_manager_< row >, sh >
	{
	public:
		struct s
		: public _bunch_<type, row, aem::amount_extent_manager_< row >, sh >::s
		{};
		bunch_( s &S )
		: _bunch_<type, row, aem::amount_extent_manager_< row >, sh >( S )
		{};
	};

	E_AUTO2( bunch )

	//m A set of static object of type 'Type'. Use this rather then 'set_set_<type>'.
	#define E_BUNCHt_( Type, r )	bunch_< Type, r >
	#define E_BUNCHt( Type, r )		bunch< Type, r >

	#define E_BUNCHxt_( Type, r, s )	bunch_< Type, r, s  >
	#define E_BUNCHxt( Type, r,s  )		bunch< Type, r, s >

	#define E_BUNCHx( Type, s )	E_BUNCHxt( Type, epeios::row__, s )
	#define E_BUNCHx_( Type, s )	E_BUNCHxt_( Type, epeios::row__, s )

	#define E_BUNCH( Type )		E_BUNCHt( Type, epeios::row__ )
	#define E_BUNCH_( Type )	E_BUNCHt_( Type, epeios::row__ )

	typedef E_BUNCH_( epeios::row__ ) relations_;
	E_AUTO( relations )

	void _GetRelations(
		const uym::untyped_memory_ &Sorted,
		const uym::untyped_memory_ &Unsorted,
		epeios::size__ Size,
		epeios::row_t__ Limit,
		epeios::datum__ *Buffer,
		E_BUNCH_( epeios::row__ ) &Relations );

	template <typename t> inline void GetRelations(
		const bch::E_BUNCH_( t ) &Sorted,
		const bch::E_BUNCH_( t ) &Unsorted,
		bch::E_BUNCH_( epeios::row__ ) &Relations )
	{
		epeios::datum__ Buffer[sizeof( t )];
#ifdef BCH_DBG
		if ( Sorted.Amount() != Unsorted.Amount() )
			ERRu();
#endif
		_GetRelations( Sorted, Unsorted, sizeof( t ), Sorted.Amount() * sizeof( t ), Buffer, Relations );
	}




	/*c A portable bunch of static object of type 'type'. Use 'E_PBUNCH_( type )' rather then directly this class. */
	template <class type, typename row> class p_bunch_
	: public _bunch_<type, row, aem::p_amount_extent_manager_< row >, _dummy_size_handler >
	{
	public:
		struct s
		: public _bunch_<type, row, aem::p_amount_extent_manager_< row >, _dummy_size_handler >::s
		{};
		p_bunch_( s &S )
		: _bunch_<type, row, aem::p_amount_extent_manager_< row >, _dummy_size_handler >( S )
		{};
	};

	E_AUTO2( p_bunch )

	//m A set of static object of type 'Type'. Use this rather then 'set_set_<type>'.
	#define E_P_BUNCHt_( Type, r )		p_bunch_< Type, r >

	#define E_P_BUNCHt( Type, r )		p_bunch< Type, r >

	#define E_P_BUNCH( Type )		E_P_BUNCHt( Type, epeios::row__ )
	#define E_P_BUNCH_( Type )	E_P_BUNCHt_( Type, epeios::row__ )

	//f Return 'S1' - 'S2' which respectively begins at 'BeginS1' et 'Begins2'.
	template <class t, typename r, typename m, typename s> inline bso::sign__ Compare(
		const _bunch_<t, r, m, s> &S1,
		const _bunch_<t, r, m, s> &S2,
		r BeginS1 = 0,
		r BeginS2 = 0 )
	{
		if ( ( S1.Amount() - *BeginS1 ) != ( S2.Amount() - *BeginS2 ) )
			if ( ( S1.Amount() - *BeginS1 ) > ( S2.Amount() - *BeginS2 ) )
				return 1;
			else
				return -1;
		else
			return tym::Compare( S1, S2, BeginS1, BeginS2, S1.Amount() - *BeginS1 );
	}


	//f Return 'S1' - 'S2' which respectively begin at 'BeginS1' et 'Begins2' and have a length of 'Amount'.
	template <class t, typename r, typename s> inline bso::sign__ Compare(
		const E_BUNCHxt_( t, r, s ) &S1,
		const E_BUNCHxt_( t, r, s ) &S2,
		r BeginS1,
		r BeginS2,
		epeios::size__ Amount )
	{
		return tym::Compare( S1, S2, BeginS1, BeginS2, Amount );
	}


	template <class t, typename r> inline bso::bool__ operator ==(
		const E_BUNCHt_( t, r ) &S1,
		const E_BUNCHt_( t, r ) &S2 )
	{
		return !Compare( S1, S2 );
	}

	template <class t, typename r> inline bso::bool__ operator !=(
		const E_BUNCHt_( t, r ) &S1,
		const E_BUNCHt_( t, r ) &S2 )
	{
		return Compare( S1, S2 ) != 0;
	}

	//c A set of maximum 'size' static objects of type 'type'. Use 'SET__( type, size )' rather then directly this class.
	template <typename type, int size, typename row, typename aem> class _bunch__
	: public _bunch< type, tym::E_MEMORYt__( type, size, row ), aem, row, _dummy_size_handler >
	{
	public:
		struct s
		: public _bunch<type, tym::E_MEMORYt__( type, size, row ), aem, row, _dummy_size_handler >::s {} S_;
		_bunch__( void ) 
		: _bunch<type, tym::E_MEMORYt__( type, size, row ), aem, row, _dummy_size_handler >( S_ ) {}
		_bunch__ &operator =( const _bunch__ &S )
		{
			_bunch<type, tym::E_MEMORYt__( type, size, row ), aem, row, _dummy_size_handler >::StoreAndAdjust( S, S.Amount_ );

			return *this;
		}
		void Init( void )
		{
			_bunch<type, tym::E_MEMORYt__( type, size, row ), aem, row, _dummy_size_handler >::Init();
//			_bunch<type, tym::E_MEMORYt__( type, size, row ), aem, row >::SetStepValue( 0 );
		}
	};

	template <typename type, int size, typename row> class bunch__
	: public _bunch__< type, size, row, aem::amount_extent_manager__<size, row> >
	{};


	//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
	#define E_BUNCHt__( c, i, r )	bunch__<c, i, r>

	#define E_BUNCH__( c, i )		E_BUNCHt__( c, i , epeios::row__ )

	//c A set of static objects of type 'type'. Use 'BUNCH___( type )' rather then directly this class.
	template <typename type, typename row, typename aem> class _bunch___
	: public _bunch< type, tym::E_MEMORYt___( type, row ), aem, row, _dummy_size_handler >
	{
	public:
		struct s
		: public _bunch<type, tym::E_MEMORYt___( type, row ), aem, row, _dummy_size_handler >::s {} S_;
		_bunch___( void ) 
		: _bunch<type, tym::E_MEMORYt___( type, row ), aem, row, _dummy_size_handler >( S_ ) {}
		_bunch___ &operator =( const _bunch___ &S )
		{
			_bunch<type, tym::E_MEMORYt___( type, row ), aem, row, _dummy_size_handler >::StoreAndAdjust( S, S.Amount_ );
	
			return *this;
		}
		void Init( void )
		{
			_bunch<type, tym::E_MEMORYt___( type, row ), aem, row, _dummy_size_handler >::Init();
			_bunch<type, tym::E_MEMORYt___( type, row ), aem, row, _dummy_size_handler >::SetStepValue( 0 );
		}
	};

	template <typename type, typename row> class bunch___
	: public _bunch___< type, row, aem::amount_extent_manager___<row> >
	{};


	//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
	#define E_BUNCHt___( c, r )	bunch___<c, r>

	#define E_BUNCH___( c )		E_BUNCHt___( c, epeios::row__ )



	template <typename type, int size, typename row> class p_bunch__
	: public _bunch__< type, size, row, aem::p_amount_extent_manager__<size, row> >
	{};


	//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
	#define E_P_BUNCHt__( c, i, r )	p_bunch__<c, i, r>

	#define E_P_BUNCH__( c, i )		E_P_BUNCHt__( c, i , epeios::row__ )



}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
