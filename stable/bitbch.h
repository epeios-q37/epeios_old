/*
	Header for the 'bitbch' library by Claude SIMON (csimon@epeios.org)
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

#ifndef BITBCH__INC
#define BITBCH__INC

#define BITBCH_NAME		"BITBCH"

#define	BITBCH_VERSION	"$Revision$"

#define BITBCH_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &BITBCHTutor;

#if defined( XXX_DBG ) && !defined( BITBCH_NODBG )
#define BITBCH_DBG
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
//D BInary digiT BunCH 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "tym.h"
#include "aem.h"

/* ATTENTION: si la taille du réceptacle n'est plus de 1 octet, modifier la valeur
des #define ci-dessous. */
#define BITBCH__RECEPTACLE_SIZE_IN_BYTES	sizeof( bitbch::receptacle__ )

#define BITBCH__RECEPTACLE_SIZE_IN_BITS		( 8 * BITBCH__RECEPTACLE_SIZE_IN_BYTES )
//#define BITBCH__RECEPTACLE_SIZE_IN_BYTES	( 8 * sizeof( receptacle__ ) )
#define BITBCH__RECEPTACLE_VALUE_MAX	((bitbch::receptacle__)-1L)


namespace bitbch {
	using namespace epeios;
	using aem::amount_extent_manager_;

	// Type du receptacle de bits.
	typedef bso::ubyte__		receptacle__;

	// Classes regroupant des fonctions agissant sur un objet de type 't'. Usage interne.
	template <class t, typename r> class functions__
	{
	private:
		static receptacle__ Offset_( r Position )
		{
			return (receptacle__)( *Position % BITBCH__RECEPTACLE_SIZE_IN_BYTES );
		}
		// retourne l'offset correpondant à 'Position'
		static r Indice_( r Position )
		{
			return *Position / BITBCH__RECEPTACLE_SIZE_IN_BYTES;
		}
		// retourne l'indice correspondant à 'Position'
		static bso::ubyte__ Masque_( r Position )
		{
			return (bso::ubyte__)( 1 << Offset_( Position ) );
		}
		// retourne le masque correspondant à 'Position'
	public:
		static bso::bool__ Lire(
			r Position,
			const t &Table )
		{
			return ( Table.Get( Indice_( Position ) ) & Masque_( Position ) ) != 0;
		}
		// retourne la valeur du bit à la position 'Position' (>=0)
		static void Ecrire(
			bso::bool__ Valeur,
			r Position,
			t &Table )
		{
			Table.Store( (receptacle__)( ( Table.Get( Indice_( Position ) ) & ~Masque_( Position ) ) | ( ( Valeur ? 1 << Offset_( Position ) : 0 ) ) ), Indice_( Position ) );
		}
		// place un bit de valeur 'Valeur' à la position 'Position'
	};

