/*
  Header for the 'tym' library by Claude L. Simon (simon@epeios.org)
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

#ifndef TYM__INC
#define TYM__INC

#define TYM_NAME		"TYM"

#define	TYM_VERSION	"$Revision$"	

#define TYM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &TYMTutor;

#if defined( XXX_DBG ) && !defined( TYM_NODBG )
#define TYM_DBG 
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

#include "err.h"
#include "flw.h"
#include "uym.h"

typedef uym__position	tym__position;

typedef uym__size		tym__size;

typedef uym__bsize		tym__bsize;

#define TYM_MAX_SIZE	UYM_MAX_SIZE

//c Typed memory core. Don't use; for internal use only.
template <typename t, typename b> class tym_memory_core_
: public b
{
private:
	/* lit 'Quantite' objets à partir de 'Position' et les place dans 'Destination'
	à la position 'Decalage' */
	void Lire_(
		tym__position Position,
		tym__size Quantite,
		tym_memory_core_ &Destination,
		tym__position Decalage ) const
	{
		b::Read( ( Position * sizeof( t ) ), ( Quantite * sizeof( t ) ), Destination, ( Decalage * sizeof( t ) ) );
	}
	// place dans 'Tampon' 'Nomnbre' objets à la position 'Position'
	void Lire_(
		tym__position Position,
		tym__bsize Nombre,
		t *Tampon ) const
	{
		b::Read( Position * sizeof( t ), Nombre * sizeof( t ), (uym__data *)Tampon );
	}
	// écrit 'Taille' objets de 'Tampon' à la position 'Position'
	void Ecrire_(
		const t *Tampon,
		tym__bsize Nombre,
		tym__position Position )
	{
		b::Write( (uym__data *)Tampon, Nombre * sizeof( t ), Position * sizeof( t ) );
	}
	/* écrit 'Nombre' objets de 'Source' à partir de 'Position'
	à la position 'Decalage' */
	void Ecrire_(
		const tym_memory_core_ &Source,
		tym__size Quantite,
		tym__position Position,
		tym__position Decalage )
	{
		b::Write( Source, ( Quantite * sizeof( t ) ),( Position * sizeof( t ) ),  ( Decalage * sizeof( t ) ) );
	}
	// allocation de 'Capacite' objets
	void Allocate_( tym__size Size )
	{
		b::Allocate( ( Size * sizeof( t ) ) );
	}
public:
	struct s
	: public b::s
	{};
	tym_memory_core_( s &S )
	: b( S )
	{}
	//f Initialization.
	void Init( void )
	{
		b::Init();
	}
	//f Put 'Amount' objects at 'Position' to 'Buffer'.
	void Read(
		tym__position Position,
		tym__bsize Amount,
		t *Buffer ) const
	{
		Lire_( Position, Amount, Buffer );
	}
	//f Put in 'Value' the object at 'Position'.
	void Read(
		tym__position Position,
		t &Value ) const
	{
		Lire_( Position, 1, &Value );
	}
	//f Put to 'Destination' at 'Offset' 'Quantity' objects at 'Position'.
	void Read(
		tym__position Position,
		tym__size Quantity,
		tym_memory_core_<t,b> &Destination,
		tym__position Offset = 0 ) const
	{
		Lire_( Position, Quantity, Destination, Offset );
	}
	//f Return the object at 'Position'.
	t Read( tym__position Position ) const
	{
		t V;

		Lire_( Position, 1, &V );

		return V;
	}
	//f Write 'Amount' object from 'Buffer' at 'Position'.
	void Write(
		const t *Buffer,
		tym__bsize Amount,
		tym__position Position )
	{
		Ecrire_( Buffer, Amount, Position );
	}
	//f Write 'Value' at 'Position'.
	void Write(
		const t &Valeur,
		tym__position Position )
	{
		Ecrire_( &Valeur, 1, Position );
	}
	/*f Write 'Quantity' objects at 'Position' from 'Source' at 'Offset'. */
	void Write(
		const tym_memory_core_<t,b> &Source,
		tym__size Quantity,
		tym__position Position = 0,
		tym__position Offset = 0 )
	{
		Ecrire_( Source, Quantity, Position, Offset );
	}
	//f Fill at 'Position' with 'Object' 'Count' times.
	void Fill(
		const t &Object,
		tym__size Count,
		tym__position Position = 0 )
	{
		b::Fill( (uym__data *)&Object, sizeof( t ), Count, Position * sizeof( t ) );
	}
	//f Return the position from 'Object' between 'Begin' and 'End' (excluded) oR 'NONE' if non-existant.
	tym__position Position(
		const t &Object,
		tym__position Begin,
		tym__position End )
	{
		tym__position Position;

		if ( ( Position = b::Position( (uym__data *)&Object, sizeof( t ), Begin * sizeof( t ), End * sizeof( t ) ) ) != NONE )
			Position /= sizeof( t );

		return Position;
	}
	//f Allocate 'Size' objects.
	void Allocate( tym__size Size )
	{
		Allocate_( Size );
	}
};

