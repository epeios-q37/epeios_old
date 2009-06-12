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
	kernel___ &Kernel )
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

			Register( Kernel, *Item, nsxpcm::QueryInterface<nsIDOMElement>( Node ), nsxpcm::efCommand );
		}
	}

ERRErr
ERREnd
ERREpilog
}

bso::bool__ ui::ui___::GetSelectedStructureItem( target__ &Target )
{
	bso::bool__ Selected = false;
ERRProlog
	str::string Type, Row, TableRow;
	field__ Field = UNDEFINED_FIELD;
	table__ Table = UNDEFINED_TABLE;
	ui_struct::browse_tree__ &Tree = Structure.BrowseTree;
ERRBegin

	if ( Tree.IsThereSelected() ) {
		Type.Init();
		Tree.GetCurrentItemAttribute( "Type", Type );

		if ( Type == "Field" ) {
			Row.Init();
			Tree.GetCurrentItemAttribute( "Row", Row );
			Field = mbdkernl::ExtractField( Row );

			TableRow.Init();
			Tree.GetCurrentItemAttribute( "TableRow", TableRow );
			Table = mbdkernl::ExtractTable( TableRow );
		} else if ( Type == "Table" ) {
			Row.Init();
			Tree.GetCurrentItemAttribute( "Row", Row );
			Table = mbdkernl::ExtractTable( Row );
		} else if ( Type != "Database" )
			ERRc();

		Selected = true;
	}
ERRErr
ERREnd
ERREpilog
	return Selected;
}

void ui::ui___::FillTableMenu( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	_K().DumpAsXML( XML );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	Main.TableMenu.RemoveChildren();

	Main.TableMenu.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/TableMenu.xsl" ), Structure.Document, Parameters ) );

	Register_( Main.TableMenu.GetElement(), _K() );
ERRErr
ERREnd
ERREpilog
}