	// N.B.: le contenu du tableau est inversé bit à bit
	//c Bits set.
	template <typename r> class bit_bunch_
	: public aem::amount_extent_manager_<r>
	{
	private:
		tym::size__ Convert_( tym::size__ Amount )
		{
			return Amount ? ( Amount - 1 ) / BITBCH__RECEPTACLE_SIZE_IN_BYTES + 1 : 0;
		}
		bso::bool__ Lire_( r Position ) const
		{
			return functions__<tym::E_MEMORYt_( receptacle__, r ), r>::Lire( Position, Table );
		}
		// retourne la valeur du bit à la position 'Position' (>=0)
		void Ecrire_(
			bso::bool__ Valeur,
			r Position )
		{
			functions__<tym::E_MEMORYt_( receptacle__, r ), r>::Ecrire( Valeur, Position, Table );

	//		Table.Ecrire( Indice, (receptacle__)( ( Table.Objet(Indice) & (receptacle__)~( (receptacle__)1 << Offset ) ) | ( !Valeur << Offset ) ) );
		}
		// place un bit de valeur 'Valeur' à la position 'Position'
		void Allouer_(
			tym::size__ Nombre,
			aem::mode Mode = aem::mDefault )
		{
			if ( amount_extent_manager_<r>::AmountToAllocate( Nombre, Mode ) )
				Table.Allocate( Convert_( Nombre ) );
		}
		// alloue 'Nombre' (>=1) bits
	public:
		// Tableau contenant les bits.
		tym::E_MEMORYt_(receptacle__, r) Table;
		struct s
		: public aem::amount_extent_manager_<r>::s
		{
			typename tym::E_MEMORYt_(receptacle__, r )::s Table;
		};
		bit_bunch_( s &S )
		: amount_extent_manager_<r>( S ),
		  Table( S.Table ){}
		void reset( bool P = true )
		{
			Table.reset( P );
			amount_extent_manager_<r>::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Table.plug( M );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MDriver )
		{
			Table.plug( MDriver );
		}
		bit_bunch_ &operator =( const bit_bunch_ &O )
		{
			amount_extent_manager_<r>::operator =( O );

			Allocate( O.Amount() );
			Table.Store( O.Table, Convert_( O.Amount() ) );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			amount_extent_manager_<r>::Init();
			Table.Init();
		}
	//	void Dup( bit_bunch &O );
		//f Return the value at position 'Position'.
		bso::bool__ Get( r Position ) const
		{
			return (bso::bool__)Lire_( Position );
		}
		//f Return the value at position 'Position'.
		bso::bool__ operator()( r Position ) const
		{
			return Get( Position );
		}
		//f Store 'Value' at 'Position'.
		void Store(
			bso::bool__ Value,
			r Position )
		{
			Ecrire_( Value, Position );
		}
		//f Allocate enough room to contain 'Size' bits.
		void Allocate(
			tym::size__ Size,
			aem::mode Mode = aem::mDefault )
		{
			Allouer_( Size, Mode );
		}
		//f Append 'Value' to the end of the set.
		r Append( bso::bool__ Value )
		{
			Allouer_( Amount() + 1, aem::mDefault );

			Store( Value, Amount() - 1 );
			return Amount() - 1;
		}
		//f Return the position of the first of 'Size' new bits.
		r New( tym::size__ Size = 1 )
		{
			row_t__ P = Amount();

			Allocate( P + Size );

			return P;
		}
		//f Set all the content to valu.
		void Reset( bso::bool__ Value = false )
		{
			receptacle__ Pattern = 0;

			if ( Value )
				Pattern = -1;

			Table.Store( Pattern, 0, Convert_( Amount() ) );
		}
	};

	E_AUTO1( bit_bunch )

	#define E_BIT_BUNCHt_( row )	bit_bunch_< row >
	#define E_BIT_BUNCHt( row )		bit_bunch< row >

	#define E_BIT_BUNCH_	E_BIT_BUNCHt_( epeios::row__ )
	#define E_BIT_BUNCH		E_BIT_BUNCHt( epeios::row__ )

	void And(
		const E_BIT_BUNCH_ &O1,
		const E_BIT_BUNCH_ &O2,
		E_BIT_BUNCH_ &R );

	void Or(
		const E_BIT_BUNCH_ &O1,
		const E_BIT_BUNCH_ &O2,
		E_BIT_BUNCH_ &R );

	void XOr(
		const E_BIT_BUNCH_ &O1,
		const E_BIT_BUNCH_ &O2,
		E_BIT_BUNCH_ &R );

	void Not(
		const E_BIT_BUNCH_ &O,
		E_BIT_BUNCH_ &R );

	template <typename r__> inline void And(
		const E_BIT_BUNCHt_(r__) &O1,
		const E_BIT_BUNCHt_(r__) &O2,
		E_BIT_BUNCHt_(r__) &R )
	{
		And( *(const E_BIT_BUNCH_ *)&O1, *(const E_BIT_BUNCH_ *)&O2, *(E_BIT_BUNCH_ *)&R );
	}

	template <typename r__> inline void Or(
		const E_BIT_BUNCHt_(r__) &O1,
		const E_BIT_BUNCHt_(r__) &O2,
		E_BIT_BUNCHt_(r__) &R )
	{
		Or( *(const E_BIT_BUNCH_ *)&O1, *(const E_BIT_BUNCH_ *)&O2, *(E_BIT_BUNCH_ *)&R );
	}

	template <typename r__> inline void XOr(
		const E_BIT_BUNCHt_(r__) &O1,
		const E_BIT_BUNCHt_(r__) &O2,
		E_BIT_BUNCHt_(r__) &R )
	{
		XOr( *(const E_BIT_BUNCH_ *)&O1, *(const E_BIT_BUNCH_ *)&O2, *(E_BIT_BUNCH_ *)&R );
	}

