/*
	'amm' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'amm' header file ('amm.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#define AMM__COMPILATION

#include "amm.h"

class ammtutor
: public ttr_tutor
{
public:
	ammtutor( void )
	: ttr_tutor( AMM_NAME )
	{
#ifdef AMM_DBG
		Version = AMM_VERSION "\b\bD $";
#else
		Version = AMM_VERSION;
#endif
		Owner = AMM_OWNER;
		Date = "$Date$";
	}
	virtual ~ammtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace amm;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */


# if 0
inline void aggregated_memory_driver__::_Free( void )
{
	if ( _Descriptor != MMM_UNDEFINED_DESCRIPTOR )
		Multimemoire_->Free( _Descriptor );
}

inline void multimemory_driver__::MDRAllocate( mdr::size__ Size )
{
	_Descriptor = Multimemoire_->Reallocate( _Descriptor, Size, _Addendum );
}

inline mdr::size__ multimemory_driver__::MDRUnderlyingSize( void )
{
	if ( _Descriptor != MMM_UNDEFINED_DESCRIPTOR )
		return Multimemoire_->Size( _Descriptor );
	else
		return 0;
}

inline void multimemory_driver__::MDRRecall(
	mdr::row_t__ Position,
	mdr::size__ Amount,
	mdr::datum__ *Buffer )
{
	Multimemoire_->Read( _Descriptor, Position, Amount, Buffer, _Addendum );
}
// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
inline void multimemory_driver__::MDRStore(
	const mdr::datum__ *Buffer,
	mdr::size__ Amount,
	mdr::row_t__ Position )
{
	Multimemoire_->Write( Buffer, Amount, _Descriptor, Position, _Addendum );
}

}

#endif

class ammpersonnalization
: public ammtutor
{
public:
	ammpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ammpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static ammpersonnalization Tutor;

ttr_tutor &AMMTutor = Tutor;
