/*
	'trunk.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef TRUNK__INC
# define TRUNK__INC

# include "dmoinf.h"

# include "frdkernl.h"
# include "xulftk.h"

# include "sclrgstry.h"

# include "registry.h"

# include "nsxpcm.h"

# include "ui.h"

# define NAME					DMOINF_LC_AFFIX	"gck"
# define VERSION				"0.1.0"
# define DESCRIPTION			"XULRunner component for '" DMOINF_MC_AFFIX "' software"
# define PROJECT_AFFILIATION	DMOINF_PROJECT_AFFILIATION
# define SOFTWARE_AFFILIATION	DMOINF_SOFTWARE_AFFILIATION
# define AUTHOR_NAME			DMOINF_AUTHOR_NAME
# define AUTHOR_CONTACT			DMOINF_AUTHOR_CONTACT
# define HELP					EPSMSC_HELP_INVITATION( NAME )
# define COPYRIGHT				DMOINF_COPYRIGHT
# define CVS_DETAILS			("$Id$\b " + 5)
# define WEBSITE_URL			DMOINF_WEBSITE_URL

# ifdef XXX_DBG
#  define DEBUG_FLAG	" (D)"
# else
#  define DEBUG_FLAG	""
# endif

namespace trunk {
	using namespace frdkernl;

	typedef xulftk::trunk___ _trunk__;

	class trunk___
	: public _trunk__
	{
	private:
		ui::ui___ _UI;
		frdkernl::kernel___		_Kernel;
		bso::bool__ _IsProjectModified( void ) const
		{
			/// To modify if there are other criterium for project modification.
			return Kernel().IsProjectModified();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_trunk__::reset( P );
			_UI.reset( P );
			_Kernel.reset( P );
		}
		E_CVDTOR( trunk___ );
		status__ Init(
			xulftk::user_callback__<trunk___>  &UserCallback,
			const rgstry::multi_level_registry_ &ConfigurationRegistry,
			const lcl::locale_ &Locale,
			const char *Language,
			geckoo::user_callback__ &Steering )
		{
			reset();

			_trunk__::Init( DMOINF_LC_AFFIX, "chrome://" DMOINF_LC_AFFIX "/content/xsl/", _UI, _Kernel, UserCallback, Steering, Locale, Language ),
			_UI.Init( *this );

			return _Kernel.Init( ConfigurationRegistry, NAME, Locale, Language, DefaultErrorReportingFunctions() );
		}
		void FinalizeDefineSession( void )
		{
			UI().Show( ui::pSessionForm );
		}
		void FinalizeApplySession( void )
		{
			UI().Show( ui::pSessionView );
		}
		void FinalizeCancelSession( void )
		{
			UI().Show( ui::pHome );
		}
		void FinalizeDropSession( bso::bool__ SavingProject )
		{
			if ( SavingProject ) {
					// Put here the code which saves the project.
			}

			UI().Show( ui::pHome );
		}
		void FinalizeExit( void )
		{
		}
		bso::bool__ IsProjectModified( void )
		{
			return _IsProjectModified();
		}
		bso::bool__ IsProjectInProgress( void )
		{
			return Kernel().IsProjectInProgress();
		}
		const frdkernl::kernel___ &Kernel( void ) const
		{
			return _Kernel;
		}
		frdkernl::kernel___ &Kernel( void )
		{
			return _Kernel;
		}
		const ui::ui___ &UI( void ) const
		{
			return _UI;
		}
		ui::ui___ &UI( void )
		{
			return _UI;
		}
		void GetDirectoryName( void );
	};

	typedef xulftk::user_callback__<trunk___> _user_callback__;

	class user_callback__
	: public _user_callback__
	{
	protected:

		virtual void XULFTKFormatedInformations(
			const char *LauncherIdentification,
			str::string_ &Informations );
		virtual void XULFTKSiteURL( str::string_ &URL )
		{
			URL = WEBSITE_URL;
		}
		virtual void XULFTKDefineSession(
			const str::string_ &ProjectFileName,
			const xpp::criterions___ &Criterions )
		{
			_DefineSession( ProjectFileName, Criterions );
			_T().FinalizeDefineSession();
		}
		virtual	void XULFTKApplySession( void )
		{
			_ApplySession( fblfrd::compatibility_informations__( DMOINF_LC_AFFIX, EDEMO_API_VERSION ) );
			_T().FinalizeApplySession();
		}
		virtual	void XULFTKCancelSession( void )
		{
			_CancelSession();
			_T().FinalizeCancelSession();
		}
		virtual bso::bool__ XULFTKDropSession( void )
		{
			if ( _T().IsProjectModified() )
				if ( _DefendSession() ) {
					_T().FinalizeDropSession( true );
					_DropSession();
					return true;
				}  else
					return false;
			else {
				_T().FinalizeDropSession( false );
				_DropSession();
				return true;
			}
		}
		virtual bso::bool__ XULFTKExit( void )
		{
			if ( _T().IsProjectInProgress() ) {
				if ( DropSession() ) {
					_T().FinalizeExit();
					_Exit();
					reset();
				}
			} else {
				_T().FinalizeExit();
				_Exit();
				reset();
			}

			return true;
		}
	};

	const str::string_ &GetFormattedVersionText(
		trunk___ &Trunk,
		str::string_ &Version );
}

#endif