/*
  Header for the 'bch' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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

#define BCH_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BCHTutor;

#if defined( XXX_DBG ) && !defined( BCH_NODBG )
#define BCH_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
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

	//c Set core. Don't use ; for internal use only.
	template <class type, class mmr, class mng, typename r> class bunch_core_
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
			const bunch_core_ &Source,
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
		bunch_core_( s &S )
		: mmr( S ),
		  mng( S )
		{}
		void reset( bso::bool__ P )
		{
			mmr::reset( P );
			mng::reset( P );
		}
		bunch_core_ &operator =( const bunch_core_ &SC )
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
		//f Allocate 'Size' objects. Extent is forced to 'Size' when 'Mode' = 'mFit'.
		void Allocate(
			epeios::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			Allouer_( Size, Mode );
		}
		//f Same as 'Write()', but allocate additionnaly mmr if needed.
		void WriteAndAdjust(
			const bunch_core_ &Set,
			epeios::size__ Amount,
			r Position = 0,
			r Offset = 0 )
		{
			AllocateIfNecessary_( Amount + Offset.V );

			mmr::Write( Set, Amount, Position, Offset );
		}
		//f Same as 'Write()', but allocate additionnaly mmr if needed.
		void WriteAndAdjust(
			const type *Buffer,
			epeios::size__ Amount,
			r Position = 0 )
		{
			AllocateIfNecessary_( Amount + Position.V );

			mmr::Write( Buffer, Amount, Position );
		}
		//f Add the object 'Object'. Return the position where object added.
		r Add( const type &Object )
		{
			epeios::size__ Amount = this->Amount();

			Allouer_( Amount + 1, aem::mDefault );

			mmr::Write( Object , Amount );

			return Amount;
		}
		//f Adding 'Amount' object from 'Buffer'. Return the position of the first object added.
		r Add(
			const type *Buffer,
			epeios::size__ Amount )
		{
			epeios::row_t__ Retour = this->Amount();

			Allouer_( Retour + Amount, aem::mDefault );

			mmr::Write( Buffer, Amount, Retour );

			return Retour;
		}
		//f Adding 'Amount' object from 'Set'. Return the position of the first object added.
		r Add(
			const bunch_core_ &Set,
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
		//f Add 'Set'. Return the position where added.
		r Add( const bunch_core_ &Set )
		{
			return Add( Set, Set.Amount() );
		}
		//f Insert at 'PosDest' 'Amount' objects from 'Source' at 'PosSource'.
		void Insert(
			const bunch_core_ &Source,
			epeios::size__ Amount,
			r PosSource,
			r PosDest )
		{
			Inserer_( Source, Amount, PosSource, PosDest );
		}
		//f Insert 'Set' at 'Position'.
		void Insert(
			const bunch_core_ &Set,
			r Position )
		{
			Inserer_( Set, Set.Amount(), 0, Position.V );
		}
		//f Insert 'Object' at 'Position'
		void Insert(
			const type &Object,
			r Position )
		{
			Inserer_( &Object, 1, Position.V );
		}
		//f Insert at 'Position' 'Amount' objects from 'Source'.
		void Insert(
			const type *Source,
			epeios::size__ Amount,
			r Position )
		{
			Inserer_( Source, Amount, Position.V );
		}
		//f Delete 'Amount' objects at position 'Position'. The size of the set is reduced.
		void Delete(
			r Position,
			epeios::size__ Amount = 1)
		{
			mmr::Write( *this, this->Amount() - ( Amount + Position.V ), Position.V + Amount, Position );

			Allouer_( this->Amount() - Amount, aem::mDefault );
		}
		//f Return the position of the first of 'Amount' new object.
		r New( epeios::size__ Amount = 1 )
		{
			epeios::row_t__ P = this->Amount();

			Allouer_( P + Amount, aem::mDefault );

			return P;
		}
		//f Return position of 'Object' between 'Begin' (included) and 'End' (excluded), or 'NONE' if not found.
		r Position(
			const type &Object,
			r Begin,
			r End ) const
		{
			if ( Amount() )
				return mmr::Position( Object, Begin, End );
		}
		//f Return position of 'Object' beginning at 'Begin' (included), or 'NONE' if not found.
		r Position(
			const type &Object,
			r Begin = 0 ) const
		{
			return Position( Object, Begin, Amount() );
		}
		//f Fill at 'Position' with 'Object' 'Count' time.
		void Fill(
			const type &Object,
			epeios::size__ Count,
			r Position = 0 )
		{
			mmr::Fill( Object, Count, Position );
		}
		//f Fill from 'Position' with 'Object'.
		void Fill(
			const type &Object,
			r Position = 0 )
		{
			Fill( Object, Position, Amount() );
		}
		//f Return reference to memory.
		mmr &Memory( void )
		{
			return *this;
		}
	};


	/*c A set of static object of type 'type'. Use 'E_BUNCH_( type )' rather then directly this class. */
	template <class type, typename r> class bunch_
	: public bunch_core_<type, tym::E_MEMORYt_( type, r ), aem::amount_extent_manager_< r >, r >
	{
	public:
		struct s
		: public bunch_core_<type, tym::E_MEMORYt_( type, r ), aem::amount_extent_manager_<r>, r >::s
		{};
		bunch_( s &S )
		: bunch_core_<type, tym::E_MEMORYt_( type, r ), aem::amount_extent_manager_<r>, r >( S )
		{};
		void reset( bool P = true )
		{
			bunch_core_<type, tym::E_MEMORYt_( type, r ), aem::amount_extent_manager_<r>, r >::reset( P );
			Memory().reset( P );
		}
		bunch_ &operator =( const bunch_ &Op )
		{
			bunch_core_<type, tym::E_MEMORYt_( type, r ), aem::amount_extent_manager_<r>, r >::operator =( Op );

			Allocate( Op.Amount() );

			Memory().Write( Op, Op.Amount() );

			return *this;
		}
		void write( flw::oflow___ &OFlow ) const
		{
			flw::Put( Amount(), OFlow );
			Memory().write( 0, bunch_core_<type, tym::E_MEMORYt_( type, r ), aem::amount_extent_manager_<r>, r >::Amount(), OFlow );
		}
		void read( flw::iflow___ &IFlow )
		{
			size__ Amount;

			flw::Get( IFlow, Amount );
			bunch_core_<type, tym::E_MEMORYt_( type, r ), aem::amount_extent_manager_<r>, r >::Allocate( Amount );
			Memory().read( IFlow, 0, bunch_core_<type, tym::E_MEMORYt_( type, r ), aem::amount_extent_manager_<r>, r >::Amount() );

		}
		//f Adjust the extent to amount.
		void Adjust( void )
		{
			if ( bunch_core_<type, tym::E_MEMORYt_( type, r ), aem::amount_extent_manager_<r>, r >::Force( Amount() ) )
				Memory().Allocate( Amount() );
		}
	};

	AUTO1( bunch )

	//m A set of static object of type 'Type'. Use this rather then 'set_set_<type>'.
	#define E_BUNCHt_( Type, r )		bunch_< Type, r >

	#define E_BUNCHt( Type, r )		bunch< Type, r >

	#define E_BUNCH( Type )		E_BUNCHt( Type, epeios::row__ )
	#define E_BUNCH_( Type )	E_BUNCHt_( Type, epeios::row__ )


	//f Return 'S1' - 'S2' which respectively begins at 'BeginS1' et 'Begins2'.
	template <class t, typename r> inline bso::sbyte__ Compare(
		const E_BUNCHt_( t, r ) &S1,
		const E_BUNCHt_( t, r ) &S2,
		r BeginS1 = 0,
		r BeginS2 = 0 )
	{
		if ( ( S1.Amount() - BeginS1.V ) != ( S2.Amount() - BeginS2.V ) )
			if ( ( S1.Amount() - BeginS1.V ) > ( S2.Amount() - BeginS2.V ) )
				return 1;
			else
				return -1;
		else
			return tym::Compare( S1, S2, BeginS1, BeginS2, S1.Amount() - BeginS1.V );
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
	template <typename type, int size, typename r> class bunch__
	: public bunch_core_< type, tym::E_MEMORYt__( type, size, r ), aem::amount_extent_manager__< size, r >, r >
	{
	public:
		struct s
		: public bunch_core_<type, tym::E_MEMORYt__( type, size, r ), aem::amount_extent_manager__< size, r >, r >::s {} S_;
		bunch__( void ) 
		: bunch_core_<type, tym::E_MEMORYt__( type, size, r ), aem::amount_extent_manager__< size, r >, r >( S_ ) {}
		bunch__ &operator =( const bunch__ &S )
		{
			bunch_core_<type, tym::E_MEMORYt__( type, size, r ), aem::amount_extent_manager__< size, r >, r >::WriteAndAdjust( S, S.Amount_ );
			Size_ = S.Amount_;

			return *this;
		}
		void Init( void )
		{
			bunch_core_<type, tym::E_MEMORYt__( type, size, r ), aem::amount_extent_manager__< size, r >, r >::Init();
			bunch_core_<type, tym::E_MEMORYt__( type, size, r ), aem::amount_extent_manager__< size, r >, r >::SetStepValue( 0 );
		}
	};

	//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
	#define E_BUNCHt__( c, i, r )	bunch__<c, i, r>

	#define E_BUNCH__( c, i )		E_BUNCHt__( c, i , epeios::row__ )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
