/*
  Header for the 'stk' library by Claude L. Simon (simon@epeios.org)
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

#ifndef STK__INC
#define STK__INC

#define STK_NAME		"STK"

#define	STK_VERSION	"$Revision$"	

#define STK_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &STKTutor;

#if defined( XXX_DBG ) && !defined( STK_NODBG )
#define STK_DBG 
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
#include "set.h"

//c Stack of static objects of type 't'. Use 'STACK_( t )' rather then directly this class.
template <class t> class stk_stack_
: public SET_( t )
{
public:
	struct s
	: public SET_( t )::s
	{
		SIZE__ Taille;
	} &S_;
	stk_stack_( s &S )
	: S_( S ),
	  SET_( t )( S ){}
	void reset( bool P = true )
	{
		S_.Taille = 0;
		SET_( t )::reset( P );
	}
/*	void ecrire( flo_sortie_ &Flot )
	{
		FLOEcrire( S_.Taille, Flot );

		SET_( t )::ecrire( Flot );
	}
	void lire( flo_entree_ &Flot )
	{
		FLOLire( Flot, S_.Taille );

		SET_( t )::lire( Flot );
	}
*/	void plug( MEMORY_DRIVER_ &MDriver )
	{
		SET_( t )::plug( MDriver );
	}
	void plug( mmm_multimemory_ &M )
	{
		SET_( t )::plug( M );
	}
	stk_stack_ &operator =( const stk_stack_ &S )
	{
		SET_( t )::operator =( S );
		S_.Taille = S.S_.Taille;

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		S_.Taille = 0;

		SET_( t )::Init();
	}
	//f Place 'Object' at the top of the stack. Return the position where this object is put.
	POSITION__ Push( t Object )
	{
		if ( SET_( t )::Extent() <= S_.Taille )
			SET_( t )::Allocate( S_.Taille + 1 );

		SET_( t )::Write( Object, S_.Taille );

		return S_.Taille++;
	}
	//f Return and remove the object at the bottom of the stack. If 'Adjust' at 'true', than adjust the size of the stack.
	t Pop( bso__bool Adjust = true )
	{
		t Objet = SET_( t )::Read( S_.Taille - 1 );

		S_.Taille--;

		if ( Adjust )
			SET_( t )::Allocate( S_.Taille );

		return Objet;
	}
	//f Amount of object in the stack, NOT the size of the stack.
	SIZE__ Amount( void ) const
	{
		return S_.Taille;
	}
	//f Return 'true' if stack is empty.
	bso__bool IsEmpty( void ) const
	{
		return S_.Taille == 0;
	}
		//f Return the first element pushed in the stack.
	POSITION__ First( void ) const
	{
		if ( S_.Taille )
			return 0;
		else
			return NONE;
	}
	//f Return the last element in the stack, the one which is return by the 'pop()' function. 
	POSITION__ Last( void ) const
	{
		if ( S_.Taille )
			return S_.Taille - 1;
		else
			return NONE;
	}
	//f Return the next element in the pile (get closer to the top of the pile).
	POSITION__ Next( POSITION__ Current ) const
	{
		if ( ++Current < S_.Taille )
			return Current;
		else
			return NONE;
	}
	/*f Return previous element. */
	POSITION__ Previous( POSITION__ Current ) const
	{
		if ( Current )
			return Current - 1;
		else
			return NONE;
	}

};

AUTO1( stk_stack )

//m A stack of static object of type 't'.
#define STACK_( t )	stk_stack_< t >

#define STACK( t )	stk_stack< t >


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
