/*
	'kernel.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef KERNEL__INC
#define KERNEL__INC

#include "frdkernl.h"
#include "frdbcknd.h"

#include "nsxpcm.h"

#include "ui.h"

// #define ADDRESS	"192.168.5.10:1234"	// Portable.
// #define ADDRESS	"10.0.2.2:1234"		// Logiplus.
#define ADDRESS	"localhost:1234"	// Local

#define DEFAULT_LOCATION	"d:\\emobda\\test"

namespace kernel {
	using namespace frdbcknd;

	using frdkernl::message__;
	using frdkernl::database_identification_state__;

	E_ROW( krow__ );	// 'kernel row', see below.

	extern nsxpcm::repository< class kernel___, krow__> Repository;

	typedef frdkernl::kernel___ _kernel___;
	typedef ui::ui___ _ui___;

	class kernel___
	: public _kernel___,
	  public _ui___
	{
	private:
		krow__ _KRow;
	protected:
		virtual void UIExposeKernel( void )
		{
			Expose();
		}
		virtual void UIRevokeKernel( void )
		{
			Revoke();
		}
		virtual void FRDBCKNDReport( const char *Message )
		{
			nsxpcm::Alert( Message );
		}
		virtual void FRDKERNLClose( void )
		{
			if ( _kernel___::IsProjectOpened() )
				SaveLocalRegistry();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_ui___::reset( P );
			_kernel___::reset( P );
			_KRow = NONE;
		}
		kernel___( void )
		{
			reset( false );
		}
		~kernel___( void )
		{
			reset();
		}
		void Init(
			krow__ KRow,
			const str::string_ &Language,
			const char *LocalesFileName )
		{
			reset();

			_kernel___::Init( Language, LocalesFileName );
			_ui___::Init( *this );
			_KRow = KRow;
		}
		void OpenProject( const char *ConfigFile )
		{
			_ui___::OpenProject( ConfigFile );
		}
		void Expose( void )
		{
			Repository.SetCurrentRow( _KRow );
		}
		void Revoke( void )
		{
			if ( Repository.GetCurrentRow() != _KRow )
				ERRc();

			Repository.DismissCurrentObject();
		}

	};
}

#endif