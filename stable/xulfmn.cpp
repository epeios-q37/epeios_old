/*
	'xulfmn' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'xulfmn' header file ('xulfmn.h').
	Copyright (C) 2004 Claude SIMON.

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

#define XULFMN__COMPILATION

#include "xulfmn.h"

class xulfmntutor
: public ttr_tutor
{
public:
	xulfmntutor( void )
	: ttr_tutor( XULFMN_NAME )
	{
#ifdef XULFMN_DBG
		Version = XULFMN_VERSION "\b\bD $";
#else
		Version = XULFMN_VERSION;
#endif
		Owner = XULFMN_OWNER;
		Date = "$Date$";
	}
	virtual ~xulfmntutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "xulftk.h"

#include "fnm.h"

using xulftk::trunk___;

using namespace xulfmn;

using nsxpcm::event__;

void xulfmn::main__::Update( void )
{
	Broadcasters.ProjectInProgress.Enable( Trunk().Kernel().IsProjectInProgress() );
}

#if 0
void xulfmn::window_eh__::NSXPCMOnEvent( event__ )
{
	Trunk().Exit();
	EventData().EventPreventDefault();	// Si l'application doit effectivement être fermée, ce sera fait par la fonctions précédente; inutile de poursuivre la prodédure de fermeture.
}
#endif

void xulfmn::new_project_eh__::NSXPCMOnEvent( event__ )
{
	Trunk().DefineSession( str::string(), xpp::criterions___() );
}

void xulfmn::user_project_eh__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Translation, FileName;
ERRBegin
	Translation.Init();
	FileName.Init();

	if ( nsxpcm::XPRJFileOpenDialogBox( Trunk().UI().Main().Window(), Trunk().Kernel().GetTranslation( xulfkl::GetLabel( xulfkl::mSelectProjectFile ), Translation ), Trunk().Kernel().Locale(), Trunk().Kernel().Language(), FileName ) )
		Trunk().DefineSession( FileName, xpp::criterions___() );
ERRErr
ERREnd
ERREpilog
}

void xulfmn::user_project_selection_eh__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Translation, FileName;
ERRBegin
	Translation.Init();
	FileName.Init();

	if ( nsxpcm::XPRJFileOpenDialogBox( Trunk().UI().Main().Window(), Trunk().Kernel().GetTranslation( xulfkl::GetLabel( xulfkl::mSelectProjectFile), Translation ), Trunk().Kernel().Locale(), Trunk().Kernel().Language(), FileName ) )
		Trunk().UI().Main().Widgets.txbUserProjectLocation.SetValue( FileName );
ERRErr
ERREnd
ERREpilog
}

void xulfmn::apply_eh__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string ProjectTypeLabel;
	str::string PredefinedProjectId;
	str::string ProjectLocation;
	str::string Log;
ERRBegin
	ProjectLocation.Init();

	ProjectTypeLabel.Init();

	switch ( frdkrn::GetProjectType( Trunk().UI().Main().Widgets.mnlProjectType.GetValue( ProjectTypeLabel ) ) ) {
	case frdkrn::ptNew:
		break;
	case frdkrn::ptPredefined:
		PredefinedProjectId.Init();
		Trunk().UI().Main().Widgets.mnlPredefinedProjectList.GetValue( PredefinedProjectId );

		frdkrn::GetPredefinedProjectLocation( PredefinedProjectId, Trunk().Kernel().Registry(), ProjectLocation );
		break;
	case frdkrn::ptUser:
		Trunk().UI().Main().Widgets.txbUserProjectLocation.GetValue( ProjectLocation );
		break;
	default:
		ERRFwk();
		break;
	}

	Log.Init( "ProjectLocation : \"" );
	Log.Append( ProjectLocation );
	Log.Append( '"' );

	Trunk().UI().LogQuietly( Log );

	Trunk().DefineSession( ProjectLocation, xpp::criterions___() );
ERRErr
ERREnd
ERREpilog
}

void xulfmn::close_project_eh__::NSXPCMOnEvent( event__ )
{
	Trunk().DropSession();
}

void xulfmn::project_type_selection_eh__::NSXPCMOnEvent( event__ )
{
ERRProlog
	str::string Value;
ERRBegin
	Value.Init();
	nsxpcm::GetAttribute( EventData().GetTarget(), "value", Value );

	switch ( frdkrn::GetProjectType( Value ) ) {
	case frdkrn::ptNew:
		Trunk().UI().Main().Widgets.dckProjectType.SetSelectedIndex( 0 );
		break;
	case frdkrn::ptPredefined:
		Trunk().UI().Main().Widgets.dckProjectType.SetSelectedIndex( 1 );
		break;
	case frdkrn::ptUser:
		Trunk().UI().Main().Widgets.dckProjectType.SetSelectedIndex( 2 );
		break;
	default:
		ERRFwk();
		break;
	}
ERRErr
ERREnd
ERREpilog
}


void xulfmn::exit_eh__::NSXPCMOnEvent( event__ )
{
	Trunk().Exit();
	EventData().EventPreventDefault();	// Si l'application doit effectivement être fermée, ce sera fait par la fonctions précédente; inutile de poursuivre la prodédure de fermeture.
}

void xulfmn::about_eh__::NSXPCMOnEvent( event__ )
{
	Trunk().About();
}

void xulfmn::web_site_eh__::NSXPCMOnEvent( event__ )
{
	Trunk().BrowseWEBSite();
}

void xulfmn::debug_eh__::NSXPCMOnEvent( event__ )
{
	Trunk().UI().CreateDebugDialog();
	Trunk().ExposeSteering();
	nsxpcm::OpenDialog( Trunk().UI().Main().Window(), "debug.xul", "_blank" );
	Trunk().UI().DeleteDebugDialog();
}

/* Registrations */

