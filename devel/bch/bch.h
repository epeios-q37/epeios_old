/*
  Header for the 'bch' library by Claude SIMON (csimon@epeios.org)
  Copyright (C) 2002 Claude SIMON (csimon@epeios.org) 

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
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D BunCH.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "tym.h"
#include "aem.h"
#include "epeios.h"

namespace bch {
	using namespace epeios;

	//c The kernel of a bunch. For internal use only.
	template <class type, class mmr, class mng, typename row> class bunch_kernel_
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
			mmr::Write( *this, Amount() - Position - Quantite, Position, Position + Quantite);
		}
		// Insere à 'PosDest' 'Quantite' objets situé à partir de 'PosSource' de 'Source'.
		void Inserer_(
			const bunch_kernel_ &Source,
			epeios::size__ Quantite,
			epeios::row_t__ PosSource,
			epeios::row_t__ PosDest )
		{
			Pousser_( PosDest, Quantite );
			mmr::Write( Source, Quantite, PosSource, PosDest );
		}
		// Insere 'Quantite' objets de 'Objets' à 'Position'.
		void Inserer_(
			const type *Objets,
			epeios::size__ Quantite,
			epeios::row_t__ Position )
		{
			Pousser_( Position, Quantite );
			mmr::Write( Objets, Quantite, Position );
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
		bunch_kernel_( s &S )
		: mmr( S ),
		  mng( S )
		{}
		void reset( bso::bool__ P )
		{
			mmr::reset( P );
			mng::reset( P );
		}
		bunch_kernel_ &operator =( const bunch_kernel_ &SC )
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
			
			WriteAndAdjust( Seed, Size );
		}
		//f Allocate 'Size' objects. Extent is forced to 'Size' when 'Mode' = 'mFit'.
		void Allocate(
			epeios::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			Allouer_( Size, Mode );
		}
		//f Same as 'Write()', but allocate additionnaly mmr if needed.
		void WriteAndAdjust(
			const bunch_kernel_ &Bunch,
			epeios::size__ Amount,
			row Row = 0,
			row Offset = 0 )
		{
			AllocateIfNecessary_( Amount + *Offset );

			mmr::Write( Set, Amount, Row, Offset );
		}
		//f Same as 'Write()', but allocate additionnaly mmr if needed.
		void WriteAndAdjust(
			const type *Buffer,
			epeios::size__ Amount,
			row Row = 0 )
		{
			AllocateIfNecessary_( Amount + *Row );

			mmr::Write( Buffer, Amount, Row );
		}
		//f Add the object 'Object'. Return the row where object added.
		row Add( const type &Object )
		{
			epeios::size__ Amount = this->Amount();

			Allouer_( Amount + 1, aem::mDefault );

			mmr::Write( Object , Amount );

			return Amount;
		}
		//f Adding 'Amount' object from 'Buffer'. Return the row of the first object added.
		row Add(
			const type *Buffer,
			epeios::size__ Amount )
		{
			epeios::row_t__ Retour = this->Amount();

			Allouer_( Retour + Amount, aem::mDefault );

			mmr::Write( Buffer, Amount, Retour );

			return Retour;
		}
		//f Adding 'Amount' object from 'Set'. Return the row of the first object added.
		row Add(
			const bunch_kernel_ &Set,
			epeios::size__ Amount )
		{
			epeios::row_t__ Retour = this->Amount();

			Allouer_( Retour + Amount, aem::mDefault );

			mmr::Write( Set, Amount, 0, Retour );

			return Retour;
		}
		//f Remove 'Amount' objects from the end of the set.
		void Truncate( epeios::size__ Amount = 1 )
		{
	#ifdef SET_DBG
			if ( Amount > this->Amount() )
				ERRu();
	#endif
			Allouer_( this->Amount() - Amount, aem::mDefault );
		}
		//f Add 'Set'. Return the row where added.
		row Add( const bunch_kernel_ &Set )
		{
			return Add( Set, Set.Amount() );
		}
		//f Insert at 'PosDest' 'Amount' objects from 'Source' at 'PosSource'.
		void Insert(
			const bunch_kernel_ &Source,
			epeios::size__ Amount,
			row RowSource,
			row RowDest )
		{
			Inserer_( Source, Amount, PosSource, PosDest );
		}
		//f Insert 'Set' at 'Row'.
		void Insert(
			const bunch_kernel_ &Set,
			row Row )
		{
			Inserer_( Set, Set.Amount(), 0, *Row );
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
			mmr::Write( *this, this->Amount() - ( Amount + *Row ), *Row + Amount, Row );

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
		//f Fill at 'Row' with 'Object' 'Count' time.
		void Fill(
			const type &Object,
			epeios::size__ Count,
			row Row = 0 )
		{
			mmr::Fill( Object, Count, Row );
		}
		//f Fill from 'Row' with 'Object'.
		void Fill(
			const type &Object,
			row Row = 0 )
		{
			Fill( Object, Row, Amount() );
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
	template <class type, typename row, typename aem> class bunch_core_
	: public bunch_kernel_<type, tym::E_MEMORYt_( type, row ), aem, row >
	{
	public:
		struct s
		: public bunch_kernel_<type, tym::E_MEMORYt_( type, row ), aem, row >::s
		{};
		bunch_core_( s &S )
		: bunch_kernel_<type, tym::E_MEMORYt_( type, row ), aem, row >( S )
		{};
		void reset( bool P = true )
		{
			bunch_kernel_<type, tym::E_MEMORYt_( type, row ), aem, row >::reset( P );
			Memory().reset( P );
		}
		bunch_core_ &operator =( const bunch_core_ &Op )
		{
			bunch_kernel_<type, tym::E_MEMORYt_( type, row ), aem, row >::operator =( Op );

			Allocate( Op.Amount() );

			Memory().Write( Op, Op.Amount() );

			return *this;
		}
		void write( flw::oflow___ &OFlow ) const
		{
			flw::Put( Amount(), OFlow );
			Memory().write( 0, bunch_kernel_<type, tym::E_MEMORYt_( type, row ), aem, row >::Amount(), OFlow );
		}
		void read( flw::iflow___ &IFlow )
		{
			size__ Amount;

			flw::Get( IFlow, Amount );
			bunch_kernel_<type, tym::E_MEMORYt_( type, row ), aem, row >::Allocate( Amount );
			Memory().read( IFlow, 0, bunch_kernel_<type, tym::E_MEMORYt_( type, row ), aem, row >::Amount() );

		}
		//f Adjust the extent to amount.
		void Adjust( void )
		{
			if ( bunch_kernel_<type, tym::E_MEMORYt_( type, row ), aem, row >::Force( Amount() ) )
				Memory().Allocate( Amount() );
		}
	};

	/*c A bunch of static object of type 'type'. Use 'E_BUNCH_( type )' rather then directly this class. */
	template <class type, typename row> class bunch_
	: public bunch_core_<type, row, aem::amount_extent_manager_< row > >
	{
	public:
		struct s
		: public bunch_core_<type, row, aem::amount_extent_manager_< row > >::s
		{};
		bunch_( s &S )
		: bunch_core_<type, row, aem::amount_extent_manager_< row > >( S )
		{};
	};

	AUTO1( bunch )

	//m A set of static object of type 'Type'. Use this rather then 'set_set_<type>'.
	#define E_BUNCHt_( Type, r )		bunch_< Type, r >

	#define E_BUNCHt( Type, r )		bunch< Type, r >

	#define E_BUNCH( Type )		E_BUNCHt( Type, epeios::row__ )
	#define E_BUNCH_( Type )	E_BUNCHt_( Type, epeios::row__ )


	/*c A portable bunch of static object of type 'type'. Use 'E_PBUNCH_( type )' rather then directly this class. */
	template <class type, typename row> class p_bunch_
	: public bunch_core_<type, row, aem::p_amount_extent_manager_< row > >
	{
	public:
		struct s
		: public bunch_core_<type, row, aem::p_amount_extent_manager_< row > >::s
		{};
		p_bunch_( s &S )
		: bunch_core_<type, row, aem::p_amount_extent_manager_< row > >( S )
		{};
	};

	AUTO1( p_bunch )

	//m A set of static object of type 'Type'. Use this rather then 'set_set_<type>'.
	#define E_P_BUNCHt_( Type, r )		p_bunch_< Type, r >

	#define E_P_BUNCHt( Type, r )		p_bunch< Type, r >

	#define E_P_BUNCH( Type )		E_P_BUNCHt( Type, epeios::row__ )
	#define E_P_BUNCH_( Type )	E_P_BUNCHt_( Type, epeios::row__ )

	//f Return 'S1' - 'S2' which respectively begins at 'BeginS1' et 'Begins2'.
	template <class t, typename r, typename m> inline bso::sbyte__ Compare(
		const bunch_core_<t, r, m> &S1,
		const bunch_core_<t, r, m> &S2,
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
	template <class t, typename r> inline bso::sbyte__ Compare(
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
	template <typename type, int size, typename row, typename aem> class bunch_core__
	: public bunch_kernel_< type, tym::E_MEMORYt__( type, size, row ), aem, row >
	{
	public:
		struct s
		: public bunch_kernel_<type, tym::E_MEMORYt__( type, size, row ), aem, row >::s {} S_;
		bunch_core__( void ) 
		: bunch_kernel_<type, tym::E_MEMORYt__( type, size, row ), aem, row >( S_ ) {}
		bunch_core__ &operator =( const bunch_core__ &S )
		{
			bunch_kernel_<type, tym::E_MEMORYt__( type, size, row ), aem, row >::WriteAndAdjust( S, S.Amount_ );
			Size_ = S.Amount_;

			return *this;
		}
		void Init( void )
		{
			bunch_kernel_<type, tym::E_MEMORYt__( type, size, row ), aem, row >::Init();
			bunch_kernel_<type, tym::E_MEMORYt__( type, size, row ), aem, row >::SetStepValue( 0 );
		}
	};

	template <typename type, int size, typename row> class bunch__
	: public bunch_core__< type, size, row, aem::amount_extent_manager_<row> >
	{};


	//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
	#define E_BUNCHt__( c, i, r )	bunch__<c, i, r>

	#define E_BUNCH__( c, i )		E_BUNCHt__( c, i , epeios::row__ )

	//c A set of static objects of type 'type'. Use 'BUNCH___( type )' rather then directly this class.
	template <typename type, typename row, typename aem> class bunch_core___
	: public bunch_kernel_< type, tym::E_MEMORYt___( type, row ), aem, row >
	{
	public:
		struct s
		: public bunch_kernel_<type, tym::E_MEMORYt___( type, row ), aem, row >::s {} S_;
		bunch_core___( void ) 
		: bunch_kernel_<type, tym::E_MEMORYt___( type, row ), aem, row >( S_ ) {}
		bunch_core___ &operator =( const bunch_core___ &S )
		{
			bunch_kernel_<type, tym::E_MEMORYt___( type, row ), aem, row >::WriteAndAdjust( S, S.Amount_ );
			Size_ = S.Amount_;

			return *this;
		}
		void Init( void )
		{
			bunch_kernel_<type, tym::E_MEMORYt___( type, row ), aem, row >::Init();
			bunch_kernel_<type, tym::E_MEMORYt___( type, row ), aem, row >::SetStepValue( 0 );
		}
	};

	template <typename type, typename row> class bunch___
	: public bunch_core___< type, row, aem::amount_extent_manager_<row> >
	{};


	//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
	#define E_BUNCHt___( c, r )	bunch___<c, r>

	#define E_BUNCH___( c )		E_BUNCHt___( c, epeios::row__ )



	template <typename type, int size, typename row> class p_bunch__
	: public bunch_core__< type, size, row, aem::p_amount_extent_manager_<row> >
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
