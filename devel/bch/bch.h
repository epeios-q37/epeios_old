/*
	Header for the 'bch' library by Claude SIMON (csimon@epeios.org)
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
	using namespace epeios;

	//c The kernel of a bunch. For internal use only.
	template <class type, class mmr, class mng, typename row> class _bunch
	: public mmr,
	  public mng
	{
	private:
		/* Pousse (décalge vers la fin de l'ensemble) les objets à la position
		'Position' de 'Quantite' positions */
		void Pousser_(
			epeios::row_t__ Position,
			epeios::size__ Quantite )
		{
			Allouer_( Amount() + Quantite, aem::mDefault );
			mmr::Store( *this, Amount() - Position - Quantite, Position, Position + Quantite);
		}
		// Insere à 'PosDest' 'Quantite' objets situé à partir de 'PosSource' de 'Source'.
		void Inserer_(
			const _bunch &Source,
			epeios::size__ Quantite,
			epeios::row_t__ PosSource,
			epeios::row_t__ PosDest )
		{
			Pousser_( PosDest, Quantite );
			mmr::Store( Source, Quantite, PosSource, PosDest );
		}
		// Insere 'Quantite' objets de 'Objets' à 'Position'.
		void Inserer_(
			const type *Objets,
			epeios::size__ Quantite,
			epeios::row_t__ Position )
		{
			Pousser_( Position, Quantite );
			mmr::Store( Objets, Quantite, Position );
		}
		// Allocation de la place nécessaire à 'Taille' objets.
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
			if ( Quantity > Amount() )
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
		//f Append 'Amount' object from 'Buffer'. Return the position where the objects are put.
		row Append(
			const type *Buffer,
			epeios::size__ Amount )
		{
			row Position = this->Amount();

			StoreAndAdjust( Buffer, Amount, Position );

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
			row Position = this->Amount();

			StoreAndAdjust( Bunch, Amount - *Position, Position, Position );

			return Position;
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
	#ifdef SET_DBG
			if ( Amount > this->Amount() )
				ERRu();
	#endif
			Allouer_( this->Amount() - Amount, aem::mDefault );
		}
		//f Insert at 'RowDest' 'Amount' objects from 'Source' at 'RowSource'.
		void Insert(
			const _bunch &Source,
			epeios::size__ Amount,
			row RowSource,
			row RowDest )
		{
			Inserer_( Source, Amount, PosSource, PosDest );
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
		//f Delete 'Amount' objects at row 'Row'. The size of the set is reduced.
		void Delete(
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
		row Row(
			const type &Object,
			row Begin,
			row End ) const
		{
			if ( Amount() )
				return mmr::Row( Object, Begin, End );
		}
		//f Return row of 'Object' beginning at 'Begin' (included), or 'NONE' if not found.
		row Row(
			const type &Object,
			row Begin = 0 ) const
		{
			return Row( Object, Begin, Amount() );
		}
		//f Store 'Count' 'Object' at 'Row'. Adjust the size of the bunch.
		void StoreAndAdjust(
			const type &Object,
			epeios::size__ Count,
			row Row = 0 )
		{
			Allocate( Row + Count );

			mmr::Store( Object, Count, Row );
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
	template <class type, typename row, typename aem> class _bunch_
	: public _bunch<type, tym::E_MEMORYt_( type, row ), aem, row >
	{
	public:
		struct s
		: public _bunch<type, tym::E_MEMORYt_( type, row ), aem, row >::s
		{};
		_bunch_( s &S )
		: _bunch<type, tym::E_MEMORYt_( type, row ), aem, row >( S )
		{};
		void reset( bool P = true )
		{
			_bunch<type, tym::E_MEMORYt_( type, row ), aem, row >::reset( P );
			Memory().reset( P );
		}
		_bunch_ &operator =( const _bunch_ &Op )
		{
			_bunch<type, tym::E_MEMORYt_( type, row ), aem, row >::operator =( Op );

			Allocate( Op.Amount() );

			Memory().Store( Op, Op.Amount() );

			return *this;
		}
		void write( flw::oflow___ &OFlow ) const
		{
			flw::Put( Amount(), OFlow );
			Memory().write( 0, _bunch<type, tym::E_MEMORYt_( type, row ), aem, row >::Amount(), OFlow );
		}
		void read( flw::iflow___ &IFlow )
		{
			size__ Amount;

			flw::Get( IFlow, Amount );
			_bunch<type, tym::E_MEMORYt_( type, row ), aem, row >::Allocate( Amount );
			Memory().read( IFlow, 0, _bunch<type, tym::E_MEMORYt_( type, row ), aem, row >::Amount() );

		}
		//f Adjust the extent to amount.
		void Adjust( void )
		{
			if ( _bunch<type, tym::E_MEMORYt_( type, row ), aem, row >::Force( Amount() ) )
				Memory().Allocate( Amount() );
		}
	};

	/*c A bunch of static object of type 'type'. Use 'E_BUNCH_( type )' rather then directly this class. */
	template <class type, typename row> class bunch_
	: public _bunch_<type, row, aem::amount_extent_manager_< row > >
	{
	public:
		struct s
		: public _bunch_<type, row, aem::amount_extent_manager_< row > >::s
		{};
		bunch_( s &S )
		: _bunch_<type, row, aem::amount_extent_manager_< row > >( S )
		{};
	};

	E_AUTO2( bunch )

	//m A set of static object of type 'Type'. Use this rather then 'set_set_<type>'.
	#define E_BUNCHt_( Type, r )		bunch_< Type, r >

	#define E_BUNCHt( Type, r )		bunch< Type, r >

	#define E_BUNCH( Type )		E_BUNCHt( Type, epeios::row__ )
	#define E_BUNCH_( Type )	E_BUNCHt_( Type, epeios::row__ )


	/*c A portable bunch of static object of type 'type'. Use 'E_PBUNCH_( type )' rather then directly this class. */
	template <class type, typename row> class p_bunch_
	: public _bunch_<type, row, aem::p_amount_extent_manager_< row > >
	{
	public:
		struct s
		: public _bunch_<type, row, aem::p_amount_extent_manager_< row > >::s
		{};
		p_bunch_( s &S )
		: _bunch_<type, row, aem::p_amount_extent_manager_< row > >( S )
		{};
	};

	E_AUTO2( p_bunch )

	//m A set of static object of type 'Type'. Use this rather then 'set_set_<type>'.
	#define E_P_BUNCHt_( Type, r )		p_bunch_< Type, r >

	#define E_P_BUNCHt( Type, r )		p_bunch< Type, r >

	#define E_P_BUNCH( Type )		E_P_BUNCHt( Type, epeios::row__ )
	#define E_P_BUNCH_( Type )	E_P_BUNCHt_( Type, epeios::row__ )

	//f Return 'S1' - 'S2' which respectively begins at 'BeginS1' et 'Begins2'.
	template <class t, typename r, typename m> inline bso::sign__ Compare(
		const _bunch_<t, r, m> &S1,
		const _bunch_<t, r, m> &S2,
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
	template <class t, typename r> inline bso::sign__ Compare(
		const E_BUNCHt_( t, r ) &S1,
		const E_BUNCHt_( t, r ) &S2,
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
	: public _bunch< type, tym::E_MEMORYt__( type, size, row ), aem, row >
	{
	public:
		struct s
		: public _bunch<type, tym::E_MEMORYt__( type, size, row ), aem, row >::s {} S_;
		_bunch__( void ) 
		: _bunch<type, tym::E_MEMORYt__( type, size, row ), aem, row >( S_ ) {}
		_bunch__ &operator =( const _bunch__ &S )
		{
			_bunch<type, tym::E_MEMORYt__( type, size, row ), aem, row >::StoreAndAdjust( S, S.Amount_ );
			Size_ = S.Amount_;

			return *this;
		}
		void Init( void )
		{
			_bunch<type, tym::E_MEMORYt__( type, size, row ), aem, row >::Init();
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
	: public _bunch< type, tym::E_MEMORYt___( type, row ), aem, row >
	{
	public:
		struct s
		: public _bunch<type, tym::E_MEMORYt___( type, row ), aem, row >::s {} S_;
		_bunch___( void ) 
		: _bunch<type, tym::E_MEMORYt___( type, row ), aem, row >( S_ ) {}
		_bunch___ &operator =( const _bunch___ &S )
		{
			_bunch<type, tym::E_MEMORYt___( type, row ), aem, row >::StoreAndAdjust( S, S.Amount_ );
			Size_ = S.Amount_;

			return *this;
		}
		void Init( void )
		{
			_bunch<type, tym::E_MEMORYt___( type, row ), aem, row >::Init();
			_bunch<type, tym::E_MEMORYt___( type, row ), aem, row >::SetStepValue( 0 );
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
