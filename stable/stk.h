/* begin of 'entete.txt' template file V1.1 */
/* Best viewed with a tab size of 4 */
/*
	This file is part of the Epeios project. For all information
	concerning the Epeios project, this file and its conditions of use,
	consult the site: 

			http://www.epeios.org/
*/
/* end of 'entete.txt' template file */
/* begin of 'entete.h' template file V1.3 */
/*
Version:
	1.0.6 24/05/2000 10:49:52

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	STacK
*/

/* Begin of automatic documentation generation part. */

//V 1.0.6
//C Claude SIMON
//D STacK
//R 24/05/2000 10:49:52

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.6 */

#ifndef STK__INC
#define STK__INC

#define STK_NAME		"STK"

#define	STK_VERSION		"1.0.6"	
#define STK_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef STK__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &STKTutor;

#if defined( XXX_DBG ) && !defined( STK_NODBG )
#define STK_DBG 
#endif

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
/* end of 'xxx.h' template file */