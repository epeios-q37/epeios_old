/*
  Header for the 'aem' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org) 

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

#ifndef AEM__INC
#define AEM__INC

#define AEM_NAME		"AEM"

#define	AEM_VERSION	"$Revision$"	

#define AEM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &AEMTutor;

#if defined( XXX_DBG ) && !defined( AEM_NODBG )
#define AEM_DBG 
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
//D Amount/Extent managing.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "tym.h"

//d The coefficient between step value and step size.
#define AEM_STEP_COEFFICIENT	256

namespace aem {
	using namespace tym;

	//t Type of a step value.
	typedef bso__ubyte step_value__;

	//t Type of a step size.
	typedef bso__ushort step_size__;

	//c A amount/extent manager.
	class amount_extent_manager_
	{
	private:
		bso__bool OnlyGrowing_( void ) const
		{
			return ( S_.Misc & 0x80 ) != 0;
		}
		size__ Adjust_( size__ Size )
		{
			step_size__ Step = StepSize_();

			S_.Amount = Size;
			S_.Misc = ( ( ( Size / Step ) + 1 ) * Step ) | ( S_.Misc & 0xff );

			return S_.Misc & 0xffffff00;
		}
		size__ Extent_( void ) const
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
			return StepValue_() * AEM_STEP_COEFFICIENT;
		}
		bso__bool Decrease_( size__ &Size )
		{
			if ( Extent_() >= ( StepSize_() + Size ) ) {
				Size = Adjust_( Size );
				return true;
			} else {
				S_.Amount = Size;
				return false;
			}
		}
		bso__bool Increase_( size__ &Size )
		{
			Size = Adjust_( Size );

			return true;
		}
	protected:
		/*f Return true if a allocation is needed for size 'Size'. 'Size' then contains
		the real size to allocate. If 'Adjust' at true, 'Extent' is forced to be equal
		to* 'Size'. */
		bso__bool AmountToAllocate(
			size__ &Size,
			bso__bool Adjust = false )
		{
			if ( Size == S_.Amount )
				return false;
			else if ( Adjust || ( StepValue_() == 0 ) )
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
		bso__bool Force( size__ Size )
		{
			bso__bool NotEqual = ( Extent() != Size ) || ( Amount() != Size );

			S_.Amount = Size;
			S_.Misc &= 0xff;

			return NotEqual;
		}
	public:
		struct s {
		// First 3 bytes are the extent / 256.
		// Next first bit is at 1 if the we don't have to reallocate when size decrease.
		// Next seven bytes is the allocation step / 256.
			size__ Misc;
			size__ Amount;
		} &S_;
		amount_extent_manager_( s &S )
		: S_( S )
		{}
		void reset( bso__bool = true )
		{
			S_.Amount = 0;
			S_.Misc = 0;
		}
		amount_extent_manager_ &operator =( const amount_extent_manager_ &AEM )
		{
			// Amount normally handled by calling function.

			if ( AEM.StepValue_() != 0 )
				S_.Misc = 0;	// AEM is a stand alone object.
			else
				S_.Misc &= 0xff;	// *this is a standalone object.

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			S_.Amount = 0;
			S_.Misc = 1;
		}
		//f Return the extent.
		size__ Extent( void ) const
		{
			return Extent_();
		}
		//f Return the amount.
		size__ Amount( void ) const
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
		bso__bool GetNoDecreasingState( void )
		{
			return OnlyGrowing_();
		}
		//f If 'State' at true, never decrease.
		void SetNoDecreasingState( bso__bool State )
		{
			S_.Misc |= ( S_.Misc & 0xffffff3f ) | ( State ? 0x80 : 0 );
		}
		//f Return position of the last object of the set.
		row__ Last( void ) const
		{
			if ( Amount() )
				return Amount() - 1;
			else
				return NONE;
		}
		//f Return position of the first object of the set.
		row__ First( void ) const
		{
			if ( Amount() )
				return 0;
			else
				return NONE;
		}
		//f Return the position of the object after 'Current' (to the top).
		row__ Next( row__ Current ) const
		{
			if ( ++Current < Amount() )
				return Current;
			else
				return NONE;
		}
		//f Return the position of the object before 'Current' (to the bottom).
		row__ Previous( row__ Current ) const
		{
			if ( Current )
				return Current - 1 ;
			else
				return NONE;
		}
		//f Return true if empty, false otherwise.
		bso__bool IsEmpty( void ) const
		{
			return Amount() == 0;
		}
	};

	AUTO( amount_extent_manager )

	//c Amount/extent manager for fixed size set of object.
	template <int extent> class amount_extent_manager__
	{
	private:
		aem::size__ Amount_;
	protected:
		/*f Return true if a allocation is needed for size 'Size'. 'Size' then contains
		the real size to allocate. */
		bso__bool AmountToAllocate( aem::size__ &Size )
		{
			return false;
		}
	public:
		struct s {};	// to simplify use in library 'SET'.
		amount_extent_manager__( s &S = *(s *)NULL )
		{
			reset( false );
		}
		// Simplifies the 'SET' library.
		void reset( bso__bool = true )
		{
			Amount_ = 0;
		}
		amount_extent_manager__ &operator =( const amount_extent_manager__ &AEM )
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
		aem::size__ Extent( void ) const
		{
			return extent;
		}
		//f Return the amount.
		aem::size__ Amount( void ) const
		{
			return Amount_;
		}
		//f Return position of the last object of the set.
		aem::row__ Last( void ) const
		{
			if ( Amount() )
				return Amount() - 1;
			else
				return NONE;
		}
		//f Return position of the first object of the set.
		aem::row__ First( void ) const
		{
			if ( Amount() )
				return 0;
			else
				return NONE;
		}
		//f Return the position of the object after 'Current' (to the top).
		aem::row__ Next( aem::row__ Current ) const
		{
			if ( ++Current < Amount() )
				return Current;
			else
				return NONE;
		}
		//f Return the position of the object before 'Current' (to the bottom).
		aem::row__ Previous( aem::row__ Current ) const
		{
			if ( Current )
				return Current - 1 ;
			else
				return NONE;
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
