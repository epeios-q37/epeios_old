/*
  Header for the 'set' library by Claude L. Simon (epeios@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org) 

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
  along with this program; if not, go to http://www.fsf.org or write to the
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef SET__INC
#define SET__INC

#define SET_NAME		"SET"

#define	SET_VERSION	"$Revision$"	

#define SET_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &SETTutor;

#if defined( XXX_DBG ) && !defined( SET_NODBG )
#define SET_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mmm.h"
#include "tym.h"

//c Ste core. Don't use ; for internal use only.
template <class type, class mem> class set_set_core_
: public mem
{
private:
	/* Pousse (décalge vers la fin de l'ensemble) les objets à la position
	'Position' de 'Quantite' positions */
	void Pousser_(
		POSITION__ Position,
		SIZE__ Quantite )
	{
		Allouer_( S_.Amount + Quantite );
		mem::Write( *this, S_.Amount - Position - Quantite, Position, Position + Quantite);
	}
	// Insere à 'PosDest' 'Quantite' objets situé à partir de 'PosSource' de 'Source'.
	void Inserer_(
		const set_set_core_ &Source,
		SIZE__ Quantite,
		POSITION__ PosSource,
		POSITION__ PosDest )
	{
		Pousser_( PosDest, Quantite );
		mem::Write( Source, Quantite, PosSource, PosDest );
	}
	// Insere 'Quantite' objets de 'Objets' à 'Position'.
	void Inserer_(
		const type *Objets,
		SIZE__ Quantite,
		POSITION__ Position )
	{
		Pousser_( Position, Quantite );
		mem::Write( Objets, Quantite, Position );
	}
	// Allocation de la place nécessaire à 'Taille' objets.
	void Allouer_( SIZE__ Taille )
	{
		mem::Allocate( Taille );
		S_.Amount = Taille;
	}
	// allocate if the set not big enough.
	void AllocateIfNecessary_( SIZE__ Quantity )
	{
		if ( Quantity > S_.Amount )
			Allouer_( Quantity );
	}
