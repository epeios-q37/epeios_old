/*
  'idxbtr' library by Claude L. Simon (simon@epeios.org)
  Requires the 'idxbtr' header file ('idxbtr.h').
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

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

#define IDXBTR__COMPILATION

#include "idxbtr.h"

class idxbtrtutor
: public ttr_tutor
{
public:
	idxbtrtutor( void )
	: ttr_tutor( IDXBTR_NAME )
	{
#ifdef IDXBTR_DBG
		Version = IDXBTR_VERSION "\b\bD $";
#else
		Version = IDXBTR_VERSION;
#endif
		Owner = IDXBTR_OWNER;
		Date = "$Date$";
	}
	virtual ~idxbtrtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

namespace idxbtr {

	/* structure de description d'un arbre.
	Est utilisé lors du rééquilibrage d'un arbre. Usage interne. */
	struct desc__
	{
		// Racine de l'arbre. N'a pas de fils droit. L'arbre de fils gauche est complet.
		tym::row__ Racine;
		// Niveau de l'arbre.
		tym::size__ Niveau;
	};

	// Retourne le premier noeud sans fils à partir de 'Position' en descendant par les fils.
	tym::row__ tree_index_::NoeudSansFils_( tym::row__ Position ) const
	{
		while( E_BTREE_::HasLeft( Position ) )
			Position = E_BTREE_::Left( Position );

		return Position;
	}

	// Retourne le premier noeud sans fille à partir de 'Position' en descendant par les fille.
	tym::row__ tree_index_::NoeudSansFille_( tym::row__ Position ) const
	{
		while( E_BTREE_::HasRight( Position ) )
			Position = E_BTREE_::Right( Position );

		return Position;
	}

	// Retourne le premier noeud qui est fils en remontant.
	tym::row__ tree_index_::PereFilsEnRemontant_( tym::row__ Position ) const
	{
		while( !E_BTREE_::IsLeft( Position ) && E_BTREE_::HasParent( Position ) )
			Position = E_BTREE_::Parent( Position );

		if ( E_BTREE_::IsLeft( Position ) )
			return E_BTREE_::Parent( Position );
		else
			return NONE;
	}

	// Retourne le premier noeud qui est fille en remontant.
	tym::row__ tree_index_::PereFilleEnRemontant_( tym::row__ Position ) const
	{
		while( !E_BTREE_::IsRight( Position ) && E_BTREE_::HasParent( Position ) )
			Position = E_BTREE_::Parent( Position );

		if ( E_BTREE_::IsRight( Position ) )
			return E_BTREE_::Parent( Position );
		else
			return NONE;
	}

	void tree_index_::Balance( void )
	{
	ERRProlog
		tym::row__ Current, Head, Temp;
		que::E_QUEUE Queue;	
	ERRBegin
		Queue.Init();
		Queue.Allocate( Extent() );

		Current = First();

		if ( Current != NONE )
		{
			Head = Temp = Current;

			Current = Next( Temp );

			while ( Current != NONE )
			{
				Queue.InsertItemAfterNode( Current, Temp );

				Temp = Current;

				Current = Next( Temp );
			}

			Fill( Queue, Head );
		}

	ERRErr
	ERREnd
	ERREpilog
	}

	/* Equilibre l'arbre, sachant que l'ordre des éléments est donnée par
	la file 'File' de tête 'Tete' et que l'on doit utiliser la pile 'Pile'. */
	tym::row__ tree_index_::Equilibrer_(
		que::E_QUEUE_ &File,
		tym::row__ Premier,
		mdr::E_MEMORY_DRIVER_ &Pilote )
	{
		tym::row__ Racine, &Courant = Premier;
	ERRProlog
		stk::E_STACK( desc__ ) Pile;
		tym::size__ Niveau = 0;
		desc__ Sommet;
		bso__bool Boucler = true;
	ERRBegin

		if ( &Pilote )
			Pile.plug( Pilote );

		Pile.Init();

		Racine = Courant;

		do
		{
			while( !Pile.IsEmpty() && ( Pile( Pile.Last() ).Niveau == Niveau ) )
			{
				Sommet = Pile.Pop();

				Niveau = Sommet.Niveau + 1;
				E_BTREE_::BecomeRight( Racine, Sommet.Racine );

				Racine = Sommet.Racine;
			}

			Courant = File.Next( Courant );

			if ( Courant != NONE )
			{
				if ( File.HasNext( Courant ) )
				{
					E_BTREE_::BecomeLeft( Racine, Courant );

					Sommet.Racine = Courant;
					Sommet.Niveau = Niveau;

					Pile.Push( Sommet );

					Courant = File.Next( Courant );
				}
				else
				{
					Boucler = false;
					E_BTREE_::BecomeRight( Courant, NoeudSansFille_( Racine ) );
				}
			}
			else
				Boucler = false;

			if ( Boucler )
			{
				Racine = Courant;
				Niveau = 0;
			}
		} while( Boucler );

		while( !Pile.IsEmpty() )
		{
			Sommet = Pile.Pop();

			E_BTREE_::BecomeRight( Racine, Sommet.Racine );

			Racine = Sommet.Racine;
		}
	ERRErr
	ERREnd
	ERREpilog
		return Racine;
	}
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class idxbtrpersonnalization
: public idxbtrtutor
{
public:
	idxbtrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~idxbtrpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static idxbtrpersonnalization Tutor;

ttr_tutor &IDXBTRTutor = Tutor;