#define I( name )\
	name.Init( Trunk, nsxpcm::supports__( Trunk.UI().Main().Window(), "bdc" #name ) )

void xulfmn::broadcasters__::Init( trunk___ &Trunk )
{
	I( ProjectInProgress );
}

#undef I

static void InitAndRegister_(
	trunk___ &Trunk,
	xulfbs::event_handler__ &EventHandler,
	const char *Id )
{
	EventHandler.Init( Trunk );
	nsxpcm::AttachEventHandler( Trunk.UI().Main().Document(), Id, EventHandler );
}

#define IR( name )\
	InitAndRegister_( Trunk, name, "eh" #name );	

void xulfmn::event_handlers__::Init( trunk___ &Trunk )
{
	IR( NewProject );
	IR( UserProject );
	IR( CloseProject );
	IR( ProjectTypeSelection );
	IR( UserProjectSelection );
	IR( Apply );

	IR( Exit );
	Exit.Add( Trunk.UI().Main().Window(), nsxpcm::efClose );	// Remplace le 'xex:onclose="..."' inopérant sur la balise 'windonw'.

	IR( About );
	IR( WebSite );
	IR( Debug );
}

#undef IR

#define I( name )\
	name.Init( Trunk, nsxpcm::supports__( Trunk.UI().Main().Window(), #name ) );

static void GetPredefinedProjects_(
	const frdrgy::registry_ &Registry,
	const lcl::locale_ &Locale,
	const char *Language,
	str::string_ &PredefinedProjects )
{
ERRProlog
	flx::E_STRING_OFLOW___ OFlow;
	txf::text_oflow__ TFlow;
	xml::writer Writer;
	str::string Default;
ERRBegin
	OFlow.Init( PredefinedProjects );
	TFlow.Init( OFlow );
	Writer.Init( TFlow, xml::oIndent, xml::e_Default );

	Writer.PushTag( "PredefinedProjects" );

	Default.Init();
	Registry.GetValue( frdrgy::DefaultPredefinedProject, Default );

	if ( Default.Amount() != 0 )
		Writer.PutAttribute( "Default", Default );

	frdkrn::GetPredefinedProjects( Registry, Locale, Language, Writer );

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

static void FillWidget_(
	const str::string_ &XML,
	const char *DefaultXSLRootPath,
	nsIDOMNode *Node,
	const char *XSLFileNameAffix,
	nsIDOMDocument *Document )
{
ERRProlog
	nsIDOMDocumentFragment *Fragment;
	FNM_BUFFER___ FileNameBuffer;
ERRBegin
	nsxpcm::RemoveChildren( Node );

	Fragment = nsxpcm::XSLTransformByFileName( XML, str::string( fnm::BuildFileName( DefaultXSLRootPath, XSLFileNameAffix, ".xsl", FileNameBuffer ) ), Document, nsxpcm::xslt_parameters() );

	nsxpcm::AppendChild( Node, Fragment );
ERRErr
ERREnd
ERREpilog
}


static void FillPredefinedProjectsMenu_( trunk___ &Trunk )
{
ERRProlog
	str::string PredefinedProjects;
ERRBegin
	PredefinedProjects.Init();

	GetPredefinedProjects_( Trunk.Registry(),Trunk.Kernel().Locale(), Trunk.Kernel().Language(), PredefinedProjects );

	Trunk.UI().LogQuietly( PredefinedProjects );

	FillWidget_( PredefinedProjects, Trunk.DefaultXSLRootPath(), Trunk.UI().Main().Widgets.mnuPredefinedProject, "PredefinedProjectsMenu", Trunk.UI().Main().Document() );
	FillWidget_( PredefinedProjects, Trunk.DefaultXSLRootPath(), Trunk.UI().Main().Widgets.mnlPredefinedProjectList, "PredefinedProjectsMenuList", Trunk.UI().Main().Document() );

	nsxpcm::SetSelectedItem( Trunk.UI().Main().Widgets.mnlPredefinedProjectList );

	// La sélection de l'item de 'Trunk.UI().Main().Widgets.mnuPredefinedProject'  est réalisé directement en XSL.
ERRErr
ERREnd
ERREpilog
}

void xulfmn::widgets__::Init( trunk___ &Trunk )
{
	I( mnuPredefinedProject );
	I( dckMain );
	I( vewHome );
	I( dckProjectType );
	I( mnlPredefinedProjectList );
	I( txbUserProjectLocation );
	I( mnlProjectType );
	I( vewSessionForm );
	I( vewSessionView );

	FillPredefinedProjectsMenu_( Trunk );	// N'a à être fait qu'une seule fois.
}

#undef I

void xulfmn::main__::Init(
	xulftk::trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	window__::Init( Trunk, nsxpcm::supports__( Window, Window ) );
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class xulfmnpersonnalization
: public xulfmntutor
{
public:
	xulfmnpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~xulfmnpersonnalization( void )
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

static xulfmnpersonnalization Tutor;

ttr_tutor &XULFMNTutor = Tutor;
