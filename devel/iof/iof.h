/*
	Header for the 'iof' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
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

#ifndef IOF__INC
#define IOF__INC

#define IOF_NAME		"IOF"

#define	IOF_VERSION	"$Revision$"

#define IOF_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &IOFTutor;

#if defined( XXX_DBG ) && !defined( IOF_NODBG )
#define IOF_DBG
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

/* Addendum to the automatic documentation generation part. */
//D Input/Output Flows 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "cpe.h"
#include "fwf.h"
#include "flw.h"
#include "iop.h"

namespace iof {
	typedef iop::input__		_input__;
	typedef iop::output__		_output__;
	typedef iop::io__			_io__;

	class _output_functions__
	: public _output__
	{
	protected:
		fwf::size__ _Write(
			const fwf::datum__ *Buffer,
			fwf::size__ Maximum )
		{
#ifdef IOF_DBG
			if ( Buffer == NULL )
				ERRu();
#endif
			return _output__::Write( Buffer, Maximum );
		}
		void _Commit( void )
		{
			_output__::Flush();
		}
	};

	class io_oflow_functions___
	: public _output_functions__,
	  public fwf::oflow_functions___
	{
	protected:
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Tampon,
			fwf::size__ Maximum )
		{
			return _output_functions__::Write( Tampon, Maximum );
		}
		virtual void FWFCommit( void )
		{
			return _output_functions__::_Commit();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_output_functions__::reset( P );
			oflow_functions___::reset( P );
		}
		io_oflow_functions___( void )
		{
			reset( false );
		}
		virtual ~io_oflow_functions___( void )
		{
			reset();
		}
		void Init( iop::descriptor__ D )
		{
			_output_functions__::Init( D );
			oflow_functions___::Init();
		}
	};

	class io_oflow___
	: public flw::oflow__
	{
	private:
		io_oflow_functions___ _Functions;
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			oflow__::reset( P );
			_Functions.reset( P );
		}
		io_oflow___( iop::amount__ AmountMax )
		: oflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~io_oflow___( void )
		{
			reset();
		}
		void Init( iop::descriptor__ D )
		{
			_Functions.Init( D );
		}
	};

	class _input_functions__
	: public _input__
	{
	protected:
		virtual fwf::size__ _Read(
			fwf::size__ Maximum,
			fwf::datum__ *Buffer )
		{
	#ifdef IOF_DBG
			if( Buffer == NULL )
				ERRu();
	#endif
			if ( !OnEOF() )
				return _input__::Read( Maximum, Buffer );
			else
				return 0;
		}
		virtual void _Dismiss( void )
		{}
	};


	class io_iflow_functions___
	: public _input_functions__,
	  public fwf::iflow_functions___
	{
	protected:
		virtual fwf::size__ FWFRead(
			fwf::size__ Maximum,
			fwf::datum__ *Buffer )
		{
			return _input_functions__::_Read( Maximum, Buffer );
		}
		virtual void FWFDismiss( void )
		{
			_input_functions__::_Dismiss();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_input_functions__::reset( P );
			iflow_functions___::reset( P );
		}
		io_iflow_functions___( void )
		{
			reset( false );
		}
		virtual ~io_iflow_functions___( void )
		{
			reset();
		}
		void Init( iop::descriptor__ D )
		{
			_input_functions__::Init( D );
			iflow_functions___::Init();
		}
	};

	class io_iflow___
	: public flw::iflow__
	{
	private:
		flw::datum__ _Cache[IOP__BUFFER_SIZE];
		io_iflow_functions___ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			_Functions.reset( P );
			iflow__::reset( P );
		}
		io_iflow___( iop::amount__ AmountMax )
		: iflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~io_iflow___( void )
		{
			reset();
		}
		void Init( iop::descriptor__ D )
		{
			_Functions.Init( D );
		}
	};

	class io_flow_functions___
	: public _output_functions__,
	  public _input_functions__,
	  public fwf::ioflow_functions___
	{
	protected:
		virtual fwf::size__ FWFWrite(
			const fwf::datum__ *Buffer,
			fwf::size__ Maximum )
		{
			return _output_functions__::_Write( Buffer, Maximum );
		}
		virtual void FWFCommit( void )
		{
			_output_functions__::_Commit();
		}
		virtual fwf::size__ FWFRead(
			fwf::size__ Maximum,
			fwf::datum__ *Buffer )
		{
			return _input_functions__::_Read( Maximum, Buffer );
		}
		virtual void FWFDismiss( void )
		{
			return _input_functions__::_Dismiss();
		}
	public:
		void Init( iop::descriptor__ D )
		{
			_input_functions__::Init( D );
			_output_functions__::Init( D );
		}

	};

	class io_flow___
	: public _io__,
	  public flw::ioflow__
	{
	private:
		io_flow_functions___ _Functions;
		flw::datum__ _Cache[2 * IOP__BUFFER_SIZE];
	public:
		void reset( bso::bool__ P = true )
		{
			_io__::reset( P );
			ioflow__::reset( P );
		}
		io_flow___( iop::amount__ AmountMax )
		: ioflow__( _Functions, _Cache, sizeof( _Cache ), AmountMax )
		{
			reset( false );
		}
		virtual ~io_flow___( void )
		{
			reset();
		}
		void Init(
			iop::descriptor__ D,
			iop::amount__ AmountMax )
		{
			_io__::Init( D );
			_Functions.Init( D );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
