/*
	Header for the 'aem' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2001-2004 Claude SIMON (csimon@epeios.org).

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

#ifndef AEM__INC
#define AEM__INC

#define AEM_NAME		"AEM"

#define	AEM_VERSION	"$Revision$"

#define AEM_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &AEMTutor;

#if defined( XXX_DBG ) && !defined( AEM_NODBG )
#define AEM_DBG
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
//D Amount/Extent managing 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Amount/Extent managing.
/* End addendum to automatic documentation generation part. */

#include "err.h"
#include "flw.h"
#include "tym.h"

//d The coefficient between step value and step size.
#define AEM_STEP_COEFFICIENT	256

namespace aem {
	using namespace tym;

	//e Mode of the allocation.
	enum mode {
		//i Undefined.
		mUndefined,
		//i Extent is handled using step value.
		mDefault,
		//i Extent fits with amount.
		mFit,
	};

	//t Type of a step value.
	typedef bso::ubyte__ step_value__;

	//t Type of a step size.
	typedef bso::ushort__ step_size__;

	//c Core amount/extent manager. Internal use.
	template <typename row, typename size> class core_amount_extent_manager_
	{
	private:
		bso::bool__ OnlyGrowing_( void ) const
		{
			return ( S_.Misc & 0x80 ) != 0;
		}
		epeios::size__ Adjust_( epeios::size__ Size )
		{
			step_size__ Step = StepSize_();

			S_.Amount = Size;
			S_.Misc = ( Size ? ( ( ( ( Size - 1 ) / Step ) + 1 ) * Step ) : 0 ) | ( S_.Misc & 0xff );

			return S_.Misc & 0xffffff00;
		}
		epeios::size__ Extent_( void ) const
		{
			if ( S_.Misc <= 0xff )
				return S_.Amount;
			else
				return S_.Misc & 0xffffff00;
		}
		step_value__ StepValue_( void ) const
		{
			return (step_value__)( S_.Misc & 0x7f );
		}
		step_size__ StepSize_( void ) const
		{
			return (step_size__)( StepValue_() * AEM_STEP_COEFFICIENT );
		}
		bso::bool__ Decrease_( epeios::size__ &Size )
		{
			if ( Extent_() >= ( StepSize_() + Size ) ) {
				Size = Adjust_( Size );
				return true;
			} else {
				S_.Amount = Size;
				return false;
			}
		}
		bso::bool__ Increase_( epeios::size__ &Size )
		{
			Size = Adjust_( Size );

			return true;
		}
	protected:
		/*f Return true if a allocation is needed for size 'Size'. 'Size' then contains
		the real size to allocate. If 'Mode' = 'mFit', 'Extent' is forced to be equal
		to 'Size'. */
		bso::bool__ AmountToAllocate(
			epeios::size__ &Size,
			mode Mode )
		{
			if ( Size == S_.Amount )
				return false;
			else if ( ( Mode == mFit ) || ( StepValue_() == 0 ) )
				return Force( Size );
			else if ( Size < S_.Amount ) {
				if ( !OnlyGrowing_() )
					return Decrease_( Size ) ;
				else {
					S_.Amount = Size;
					return false;
				}
			}
			else
				if ( Extent_() < Size )
					return Increase_( Size );
				else {
					S_.Amount = Size;
					return false;
				}
		}
		/*f Force the amount and extent to exactly 'Size'.
		Return true if the amount or the extent wasn't equal to 'Size'. */
		bso::bool__ Force( epeios::size__ Size )
		{
			bso::bool__ NotEqual = ( Extent() != Size ) || ( Amount() != Size );

			S_.Amount = Size;
			S_.Misc = S_.Misc & 0xff;	// Avoid the '&=' notation due to possible portable type of 'S_.Misc'.

			return NotEqual;
		}
	public:
		struct s {
		// First 3 bytes are the extent / 256.
		// Next first bit is at 1 if the we don't have to reallocate when size decrease.
		// Next seven bytes is the allocation step / 256.
		// If the allocation step at 0, then the extent fits with the amount
		// even in no decreasin state. */
			size Misc;
			size Amount;
		} &S_;
		core_amount_extent_manager_( s &S )
		: S_( S )
		{}
		void reset( bso::bool__ = true )
		{
			S_.Amount = 0;
			S_.Misc = 0;
		}
		core_amount_extent_manager_ &operator =( const core_amount_extent_manager_ &AEM )
		{
			// Amount normally handled by calling function.

			if ( AEM.StepValue_() != 0 )	// 'AEM' is a stand alone object.
				S_.Misc = 0;	
			else	// '*this' is a standalone object.
				S_.Misc = S_.Misc & 0xff;	// Avoid the '&=' notation due to possible portable type of 'S_.Misc'.

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			S_.Amount = 0;
			S_.Misc = 1;
		}
		//f Return the extent.
		epeios::size__ Extent( void ) const
		{
			return Extent_();
		}
		//f Return the amount.
		epeios::size__ Amount( void ) const
		{
			return S_.Amount;
		}
		//f Set the step to 'StepValue' (< 128).
		void SetStepValue( step_value__ StepValue )
		{
			if ( StepValue >= 128 )
				ERRu();

			S_.Misc = ( S_.Misc & 0xffffff40 | StepValue );

		}
		//f Return the step value.
		step_value__ GetStepValue( void ) const
		{
			return StepValue_();
		}
		//f Return the step size.
		step_size__ GetStepSize( void ) const
		{
			return StepSize_();
		}
		//f Return true if it grows only.
		bso::bool__ GetNoDecreasingState( void )
		{
			return OnlyGrowing_();
		}
		//f If 'State' at true, never decrease.
		void SetNoDecreasingState( bso::bool__ State )
		{
			S_.Misc |= ( S_.Misc & 0xffffff3f ) | ( State ? 0x80 : 0 );
		}
		//f Return position of the last object of the set.
		row Last( void ) const
		{
			if ( Amount() )
				return Amount() - 1;
			else
				return NONE;
		}
		//f Return position of the first object of the set.
		row First( void ) const
		{
			if ( Amount() )
				return 0;
			else
				return NONE;
		}
		//f Return the position of 'Offset' next to 'Current'.
		row Next(
			row Current,
			size__ Offset ) const
		{
			if ( ( *Current += Offset ) < Amount() )
				return Current;
			else
				return NONE;
		}
		//f Return the position of the object after 'Current' (to the top).
		row Next( row Current ) const
		{
			return Next( Current, 1 );
		}
		//f Return the position of 'Offset' previous to 'Current'.
		row Previous(
			row Current,
			size__ Offset ) const
		{
			if ( *Current >= Offset )
				return *Current - Offset;
			else
				return NONE;
		}
		//f Return the position of the object before 'Current' (to the bottom).
		row Previous( row Current ) const
		{
			return Previous( Current, 1 );
		}
		//f Return true if empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return Amount() == 0;
		}
		//f Return true if 'Row' exists, false otherwise.
		bso::bool__ Exists( row Row ) const
		{
			return *Row < Amount();
		}
	};


	//c A amount/extent manager.
	template <typename row> class amount_extent_manager_
	: public core_amount_extent_manager_<row, mdr::size__>
	{
	public:
		struct s
		: core_amount_extent_manager_<row, mdr::size__>::s
		{};
		amount_extent_manager_( s &S )
		: core_amount_extent_manager_<row, mdr::size__>( S )
		{}
	};

	//c A portable amount/extent manager.
	template <typename row> class p_amount_extent_manager_
	: public core_amount_extent_manager_<row, mdr::p_size__>
	{
	public:
		struct s
		: core_amount_extent_manager_<row, mdr::p_size__>::s
		{};
		p_amount_extent_manager_( s &S )
		: core_amount_extent_manager_<row, mdr::p_size__>( S )
		{}
	};


	//c Core amount/extent manager for fixed size set of object. Internal use only.
	template <int extent, typename row, typename size> class core_amount_extent_manager__
	{
	private:
		size Amount_;
	protected:
		/*f Return true if a allocation is needed for size 'Size'. 'Size' then contains
		the real size to allocate. */
		bso::bool__ AmountToAllocate(
			epeios::size__ &Size,
			mode Mode)
		{
			Amount_ = Size;
			return false;
		}
	public:
		struct s {};	// to simplify use in library 'BCH'.
		core_amount_extent_manager__( s &S = *(s *)NULL )
		{
			reset( false );
		}
		// Simplifies the 'SET' library.
		void reset( bso::bool__ = true )
		{
			Amount_ = 0;
		}
		core_amount_extent_manager__ &operator =( const core_amount_extent_manager__ &AEM )
		{
			// Amount normally already handled by calling function.

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Amount_ = 0;
		}
		//f Return the extent.
		epeios::size__ Extent( void ) const
		{
			return extent;
		}
		//f Return the amount.
		epeios::size__ Amount( void ) const
		{
			return Amount_;
		}
		//f Return position of the last object of the set.
		row Last( void ) const
		{
			if ( Amount() )
				return Amount() - 1;
			else
				return NONE;
		}
		//f Return position of the first object of the set.
		row First( void ) const
		{
			if ( Amount() )
				return 0;
			else
				return NONE;
		}
		//f Return the position of 'Offset' next to 'Current'.
		row Next(
			row Current,
			size__ Offset ) const
		{
			if ( ( Current += Offset ) < Amount() )
				return Current;
			else
				return NONE;
		}
		//f Return the position of the object after 'Current' (to the top).
		row Next( row Current ) const
		{
			return Next( Current, 1 );
		}
		//f Return the position of 'Offset' previous to 'Current'.
		row Previous(
			row Current,
			size__ Offset ) const
		{
			if ( Current >= Offset )
				return Current - Offset;
			else
				return NONE;
		}
		//f Return the position of the object before 'Current' (to the bottom).
		row Previous( row Current ) const
		{
			return Previous( Current, 1 );
		}
		//f Return true if empty, false otherwise.
		bso::bool__ IsEmpty( void ) const
		{
			return Amount() == 0;
		}
		//f Return true if 'Row' exists, false otherwise.
		bso::bool__ Exists( row Row ) const
		{
			return *Row < Amount();
		}
	};

	//c Core amount/extent manager for fixed size set of object.
	template <int extent, typename row> class amount_extent_manager__
	: public core_amount_extent_manager__<extent, row, mdr::size__>
	{
	public:
		struct s
		: public core_amount_extent_manager__<extent, row, mdr::size__>::s
		{};
		amount_extent_manager__( s &S = *(s *)NULL )
		: core_amount_extent_manager__<extent, row, mdr::size__>( S )
		{}
	};

	//c Core amount/extent manager for fixed size set of object.
	template <int extent, typename row> class p_amount_extent_manager__
	: public core_amount_extent_manager__<extent, row, mdr::p_size__>
	{
	public:
		struct s
		: public core_amount_extent_manager__<extent, row, mdr::p_size__>::s
		{};
		p_amount_extent_manager__( s &S = *(s *)NULL )
		: core_amount_extent_manager__<extent, row, mdr::p_size__>( S )
		{}
	};

	//c A amount/extent manager.
	template <typename row> class amount_extent_manager___
	: public amount_extent_manager_<row>
	{
	public:
		struct s
		: amount_extent_manager_<row>::s
		{};
		amount_extent_manager___( s &S )
		: amount_extent_manager_<row>( S )
		{}
	};

	//c A portable amount/extent manager.
	template <typename row> class p_amount_extent_manager___
	: public p_amount_extent_manager_<row>
	{
	public:
		struct s
		: p_amount_extent_manager_<row>::s
		{};
		p_amount_extent_manager___( s &S )
		: p_amount_extent_manager_<row>( S )
		{}
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