	template <typename r__> inline void Not(
		const E_BIT_BUNCHt_(r__) &O,
		E_BIT_BUNCHt_(r__) &R )
	{
		Not( *(const E_BIT_BUNCH_ *)&O, *(E_BIT_BUNCH_ *)&R );
	}


#if 0	// Code in .cpp as deseapered ...
	bso::bool__ ComparerBitABit_(
		bit_bunch<r> &E1,
		bit_bunch<r> &E2,
		tym::size__ Nombre,
		tym::row__ P1,
		tym::row__ P2 );

	//f Compare 'Quantity' bits from 'S1' and 'S2' beginning at 'P1' and 'P2'.
	inline bso::bool__ Compare(
		bit_bunch &S1,
		bit_bunch &S2,
		tym::size__ Quantity = 0,
		tym::row__ P1 = 0,
		tym::row__ P2 = 0 )
	{
		if ( !Quantity )
			if ( ( S1.Amount() - P1 ) < ( S2.Amount() - P2 ) )
				Quantity = S1.Amount() - P1;
			else
				Quantity = S2.Amount() - P2;

		if ( ( P1 % 8 ) == ( P2 % 8 ) )
		{
			tym::row__ P1C, P2C;
			tym::size__ NC;
			bso::bool__ Resultat = true;


			P1C = ( P1 ? ( P1 - 1 ) / 8 + 1 : 0 );
			P2C = ( P2 ? ( P2 - 1 ) / 8 + 1 : 0 );

			NC = ( Quantity - ( P1C * 8 - P1 ) ) / 8;

			if ( ( P1 * 8 ) != P1C )
				Resultat = ComparerBitABit_( S1, S2,  P1C * 8 - P1, P1, P2 );

			if ( Resultat )
			{
				if ( ( NC * 8 - ( P1C * 8 - P1 ) ) != Quantity )
					Resultat = ComparerBitABit_( S1, S2,
												 Quantity - NC * 8 - ( P1C * 8 - P1 ),
											   ( P1C + NC ) * 8,
											   ( P2C + NC ) * 8 );

				if ( Resultat && NC )
					Resultat = !tym::Compare( S1.Table, S2.Table, P1C, P2C, NC );
			}

			return Resultat;
		}
		else
			return ComparerBitABit_( S1, S2, Quantity, P1, P2 );
	}

	inline bso::bool__ operator ==(
		bit_bunch &S1,
		bit_bunch &S2 )
	{
		if ( S1.Amount() != S2.Amount() )
			return 0;
		else
			return Compare( S1, S2 );
	}

	txf::text_oflow___ &operator <<(
		txf::text_oflow___ &OStream,
		bit_bunch &Set );

	txf::text_iflow___ &operator >>(
		txf::text_iflow___ &IStream,
		bit_bunch &Set );
#endif

	//c A pairs of bits set.
	template <typename r> class bibit_bunch_
	{
	public:
		//o Les tableau des bits.
		bit_bunch_<r> T1, T2;
		struct s
		{
			typename bit_bunch_<r>::s T1, T2;
		};
		bibit_bunch_( s &S )
		: T1( S.T1 ),
		  T2( S.T2 ){}
		void reset( bool P = true )
		{
			T1.reset( P );
			T2.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			T1.plug( M );
			T2.plug( M );
		}
		// Operateur d'affectation.
		bibit_bunch_ &operator =( const bibit_bunch_ &O)
		{
			T1 = O.T1;
			T2 = O.T2;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			T1.Init();
			T2.Init();
		}
		//f Store 'Value' tou 'Position'.
		void Store(
			receptacle__ Value,
			r Position )
		{
			T1.Store(   Value & 1       , Position );
			T2.Store( ( Value & 2 ) != 0, Position );	// to avoid warning on some compilers.
		}
		//f Return the value at 'Position'.
		receptacle__ Get( r Position ) const
		{
			return (int)T1.Get( Position ) | (int)( T2.Get( Position ) << 1 );
		}
		//f Return the value at position 'Position'.
		receptacle__ operator()( r Position ) const
		{
			return Get( Position );
		}
		//f Allocate enough room to contain 'Size' pairs of bits.
		void Allocate( tym::size__ Size )
		{
			T1.Allocate( Size );
			T2.Allocate( Size );
		}
		//f Append 'Value'. Return position where put.
		r Append( receptacle__ Value )
		{
			T1.Append(   Value & 1 );
			return T2.Append( ( Value & 2 ) != 0 );	// to avoid warning on some compilers.
		}
	};


