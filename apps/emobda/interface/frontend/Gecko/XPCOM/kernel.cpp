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

#define CASE( m )	\
	case mi##m:\
		Message = #m;\
		break

static const char *GetRawMessage_( message_id__ MessageId )
{
	const char *Message = NULL;

	switch ( MessageId ) {
	default:
		ERRu();
		break;
	}

	return Message;
}

typedef msg::i18_messages_ _messages_;
typedef msg::i18_messages _messages;

static class messages
: public _messages
{
protected:
	const char *MSGGetRawMessage( int MessageId ) const
	{
		return GetRawMessage_( (message_id__)MessageId );
	}
} Messages_;

static const char *GetMessage_(
	message_id__ MessageId,
	lgg::language__ Language,
	msg::buffer__ &Buffer )
{
	return ::Messages_.GetMessage( MessageId, Language, Buffer );
}

const char *kernel::kernel___::GetMessage( message_id__ MessageId )
{
	return GetMessage_( MessageId, _Language, _MessageBuffer );
}

void kernel::kernel___::GetTableInfo(
	table__ Table,
	str::string_ &Name,
	str::string_ &Comment,
	table_id__ &Id )
{
ERRProlog
	tables Tables;
	bkdacc::strings Names, Comments;
	table_ids Ids;
ERRBegin
	Tables.Init();
	Tables.Append( Table );

	Names.Init();
	Comments.Init();
	Ids.Init();

	GetTablesInfos( Tables, Names, Comments, Ids );

	Name = Names( Names.First() );
	Comment = Comments( Comments.First() );
	Id = Ids( Ids.First() );
ERRErr
ERREnd
ERREpilog
}

static void DumpFieldStructure_(
	field__ Field,
	const bkdacc::string_ &Name,
	const bkdacc::string_ &Comment,
	field_id__ Id,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( "Field" );

	Writer.PutAttribute( "Row", bso::Convert( **Field, Buffer ) );
	Writer.PutAttribute( "Id", bso::Convert( **Id, Buffer ) );

	Writer.PushTag( "Name" );
	Writer.PutValue( Name );

	Writer.PushTag( "Comment" );
	Writer.PutValue( Comment );
}


static void DumpFieldsStructure_(
	const fields_ &Fields,
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
ERRProlog
	bkdacc::strings Names, Comments;
	field_ids Ids;
	epeios::row__ Row = NONE;
	bso::integer_buffer__ Buffer;
ERRBegin
	Writer.PushTag( "Fields" );
	Writer.PutAttribute( "Amount", bso::Convert( Fields.Amount(), Buffer ) );

	Names.Init();
	Comments.Init();
	Ids.Init();

	Kernel.GetFieldsInfos( Fields, Names, Comments, Ids );

	Row = Fields.First();

	while ( Row != NONE ) {
		DumpFieldStructure_( Fields( Row ), Names( Row ), Comments( Row ), Ids( Row ), Writer );

		Row = Fields.Next( Row );
	}

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}


static void DumpFieldsStructure_(
	table__ Table,
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
ERRProlog
	fields Fields;
ERRBegin	
	Fields.Init();

	Kernel.GetFields( Table, Fields );

	DumpFieldsStructure_( Fields, Kernel, Writer );
ERRErr
ERREnd
ERREpilog
}


static void DumpTableStructure_(
	const bkdacc::string_ &Name,
	const bkdacc::string_ &Comment,
	table__ Table,
	table_id__ Id,
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PutAttribute( "Id", bso::Convert( **Id, Buffer ) );
	Writer.PutAttribute( "Row", bso::Convert( **Table, Buffer ) );

	Writer.PushTag( "Name" );
	Writer.PutValue( Name );
	Writer.PopTag();

	Writer.PushTag( "Comment" );
	Writer.PutValue( Comment );
	Writer.PopTag();

	DumpFieldsStructure_( Table, Kernel, Writer );
}

static void DumpTablesStructure_(
	const bkdacc::strings_ &Names,
	const bkdacc::strings_ &Comments,
	const tables_ &Tables,
	const table_ids_ &Ids,
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
	ctn::E_CMITEM( bkdacc::string_ ) Name, Comment;
	epeios::row__ Row = Names.First();

	if ( Names.Amount() != Comments.Amount() )
		ERRc();

	if ( Names.Amount() != Ids.Amount() )
		ERRc();

	Name.Init( Names );
	Comment.Init( Comments );

	while ( Row != NONE ) {
		Writer.PushTag( "Table" );

		DumpTableStructure_( Name( Row ), Comment( Row ), Tables( Row ), Ids( Row ), Kernel, Writer );

		Writer.PopTag();

		Row = Names.Next( Row );
	}
}

static void DumpTablesStructure_(
	kernel___ &Kernel,
	xml::writer_ &Writer )
{
ERRProlog
	tables Tables;
	bkdacc::strings Names, Comments;
	table_ids Ids;
ERRBegin
	Tables.Init();

	Kernel.GetTables( Tables );

	Names.Init();
	Comments.Init();
	Ids.Init();

	Kernel.GetTablesInfos( Tables, Names, Comments, Ids );

	if ( Tables.Amount() != Names.Amount() )
		ERRc();

	if ( Tables.Amount() != Ids.Amount() )
		ERRc();

	Writer.PushTag( "Tables" );

	DumpTablesStructure_( Names, Comments, Tables, Ids, Kernel, Writer );

	Writer.PopTag();

ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::_DumpDatabaseStructure( xml::writer_ &Writer )
{
ERRProlog
	bkdacc::string Name, Comment;
ERRBegin
	Name.Init();
	Comment.Init();

	_H( Manager.GetDatabaseInfos( Name, Comment ) );

	if ( Name.Amount() != 0 ) {

		Writer.PushTag( "Database" );

		Writer.PushTag( "Name" );
		Writer.PutValue( Name );
		Writer.PopTag();

		Writer.PushTag( "Comment" );
		Writer.PutValue( Comment );
		Writer.PopTag();

		_DumpTablesStructure( Writer );

		Writer.PopTag();
	}
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::_DumpStructureAsXML( str::string_ &XML )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow( Flow );
	xml::writer Writer;
ERRBegin
	Flow.Init( XML );
	Writer.Init( TFlow, false );

	Writer.PushTag( "Structure" );

	_DumpDatabaseStructure( Writer );

	Writer.PopTag();
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

	_DumpStructureAsXML( XML );

	nsxpcm::Log( XML );

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




static class starter 
{
public:
	starter( void )
	{
		::Messages_.Init( mi_amount );
	}
	~starter( void )
	{}
} Starter_;