/*
	'dtfrmf' library by Claude SIMON (csimon@epeios.org)
	Requires the 'dtfrmf' header file ('dtfrmf.h').
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

#define DTFRMF__COMPILATION

#include "dtfrmf.h"

class dtfrmftutor
: public ttr_tutor
{
public:
	dtfrmftutor( void )
	: ttr_tutor( DTFRMF_NAME )
	{
#ifdef DTFRMF_DBG
		Version = DTFRMF_VERSION "\b\bD $";
#else
		Version = DTFRMF_VERSION;
#endif
		Owner = DTFRMF_OWNER;
		Date = "$Date$";
	}
	virtual ~dtfrmftutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dtfrmf;


#define M1	255
#define M2	65535
#define M3	16777215

void dtfrmf::PutSize(
	bso::ulong__ Size,
	flw::oflow___ &Flow )
{
	if ( Size >= M1 )
	{
		Flow.Put( (flw::datum__)M1 );
		Size -= M1;

		if ( Size >= M2 )
		{
			Flow.Put( (flw::datum__)M1 );
			Flow.Put( (flw::datum__)M1 );
			Size -= M2;

			if ( Size >= M3 )
			{
				Flow.Put( (flw::datum__)M1 );
				Flow.Put( (flw::datum__)M1 );
				Flow.Put( (flw::datum__)M1 );

				Size -= M3;

				Flow.Put( (flw::datum__)( ( Size & ( 255 << 24 ) ) >> 24 ) );
			}
			Flow.Put( (flw::datum__)( ( Size & ( 255 << 16 ) ) >> 16 ) );
		}
		Flow.Put( (flw::datum__)( ( Size & ( 255 << 8 ) ) >> 8 ) );
	}
	Flow.Put( (flw::datum__)( Size & 255 ) );
}

bso::ulong__ dtfrmf::GetSize( flw::iflow___ &IFlow )
{
	bso::ulong__ Size = (bso::ubyte__)IFlow.Get();

	if ( Size == M1 )
	{
		flw::datum__ Data[4];

		IFlow.Get( 2, Data );

		Size += ( Data[0] << 8 ) | Data[1];

		if ( Size == ( M1 + M2 ) )
		{
			IFlow.Get( 3, Data );

			Size += ( Data[0] << 16 ) | ( Data[1] << 8 ) | Data[2];

			if ( Size == ( M1 + M2 + M3 ) ) {

				IFlow.Get( 4, Data );

				Size += ( Data[0] << 24 ) | ( Data[1] << 16 ) | ( Data[2] << 8 ) | Data[3];
			}
		}
	}

	return Size;
}

void dtfrmf::Encapsulate(
	const flx::bunch_ &Data,
	flw::oflow___ &Flow )
{
ERRProlog
	flx::bunch_iflow___ IFlow;
	tym::size__ Size;
ERRBegin
	IFlow.Init( Data );
	
	Size = Data.Amount();
	
	PutSize( Size, Flow );

	while( Size-- )	// Should be optimized by using a buffer.
		Flow.Put( IFlow.Get() );
ERRErr
ERREnd
ERREpilog
}

void dtfrmf::Decapsulate(
	flw::iflow___ &Flow,
	flx::bunch_ &Data )
{
ERRProlog
	flx::bunch_oflow___ OFlow;
	tym::size__ Size;
ERRBegin
	OFlow.Init( Data );

	Size = GetSize( Flow );

	while( Size-- )	// Should be optimized by using a buffer.
		OFlow.Put( Flow.Get() );
ERRErr
ERREnd
ERREpilog
}




/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dtfrmfpersonnalization
: public dtfrmftutor
{
public:
	dtfrmfpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dtfrmfpersonnalization( void )
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

static dtfrmfpersonnalization Tutor;

ttr_tutor &DTFRMFTutor = Tutor;