	E_AUTO1( bibit_bunch )

#if 0
	/* Stocke dans 'D' 'O1' & 'O2', tous étant de taille 'Taille'.
	'D' peut être 'O1' ou 'O2'. Usage interne. */
	void Et_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		tym::bsize__ Taille );

	/* Stocke dans 'D' 'O1' | 'O2', tous étant de taille 'Taille'.
	'D' peut être 'O1' ou 'O2'. Usage interne. */
	void Ou_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		tym::bsize__ Taille );

	/* Stocke dans 'D' ~'O' tou deux de taille 'Taille'. 'O' peut être 'D'.
	Usage interne. */
	void Non_(
		const receptacle__ *O,              
		receptacle__ *D,
		tym::bsize__ Taille );

	/* Stocke dans 'D' la comparaison bit à bit de 'O1' et 'O2', tous 2 de taille 'Taille'.
	Usage interne. */
	void Dff_(
		const receptacle__ *O1,
		const receptacle__ *O2,
		receptacle__ *D,
		tym::bsize__ Taille );

	// Retourne le nombre de bits à un dans 'O4.
	tym::bsize__ Compter_(
		const receptacle__ *O,
		tym::bsize__ Taille );

	tym::row__ Suivant_(
		const receptacle__ *O,
		tym::bsize__ Taille,
		bso::bool__ Valeur,
		tym::row__ Courant );

	tym::row__ Precedent_(
		const receptacle__ *O,
		bso::bool__ Valeur,
		tym::row__ Courant );

#endif

	template <int t> class receptacles__
	{
	private:
		receptacle__ Table_[t];
	public:
		receptacle__ Get( row_t__ Position ) const
		{
			return Table_[Position];
		}
		void Store(
			receptacle__ Value,
			row_t__ Position )
		{
			Table_[Position] = Value;
		}
		void RAZ( void )
		{
			memset( Table_, 0, t * sizeof( receptacle__ ) );
		}
		void MAU( void )
		{
			memset( Table_, BITBCH_VALEUR_MAX_RECEPTACLE, t * sizeof( receptacle__ ) );
		}
		tym::bsize__ Taille( void ) const
		{
			return t;
		}
		row_t__ Suivant(
			bso::bool__ Value,
			row_t__ Courant ) const
		{
			return Suivant_( Table_, t, Value, Courant );
		}
		row_t__ Precedent(
			bso::bool__ Value,
			row_t__ Courant ) const
		{
			return Precedent_( Table_, Value, Courant );
		}
	};
#if 0
	/* Place dans 'D', 'O1' & 'O2'. 'D' peut être le même objet que 'O1' ou 'O2'.
	Usage interne. */
	template <class t> inline void Et_(
		const t &O1,
		const t &O2,
		t &D )
	{
		Et_( O1.Table_, O2.Table_, D.Table_, D.Taille() );
	}

	/* Place dans 'D', 'O1' | 'O2'. 'D' peut être le même objet que 'O1' ou 'O2'.
	Usage interne. */
	template <class t> inline void Ou_(
		const t &O1,
		const t &O2,
		t &D )
	{
		Ou_( O1.Table_, O2.Table_, D.Table_, D.Taille() );
	}

	// Place dans 'D', ~'O'. 'D' peut être le même objet que 'O'. Usage interne.
	template <class t> inline void Non_(
		const t &O,
		t &D )
	{
		Non_( O.Table_, D.Table_, D.Taille() );
	}

	/* Place dans 'D', 'O1' | 'O2'. 'D' peut être le même objet que 'O1' ou 'O2'.
	Usage interne. */
	template <class t> inline void Dff_(
		const t &O1,
		const t &O2,
		t &D )
	{
		Dff_( O1.Table_, O2.Table_, D.Table_, D.Taille() );
	}

	// Retourne le nombre de bits à un dans 'O'.
	template <class t> inline tym::bsize__ Compter_( const t &O )
	{
		return Compter_( O.Table_, O.Taille() );
	}
