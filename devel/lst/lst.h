/*
  Header for the 'lst' library by Claude L. Simon (simon@epeios.org)
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

#ifndef LST__INC
#define LST__INC

#define LST_NAME		"LST"

#define	LST_VERSION	"$Revision$"	

#define LST_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &LSTTutor;

#if defined( XXX_DBG ) && !defined( LST_NODBG )
#define LST_DBG 
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
#include "bitbch.h"
#include "stk.h"
#include "aem.h"

namespace lst {

	//c Handle a list of objects. Use 'LIST_' rather than directly this class.
	class list_
	: public aem::amount_extent_manager_
	{
	protected:
		/*v Cette fonction est appelée lors d'allocations dans la liste;
		permet de synchroniser la taille de la liste avec d'autres ensembles;
		'Size' est la capacité allouée. Ne fait rien par défaut. */
		virtual void LSTAllocate( tym::size__ Size ) {}
	private:
		tym::row__ Nouveau_( void )
		{
			tym::row__ Numero;

			if ( Libres.Amount() )
				Numero = Libres.Pop();
			else
			{
				tym::size__ Size = ( Numero = Amount() ) + 1;

				if ( amount_extent_manager_::AmountToAllocate( Size ) )
					LSTAllocate( Size );
			}

			return Numero;
		}
		// Retourne l'élément succédant à 'Element', ou LST_INEXISTANT si inexistant.
		tym::row__ Successeur_( tym::row__ Element ) const;
		// Retourne l'élément précédent 'Element', ou LST_INEXISTANT si inexistant.
		tym::row__ Predecesseur_( tym::row__ Element ) const;
		// Retourne vrai si 'Element' existe dans la liste.
		bso__bool Existe_( tym::row__ Position ) const;
	public:
		//o Stack which contains the free locations.
		stk::E_STACK_( tym::row__ ) Libres;
		struct s
		: public aem::amount_extent_manager_::s
		{
			stk::E_STACK_( tym::row__ )::s Libres;
			// La position du premier élément de la liste. N'a de sens que si 'Vide' = false.
	//		tym::size__ Nombre;
		};
	// fonctions
		list_( s &S )
		: Libres( S.Libres ),
		  amount_extent_manager_( S)
		{}
		void reset( bool P = true )
		{
			amount_extent_manager_::reset( P );
			Libres.reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &M )
		{
			Libres.plug( M );
		}
		void plug( mmm::multimemory_ &M )
		{
			Libres.plug( M );
		}
		list_ &operator =( const list_ &T )
		{
			amount_extent_manager_::Force( T.amount_extent_manager_::Amount() );
			Libres = T.Libres;

			return *this;
		}
	/*	void ecrire( flo_sortie_ &Flot )
		{
			FLOEcrire( S_.Nombre, Flot );

			Libres.ecrire( Flot );
		}
		void lire( flo_entree_ &Flot )
		{
			FLOLire( Flot, S_.Nombre );

			Libres.lire( Flot );
		}
	*/	//f Initialiration.
		void Init( void )
		{
			amount_extent_manager_::Init();
			Libres.Init();
		}
		//f Remove 'Entry'.
		void Remove( tym::row__ Entry )
		{
			Libres.Push( Entry );
		}
		//f Return the position of a new entry.
		tym::row__ CreateEntry( void )
		{
			return Nouveau_();
		}
		//f Return the first entry if exists, 'NONE' if list empty.
		tym::row__ First( void ) const
		{
			if ( amount_extent_manager_::Amount() )
				if ( Exists( 0 ) )
					return 0;
				else
					return Successeur_( 0 );
			else
				return NONE;
		}
		//f Return the last entry, 'NONE' if list empty.
		tym::row__ Last( void ) const
		{
			if ( amount_extent_manager_::Amount() )
			{
				tym::row__ P = amount_extent_manager_::Amount() - 1;

				if ( Existe_( P ) )
					return P;
				else
					return Predecesseur_( P );
			}
			else
				return NONE;
		}
		//f Return true if empty, false otherway.
		bso__bool IsEmpty( void ) const
		{
			return Amount() == 0;
		}
		//f Return the entry next to 'Entry', 'NONE' if 'Entry' is the last one.
		tym::row__ Next( tym::row__ Entry ) const
		{
			if ( ++Entry < amount_extent_manager_::Amount() )
				if( Libres.IsEmpty() || Existe_( Entry ) )
					return Entry;
				else
					return Successeur_( Entry );
			else
				return NONE;
		}
		//f Return the previous entry of 'Entry', 'NONE' if 'Entry' the first one.
		tym::row__ Previous( tym::row__ Entry ) const
		{
			if ( Entry-- > 0 )
				if( Libres.IsEmpty() || Existe_( Entry ) )
					return Entry;
				else
					return Predecesseur_( Entry );
			else
				return NONE;
		}
		//f Amount of entries, NOT the extent of the list.
		tym::size__ Amount( void ) const
		{
			return amount_extent_manager_::Amount() - Libres.Amount();
		}
		//f Return true if 'Entry' exists, false otherwise.
		bso__bool Exists( tym::row__ Entry ) const
		{
			if ( Entry >= amount_extent_manager_::Amount() )
				return false;
			else if ( Libres.IsEmpty() )
				return true;
			else
				return Existe_( Entry );
		}
	};

	AUTO( list )

	//d Handle a list of objects.
	#define E_LIST			list

	#define E_LIST_			list_


	//c Handle a list with a maximum of 't' entries. Use 'LIST__' rather than directly this class.
	template <int t> class list__
	{
	private:
		// Table de l'occupation de la liste.
		bitbch::bit_bunch__<t> Occupation_;
		// Nombre d'éléments dans la liste.
		tym::size__ Nombre_;
	public:
		list__( void )
		{
			Occupation_.Reset( false );
			Nombre_ = 0;
		}
		//f First entry, 'NONE' if no entries.
		tym::row__ First( void ) const
		{
			return Occupation_.First( true );
		}
		//f LAsttry, 'NONE' if no entries.
		tym::row__ Last( void ) const
		{
			return Occupation_.Last( true );
		}
		//f Entry next to 'Entry', none if 'Entry' the last one.
		tym::row__ Next( tym::row__ Entry ) const
		{
			return Occupation_.Next( Entry, true );
		}
		//f Previous entry from 'Entry', none if 'Entry' the first one.
		tym::row__ Previous( tym::row__ Entry ) const
		{
			return Occupation_.Previous( Entry, true );
		}
		//f Return the position of a new entry.
		tym::row__ CreateEntry( err::handle Error = err::hUsual  )
		{
			tym::row__ Position = NONE;

			if ( Nombre_ == t ) 
			{
				if ( Error == err::hUsual )
					ERRu();
			}
			else
			{
				Position = Occupation_.First( false );

				Occupation_.Write( true, Position );

				Nombre_++;
			}

			return Position;
		}
		//f Remove 'Entry', which MUST exists.
		void Remove( tym::row__ Entry )
		{
			Occupation_.Write( false, Entry );

			Nombre_--;
		}
		//f Return true if 'Entry' exists, fals otherwise.
		bso__bool Exists( tym::row__ Entry ) const
		{
			return Occupation_.Read( Courant );
		}
		//f Return true if no entry, false otherwise.
		bso__bool IsEmpty( void ) const
		{
			return !Nombre_;
		}
		//f Mark the list as full.
		void Fill( void )
		{
			Occupation_.Reset( true );
			Nombre_ = t;
		}
		//f Mark the list as empty.
		void Empty( void )
		{
			Occupation_.Reset( false );
			Nombre_ = 0;
		}
		//f Return true if totally full, false otherwise.
		bso__bool IsFull( void )
		{
			return Nombre_ >= t;
		}

		//m Handle a list with a maximum of 'n' entries.
		#define E_LIST__( n )		llist< n >
	};

}



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
