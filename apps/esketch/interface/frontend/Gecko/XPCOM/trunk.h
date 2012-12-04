/*
	'trunk.h' by Claude SIMON (http://zeusw.org/).

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

#ifndef TRUNK__INC
# define TRUNK__INC


# include "xulftk.h"
# include "nsxpcm.h"

# include "sclrgstry.h"

# include "sktinf.h"
# include "frdkernl.h"
# include "ui.h"

# define APP_NAME					SKTINF_LC_AFFIX	"gck"
# define VERSION				"0.1.0"
# define DESCRIPTION			"XULRunner component for '" SKTINF_MC_AFFIX "' software"
# define PROJECT_AFFILIATION	SKTINF_PROJECT_AFFILIATION
# define SOFTWARE_AFFILIATION	SKTINF_SOFTWARE_AFFILIATION
# define AUTHOR_NAME			SKTINF_AUTHOR_NAME
# define AUTHOR_CONTACT			SKTINF_AUTHOR_CONTACT
# define HELP					EPSMSC_HELP_INVITATION( NAME )
# define COPYRIGHT				SKTINF_COPYRIGHT
# define CVS_DETAILS			("$Id$\b " + 5)
# define WEBSITE_URL			SKTINF_WEBSITE_URL

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
		frdkernl::kernel___ _Kernel;
		bso::bool__ _IsProjectModified( void ) const
		{
			/// To modify if there are other criterium for project modification.
			return Kernel().IsProjectModified();
		}
		void TestLocaleInProjectFile_( void );
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
			const rgstry::registry_ &ConfigurationRegistry,
			rgstry::row__ ConfigurationRegistryRoot,
			const lcl::locale_ &Locale,
			const char *Language,
			geckoo::steering_callback__ &Steering )
		{
			reset();

			_trunk__::Init( SKTINF_LC_AFFIX, "chrome://" SKTINF_LC_AFFIX "/content/xsl/", _UI, _Kernel, UserCallback, Steering ),
			_UI.Init( *this );

			return _Kernel.Init( ConfigurationRegistry, ConfigurationRegistryRoot, APP_NAME, Locale, Language, DefaultReportingFunctions() );
		}
		void FinalizeGoToHome( void )
		{
			UI().Show( ui::dHome );
		}
		void FinalizeDefineSession( void )
		{
			UI().Show( ui::dSessionForm );
			TestLocaleInProjectFile_();
		}
		void FinalizeApplySession( void )
		{
			UI().Show( ui::dSessionView );
		}
		void FinalizeCancelSession( void )
		{
			UI().Show( ui::dHome );
		}
		void FinalizeDropSession( bso::bool__ SavingProject )
		{
			if ( SavingProject ) {
					// Put here the code which saves the project.
			}

			UI().Show( ui::dHome );
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
	};

	typedef xulftk::user_callback__<trunk___> _user_callback__;

	class user_callback__
	: public _user_callback__
	{
	protected:
		virtual void XULFTKAboutText(
			const char *LauncherIdentification,
			const char *BuildInformations,
			str::string_ &Text );
		virtual void XULFTKSiteURL( str::string_ &URL )
		{
			URL = WEBSITE_URL;
		}
		virtual void XULFTKGoToHome( void )
		{
			_GoToHome();
			_T().FinalizeGoToHome();
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
			_ApplySession( fblfrd::compatibility_informations__( SKTINF_LC_AFFIX, ESKETCH_API_VERSION ) );
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
}

#endif