void ui::ui___::UpdateDecks( void )
{
ERRProlog
	str::string Type;
	str::string Name, Comment;
	str::string Row;
	epeios::row__ Error = NONE;
ERRBegin
	Name.Init();
	Comment.Init();

	if ( Structure.BrowseTree.GetCurrentIndex() != -1 ) {

		Type.Init();
		Structure.BrowseTree.GetCurrentItemAttribute( "Type", Type );
		
		if ( Type == "Database" ) {
			if ( !_K().GetDatabaseInfos( Name, Comment ) )
				ERRReturn;
		} else if ( Type == "Table" ) {
			table_id__ TableId = UNDEFINED_TABLE_ID;

			Row.Init();
			Structure.BrowseTree.GetCurrentItemAttribute( "Row", Row );

			if ( !_K().GetTableInfos( mbdkernl::ExtractTable( Row ), Name, Comment, TableId ) )
				ERRReturn;
		} else if ( Type == "Field" ) {
			field_id__ FieldId = UNDEFINED_FIELD_ID;
			table__ Table = UNDEFINED_TABLE;
			
			Row.Init();
			Structure.BrowseTree.GetCurrentItemAttribute( "Row", Row );

			if ( !_K().GetFieldInfos( mbdkernl::ExtractField( Row ), Name, Comment, FieldId, Table ) )
				ERRReturn;
		}

		Structure.NameTextbox.SetValue( Name );

		Structure.CommentTextbox.SetValue( Comment );

		if ( Type == "Database" ) {
			Structure.Broadcasters.Database.Deletion.Enable();
			Structure.Broadcasters.Database.Modification.Enable();

			Structure.Broadcasters.Table.Creation.Enable();
			Structure.Broadcasters.Table.Modification.Disable();
			Structure.Broadcasters.Table.Creation.Disable();

			Structure.Broadcasters.Field.Creation.Disable();
			Structure.Broadcasters.Field.Modification.Disable();
			Structure.Broadcasters.Field.Creation.Disable();

			Structure.ActionDeck.SetSelectedPanel( Structure.DatabaseSelectionPanel );
		} else if ( Type == "Table" ) {
			Structure.Broadcasters.Database.Deletion.Disable();
			Structure.Broadcasters.Database.Modification.Disable();

			Structure.Broadcasters.Table.Creation.Enable();
			Structure.Broadcasters.Table.Modification.Enable();
			Structure.Broadcasters.Table.Creation.Enable();

			Structure.Broadcasters.Field.Creation.Enable();
			Structure.Broadcasters.Field.Modification.Disable();
			Structure.Broadcasters.Field.Deletion.Disable();

			Structure.ActionDeck.SetSelectedPanel( Structure.TableSelectionPanel );
		}else if ( Type == "Field" ) {
			Structure.Broadcasters.Database.Deletion.Disable();
			Structure.Broadcasters.Database.Modification.Disable();

			Structure.Broadcasters.Table.Creation.Disable();
			Structure.Broadcasters.Table.Modification.Disable();
			Structure.Broadcasters.Table.Creation.Disable();

			Structure.Broadcasters.Field.Creation.Enable();
			Structure.Broadcasters.Field.Modification.Enable();
			Structure.Broadcasters.Field.Deletion.Enable();

			Structure.ActionDeck.SetSelectedPanel( Structure.FieldSelectionPanel );
		} else
			ERRu();
	} else {
		Structure.ActionDeck.SetSelectedIndex( -1 );
	}

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

	_K().DumpAsXML( XML );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

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

	_K().DumpAsXML( XML );

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

	_K().DumpAsXML( XML );

	Parameters.Init();

	RecordView.RecordBox.RemoveChildren();

	RecordView.RecordBox.AppendChild( nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/RecordView.xsl" ), Structure.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}

static void SelectStructItem_(
	const str::string_ &Type,
	const str::string_ &Row,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
ERRProlog
	str::string RowBuffer, TypeBuffer;
	nsxpcm::browser__ Browser;
	nsIDOMNode *&Node = Root;
ERRBegin
	Browser.Init( Root );

	TypeBuffer.Init();
	RowBuffer.Init();

	while ( ( ( Node = Browser.GetNext() ) != NULL )
		&& ( ( nsxpcm::GetAttribute( Node, "Type", TypeBuffer ) != Type )
		      || ( nsxpcm::GetAttribute( Node, "Row", RowBuffer ) != Row ) ) ) {
		TypeBuffer.Init();
		RowBuffer.Init();
	}

	if ( Node == NULL )
		ERRc();

	Tree.Select( Node );
ERRErr
ERREnd
ERREpilog
}

static inline void SelectStructItem_(
	const char *Type,
	const char *Row,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	SelectStructItem_( str::string( Type ), str::string( Row ), Root, Tree );
}

static inline void SelectStructItem_(
	field__ Field,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	bso::integer_buffer__ Buffer;

	SelectStructItem_( "Field", bso::Convert( **Field, Buffer ), Root, Tree );
}

static inline void SelectStructItem_(
	table__ Table,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	bso::integer_buffer__ Buffer;

	SelectStructItem_( "Table", bso::Convert( **Table, Buffer ), Root, Tree );
}

static inline void SelectStructItem_(
	const target__ &Target,
	nsIDOMNode *Root,
	nsxpcm::tree__ &Tree )
{
	if ( Target.Field != UNDEFINED_FIELD )
		SelectStructItem_( Target.Field, Root, Tree );
	else  if ( Target.Table != UNDEFINED_TABLE )
		SelectStructItem_( Target.Table, Root, Tree );
}

void ui::ui___::FillStructureView( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
	target__ Target;
ERRBegin
	if ( _K().GetTransientContext() == mbdtrnsnt::cStructureManagement )
		Target = _K().StructureManagement().Target;
	else
		GetSelectedStructureItem( Target );

	XML.Init();

	_K().DumpAsXML( XML );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	nsxpcm::RemoveChildren( Structure.Items );	// Launch an event which reset '_Target'.

	nsxpcm::AppendChild( Structure.Items, nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/StructureView.xsl" ), Structure.Document, Parameters ) );

	UpdateDecks();

	Structure.Broadcasters.ItemEdition.Disable();

	SelectStructItem_( _K().StructureManagement().Target, Structure.Items, Structure.BrowseTree );	// Launch an event which set '_Target'.

	_K().ResetTransient();
ERRErr
ERREnd
ERREpilog
}

void ui::ui___::_SwitchTo( context__ Context )
{
	switch ( Context ) {
	case cSessionForm:
		FillTableMenu();
		Main.Broadcasters.DatabaseOpened.Disable();
		Main.MainDeck.SetSelectedPanel( Main.Panels.Home );
		break;
	case cStructureView:
		FillStructureView();
		FillTableMenu();
		Main.Broadcasters.DatabaseOpened.Enable();
		Structure.Broadcasters.ItemBrowsing.Enable();
		Structure.Broadcasters.ItemEdition.Disable();
		Main.MainDeck.SetSelectedPanel( Main.Panels.StructureFormAndView );
		break;
	case cStructureItemView:
		UpdateDecks();
		Structure.Broadcasters.ItemBrowsing.Enable();
		Structure.Broadcasters.ItemEdition.Disable();
		Main.MainDeck.SetSelectedPanel( Main.Panels.StructureFormAndView );
		break;
	case cStructureItemDefinition:
	{
		Structure.Broadcasters.ItemBrowsing.Disable();
		Structure.Broadcasters.ItemEdition.Enable();

		Structure.NameTextbox.Select();
		break;
	}
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
		Main.MainDeck.SetSelectedPanel( Main.Panels.RecordView );
		break;
	default:
		ERRc();
		break;
	}

	Main.Broadcasters.RecordSelected.Enable( _K().Target().Record != UNDEFINED_RECORD );
	Main.Broadcasters.TableWithFieldSelected.Enable( _K().Target().Table != UNDEFINED_TABLE );
}

