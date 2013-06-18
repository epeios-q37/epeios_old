/*
	'frdkernl.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef FRDKERNL__INC
# define FRDKERNL__INC

# include "sktinf.h"
# include "sktapi.h"

# include "frdrgstry.h"

# include "csdsnc.h"
# include "csducl.h"
# include "xml.h"
# include "fblfub.h"

# include "frdkrn.h"

namespace frdkernl {
	typedef frdkrn::kernel___ _kernel___;

	using frdkrn::status__;

	// If modified, modify 'GetRawMessage_(...)' in 'frdkernel.cpp' according.
	enum message__ 
	{
		mNotImplementedYet,
		m_amount,
		m_Undefined
	};

	const char *GetLabel( message__ Message );

	class kernel___
	: public _kernel___
	{
	private:
		esketch::statics___ _Statics;
		esketch::skt_myobject_common__ _Common;
		esketch::skt_myobject___ _Object;
	protected:
		virtual void FRDKRNConnection( fblfrd::backend_access___ &BackendAccess )
		{
			_Statics.Init( BackendAccess );
			_Common.Init( BackendAccess );
			_Object.Init( _Common );
		}
		virtual void FRDKRNDisconnection( void )
		{
			_Object.reset();
			_Common.reset();
			_Statics.reset();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_kernel___::reset( P );
		}
		kernel___( void )
		{
			reset( false );
		}
		~kernel___( void )
		{
			reset();
		}
		status__ Init(
			const rgstry::registry_ &ConfigurationRegistry,
			rgstry::row__ ConfigurationRegistryRoot,
			const lcl::locale_ &Locale,
			const char *Language,
			frdkrn::reporting_functions__ &ReportingFunctions )
		{
			return _kernel___::Init( ConfigurationRegistry, ConfigurationRegistryRoot, Locale, Language, ReportingFunctions );
		}
	};

}

#endif
