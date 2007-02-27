/*
	Header for the 'mmm0' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef MMM0__INC
#define MMM0__INC

#define MMM0_NAME		"MMM0"

#define	MMM0_VERSION	"$Revision$"

#define MMM0_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &MMM0Tutor;

#if defined( XXX_DBG ) && !defined( MMM0_NODBG )
#define MMM0_DBG
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
//D MultiMeMory base 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

// Ce module contient les d�finitions communes pour tous les 'mmmX' (X > 0), d'o� son namesapce 'mmm'.

#include "err.h"
#include "flw.h"
#include "mdr.h"

namespace mmm {
	class multimemory_;	// Pr�d�claration.

	//t Type d'un descripteur de sous-m�m�oire dans le multim�moire.
	typedef mdr::row_t__	descriptor__;

	//c The standard memory driver for the multimemory.
	class multimemory_driver__
	: public mdr::E_MEMORY_DRIVER__
	{
	private:
		descriptor__ &_Descriptor;
		// memoire � laquelle il a �t� affect�
		class multimemory_ *Multimemoire_;
		void Liberer_();
	protected:
		// fonction d�port�e
		// lit � partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
		virtual void MDRRecall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer );
		// fonction d�port�e
		// �crit 'Nombre' octets � la position 'Position'
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position );
		// fonction d�port�e
		// alloue 'Capacite' octets
		virtual void MDRAllocate( mdr::size__ Size );
		virtual void MDRFlush( void );
	public:
		multimemory_driver__(
			descriptor__ &Descriptor,
			mdr::size__ &Extent )
		: _Descriptor( Descriptor ),
		  E_MEMORY_DRIVER__( Extent )
		{}
		void reset( bool P = true )
		{
			if ( P )
				Liberer_();
			else
				Multimemoire_ = NULL;

			_Descriptor = 0;
			
			E_MEMORY_DRIVER__::reset( P );
		}
		//f Initialization with the 'Multimemory' multimemory.
		void Init( multimemory_ &Multimemory )
		{
			Liberer_();

			Multimemoire_ = &Multimemory;
			_Descriptor = 0;

			E_MEMORY_DRIVER__::Init();
		}
		//f Return the current descriptor.
		descriptor__ Descriptor( void ) const
		{
			return _Descriptor;
		}
		//f Return the used multimemory.
		multimemory_ *Multimemory( void ) const
		{
			return Multimemoire_;
		}
	};

	class standalone_multimemory_driver__
	: public multimemory_driver__
	{
	private:
		mdr::size__ _Extent;
		descriptor__ _Descriptor;
	public:
		standalone_multimemory_driver__( void )
		: multimemory_driver__( _Descriptor, _Extent )
		{}
	};

	#define E_MULTIMEMORY_DRIVER__	standalone_multimemory_driver__

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
