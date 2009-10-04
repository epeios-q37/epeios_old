/*
  Header for the 'itg' library by Claude SIMON (http://zeusw.org/intl/contact.html)
  Copyright (C) 2000,2001 Claude SIMON (http://zeusw.org/intl/contact.html) 

  This file is part of the Epeios (http://zeusw.org/epeios/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
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

#define ITG_OWNER		"the Epeios project (http://zeusw.org/epeios/)"

#include "ttr.h"

extern class ttr_tutor &ITGTutor;

#if defined( XXX_DBG ) && !defined( ITG_NODBG )
#define ITG_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
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
#define ITG_MAX_SIZE	((base__)( 0xffffU >> 1 ))

#define ITG_SIGN_POS	((base__)15U)
#define ITG_SIGN_MASK	((base__)( 1U << ITG_SIGN_POS ))

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
			S_.Size = ( ( S_.Size & ~ITG_SIGN_MASK ) | ( Sign << ITG_SIGN_POS ) );
			return GetSign_();
		}
		void Adjust_( aem::mode Mode = aem::mDefault );
		void Dup_( const integer_ &Interger );
		friend int Comp_(
			const integer_ &Num,
			const integer_ &Den );
		friend integer Divide_(
			const integer_ &Num,
			const integer_ &Den,
			integer_ &Rest );
		friend integer Div_(
			const integer_ &Num,
			const integer_ &Den,
			integer_ &Rest );
		friend integer Add_(
			const integer_ &Op1,
			const integer_ &Op2 );
		friend integer Sub_(
			const integer_ &Op1,
			const integer_ &Op2 );
		friend integer Mul_(
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
			void reset( bso__bool P = true )
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
			friend flw::oflow___ &operator <<(
				flw::oflow___ &Flow,
				integer_ &Integer );
			integer Add( const integer_ &Op ) const;
			integer Sub( const integer_ &Op ) const;
			integer Mul( const integer_ &Op ) const;
			integer Div( const integer_ &Op ) const;
			integer Mod( const integer_ &Op ) const;
			integer Abs( void ) const;
			integer operator -( void ) const;
			int operator !( void )
			{
				Adjust_();
				
				return !( ( GetSize_() != 1 ) || ( Core( 0 ) ) );
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
			reset( false );
		}
		~integer( void )
		{
			reset( true );
		}
		integer( const integer_ &I )
		: integer_( S_ )
		{
			reset( false );
			
			integer_::Init();
			
			integer_::operator =( I );
		}
		integer( const integer &I )
		: integer_( S_ )
		{
			reset( false );
			
			integer_::Init();
			
			integer_::operator =( I );
		}
		integer( bso__slong Seed )
		: integer_( S_ )
		{
			reset( false );

			integer_::Init();

			if ( Seed < 0 )
			{
				Seed = -Seed;
				PutSign_( 1 );
			}
			else
				PutSign_( 0 );

			if ( Seed < 0x10000UL )
			{
				Core.Allocate( 1 );

				Core.Write( (base__)Seed, 0 );
				PutSize_( 1 );
			}
			else
			{
				Core.Allocate( 2 );

				Core.Write( (base__)( Seed % 0x10000UL ), 0 );
				Core.Write( (base__)( Seed >> 16 ), 1 );
				PutSize_( 2 );
			}
		}

	};		

	inline integer integer_::operator -( void ) const 
	{
		integer R = *this;

		R.PutSign_( !R.GetSign_() );

		return R;
	}

	inline integer integer_::Abs( void ) const
	{
		integer R = *this;
		R.PutSign_( false );

		return R;
	}
}

inline itg::integer Abs( const itg::integer_ &I )
{
	return I.Abs();
}

inline itg::integer operator +(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return Op1.Add( Op2 );
}

inline itg::integer operator -(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return Op1.Sub( Op2 );
}

inline itg::integer operator *(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return Op1.Mul( Op2 );
}

inline itg::integer operator /(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return Op1.Div( Op2 );
}

inline itg::integer operator %(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return Op1.Mod( Op2 );
}

inline itg::integer operator +=(
	 itg::integer_ &I1,
	 const itg::integer_ &I2 )
{
	return I1  = I1 + I2;
}

inline itg::integer operator -=(
	 itg::integer_ &I1,
	 const itg::integer_ &I2 )
{
	return I1  = I1 - I2;
}

inline itg::integer operator *=(
	 itg::integer_ &I1,
	 const itg::integer_ &I2 )
{
	return I1  = I1 * I2;
}

inline itg::integer operator /=(
	 itg::integer_ &I1,
	 const itg::integer_ &I2 )
{
	return I1  = I1 / I2;
}

inline itg::integer operator %=(
	 itg::integer_ &I1,
	 const itg::integer_ &I2 )
{
	return I1  = I1 % I2;
}

inline int operator ==(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return itg::Comp_( Op1, Op2 ) == 0;
}	
inline int operator !=(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return itg::Comp_( Op1, Op2 );
}	
inline int operator >=(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return itg::Comp_( Op1, Op2 ) >= 0;
}	
inline int operator >(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return itg::Comp_( Op1, Op2 ) > 0;
}	
inline int operator <(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return itg::Comp_( Op1, Op2 ) < 0;
}	
inline int operator <=(
	const itg::integer_ &Op1,
	const itg::integer_ &Op2 )
{
	return itg::Comp_( Op1, Op2 ) <= 0;
}	


txf::text_oflow___ &operator <<(
	txf::text_oflow___ &OFlow,
	const itg::integer_ &Integer );

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