public:
	struct s
	: public mem::s
	{
		SIZE__ Amount;
	} &S_;
	set_set_core_( s &S )
	: S_( S ),
	  mem( S )
	{}
	void reset( bso__bool P )
	{
		S_.Amount = 0;
	}
	//f Initialization.
	void Init( void )
	{
		S_.Amount = 0;

		mem::Init();
	}
	//f Allocate 'Size' objects.
	void Allocate( SIZE__ Size )
	{
		Allouer_( Size );
	}
	//f Return the extent (number of objects) of the set.
	SIZE__ Extent( void ) const
	{
		return S_.Amount;
	}
	//f Same as 'Write()', but allocate additionnaly memory if needed.
	void WriteAndAdjust(
		const set_set_core_ &Set,
		SIZE__ Amount,
		POSITION__ Position = 0,
		POSITION__ Offset = 0 )
	{
		AllocateIfNecessary_( Amount + Offset );

		mem::Write( Set, Amount, Position, Offset );
	}
	//f Same as 'Write()', but allocate additionnaly memory if needed.
	void WriteAndAdjust(
		const type *Buffer,
		BSIZE__ Amount,
		POSITION__ Position = 0 )
	{
		AllocateIfNecessary_( Amount + Position );

		mem::Write( Buffer, Amount, Position );
	}
	//f Add the object 'Object'. Return the position where object added.
	POSITION__ Add( const type &Object )
	{
		Allouer_( S_.Amount + 1 );

		mem::Write( Object , S_.Amount - 1 );

		return S_.Amount - 1;
	}
	//f Adding 'Amount' object from 'Buffer'. Return the position of the first object added.
	POSITION__ Add(
		const type *Buffer,
		BSIZE__ Amount )
	{
		POSITION__ Retour = S_.Amount;

		Allouer_( Retour + Amount );

		mem::Write( Buffer, Amount, Retour );

		return Retour;
	}
	//f Adding 'Amount' object from 'Set'. Return the position of the first object added.
	POSITION__ Add(
		const set_set_core_ &Set,
		SIZE__ Amount )
	{
		POSITION__ Retour = S_.Amount;

		Allocate( Retour + Amount );

		mem::Write( Set, Amount, 0, Retour );

		return Retour;
	}
	//f Remove 'Amount' objects from the end of the set.
	void Truncate( SIZE__ Amount = 1 )
	{
#ifdef SET_DBG
		if ( Amount > S_.Amount )
			ERRu();
#endif
		Allocate( S_.Amount - Amount );
	}
	//f Add 'Set'. Return the position where added.
	POSITION__ Add( const set_set_core_ &Set )
	{
		return Add( Set, Set.Amount() );
	}
	//f Insert at 'PosDest' 'Amount' objects from 'Source' at 'PosSource'.
	void Insert(
		const set_set_core_ &Source,
		SIZE__ Amount,
		POSITION__ PosSource,
		POSITION__ PosDest )
	{
		Inserer_( Source, Amount, PosSource, PosDest );
	}
	//f Insert 'Set' at 'Position'.
	void Insert(
		const set_set_core_ &Set,
		POSITION__ Position )
	{
		Inserer_( Set, Set.S_.Amount, 0, Position );
	}
	//f Insert 'Object' at 'Position'
	void Insert(
		const type &Object,
		POSITION__ Position )
	{
		Inserer_( &Object, 1, Position );
	}
	//f Insert at 'Position' 'Amount' objects from 'Source'.
	void Insert(
		const type *Source,
		BSIZE__ Amount,
		POSITION__ Position )
	{
		Inserer_( Source, Amount, Position );
	}
	//f Delete 'Amount' objects at position 'Position'. The size of the set is reduced.
	void Delete(
		POSITION__ Position,
		SIZE__ Amount = 1)
	{
		mem::Write( *this, S_.Amount - ( Amount + Position ), Position + Amount, Position );

		Allouer_( S_.Amount - Amount );
	}
	//f Return the object at 'Position'..
	type operator ()( POSITION__ Position ) const
	{
		return Read( Position );
	}
	//f Put 'Object' at 'Position'.
	void operator ()(
		POSITION__ Position,
		type &Object ) const
	{
		Read( Position, Object );
	}
	//f Return position of the last object of the set.
	POSITION__ Last( void ) const
	{
		if ( S_.Amount )
			return S_.Amount - 1;
		else
			return NONE;
	}
	//f Return position of the first object of the set.
	POSITION__ First( void ) const
	{
		if ( S_.Amount )
			return 0;
		else
			return NONE;
	}
	//f Return the position of the object after 'Current' (to the top).
	POSITION__ Next( POSITION__ Current ) const
	{
		if ( ++Current < S_.Amount )
			return Current;
		else
			return NONE;
	}
	//f Return the position of the object before 'Current' (to the bottom).
	POSITION__ Previous( POSITION__ Current ) const
	{
		if ( Current )
			return Current - 1 ;
		else
			return NONE;
	}
	//f Return the amount of objects stored in the set.
	SIZE__ Amount( void ) const
	{
		return S_.Amount;
	}
	//f Return the position of the first of 'Amount' new object.
	POSITION__ New( SIZE__ Amount = 1 )
	{
		POSITION__ P = S_.Amount;

		Allocate( P + Amount );

		return P;
	}
	//f Return position of 'Object' between 'Begin' (included) and 'End' (excluded), or 'NONE' if not found.
	POSITION__ Position(
		const type &Object,
		POSITION__ Begin,
		POSITION__ End ) const
	{
		if ( Size_ )
			return mem::Position( Object, Begin, End );
	}
	//f Return position of 'Object' beginning at 'Begin' (included), or 'NONE' if not found.
	POSITION__ Position(
		const type &Object,
		POSITION__ Begin = 0 ) const
	{
		return Position( Object, Begin, S_.Amount );
	}
	//f Fill at 'Position' with 'Object' 'Count' time.
	void Fill(
		const type &Object,
		SIZE__ Count,
		POSITION__ Position = 0 )
	{
		mem::Fill( Object, Count, Position );
	}
	//f Fill from 'Position' with 'Object'.
	void Fill(
		const type &Object,
		POSITION__ Position = 0 )
	{
		Fill( Object, Position, S_.Amount );
	}
};