#endif
	//c A set of a maximum of 't' bits.
	template <int t, typename r> class bit_bunch__
	{
	private:
		receptacles__<((t - 1)/BITBCH__RECEPTACLE_SIZE_IN_BITS)+1> Table_;
	public:
		bit_bunch__( void )
		{
			Reset();
		}
		//f Return the value at position 'Position'.
		bso::bool__ Get( r Position ) const
		{
			return (int)functions__< receptacles__<((t - 1)/BITBCH__RECEPTACLE_SIZE_IN_BYTES)+1>, r >::Lire( Position, Table_ );
		}
			//f Return the value at position 'Position'.
		bso::bool__ operator()( r Position )
		{
			return Get( Position );
		}
		//f Store 'Value' to 'Position'.
		void Store(
			bso::bool__ Value,
			r Position )
		{
			functions__< receptacles__<((t - 1)/BITBCH__RECEPTACLE_SIZE_IN_BYTES)+1>, r >::Ecrire( Value, Position, Table_ );
		}
		//f Set all the bits to 'Value'.
		void Reset( bso::bool__ Value = false )
		{
			if ( Value )
				Table_.MAU();
			else
				Table_.RAZ();
		}
		//f Return the position of the next bit after 'Current' at 'Value'.
		r Next(
			r Current,
			bso::bool__ Value ) const
		{
			Current = Table_.Suivant( Value, Current );

			if ( Current >= t )
				Current = NONE;

			return Current;
		}
		//f Return the position of the previous bit after 'Current' at 'Value'.
		r Previous(
			r Courant,
			bso::bool__ Value ) const
		{
			return Table_.Precedent( Value, Courant );
		}
		//f Return the first bit at 'Value'.
		r First( bso::bool__ Value ) const
		{
			if ( Get( 0 ) == Value )
				return 0;
			else
				return Next( 0, Value );
		}
		//f Return the last bit at 'Value'.
		r Last( bso::bool__ Value ) const
		{
			if ( Lire( t - 1 ) == Value )
				return t - 1;
			else
				return Prev( t - 1, Value );
		}
		//f Return the size.
		tym::size__ Size( void ) const
		{
			return t;
		}
		//f Return the position of the top of the set.
		r Top( void ) const
		{
			return t - 1;
		}
		//f Return the position of the bottom of the set.
		r Bottom( void )
		{
			return 0;
		}
		//f Return the position of the bit next to 'Current'.
		r Up( r Current ) const
		{
			if ( ++Current < t )
				return Current;
			else
				return NONE;
		}
		//f Return the position of the bit previous to 'Current'.
		r Down( r Current ) const
		{
			if ( Current )
				return Current - 1 ;
			else
				return NONE;
		}
	};
#if 0
	//f 'D' = 'O1' AND 'O2'.
	template <class t> inline void And(
		const t &O1,
		const t &O2,
		t &D )
	{
		Et_( O1.Table_, O2.Table_, D.Table_ );
	}

	//f 'D' = 'O1' OR 'O2'.
	template <class t> inline void Or(
		const t &O1,
		const t &O2,
		t &D )
	{
		Ou_( O1.Table_, O2.Table_, D.Table_ );
	}

	//f 'D' = NOT 'O'.
	template <class t> inline void Not(
		const t &O,
		t &D )
	{
		Non_( O.Table_, D.Table_ );
	}

	//f 'D' = 'O1' XOR 'O2'.
	template <class t> inline void XOr(
		const t &O1,
		const t &O2,
		t &D )
	{
		Dff_( O1.Table_, O2.Table_, D.Table_ );
	}

	//f Return the count of bit at true in 'O'.
	template <class t> inline tym::bsize__ Count( const t &O )
	{
		return BSTCompter_( O.Table );
	}
#endif
}

txf::text_oflow__ &operator <<(
	txf::text_oflow__ &OStream,
	const bitbch::E_BIT_BUNCH_ &Bunch );

template <typename r__> txf::text_oflow__ &operator <<(
	txf::text_oflow__ &OStream,
	const bitbch::E_BIT_BUNCHt_(r__) &Bunch )
{
	return OStream << *(const bitbch::E_BIT_BUNCH_ *)&Bunch;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
