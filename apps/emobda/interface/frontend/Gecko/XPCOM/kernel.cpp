/*
	'kernel.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "kernel.h"
#include "flx.h"

using namespace kernel;

static void _DumpTableStructure(
	const bkdacc::string_ &Name,
	xml::writer_ &Writer )
{
	Writer.PushTag( "Name" );

	Writer.PutValue( Name );

	Writer.PopTag();
}

static void _DumpTablesStructure(
	const bkdacc::strings_ &Names,
	xml::writer_ &Writer )
{
	ctn::E_CMITEM( bkdacc::string_ ) Name;
	epeios::row__ Row = Names.First();

	Name.Init( Names );

	while ( Row != NONE ) {
		Writer.PushTag( "Table" );

		_DumpTableStructure( Name( Row ), Writer );

		Writer.PopTag();

		Row = Names.Next( Row );
	}
}

void kernel::kernel___::_DumpTablesStructure( xml::writer_ &Writer )
{
ERRProlog
	bkdacc::ids32 Rows;
	bkdacc::strings Names;
	bkdacc::ids16 Ids;
ERRBegin
	Rows.Init();
	Names.Init();
	Ids.Init();

	_H( Manager.GetTables( Rows, Names, Ids ) );

	if ( Rows.Amount() != Names.Amount() )
		ERRc();

	if ( Rows.Amount() != Ids.Amount() )
		ERRc();

	Writer.PushTag( "Tables" );

	::_DumpTablesStructure( Names, Writer );

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::_DumpDatabaseStructure( xml::writer_ &Writer )
{
ERRProlog
	bkdacc::string DatabaseName;
ERRBegin
	DatabaseName.Init();

	_H( Manager.GetDatabaseInfos( DatabaseName ) );

	Writer.PushTag( "Database" );

	Writer.PushTag( "Name" );

	Writer.PutValue( DatabaseName );

	_DumpTablesStructure( Writer );

	Writer.PopTag();

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::_DumpDatabaseStructureAsXML( str::string_ &XML )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow( Flow );
	xml::writer Writer;
ERRBegin
	Flow.Init( XML );
	Writer.Init( TFlow, false );

	_DumpDatabaseStructure( Writer );
ERRErr
ERREnd
ERREpilog
}


void kernel::kernel___::RefreshStructureView( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

	_DumpDatabaseStructureAsXML( XML );

	// XML.Append( "<Structure><Tables><Table Name='T1'><Fields><Field Name='T1 F1'/><Field Name='T1 F2'/></Fields></Table><Table Name='T2'><Fields><Field Name='T2 F1'/><Field Name='T2 F2'/><Field Name='T2 F3'/></Fields></Table></Tables></Structure>" );

	Parameters.Init();

	nsxpcm::RemoveChildren( UI.Structure.Items );

	nsxpcm::AppendChild( UI.Structure.Items, nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/StructureView.xsl" ), UI.Structure.Document, Parameters ) );

	UI.Structure.UpdateDecks();

	UI.Structure.Broadcasters.ItemEdition.Disable();
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::RefreshFieldList( void )
{
ERRProlog
	nsxpcm::xslt_parameters Parameters;
	str::string XML;
ERRBegin
	XML.Init();

//	_DumpFieldsAsXML( XML );

	Parameters.Init();

	nsxpcm::RemoveChildren( UI.Main.FieldListListbox.GetObject() );

	nsxpcm::AppendChild( UI.Main.FieldListListbox.GetObject(), nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/FieldList.xsl" ), UI.Main.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}
