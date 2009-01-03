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

static void DumpFields_(
	const bkdacc::items32_ &Items,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;
	ctn::E_CMITEM( bkdacc::item32_ ) Item;
	epeios::row__ Row = Items.First();

	Item.Init( Items );

	while ( Row != NONE ) {
		Writer.PushTag( "Field" );
		Writer.PutAttribute( "Id", bso::Convert( *Item( Row ).ID(), Buffer ) );

		Writer.PushTag( "Name" );
		Writer.PutValue( Item( Row ).Value );
		Writer.PopTag();

		Writer.PopTag();

		Row = Items.Next( Row );
	}
}

void kernel::kernel___::_DumpFields( xml::writer_ &Writer )
{
ERRProlog
	bkdacc::items32 Fields;
ERRBegin
	Fields.Init();

	_H( Manager.GetFields( Fields ) );

	Writer.PushTag( "Fields" );

	DumpFields_( Fields, Writer );

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}

void kernel::kernel___::_DumpFieldsAsXML( str::string_ &XML )
{
ERRProlog
	flx::E_STRING_OFLOW___ Flow;
	txf::text_oflow__ TFlow( Flow );
	xml::writer Writer;
ERRBegin
	Flow.Init( XML );

	Writer.Init( TFlow, false );

	_DumpFields( Writer );

	TFlow.Synchronize();
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

	_DumpFieldsAsXML( XML );

	Parameters.Init();

	nsxpcm::RemoveChildren( UI.Main.FieldListListbox.GetObject() );

	nsxpcm::AppendChild( UI.Main.FieldListListbox.GetObject(), nsxpcm::XSLTTransform( XML, str::string( "chrome://emobda/content/FieldList.xsl" ), UI.Main.Document, Parameters ) );
ERRErr
ERREnd
ERREpilog
}
