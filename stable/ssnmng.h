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
	1.1.0 26/05/2000 14:36:15

Instigators:
	Claude SIMON

Authors:
	Claude SIMON

Contributors:
	/

Coordinator:
	Claude SIMON

Description:
	SeSsion MaNaGer
*/

/* Begin of automatic documentation generation part. */

//V 1.1.0
//C Claude SIMON
//D SeSsion MaNaGer
//R 26/05/2000 14:36:15

/* End of automatic documentation generation part. */

/* end of 'entete.h' template file */
/* begin of 'xxx.h' template file V2.6 */

#ifndef SSNMNG__INC
#define SSNMNG__INC

#define SSNMNG_NAME		"SSNMNG"

#define	SSNMNG_VERSION		"1.1.0"	
#define SSNMNG_OWNER		"The Epeios Project (www.epeios.org)"	

//  	Substitution is not applied to the rest when emancipated

#ifdef SSNMNG__LINE
#line 14
#endif

#include "ttr.h"

extern class ttr_tutor &SSNMNGTutor;

#if defined( XXX_DBG ) && !defined( SSNMNG_NODBG )
#define SSNMNG_DBG 
#endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "lst.h"
#include "idxbtq.h"

#ifndef SSNMNG_SIZE
//d Size of the session id.
#define SSNMNG_SIZE	5
#endif

//c A session id.
class ssnmng__session_id
{
	char Raw_[SSNMNG_SIZE+1];
public:
	ssnmng__session_id( void )
	{
		Raw_[0] = 0;
	}
	//f Create a new session id.
	void New( void );
	//f Return the value of the session id.
	const char *Value( void ) const
	{
		return Raw_;
	}
};


struct ssnmng__chrono {
	time_t Relative;
	time_t Absolute;
};

//c A session manager.
class ssnmng_sessions_manager_
: public LIST_,
  public MQUEUE_
{
protected:
	virtual void LSTAllocate( SIZE__ Size )
	{
		Table.Allocate( Size );
		Index.Allocate( Size );
		Chronos.Allocate( Size );
		SSNMNGAllocate( Size );
		MQUEUE_::Allocate( Size );
		
	}
	//v Permit to make an allocation with a affected structure.
	virtual void SSNMNGAllocate( SIZE__ Size ){}
public:
	struct s
	: public LIST_::s,
	public MQUEUE_::s
	{
		SET_( ssnmng__session_id )::s Table;
		idxbtq_tree_queue_index_::s Index;
		SET_( ssnmng__chrono )::s Chronos;
		bso__ushort Absolute;
		bso__ushort Relative;
	} &S_;
	//o The table of session ids.
	SET_( ssnmng__session_id ) Table;
	//o The index.
	idxbtq_tree_queue_index_ Index;
	//o The timing.
	SET_( ssnmng__chrono ) Chronos;
	ssnmng_sessions_manager_( s &S )
	: S_( S ),
	  LIST_( S ),
	  Table( S.Table ),
	  Index( S.Index ),
	  Chronos( S.Chronos ),
	  MQUEUE_( S )
	{}
	void reset( bool P = true )
	{
		S_.Absolute = S_.Relative = 0;

		LIST_::reset( P );
		Table.reset( P );
		Index.reset( P );
		Chronos.reset( P );
		MQUEUE_::reset( P );
	}
	void plug( mmm_multimemory_ &M )
	{
		LIST_::plug( M );
		Table.plug( M );
		Index.plug( M );
		Chronos.plug( M );
		MQUEUE_::plug( M );
	}
	ssnmng_sessions_manager_ &operator =( const ssnmng_sessions_manager_ &S )
	{
		LIST_::operator =( S );
		Table = S.Table;
		Index = S.Index;
		Chronos = S.Chronos;
		MQUEUE_::operator =( S );

		S_.Relative = S.S_.Relative;
		S_.Absolute = S.S_.Absolute;

		return *this;
	}
	//f Initialization with 'Relative' and 'Absolute' amonut of second.
	void Init(
		bso__ushort Relative = BSO_USHORT_MAX,
		bso__ushort Absolute = BSO_USHORT_MAX )
	{
		LIST_::Init();
		Table.Init();
		Index.Init();
		Chronos.Init();
		MQUEUE_::Init();

		S_.Relative = Relative;
		S_.Absolute = Absolute;
	}
	//f Return the position of a mandatory new session.
	POSITION__ Open( void );
	//f Remove the session id at position 'Position'.
	void Close( POSITION__ Position )
	{
		Index.Remove( Position );
		LIST_::Remove( Position );
		MQUEUE_::Remove( Position );
	}
	//f Return the position of 'SessionID' or NONE if non-existent.
	POSITION__ Position( const ssnmng__session_id &SessionID ) const
	{
		return Position( SessionID.Value() );
	}
	//f Return the position of 'SessionID' or NONE if non-existent.
	POSITION__ Position( const char *SessionID ) const;
	//f Return the session id. corresponding to 'Position'.
	ssnmng__session_id SessionID( POSITION__ Position )
	{
		return Table( Position );
	}
	//f Touche the session corresponding at position 'P'.
	void Touch( POSITION__ P )
	{
		ssnmng__chrono C = Chronos.Read( P );

		if ( time( &C.Relative ) == -1 )
			ERRs();

		Chronos.Write( C, P );

#ifdef SSNMNG_DBG
		if ( MQUEUE_::Amount() == 0 )
			ERRu();
#endif

		if ( ( MQUEUE_::Amount() != 1 ) && ( MQUEUE_::Tail() != P ) ) {
			MQUEUE_::Remove( P );
			MQUEUE_::InsertItemAfterNode( P, MQUEUE_::Tail() );
		}
	}
	//f Return true if session corresponding to 'P' is valid.
	bso__bool IsValid( POSITION__ P ) const
	{
		ssnmng__chrono C = Chronos.Read( P );

		return ( difftime( time( NULL ), C.Absolute ) < S_.Absolute )
			   && ( difftime( time( NULL ), C.Relative ) < S_.Relative );
	}
	//f Balance the index. 
	void Balance( void )
	{
		Index.Balance();
	}
};

AUTO( ssnmng_sessions_manager )


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
/* end of 'xxx.h' template file */