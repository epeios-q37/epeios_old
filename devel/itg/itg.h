/*
  Header for the 'itg' library by Claude L. Simon (simon@epeios.org)
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

#ifndef ITG__INC
#define ITG__INC

#define ITG_NAME		"ITG"

#define	ITG_VERSION	"$Revision$"	

#define ITG_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &ITGTutor;

#if defined( XXX_DBG ) && !defined( ITG_NODBG )
#define ITG_DBG 
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
//D InTeGer
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "bso.h"
#include "mdr.h"
#include "mmm.h"
#include "bch.h"

//d Max size of an integer.
#define ITG_MAX_SIZE	( 0xffffU >> 1 )

#define ITG_SIGN_POS	15
#define ITG_SIGN_MASK	( 1U << ITG_SIGN_POS )

namespace itg {
	typedef bso__ushort	base__;
	typedef bso__ushort	size__;
	
	class integer;
	class integer_;
	
	//c An integer.
	class integer_ {
	protected:
		size__ GetSize_( void ) const
		{
			return S_.Size & ~ITG_SIGN_MASK;
		}
		size__ GetSign_( void ) const
		{
			return ( ( S_.Size & ITG_SIGN_MASK ) >> ITG_SIGN_POS );
		}
		size__ PutSize_( size__ Size )
		{
			S_.Size = ( ( S_.Size & ITG_SIGN_MASK ) | Size );
			return GetSize_();
		}
		size__ PutSign_( size__ Sign )
		{
			S_.Size = ( ( S_.Size & ITG_SIGN_MASK ) | ( Sign << ITG_SIGN_POS ) );
			return GetSign_();
		}
		void Adjust_( void );
		void Dup_( const integer_ &Interger );
		friend int Comp_(
			const integer_ &Num,
			const integer_ &Den );
		friend integer_ Divide_(
			const integer_ &Num,
			const integer_ &Den,
			integer_ &Rest );
		friend integer_ Div_(
			const integer_ &Num,
			const integer_ &Den,
			integer_ &Rest );
		friend integer_ Add_(
			const integer_ &Op1,
			const integer_ &Op2 );
		friend integer_ Sub_(
			const integer_ &Op1,
			const integer_ &Op2 );
		friend integer_ Mul_(
			const integer_ &Op1,
			const integer_ &Op2 );
		public:
			//o The core.
			bch::E_BUNCH_( base__ ) Core;
			struct s {
				/* The most significant is the sign (0:+, 1:-) (this are not smileys).
				Other bits aer the size, in word, of the integer. If this bits are all
				at 0, then the integer is invalid. */
				bch::E_BUNCH_( base__ )::s Core;
				size__ Size;
			} &S_;
			integer_( s &S )
			: S_( S ),
			  Core( S.Core )
			 {}
			void reset( bso__bool P = false )
			{
				Core.reset( P );
				S_.Size = 0;
			}
			void plug( mdr::E_MEMORY_DRIVER_ &MD )
			{
				Core.plug( MD );
			}
			void plug( mmm::multimemory_ &M )
			{
				Core.plug( M );
			}
			integer_ &operator =( const integer_ &I )
			{
				S_.Size = I.S_.Size;
				Core = I.Core;
				
				return *this;
			}
			//f Initialization.
			void Init( void )
			{
				S_.Size = 0;
				Core.Init();
			}
			friend flw::oflow___ operator <<(
				flw::oflow___ &Flow,
				integer_ &Integer );
			friend integer_ operator +(
				const integer_ &Op1,
				const integer_ &Op2 );
			friend integer_ operator -(
				const integer_ &Op1,
				const integer_ &Op2 );
			friend integer_ operator *(
				const integer_ &Op1,
				const integer_ &Op2 );
			friend integer_ operator /(
				const integer_ &Op1,
				const integer_ &Op2 );
			friend integer_ operator %(
				const integer_ &Op1,
				const integer_ &Op2 );
			friend integer_ Abs( const integer_ &Integer );
			integer_ operator -( void ) const;
			int operator !( void )
			{
				Adjust_();
				
				return !( ( GetSize_() != 1 ) || ( Core( 0 ) ) );
			}
			integer_ &operator +=( const integer_ &Integer )
			{
				return *this  = *this + Integer;
			}
			integer_ &operator -=( const integer_ &Integer )
			{
				return *this  = *this - Integer;
			}
			integer_ &operator *=( const integer_ &Integer )
			{
				return *this  = *this * Integer;
			}
			integer_ &operator /=( const integer_ &Integer )
			{
				return *this  = *this / Integer;
			}
			integer_ &operator %=( const integer_ &Integer )
			{
				return *this  = *this % Integer;
			}
			//f Return true if valid, fasle otherwise.
			bso__bool IsValid( void )
			{
				return !GetSize_();
			}
			bso__lfloat GetLongFloat( void ) const;
	};			
		
	inline int Compare(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		return 	Comp_( Op1, Op2 );
	}
	
	class integer
	: public integer_
	{
	private:
		s S_;
	public:
		integer( void )
		: integer_( S_ )
		{
			reset( true );
		}
		~integer( void )
		{
			reset( false );
		}
		integer( const integer_ &I )
		: integer_( S_ )
		{
			reset( true );
			
			integer_::Init();
			
			integer_::operator =( I );
		}
		integer( unsigned long int Seed );
	};		

	inline int operator ==(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		return !Comp_( Op1, Op2 );
	}	
	inline int operator !=(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		return Comp_( Op1, Op2 );
	}	
	inline int operator >=(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		return Comp_( Op1, Op2 ) >= 0;
	}	
	inline int operator >(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		return Comp_( Op1, Op2 ) > 0;
	}	
	inline int operator <(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		return Comp_( Op1, Op2 ) < 0;
	}	
	inline int operator <=(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		return Comp_( Op1, Op2 ) <= 0;
	}	
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
