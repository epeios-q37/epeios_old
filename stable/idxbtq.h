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
	1.1.14 24/05/2000 11:05:31

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	InDeX Best of Tree and Queue
*/

/* Begin of automatic documentation generation part. */

//V 1.1.14
//C Claude SIMON
//D InDeX Best of Tree and Queue
//R 24/05/2000 11:05:31

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.6 */

#ifndef IDXBTQ__INC
#define IDXBTQ__INC

#define IDXBTQ_NAME		"IDXBTQ"

#define	IDXBTQ_VERSION		"1.1.14"	
#define IDXBTQ_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef IDXBTQ__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &IDXBTQTutor;

#if defined( XXX_DBG ) && !defined( IDXBTQ_NODBG )
#define IDXBTQ_DBG 
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "idxque.h"
#include "idxbtr.h"

//c Index using a tree-based index and a queue-based index. Fast browsing and sorting.
class idxbtq_tree_queue_index_
: public idxbtr_tree_index_,
  public idxque_queue_index_
{
public:
	struct s
	: public idxbtr_tree_index_::s,
	  public idxque_queue_index_::s
	{};
	idxbtq_tree_queue_index_( s &S )
	: idxbtr_tree_index_( S ),
	  idxque_queue_index_( S )
	{}
	void reset( bool P = true )
	{
		idxbtr_tree_index_::reset( P );
		idxque_queue_index_::reset( P );
	}
	void plug( mmm_multimemory_ &MM )
	{
		idxbtr_tree_index_::plug( MM );
		idxque_queue_index_::plug( MM );
	}
	idxbtq_tree_queue_index_ &operator =( const idxbtq_tree_queue_index_ &I )
	{
		idxbtr_tree_index_::operator =( I );
		idxque_queue_index_::operator =( I );

		return *this;
	}
/*	void ecrire( flo_sortie_ &Flot ) const
	{
		idxbtr_tree_index_::ecrire( Flot );
		idxque_queue_index_::ecrire( Flot );
	}
	void lire( flo_entree_ &Flot )
	{
		idxbtr_tree_index_::lire( Flot );
		idxque_queue_index_::lire( Flot );
	}
*/	//f Initializtion.
	void Init( void )
	{
		idxbtr_tree_index_::Init();
		idxque_queue_index_::Init();
	}
	FNLPA( idxque_queue_index_:: )
	//f 'Item' becomes the first item of the index, if empty.
	void Create( POSITION__ Item )
	{
		if ( !IsEmpty() )
			ERRu();

		idxbtr_tree_index_::Create( Item );
		idxque_queue_index_::Create( Item );
	}
	//f Allocate enough room to contain 'Size' items.
	void Allocate( SIZE__ Size )
	{
		idxbtr_tree_index_::Allocate( Size );
		idxque_queue_index_::Allocate( Size );
	}
	/*f 'New' becomes the next item of 'Item'. 'NextAvailable( Item )' must
	return true to use this function. */
	void BecomeNext(
		POSITION__ New,
		POSITION__ Item )
	{
		idxbtr_tree_index_::BecomeNext( New, Item );
		idxque_queue_index_::BecomeNext( New, Item );
	}
	/*f 'New' becomes the previous item of 'Item'. 'PreviousAvailable( Item )' must
	return true to use this function. */
	void BecomePrevious(
		POSITION__ New,
		POSITION__ Item )
	{
		idxbtr_tree_index_::BecomePrevious( New, Item );
		idxque_queue_index_::BecomePrevious( New, Item );
	}
	//f Remove 'Item'.
	void Remove( POSITION__ Item )
	{
		idxque_queue_index_::Remove( Item );
		idxbtr_tree_index_::Remove( Item );
	}
	//f Balances the tree of the index.
	void Balance( MEMORY_DRIVER_ &MD = *(MEMORY_DRIVER_ *)NULL )
	{
		if ( idxque_queue_index_::Amount() )
			idxbtr_tree_index_::Fill( idxque_queue_index_::Queue, idxque_queue_index_::First(), MD );
	}
	//f Return the extent of the index, NOT the amount of item.
	SIZE__ Extent( void ) const
	{
		return idxbtr_tree_index_::Extent();
	}
};

AUTO( idxbtq_tree_queue_index )



/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */