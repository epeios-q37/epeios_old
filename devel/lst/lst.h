/*
  Header for the 'lst' library by Claude L. Simon (epeios@epeios.org)
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
//C Claude L. SIMON (epeios@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "bst.h"
#include "stk.h"


//c Handle a list of objects. Use 'LIST_' rather than directly this class.
class lst_list_
{
protected:
	/*v Cette fonction est appelée lors d'allocations dans la liste;
	permet de synchroniser la taille de la liste avec d'autres ensembles;
	'Size' est la capacité allouée. Ne fait rien par défaut. */
	virtual void LSTAllocate( SIZE__ Size ) {}
private:
	POSITION__ Nouveau_( void )
	{
		POSITION__ Numero;

		if ( Libres.Amount() )
			Numero = Libres.Pop();
		else
		{
			Numero = S_.Nombre;

			LSTAllocate( Numero + 1 );
		}

		return Numero;
	}
	// Retourne l'élément succédant à 'Element', ou LST_INEXISTANT si inexistant.
	POSITION__ Successeur_( POSITION__ Element ) const;
	// Retourne l'élément précédent 'Element', ou LST_INEXISTANT si inexistant.
	POSITION__ Predecesseur_( POSITION__ Element ) const;
	// Retourne vrai si 'Element' existe dans la liste.
	bso__bool Existe_( POSITION__ Position ) const;
	// Retourne la capacite de la liste.
	POSITION__ Capacite_( void ) const
	{
		return S_.Nombre + Libres.Amount();
	}
public:
	//o Stack which contains the free locations.
	STACK_( POSITION__ ) Libres;
	struct s
	{
		STACK_( POSITION__ )::s Libres;
		// La position du premier élément de la liste. N'a de sens que si 'Vide' = false.
		SIZE__ Nombre;
	} &S_;
// fonctions
	lst_list_( s &S )
	: S_( S ),
	  Libres( S.Libres ){}
	void reset( bool P = true )
	{
		S_.Nombre = 0;

		Libres.reset( P );
	}
	void plug( MEMORY_DRIVER_ &M )
	{
		Libres.plug( M );
	}
	void plug( mmm_multimemory_ &M )
	{
		Libres.plug( M );
	}
	lst_list_ &operator =( const lst_list_ &T )
	{
		Libres = T.Libres;

		S_.Nombre = T.S_.Nombre;

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
		S_.Nombre = 0;

		Libres.Init();
	}
	//f Remove 'Entry'.
	void Remove( POSITION__ Entry )
	{
		Libres.Push( Entry );

		S_.Nombre--;
	}
	//f Return the position of a new entry.
	POSITION__ CreateEntry( void )
	{
		POSITION__ Nouveau = Nouveau_();

		S_.Nombre++;

		return Nouveau;
	}
	//f Return the first entry if exists, 'NONE' if list empty.
	POSITION__ First( void ) const
	{
		if ( S_.Nombre )
			if ( Exists( 0 ) )
				return 0;
			else
				return Successeur_( 0 );
		else
			return NONE;
	}
	//f Return the last entry, 'NONE' if list empty.
	POSITION__ Last( void ) const
	{
		if ( S_.Nombre )
		{
			POSITION__ P = Capacite_() - 1;

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
		return !S_.Nombre;
	}
	//f Return the entry next to 'Entry', 'NONE' if 'Entry' is the last one.
	POSITION__ Next( POSITION__ Entry ) const
	{
		if ( ++Entry < Capacite_() )
			if( Libres.IsEmpty() || Existe_( Entry ) )
				return Entry;
			else
				return Successeur_( Entry );
		else
			return NONE;
	}
	//f Return the previous entry of 'Entry', 'NONE' if 'Entry' the first one.
	POSITION__ Previous( POSITION__ Entry ) const
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
	SIZE__ Amount( void ) const
	{
		return S_.Nombre;
	}
	//f Return true if 'Entry' exists, false otherwise.
	bso__bool Exists( POSITION__ Entry ) const
	{
		if ( Entry >= Capacite_() )
			return false;
		else if ( Libres.IsEmpty() )
			return true;
		else
			return Existe_( Entry );
	}
	//f Extent of the list, NOT the amount of entries.
	SIZE__ Extent( void ) const
	{
		return Capacite_();
	}
};

AUTO( lst_list )

//d Handle a list of objects.
#define LIST			lst_list

#define LIST_			lst_list_


//c Handle a list with a maximum of 't' entries. Use 'LIST__' rather than directly this class.
template <int t> class lst__list
{
private:
	// Table de l'occupation de la liste.
	bst__bits_set<t> Occupation_;
	// Nombre d'éléments dans la liste.
	SIZE__ Nombre_;
public:
	lst__list( void )
	{
		Occupation_.Reset( false );
		Nombre_ = 0;
	}
	//f First entry, 'NONE' if no entries.
	POSITION__ First( void ) const
	{
		return Occupation_.First( true );
	}
	//f LAsttry, 'NONE' if no entries.
	POSITION__ Last( void ) const
	{
		return Occupation_.Last( true );
	}
	//f Entry next to 'Entry', none if 'Entry' the last one.
	POSITION__ Next( POSITION__ Entry ) const
	{
		return Occupation_.Next( Entry, true );
	}
	//f Previous entry from 'Entry', none if 'Entry' the first one.
	POSITION__ Previous( POSITION__ Entry ) const
	{
		return Occupation_.Previous( Entry, true );
	}
	//f Return the position of a new entry.
	POSITION__ CreateEntry( err::handle Error = err::hUsual  )
	{
		POSITION__ Position = NONE;

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
	void Remove( POSITION__ Entry )
	{
		Occupation_.Write( false, Entry );

		Nombre_--;
	}
	//f Return true if 'Entry' exists, fals otherwise.
	bso__bool Exists( POSITION__ Entry ) const
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
};


//m Handle a list with a maximum of 'n' entries.
#define LIST__( n )		lst__list< n >



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
