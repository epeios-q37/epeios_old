/*
  Header for the 'bitmmr' library by Claude L. Simon (simon@epeios.org)
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

#ifndef BITMMR__INC
#define BITMMR__INC

#define BITMMR_NAME		"BITMMR"

#define	BITMMR_VERSION	"$Revision$"	

#define BITMMR_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &BITMMRTutor;

#if defined( XXX_DBG ) && !defined( BITMMR_NODBG )
#define BITMMR_DBG 
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
//D BIT MeMoRy.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "tym.h"

namespace bitmmr {

	// Type du receptacle de bits.
	typedef bso__ubyte		receptacle__;

	// Classes regroupant des fonctions agissant sur un objet de type 't'. Usage interne.
	template <class mem> class bit_memory_core_
	: public mem
	{
	private:
		receptacle__ Offset_( POSITION__ Position )
		{
			return (int)( Position % BST_NB_BITS_RECEPTACLE );
		}
		// retourne l'offset correpondant à 'Position'
		POSITION__ Indice_( POSITION__ Position )
		{
			return Position / BST_NB_BITS_RECEPTACLE;
		}
		// retourne l'indice correspondant à 'Position'
		bso__ubyte Masque_( POSITION__ Position )
		{
			return 1 << Offset_( Position );
		}
		// retourne le masque correspondant à 'Position'
	public:
		struct s
		: public mem::s
		{};
		bit_memory_core_( s &S )
		: mem( S )
		{}
		//f Initialization.
		void Init( void )
		{
			mem::Init();
		}
		//f Retunr value of bit at position 'Position'.
		bso__bool Read( POSITION__ Position )
		{
			return ( mem::Read( Indice_( Position ) ) & Masque_( Position ) ) != 0;
		}
		//f Write 'Value' for bit at position 'Position'.
		void Write(
			bso__bool Value,
			POSITION__ Position )
		{
			Table.Write( ( Table.Read(Indice_( Position )) & ~Masque_( Position ) ) | ( ( Valeur ? 1 << Offset_( Position ) : 0 ) ), Indice_( Position ) );
		}
		//f Allocate 'Size' objects.
		void Allocate( SIZE__ Size )
		{
			mem::Allocate( ( Size ) / BST_NB_BITS_RECEPTACLE + 1 );

		}
		//f Return the object at 'Position'..
		bso__bool operator ()( POSITION__ Position ) const
		{
			return Read( Position );
		}
	};

	//c A memory containg bits.
	typedef bit_memory_core_< tym_memory_< receptacle__ > >	bit_memory_;
	typedef bit_memory_core_< tym_memory<receptacle__ > >	bit_memory;

	template <int t> typedef bit_memory_core_< MEMORY__( receptacle__, ( t - 1 ) / 8 + 1 ) > bit_memory__;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
