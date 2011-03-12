/*
	Header for the 'crptgr' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20112004 Claude SIMON.

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

#ifndef CRPTGR__INC
#define CRPTGR__INC

#define CRPTGR_NAME		"CRPTGR"

#define	CRPTGR_VERSION	"$Revision$"

#define CRPTGR_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &CRPTGRTutor;

#if defined( XXX_DBG ) && !defined( CRPTGR_NODBG )
#define CRPTGR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D CRyPToGRaphy 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "fwf.h"

namespace crptgr {

	typedef fwf::oflow_functions___<>	_oflow_functions___;
	
	class encrypt_oflow_functions___
	: public _oflow_functions___
	{
	private:
		flw::oflow__ *_Flow;
		const char *_Key;
		bso::size__ _Position;
		fwf::datum__ _Encrypt( fwf::datum__ Datum )
		{
			if ( _Key[_Position] == 0 )
				_Position = 0;

			return Datum + _Key[_Position++];
		}
	protected:
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Buffer,
			fwf::size__ Maximum )
		{
			fwf::size__ Amount = 0;

			// A optimiser.
			while ( Amount < Maximum )
				_Flow->Put( _Encrypt( Buffer[Amount++] ) );

			return Amount;
		}
		virtual void FWFCommit( void )
		{
			_Flow->Commit();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow_functions___::reset( P );

			_Flow = NULL;
			_Key = NULL;
			_Position = 0;
		}
		encrypt_oflow_functions___( void )
		{
			reset( false );
		}
		~encrypt_oflow_functions___( void )
		{
			reset( false );
		}
		void Init(
			flw::oflow__ &Flow,
			const char *Key,
			fwf::thread_safety__ ThreadSafety )	// 'Key' n'est PAS dupliqu�.
		{
			_oflow_functions___::Init( ThreadSafety );

			_Flow = &Flow;
			_Key = Key;
			_Position = 0;
		}
	};

	typedef flw::standalone_oflow__<> _oflow__;

	class encrypt_oflow___
	: public _oflow__
	{
	private:
		encrypt_oflow_functions___ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			_oflow__::reset( P );
			_Functions.reset( P );
		}
		encrypt_oflow___( void )
		{
			reset( false );
		}
		~encrypt_oflow___( void )
		{
			reset();
		}
		void Init(
			flw::oflow__ &Flow,
			const char *Key,
			flw::size__ AmountMax = FLW_SIZE_MAX )	// 'Key' n'est PAS dupliqu�.
		{
			_Functions.Init( Flow, Key, fwf::tsDisabled );
			_oflow__::Init( _Functions, AmountMax );
		}
	};


	typedef fwf::iflow_functions___<>	_iflow_functions___;
	
	class decrypt_iflow_functions___
	: public _iflow_functions___
	{
	private:
		flw::iflow__ *_Flow;
		const char *_Key;
		bso::size__ _Position;
		fwf::datum__ _Decrypt( fwf::datum__ Datum )
		{
			if ( _Key[_Position] == 0 )
				_Position = 0;

			return Datum - _Key[_Position++];
		}
	protected:
		virtual fwf::size__ FWFRead(
			fwf::size__ Maximum,
			fwf::datum__ *Buffer )
		{
			fwf::size__ Amount = 0;

			Maximum = _Flow->ReadUpTo( Maximum, Buffer, 0 );

			// A optimiser.
			while ( Amount < Maximum ) {
				Buffer[Amount] = _Decrypt( Buffer[Amount] );
				Amount++;
			}

			return Maximum;
		}
		virtual void FWFDismiss( void )
		{
			_Flow->Dismiss();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow_functions___::reset( P );

			_Flow = NULL;
			_Key = NULL;
			_Position = 0;
		}
		decrypt_iflow_functions___( void )
		{
			reset( false );
		}
		~decrypt_iflow_functions___( void )
		{
			reset( false );
		}
		void Init(
			flw::iflow__ &Flow,
			const char *Key,
			fwf::thread_safety__ ThreadSafety )	// 'Key' n'est PAS dupliqu�.
		{
			_iflow_functions___::Init( ThreadSafety );

			_Flow = &Flow;
			_Key = Key;
			_Position = 0;
		}
	};

	typedef flw::standalone_iflow__<> _iflow__;

	class decrypt_iflow___
	: public _iflow__
	{
	private:
		decrypt_iflow_functions___ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			_iflow__::reset( P );
			_Functions.reset( P );
		}
		decrypt_iflow___( void )
		{
			reset( false );
		}
		~decrypt_iflow___( void )
		{
			reset();
		}
		void Init(
			flw::iflow__ &Flow,
			const char *Key,
			flw::size__ AmountMax = FLW_SIZE_MAX )	// 'Key' n'est PAS dupliqu�.
		{
			_Functions.Init( Flow, Key, fwf::tsDisabled );
			_iflow__::Init( _Functions, AmountMax );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
