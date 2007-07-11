/*
	Header for the 'ssnmng' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

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

#ifndef SSNMNG__INC
#define SSNMNG__INC

#define SSNMNG_NAME		"SSNMNG"

#define	SSNMNG_VERSION	"$Revision$"

#define SSNMNG_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &SSNMNGTutor;

#if defined( XXX_DBG ) && !defined( SSNMNG_NODBG )
#define SSNMNG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D SeSsioN MaNaGer 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "lst.h"
#include "idxbtq.h"
#include "str.h"

#ifndef SSNMNG_SIZE
//d Size of the session id.
#define SSNMNG_SIZE	5
#endif

namespace ssnmng {

	//t The type of the row of a session.
	E_ROW( row__ );

	typedef bch::E_BUNCH_( row__ ) rows_;
	E_AUTO( rows )

	//c A session id.
	class session_id__
	{
		char Raw_[SSNMNG_SIZE+1];
	public:
		session_id__( void )
		{
			Raw_[0] = 0;
		}
		//f Initialization.
		void Init( void )
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
		//f Dump.
		void Dump( void )
		{
			Raw_[0] = 0;
		}
		//f Return true if empty.
		bso::bool__ IsEmpty( void ) const
		{
			return Raw_[0] == 0;
		}
	};


	struct chrono__ {
		time_t Relative;
		time_t Absolute;
		bso::bool__ AlwaysValid;	// A vrai si la session est toujours considèré comme valide, quel que soit sa durée d'existence.
	};

	typedef lst::E_LISTt_( row__ )	_list_;
	typedef que::E_MQUEUEt_( row__ ) _queue_;

	//c A session manager.
	class base_sessions_manager_
	: public _list_,
	  public _queue_
	{
	protected:
		virtual void LSTAllocate(
			epeios::size__ Size,
			aem::mode__ Mode )
		{
			Table.Allocate( Size, Mode );
			Index.Allocate( Size, Mode );
			Chronos.Allocate( Size, Mode );
			_Allocate( Size, Mode );
			SSNMNGAllocate( Size, Mode );
			_queue_::Allocate( Size, Mode );
			
		}
		//v Permit to make an allocation with a affected structure.
		virtual void SSNMNGAllocate(
			epeios::size__ Size,
			aem::mode__ Mode ) = 0;
		virtual void _Allocate(
			epeios::size__ Size,
			aem::mode__ Mode ) = 0;
	public:
		struct s
		: public _list_::s,
		public _queue_::s
		{
			bch::E_BUNCHt_( session_id__, row__ )::s Table;
			idxbtq::E_INDEXt_( row__ )::s Index;
			bch::E_BUNCHt_( chrono__, row__ )::s Chronos;
			bso::ushort__ Absolute;
			bso::ushort__ Relative;
			row__ Root;
		} &S_;
		//o The table of session ids.
		bch::E_BUNCHt_( session_id__, row__ ) Table;
		//o The index.
		idxbtq::E_INDEXt_( row__ ) Index;
		//o The timing.
		bch::E_BUNCHt_( chrono__, row__ ) Chronos;
		base_sessions_manager_( s &S )
		: S_( S ),
		  _list_( S ),
		  Table( S.Table ),
		  Index( S.Index ),
		  Chronos( S.Chronos ),
		  _queue_( S )
		{}
		void reset( bool P = true )
		{
			S_.Absolute = S_.Relative = 0;
			S_.Root = NONE;

			_list_::reset( P );
			Table.reset( P );
			Index.reset( P );
			Chronos.reset( P );
			_queue_::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			_list_::plug( M );
			Table.plug( M );
			Index.plug( M );
			Chronos.plug( M );
			_queue_::plug( M );
		}
		base_sessions_manager_ &operator =( const base_sessions_manager_ &S )
		{
			_list_::operator =( S );
			Table = S.Table;
			Index = S.Index;
			Chronos = S.Chronos;
			_queue_::operator =( S );

			S_.Relative = S.S_.Relative;
			S_.Absolute = S.S_.Absolute;
			S_.Root = S.S_.Root;

			return *this;
		}
		//f Initialization with 'Relative' and 'Absolute' amonut of second.
		void Init(
			bso::ushort__ Relative,
			bso::ushort__ Absolute  )
		{
			_list_::Init();
			Table.Init();
			Index.Init();
			Chronos.Init();
			_queue_::Init();

			S_.Relative = Relative;
			S_.Absolute = Absolute;
			S_.Root = NONE;
		}
		//f Return the position of a mandatory new session.
		row__ Open( void );
		//f Remove the session id at position 'Position'.
		void Close( row__ Position )
		{
			S_.Root = Index.Delete( Position, S_.Root );
			_list_::Delete( Position );
			_queue_::Delete( Position );
		}
		//f Return the position of 'SessionID' or NONE if non-existent.
		row__ Position( const session_id__ &SessionID ) const
		{
			return Position( SessionID.Value() );
		}
		//f Return the position of 'SessionID' or NONE if non-existent.
		row__ Position( const char *SessionID ) const;
		//f Return the position of 'SessionID' or NONE if non-existent.
		row__ Position( const str::string_ &SessionID ) const;
		//f Return the session id. corresponding to 'Position'.
		session_id__ SessionID( row__ Position )
		{
			return Table( Position );
		}
		//f Touch the session corresponding at position 'P'.
		void Touch( row__ P )
		{
			chrono__ C = Chronos.Get( P );

			if ( time( &C.Relative ) == -1 )
				ERRs();

			C.AlwaysValid = false;

			Chronos.Store( C, P );
	#ifdef SSNMNG_DBG
			if ( _queue_::Amount() == 0 )
				ERRu();
	#endif

			if ( ( _queue_::Amount() != 1 ) && ( _queue_::Tail() != P ) ) {
				_queue_::Delete( P );
				_queue_::BecomeNext( P, _queue_::Tail() );
			}
		}
		//f La session concernée est toujours considèrée comme valide, jusqu'au prochain 'Touch()',
		void MarkAsAlwaysValid( row__ P )
		{
			chrono__ C = Chronos.Get( P );

			C.AlwaysValid = true;

			Chronos.Store( C, P );
		}
		//f Return true if session corresponding to 'P' is valid.
		bso::bool__ IsValid( row__ P ) const
		{
			chrono__ C = Chronos.Get( P );

			return ( C.AlwaysValid
				     || ( ( difftime( time( NULL ), C.Absolute ) < S_.Absolute )
				          && ( difftime( time( NULL ), C.Relative ) < S_.Relative ) ) );
		}
		//f Return true if session (which must exists) corresponding to row 'Row' is expired, false otherwise.
		bso::bool__ IsExpired( row__ Row ) const
		{
			return !IsValid( Row );
		}
		//f Balance the index. 
		void Balance( void )
		{
			if ( S_.Root != NONE )
				S_.Root = Index.Balance( S_.Root );
		}
		//f Put in 'Expired' the expired sessions.
		void GetExpired( rows_ &Expired ) const;
		void GetAll( rows_ &Rows ) const;
		E_NAVt( _queue_::, row__ )
	};

	E_AUTO( base_sessions_manager )

	class user_functions__
	{
	protected:
		virtual void SSNMNGDelete( void *UP ) = 0;
	public:
		void Delete( void *UP )
		{
			SSNMNGDelete( UP );
		}
	};

	typedef bch::E_BUNCHt_( void *, row__ ) pointers_;
	E_AUTO( pointers )

	class sessions_manager_
	: public base_sessions_manager_,
	  public pointers_
	{
	private:
		void _Allocate(
			epeios::size__ Size,
			aem::mode__ Mode )
		{
			pointers_::Allocate( Size, Mode );
		}
		user_functions__ *_UserFunctions;
		void _Close( const rows_ &Rows );
	public:
		struct s
		: public base_sessions_manager_::s,
		  public pointers_::s
		{};
		sessions_manager_( s &S )
		: base_sessions_manager_( S ),
		  pointers_( S )
		{}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			base_sessions_manager_::plug( MM );
			pointers_::plug( MM );
		}
		sessions_manager_ &operator =( const sessions_manager_ &SM )
		{
			base_sessions_manager_::operator =( SM );
			pointers_::operator =( SM );

			return *this;
		}
		void reset( bso::bool__ P = true )
		{
			if ( P )
				CloseAll();
			_UserFunctions = NULL;
			base_sessions_manager_::reset( P );
	  		pointers_::reset( P );

		}
		void Init(
			bso::ushort__ Relative,
			bso::ushort__ Absolute,
			user_functions__ &UserFunctions )
		{
			base_sessions_manager_::Init( Relative, Absolute );
			pointers_::Init();
			_UserFunctions = &UserFunctions;
		}
		void Close( row__ Row )
		{
			_UserFunctions->Delete( pointers_::Get( Row ) );
			base_sessions_manager_::Close( Row );
		}
		void CloseAll( void );
		void CloseExpired( void );
		E_NAVt( base_sessions_manager_::, row__ )
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
