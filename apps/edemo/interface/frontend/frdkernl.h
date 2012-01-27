/*
	'frdkernl.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eDemo' software.

    'eDemo' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eDemo' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eDemo'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef FRDKERNL__INC
# define FRDKERNL__INC

# include "dmoinf.h"
# include "dmoapi.h"

# include "frdrgstry.h"

# include "csdsnc.h"
# include "csducl.h"
# include "xml.h"
# include "fblfub.h"

# include "xulfkl.h"

# define FRDKERNL_FRONTEND_NAME		DMOINF_LC_AFFIX "frd"
# define FRDKERNL_FRONTEND_VERSION	"0.1.0"

# define FRDKERNL_DEFAULT_LANGUAGE	"en"

namespace frdkernl {
	typedef xulfkl::kernel___ _kernel___;

	using frdkrn::status__;

	// If modified, modify 'GetRawMessage_(...)' in 'frdkernel.cpp' according.
	enum message__ 
	{
		mNotImplementedYet,
		m_amount,
		m_Undefined
	};

	class kernel___
	: public _kernel___
	{
	private:
		edemo::statics___ _Statics;
		edemo::dmo_myobject_common__ _Common;
		edemo::dmo_myobject___ _Object;
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
			const rgstry::multi_level_registry_ &ConfigurationRegistry,
			const char *TargetName,
			const lcl::locale_ &Locale,
			const char *Language,
			frdkrn::error_reporting_functions__ &ErrorReportingFunctions )
		{
			return _kernel___::Init( ConfigurationRegistry, TargetName, Locale, Language, ErrorReportingFunctions );
		}
		const str::string_ &GetTranslation(
			message__ Message,
			str::string_ &Translation );
		const char *GetTranslation(
			message__ Message,
			STR_BUFFER___ &Buffer );
	};

}

#endif