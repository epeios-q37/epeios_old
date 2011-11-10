/*
	Header for the 'fblfub' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef FBLFUB__INC
#define FBLFUB__INC

#define FBLFUB_NAME		"FBLFUB"

#define	FBLFUB_VERSION	"$Revision$"

#define FBLFUB_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLFUBTutor;

#if defined( XXX_DBG ) && !defined( FBLFUB_NODBG )
#define FBLFUB_DBG
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
//D Frontend/Backend Layout Frontend for Universal Backend 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "fblfrd.h"
# include "fblfeb.h"
# include "fblfrb.h"

namespace fblfub {

	using namespace fblovl;

	using fblfrd::compatibility_informations__;

	using fblfrd::incompatibility_informations_;
	using fblfrd::incompatibility_informations;

	typedef fblfrd::parameters_handling_functions__ _parameters_handling_functions__;


	class universal_parameters_handling_functions__
	: public _parameters_handling_functions__
	{
	private:
		fblfeb::backend_local_access_base__ _Embedded;
		fblfrb::backend_remote_access_base___ _Remote;
		mode__ _Mode;
	protected:
		virtual void FBLFRDPreProcess( void )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.PreProcess();
				break;
			case mRemote:
				_Remote.PreProcess();
				break;
			default:
				ERRu();
				break;
			}
		}
		virtual void FBLFRDIn(
			fblcst::cast Cast,
			const void *Pointer,
			flw::ioflow__ &Flow )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.In( Cast, Pointer, Flow );
				break;
			case mRemote:
				_Remote.In( Cast, Pointer, Flow );
				break;
			default:
				ERRu();
				break;
			}
		}
		virtual void FBLFRDOut(
			flw::ioflow__ &Flow,
			fblcst::cast Cast,
			void *Pointer )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.Out( Flow, Cast, Pointer );
				break;
			case mRemote:
				_Remote.Out( Flow, Cast, Pointer );
				break;
			default:
				ERRu();
				break;
			}
		}
		virtual void FBLFRDPostProcess( flw::ioflow__ &Flow )
		{
			switch ( _Mode ) {
			case mEmbedded:
				_Embedded.PostProcess( Flow );
				break;
			case mRemote:
				_Remote.PostProcess( Flow );
				break;
			default:
				ERRu();
				break;
			}
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_Embedded.reset( P );
			_Remote.reset( P );
			_parameters_handling_functions__::reset( P );
			_Mode = m_Undefined;
		}
		universal_parameters_handling_functions__( void )
		{
			reset( false );
		}
		virtual ~universal_parameters_handling_functions__( void )
		{
			reset();
		}
		void Init( mode__ Mode )
		{
			reset();

			switch ( Mode ) {
			case mEmbedded:
				_Embedded.Init();
				break;
			case mRemote:
				_Remote.Init();
				break;
			default:
				ERRu();
				break;
			}

			_Mode = Mode;

			_parameters_handling_functions__::Init();
		}
	};

	typedef fblfrd::backend_access___ _backend_access___;

	class backend_universal_access___
	: public _backend_access___
	{
	private:
		universal_parameters_handling_functions__ _Functions;
	public:
		void reset( bso::bool__ P = true )
		{
			_backend_access___::reset( P );
			_Functions.reset( P );
		}
		backend_universal_access___( void )
		{
			reset( false );
		}
		~backend_universal_access___( void )
		{
			reset();
		}
		bso::bool__ Init(
			const char *Language,
			const compatibility_informations__ &CompatibilityInformations,
			flw::ioflow__ &Flow,
			mode__ Mode,
			fblfrd::error_reporting_functions__ &ErrorReportingFunctions,
			incompatibility_informations_ &IncompatibilityInformations )
		{
			_Functions.Init( Mode );

			return _backend_access___::Init( Language, CompatibilityInformations, Flow, _Functions, ErrorReportingFunctions, IncompatibilityInformations  );
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