/*c Memory of statical object of type 't'. */
template <typename t> class tym_memory_
: public tym_memory_core_< t, uym_basic_memory_ >
/* NOTA: See 'tym_memory_core about' '::s'. */
{
public:
	struct s
	: public tym_memory_core_< t, uym_basic_memory_ >::s
	{};
	tym_memory_( tym_memory_core_<t, uym_basic_memory_ >::s &S )
	: tym_memory_core_< t, uym_basic_memory_ >( S ) 
	{}
	void write(
		uym__position Position,
		uym__size Quantity,
		flw::oflow___ &OFlow ) const
	{
		tym_memory_core_<t, uym_basic_memory_ >::write( Position * sizeof( t ), Quantity * sizeof( t ) , OFlow );
	}
	void read(
		flw::iflow___  &IFlow,
		uym__position Position,
		uym__size Quantite )
	{
		tym_memory_core_<t, uym_basic_memory_ >::read( IFlow, Position * sizeof( t ), Quantite * sizeof( t ) );
	}
};

/*
template <class t> class tym_memory
: public tym_memory_<t>
{};
*/

AUTO1( tym_memory )

//m 'tym_memory' would be often used, then create a special name.
#define MEMORY( t )		tym_memory< t >
#define MEMORY_( t )	tym_memory_< t >

//d 'tym__position' would be often used in other libraries, so give him a special name.
#define POSITION__	tym__position

//d 'tym__size' would be often used in other libraries, so give him a special name.
#define SIZE__	tym__size

//d 'tym__bsize' would be often used in other libraries, so give him a special name.
#define BSIZE__	tym__bsize

//f Return 'E1' - 'E2' which begin at 'BeginS1' and 'BeginS2' and have a length of 'Quantity'.
template <class t> inline bso__sbyte TYMCompare(
	const MEMORY_( t ) &S1,
	const MEMORY_( t ) &S2,
	POSITION__ BeginS1,
	POSITION__ BeginS2,
	tym__size Quantity )
{
	return UYMCompare( S1, S2, BeginS1 * sizeof( t ), BeginS2 * sizeof( t ), Quantity * sizeof( t ) );
}

/*c A static set of object of 'amount' objects of type 't' of size 'size'.
The size parameter was added due to a bug of Borland C++, which doesn't like a 'sizeof'
as template parameter. Use 'MEMORY__', it's easier. */
template <class t, int amount, int size> class tym__memory_
: public tym_memory_core_< t, uym__basic_memory_< amount * size > >
{
private:
	tym_memory_core_<t, uym__basic_memory_< amount * size > >::s Static_;
public:
	tym__memory_( tym_memory_core_<t, uym__basic_memory_<  amount * size > >::s &S = *(tym_memory_core_<t, uym__basic_memory_<  amount * size > >::s *) NULL )	// To simplify use in 'SET'.
	: tym_memory_core_<t, uym__basic_memory_< amount * size > >( Static_ )
	{}
};

//d A static set of 'amount' object of type 'Type'.
#define MEMORY__( type, amount ) tym__memory_< type, amount, sizeof( type ) > 

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
