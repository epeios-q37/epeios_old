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
	1.2.0 26/04/2000 07:03:51

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	InDeX QUEue
*/

/* Begin of automatic documentation generation part. */

//V 1.2.0
//C Claude SIMON
//D InDeX QUEue
//R 26/04/2000 07:03:51

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.4 */

#ifndef IDXQUE__INC
#define IDXQUE__INC

#define IDXQUE_NAME		"IDXQUE"

#define	IDXQUE_VERSION		"1.2.0"	
#define IDXQUE_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef IDXQUE__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &IDXQUETutor;

#if defined( XXX_DBG ) && !defined( IDXQUE_NODBG )
#define IDXQUE_DBG 
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "txf.h"
#include "que.h"
#include "stk.h"

//d The end of the queue.
#define IDXQUE_END	NONE

struct idxque
{
	enum direction
	{
		dAscending = false,
		dDescending = true
	};
};

//c Queue-based index, fast browsing, but slow sorting.
class idxque_queue_index_
: public MQUEUE_
{
public:
	struct s
	: public MQUEUE_::s
	{};
// fonctions
	idxque_queue_index_( s &S )
	: MQUEUE_( S )
	{}
	void reset( bool P = true )
	{
		MQUEUE_::reset( P );
	}
	void plug( mmm_multimemory_ &MM )
	{
		MQUEUE_::plug( MM );
	}
	void plug( MEMORY_DRIVER_ &MD )
	{
		MQUEUE_::plug( MD );
	}
	idxque_queue_index_ &operator =( const idxque_queue_index_ &T )
	{
		MQUEUE_::operator =( T );

		return *this;
	}
	//f Initialization.
	void Init( void )
	{
		MQUEUE_::Init();
	}
	/*f Dump to the stack 'Stack' the queue beginning at 'Begin' and in the 'Direction ' direction.. */
	void Dump(
		stk_stack_<POSITION__> &Stack,
		POSITION__ Begin,
		idxque::direction Direction );
	//f Item becomes node next to 'Node'.
	void BecomeNext(
		POSITION__ Item,
		POSITION__ Node )
	{
		MQUEUE_::InsertItemAfterNode( Item, Node );
	}
	//f Item becomes node previous to 'Node'.
	void BecomePrevious(
		POSITION__ Item,
		POSITION__ Node )
	{
		MQUEUE_::InsertItemBeforeNode( Item, Node );
	}
};


AUTO( idxque_queue_index )


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */