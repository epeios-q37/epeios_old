/*
	'ui.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "ui.h"

using namespace ui;

static void Register_(
	nsIDOMNode *Node,
	ui_base::bridge_functions__ &Functions )
{
ERRProlog
	ui_main::table_menu_item__ *Item = NULL;
	nsxpcm::browser__ Browser;
	str::string Row;
ERRBegin 
	Browser.Init( Node );

	while ( ( Node = Browser.GetNext() ) != NULL ) {
		Row.Init();

		nsxpcm::GetAttribute( Node, "Row", Row );

		if ( Row.Amount() ) {
			Item = new ui_main::table_menu_item__;

			Register( Functions, *Item, nsxpcm::QueryInterface<nsIDOMElement>( Node ), nsxpcm::efCommand );
		}
	}

ERRErr
ERREnd
ERREpilog
}

void ui::ui___::FillTableMenu( bso::bool__ ContextIsStandard )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	_K().DumpAsXML( XML, ContextIsStandard );

	nsxpcm::Log( XML );

	Parameters.Init();

	Main.TableMenu.RemoveChildren();

	Main.TableMenu.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/TableMenu.xsl" ), Structure.Document, Parameters ) );

	Register_( Main.TableMenu.GetElement(), *this );
ERRErr
ERREnd
ERREpilog
}

void ui::ui___::FillListView( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	_K().DumpAsXML( XML, true );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

//	nsxpcm::Log( XML );

	Parameters.Init();

	ListView.ContentTree.RemoveChildren();

	ListView.ContentTree.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/ListView.xsl" ), Structure.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}

void ui::ui___::FillRecordForm( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	_K().DumpAsXML( XML, true );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	RecordForm.RecordBox.RemoveChildren();

	RecordForm.RecordBox.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/RecordForm.xsl" ), Structure.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}

void ui::ui___::FillRecordView( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	_K().DumpAsXML( XML, true );

	Parameters.Init();

	RecordView.RecordBox.RemoveChildren();

	RecordView.RecordBox.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/RecordView.xsl" ), Structure.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}

void ui::ui___::_SwitchTo( context__ Context )
{
	switch ( Context ) {
	case cSessionForm:
		FillTableMenu( true );
		Main.Broadcasters.Connected.Disable();
		Main.Broadcasters.DatabaseOpened.Disable();
		Main.Broadcasters.RecordSelected.Disable();
		Main.Broadcasters.TableWithFieldSelected.Disable();
		Main.MainDeck.SetSelectedPanel( Main.Panels.Home );
		break;
	case cSessionView:
		FillTableMenu( true );
		Main.Broadcasters.ProjectOpened.Enable();
		Main.Broadcasters.Connected.Enable();
		Main.Broadcasters.DatabaseOpened.Disable();
		Main.Broadcasters.RecordSelected.Disable();
		Main.Broadcasters.TableWithFieldSelected.Disable();
		Main.MainDeck.SetSelectedPanel( Main.Panels.Home );
		break;
	case cStructureView:
		Structure.FillView();
		FillTableMenu( false );
		Main.Broadcasters.DatabaseOpened.Enable();
		Structure.Broadcasters.ItemBrowsing.Enable();
		Structure.Broadcasters.ItemEdition.Disable();
		Main.MainDeck.SetSelectedPanel( Main.Panels.StructureFormAndView );
		break;
	case cStructureItemView:
		Structure.UpdateDecks();
		Structure.Broadcasters.ItemBrowsing.Enable();
		Structure.Broadcasters.ItemEdition.Disable();
		Main.MainDeck.SetSelectedPanel( Main.Panels.StructureFormAndView );
		break;
	case cListView:
		FillListView();
		Main.Broadcasters.RecordSelected.Disable();
		Main.MainDeck.SetSelectedPanel( Main.Panels.ListView );
		ListView.ContentTree.ClearSelection();
		break;
	case cRecordForm:
		FillRecordForm();
		Main.MainDeck.SetSelectedPanel( Main.Panels.RecordForm );
		break;
	case cRecordView:
		FillRecordView();

		UI().RecordView.UpdateInterface( K().Target().RecordPosition, K().GetRecordsAmount() );

		Main.MainDeck.SetSelectedPanel( Main.Panels.RecordView );
		break;
	default:
		ERRc();
		break;
	}

	Main.Broadcasters.RecordSelected.Enable( _K().Target().Record != UNDEFINED_RECORD );
	Main.Broadcasters.TableWithFieldSelected.Enable( _K().Target().Table != UNDEFINED_TABLE );
}

bso::bool__ ui::ui___::GetDatabaseIdentification ( void )
{
	bso::bool__ Validated = false;
ERRProlog
	str::string Name;
	str::string Path;
	str::string Comment;
ERRBegin
	Name.Init();
	Path.Init();
	Comment.Init();

	if ( Validated = GetDatabaseIdentification( Name, Path, Comment ) )
		K().DatabaseIdentification().Set( Name, Path, Comment );
ERRErr
ERREnd
ERREpilog
	return Validated;
}

bso::bool__ ui::ui___::GetSelectedDatabase( void )
{
	bso::bool__ Validated = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init();

	if ( Validated = GetSelectedDatabase( Path ) )
		K().DatabaseSelection().Set( Path );
ERRErr
ERREnd
ERREpilog
	return Validated;
}

void ui::ui___::SelectRecord( void )
{
ERRProlog
	str::string Value;
	record__ Record = UNDEFINED_RECORD;
	record_position__ RecordPosition = UNDEFINED_RECORD_POSITION;
	ui_lst_v::content_tree__ &Tree = UI().ListView.ContentTree;
ERRBegin

	if ( Tree.IsThereSelected() ) {
		Value.Init();
		Tree.GetCurrentItemAttribute( "Record", Value );
		Record = mbdkernl::ExtractRecord( Value );

		Value.Init();
		Tree.GetCurrentItemAttribute( "RecordPosition", Value );
		RecordPosition = mbdkernl::ExtractRecordPosition( Value );
	}

	K().Target().Set( Record, RecordPosition );

	Main.Broadcasters.RecordSelected.Enable();
ERRErr
ERREnd
ERREpilog
}

static void RetrieveData_(
	nsIDOMNode *Node,
	bkdacc::items32_ &Data )
{
ERRProlog
	nsxpcm::browser__ Browser;
	str::string Value;
	epeios::row__ Error = NONE;
	bkdacc::id32__ Id = BKDACC_UNDEFINED_ID32;
	bkdacc::item32 Item;
ERRBegin
	Browser.Init( Node );

	while ( ( Node = Browser.GetNext() ) != NULL ) {
		Value.Init();

		nsxpcm::GetAttribute( Node, "Field", Value, err::hSkip );

		if ( Value.Amount() != 0 ) {
			Id = Value.ToUL( &Error );

			if ( Error != NONE )
				ERRu();

			Value.Init();
			nsxpcm::GetValue( nsxpcm::QueryInterface<nsIDOMXULTextBoxElement>( Node ), Value );

			if ( Value.Amount() != 0 ) {
				Item.Init( Id, Value );
	
				Data.Append( Item );
			}
		}
	}
ERRErr
ERREnd
ERREpilog
}

void ui::ui___::ApplyRecord( void )
{
ERRProlog
	bkdacc::items32 Data;
ERRBegin
	Data.Init();
	RetrieveData_( RecordForm.RecordBox.GetObject(), Data );

	switch ( K().RecordInput().GetState() ) {
		case mbdtrnsnt::risCreation:
		case mbdtrnsnt::risDuplication:
			K().InsertRecord( Data );
			break;
		case mbdtrnsnt::risModification:
			K().ModifyRecord( Data );
			break;
		default:
			ERRc();
			break;
	}

	K().ResetTransient();

	_SwitchTo( cRecordView );
ERRErr
ERREnd
ERREpilog
}

void ui::ui___::ApplyStructureItem( void )
{
	if ( K().Target().Table == UNDEFINED_TABLE ) {
		if ( K().StructureManagement().GetState() != mbdtrnsnt::smsCreation )
			ERRc();

		K().Target().Set( CreateOrModifyTable() );
	} else if ( K().Target().Field == UNDEFINED_FIELD ) {
		switch ( K().StructureManagement().GetState() ) {
		case mbdtrnsnt::smsCreation:
			K().Target().Set( CreateOrModifyField() );
			break;
		case mbdtrnsnt::smsModification:
			K().Target().Set( CreateOrModifyTable() );
			break;
		case mbdtrnsnt::smsDuplication:
			ERRc();
			break;
		default:
			ERRc();
			break;
		}
	} else {
		if ( K().StructureManagement().GetState() != mbdtrnsnt::smsModification )
			ERRc();

		K().Target().Set( CreateOrModifyField() );
	}

	_SwitchTo( cStructureView );
}

void ui::ui___::SelectTable( ui_main::table_menu_item__ &MenuItem )
{
ERRProlog
	str::string Row;
	epeios::row__ Error = NONE;
	table__ Table;
ERRBegin
	Row.Init();
	nsxpcm::GetAttribute( MenuItem.GetElement(), "Row", Row );

	*Table = Row.ToUL( &Error );

	if ( Error != NONE )
		ERRc();

	SelectTable( Table );
ERRErr
ERREnd
ERREpilog
}

void ui::ui___::FillDatabaseSelectionList( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	K().DumpAsXML( XML, false );

	Parameters.Init();

	DatabaseSelection.DatabaseTree.RemoveChildren();

	DatabaseSelection.DatabaseTree.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/DatabaseSelectionList.xsl" ), Structure.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}