/*c A set of static object of type 'type'. Use 'SET_( type )' rather then directly this class. */
template <class type> class set_set_
: public set_set_core_<type, MEMORY_( type )>
{
private:
	// l'éventuel pilote de la multimemoire
	mmm_multimemory_driver_ PiloteMultimemoire_;
	/* Pousse (décalge vers la fin de l'ensemble) les objets à la position
	'Position' de 'Quantite' positions */
public:
	struct s
	: public set_set_core_<type, MEMORY_( type )>::s
	{
		mmm_multimemory_driver_::s PiloteMultimemoire_;
	};
	set_set_( s &S )
	: set_set_core_<type, MEMORY_( type )>( S ),
	  PiloteMultimemoire_( S.PiloteMultimemoire_ )
	{};
	void reset( bool P = true )
	{
		set_set_core_<type, MEMORY_( type )>::reset( P );
		PiloteMultimemoire_.reset( P );
		MEMORY_( type )::reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		PiloteMultimemoire_.Init( M );
		MEMORY_( type )::plug( PiloteMultimemoire_ );
	}
	void plug( MEMORY_DRIVER_ &Pilote )
	{
		PiloteMultimemoire_.reset();
		MEMORY_( type )::plug( Pilote );
	}
	set_set_ &operator =( const set_set_ &Op )
	{
		Allocate( Op.Amount() );

		MEMORY_( type )::Write( Op, Op.Amount() );

		return *this;
	}
	void write( flw::oflow___ &OFlow ) const
	{
		flw::Put( Amount(), OFlow );
		MEMORY_( type )::write( 0, set_set_core_<type, MEMORY_( type )>::Amount(), OFlow );
	}
	void read( flw::iflow___ &IFlow )
	{
		flw::Get( IFlow, set_set_core_<type, MEMORY_( type )>::S_.Amount );
		MEMORY_( type )::read( IFlow, 0, set_set_core_<type, MEMORY_( type )>::Amount() );
	}
};

AUTO1( set_set )


//m A set of static object of type 'Type'. Use this rather then 'set_set_<type>'.
#define SET_( Type )	set_set_< Type >

#define SET( Type )		set_set< Type >


//f Return 'S1' - 'S2' which respectively begins at 'BeginS1' et 'Begins2'.
template <class t> inline bso__sbyte SETCompare(
	const SET_( t ) &S1,
	const SET_( t ) &S2,
	POSITION__ BeginS1 = 0,
	POSITION__ BeginS2 = 0 )
{
	if ( ( S1.Amount() - BeginS1 ) != ( S2.Amount() - BeginS2 ) )
		if ( ( S1.Amount() - BeginS1 ) > ( S2.Amount() - BeginS2 ) )
			return 1;
		else
			return -1;
	else
		return TYMCompare( S1, S2, BeginS1, BeginS2, S1.Amount() - BeginS1 );
}

//f Return 'S1' - 'S2' which respectively begin at 'BeginS1' et 'Begins2' and have a length of 'Amount'.
template <class t> inline bso__sbyte SETCompare(
	const SET_( t ) &S1,
	const SET_( t ) &S2,
	POSITION__ BeginS1,
	POSITION__ BeginS2,
	SIZE__ Amount )
{
	return TYMCompare( S1, S2, BeginS1, BeginS2, Amount );
}


template <class t> inline bso__bool operator ==(
	const SET_( t ) &S1,
	const SET_( t ) &S2 )
{
	return !SETCompare( S1, S2 );
}

template <class t> inline bso__bool operator !=(
	const SET_( t ) &S1,
	const SET_( t ) &S2 )
{
	return SETCompare( S1, S2 ) != 0;
}

//c A set of maximum 'size' static objects of type 'type'. Use 'SET__( type, size )' rather then directly this class.
template <typename type, int size> class set__set
: public set_set_core_<type, MEMORY__( type, size )>
{
	struct s
		: set_set_core_<type, MEMORY__( type, size )>::s {} S_;
public:
	set__set( void ) 
	: set_set_core_<type, MEMORY__( type, size )>( S_ ) {}
	set__set &operator =( const set__set &S )
	{
		set_set_core_<type, MEMORY__( type, size )>::WriteAndAdjust( S, S.Amount_ );
		Size_ = S.Amount_;

		return *this;
	}
};

//m A set of maximum 'i' statical objects of type 'c'. Use this rather then 'set__set<c,i>'.
#define SET__( c, i ) set__set<c,i>

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
