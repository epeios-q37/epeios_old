/*
	'mscrmi' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'mscrmi' header file ('mscrmi.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#define MSCRMI__COMPILATION

#include "mscrmi.h"

class mscrmitutor
: public ttr_tutor
{
public:
	mscrmitutor( void )
	: ttr_tutor( MSCRMI_NAME )
	{
#ifdef MSCRMI_DBG
		Version = MSCRMI_VERSION "\b\bD $";
#else
		Version = MSCRMI_VERSION;
#endif
		Owner = MSCRMI_OWNER;
		Date = "$Date$";
	}
	virtual ~mscrmitutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "mscmdm.h"

using namespace mscrmi;

using xml::browser___;

// Encodage pour flux MIDI.
static inline bso::ulong__ Encode_( bso::ulong__ Value )
{
	return ( Value & 0x7f ) | ( ( Value & 0x3f80) << 1 ) | ( ( Value & 0x1fc000 ) << 2 ) | ( ( Value & 0xfe00000 ) << 3 );
}

// Decodage d'un flux MIDI.
static inline bso::ulong__ Decode_( bso::ulong__ Value )
{
	return ( Value & 0x7f ) + ( ( Value & 0x7f00 ) >> 1 ) + ( ( Value & 0x7f0000 ) >> 2 ) + ( ( Value & 0x7f000000 ) >> 3 );
}

static inline xaddress__ Sum_(
	xaddress__ Address,
	size__ Size )
{
	bso::ubyte__ StencilSize = mscrmi::_StencilSize( Address );

	Address = mscrmi::_Address( Address );

	if ( ( Address & 0x80808080 ) != 0 )
		ERRc();

	Address = Decode_( Address ) + Size;

	Address = Encode_( Address );

	Address = mscrmi::_SetStencilSize( Address, StencilSize );

	return Address;
}

void mscrmi::GetBlocs(
	const parameter_definitions_ &Definitions,
	blocs_ &Blocs )
{
	bloc__ Bloc;
	row__ Row = Definitions.First();
	ctn::E_CMITEMt( parameter_definition_, row__ ) Definition;

	Definition.Init( Definitions );

	if ( Row != NONE ) {
		while ( ( Row != NONE ) && ( Definition( Row ).Size() == 0 ) )
			Row = Definitions.Next( Row );

		if ( Row != NONE ) {
			Bloc.Address = Definition( Row ).Address();
			Bloc.Size = Definition( Row ).Size();

			Row = Definitions.Next( Row );
		}
	}


	while ( Row != NONE ) {
		while ( ( Row != NONE ) && ( Definition( Row ).Size() == 0 ) )
			Row = Definitions.Next( Row );

		if ( Row != NONE ) {
			if ( Sum_( Bloc.Address, Bloc.Size ) == Definition( Row ).Address() )
				Bloc.Size += Definition( Row ).Size();
			else {
				Blocs.Append( Bloc );
				Bloc.Address = Definition( Row ).Address();
				Bloc.Size = Definition( Row ).Size();
			}
		}

		Row = Definitions.Next( Row );
	}

	Blocs.Append( Bloc );
}


static void PrintPath_(
	row__ Row,
	const parameter_definitions_ &Definitions,
	txf::text_oflow__ &Flow )
{
ERRProlog
	str::string Path;
	ctn::E_CMITEMt( parameter_definition_, row__ ) Definition;
ERRBegin
	Definition.Init( Definitions );
	Path.Init();

	Row = Definition( Row ).GroupRow();

	while ( Row != NONE ) {
		Path.Insert( '/', 0 );
		Path.Insert( Definition( Row ).Label, 0 );

		Row = Definition( Row ).GroupRow();
	}

	Flow << Path;

ERRErr
ERREnd
ERREpilog
}

static inline void Print_(
	const parameter_definition_ &Definition,
	xml::writer_ &Writer )
{
	char Address[] = "12345678";
	bso::integer_buffer__ Buffer;

	Writer.PutAttribute( "Label", Definition.Label );

	sprintf( Address, "%08X", Definition.Address() );
	Writer.PutAttribute( "Address", Address );

	sprintf( Address, "%0*X", 2 * ( 4 - _StencilSize( Definition.S_.Address ) ), Definition.Offset() );
	Writer.PutAttribute( "Offset", Address );

	if ( Definition.Size() != 0 )
		Writer.PutAttribute( "Size", bso::Convert( Definition.Size(), Buffer ) );
}

typedef stk::E_BSTACK_( row__ ) srows_;
E_AUTO( srows );

void mscrmi::Print(
	const parameter_definitions_ &Definitions,
	xml::writer_ &Writer )
{
ERRProlog
	srows Rows;
	row__ Row = NONE;
	row__ ParentRow = NONE;
	ctn::E_CMITEMt( parameter_definition_, row__ ) Definition;
	Definition.Init( Definitions );
ERRBegin
	Definition.Init( Definitions );
	Row = Definitions.First();

	Rows.Init();

	while ( Row != NONE ) {

		while ( Definition( Row ).GroupRow() != ParentRow ) {
			Writer.PopTag();
			ParentRow = Rows.Pop();
		}

		if ( Definition( Row ).Size() == 0 ) {
			Rows.Push( ParentRow );
			ParentRow = Row;
			Writer.PushTag( "Parameters" );
		} else
			Writer.PushTag( "Parameter" );

		Print_( Definition( Row ), Writer );

		if ( Definition( Row ).Size() != 0 )
			Writer.PopTag();

		Row = Definitions.Next( Row );
	}

	while ( Rows.Amount() != 0 ) {
		Rows.Pop();
		Writer.PopTag();
	}
ERRErr
ERREnd
ERREpilog
}

#define CASE( label )\
	case fs##label:\
	return #label;\
	break;


const char *mscrmi::Label( fill_status__ Status )
{
	switch ( Status ) {
		CASE( BrowserError )
		CASE( UnexpectedTag )
		CASE( AttributeDefinedTwice )
		CASE( UnexpectedAttribute )
		CASE( MissingAttribute )
		CASE( BadValue )
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.

}

static bso::bool__ ConvertModelID_(
	const str::string_ &RawModelID,
	str::string_ &ModelID )
{
	bso::size__ Amount = RawModelID.Amount();
	bso::ulong__ Value;
	epeios::row__ Error = NONE;

	if ( Amount == 0 )
		return false;

	if ( Amount & 1 )
		return false;

	if ( Amount > 8 )
		return false;

	Value = RawModelID.ToUL( &Error, str::b16 );

	if ( Error != NONE )
		return false;

	switch ( Amount >> 1 ) {
	case 4:
		ModelID.Append( (bso::ubyte__)( ( Value & 0xff000000 ) >> 24 ) );
	case 3:
		ModelID.Append( (bso::ubyte__)( ( Value & 0xff0000 ) >> 16 ) );
	case 2:
		ModelID.Append( (bso::ubyte__)( ( Value & 0xff00 ) >> 8 ) );
	case 1:
		ModelID.Append( (bso::ubyte__)( Value & 0xff ) );
		break;
	default:
		ERRc();
		break;
	}

	return true;
}


static fill_status__ BrowseImplementationSpecifications_(
	browser___ &Browser,
	midi_implementation_ &Implementation )
{
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfAttribute | xml::tfStartTagClosed ) ) {
		case xml::tAttribute:
			if ( Browser.AttributeName() == "ModelID" ) {
				if ( Implementation.ModelID.Amount() != 0 )
					return fsAttributeDefinedTwice;
				if ( !ConvertModelID_( Browser.Value(), Implementation.ModelID ) )
					return fsBadValue;
			} else if ( Browser.AttributeName() == "ModelLabel" ) {
				if ( Implementation.ModelLabel.Amount() != 0 )
					return fsAttributeDefinedTwice;
				Implementation.ModelLabel = Browser.Value();
			} else
				return fsUnexpectedAttribute;
			break;
		case xml::tStartTagClosed:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			return fsBrowserError;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( ( Implementation.ModelID.Amount() == 0 ) || ( Implementation.ModelLabel.Amount() == 0 ) )
		return fsMissingAttribute;

	return fsOK;
}

static xaddress__ HandleAddress_(
	xaddress__ Address,
	const str::string_ &Value )
{
	epeios::row__ Error = NONE;
	address__ Long = Value.ToUL( &Error, str::b16 );
	bso::size__ Amount = Value.Amount();
	bso::ubyte__ StencilSize = 0;

	if ( Error != NONE )
		return MSCRMI_UNDEFINED_ADDRESS;

	if ( Amount == 0 )
		return MSCRMI_UNDEFINED_ADDRESS;

	if ( Amount > 8 )
		return MSCRMI_UNDEFINED_ADDRESS;

	if ( Amount & 1 )	// 'Amount' doit être pair.
		return MSCRMI_UNDEFINED_ADDRESS;

	StencilSize = (bso::ubyte__)( 4 - ( Amount >> 1 ) );

#ifdef MSCRMI_DBG
	if ( Long > ~_Stencil( StencilSize ) )
		ERRc();
#endif

	return _SetStencilSize( ( _Address( Address ) & _Stencil( StencilSize ) ) | Long, StencilSize );
}


typedef ctn::E_XMCONTAINER_( str::string_ ) labels_;
E_AUTO( labels );

static fill_status__ BrowseParametersSpecifications_(
	browser___ &Browser,
	xaddress__ &Address,
	row__ &GroupRow,
	midi_implementation_ &Implementation )
{
	fill_status__ Status = fs_Undefined;
ERRProlog
	bso::bool__ NoLabel = true;
	bso::bool__ Continue = true;
	parameter_definition Definition;
	label Label;
ERRBegin
	while ( Continue ) {
		switch ( Browser.Browse( xml::tfAttribute | xml::tfStartTagClosed ) ) {
		case xml::tAttribute:
			if ( Browser.AttributeName() == "Address" ) {
				if ( ( Address = HandleAddress_( Address, Browser.Value() ) ) == MSCRMI_UNDEFINED_ADDRESS ) {
					Status = fsBadValue;
					ERRReturn;
				}
			} else if ( Browser.AttributeName() == "Label" ) {
				if ( Browser.Value().Amount() == 0 ) {
					Status = fsBadValue;
					ERRReturn;
				}

				NoLabel = false;

				Label.Init( Browser.Value() );
			} else {
				Status = fsUnexpectedAttribute;
				ERRReturn;
			}
			break;
		case xml::tStartTagClosed:
			Continue = false;
			break;
		case xml::tProcessed:
			break;
		case xml::tError:
			Status = fsBrowserError;
			ERRReturn;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( NoLabel ) {
		Status = fsMissingAttribute;
		ERRReturn;
	}

	Status = fsOK;

	Definition.Init();

	Definition.Init( Label, Address, 0, GroupRow );

	GroupRow = Implementation.Definitions.Add( Definition );
ERRErr
ERREnd
ERREpilog
	return Status;
}

static fill_status__ BrowseParameter_(
	browser___ &Browser,
	xaddress__ &Address,
	size__ &Size,
	row__ &GroupRow,
	parameter_definition_ &Definition )
{
	fill_status__ Status = fs_Undefined;
ERRProlog
	bso::bool__ Continue = true;
	label Label;
ERRBegin
	Label.Init();

	while ( Continue ) {
		switch ( Browser.Browse( xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tAttribute:
			if ( Browser.AttributeName() == "Label" ) {
				if ( Label.Amount() != 0 ) {
					Status = fsAttributeDefinedTwice;
					ERRReturn;
				}

				Label = Browser.Value();

				if ( Label.Amount() == 0 ) {
					Status = fsBadValue;
					ERRReturn;
				}
			} else if ( Browser.AttributeName() == "Address" ) {
				if ( ( Address = HandleAddress_( Address, Browser.Value() ) ) == MSCRMI_UNDEFINED_ADDRESS )
					return fsBadValue;
			} else if ( Browser.AttributeName() == "Size" ) {
				  epeios::row__ Error = NONE;

				  Size = Browser.Value().ToUB( &Error );

				  if ( Error != NONE ) {
					  Status = fsBadValue;
					  ERRReturn;
				  }

				  if ( Size == 0 ) {
					  Status = fsBadValue;
					  ERRReturn;
				  }
			} else {
				Status = fsUnexpectedAttribute;
				ERRReturn;
			}
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			return fsBrowserError;
			break;
		default:
			ERRc();
			break;
		}
	}

	if( Label.Amount() == 0 ) {
		Status = fsMissingAttribute;
		ERRReturn;
	}

	Status = fsOK;

	Definition.Init( Label, Address, Size, GroupRow );

	Address = Sum_( Address, Size );

ERRErr
ERREnd
ERREpilog
	return Status;

}

static fill_status__ BrowseParameters_(
	browser___ &Browser,
	xaddress__ &Address,
	size__ &Size,
	row__ &GroupRow,
	midi_implementation_ &Implementation )
{
	fill_status__ Status = fs_Undefined;
ERRProlog
	bso::bool__ Continue = true;
	parameter_definition Definition;
ERRBegin
	while ( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == "Parameters" ) {
				if ( ( Status = BrowseParametersSpecifications_( Browser, Address, GroupRow, Implementation ) ) != fsOK )
					ERRReturn;

				if ( ( Status = BrowseParameters_( Browser, Address, Size, GroupRow, Implementation ) ) != fsOK )
					ERRReturn;

				if ( ( GroupRow = Implementation.Definitions( GroupRow ).GroupRow() ) == NONE )
					ERRc();

				Implementation.Definitions.Flush();
			} else if ( Browser.TagName() == "Parameter" ) {
				Definition.Init();
				if ( ( Status = BrowseParameter_( Browser, Address, Size, GroupRow, Definition ) ) != fsOK )
					ERRReturn;

				Implementation.Definitions.Add( Definition );
			} else {
				Status = fsUnexpectedTag;
				ERRReturn;
			}

			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			return fsBrowserError;
			break;
		default:
			ERRc();
			break;
		}
	}

	Status = fsOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

static fill_status__ BrowseImplementation_( 
	browser___ &Browser,
	midi_implementation_ &Implementation )
{
	fill_status__ Status = fs_Undefined;
ERRProlog
	xaddress__ Address = 0;
	row__ GroupRow = NONE;
	bso::bool__ Continue = true;
	size__ Size = 1;
ERRBegin
	while( Continue ) {
		switch ( Browser.Browse( xml::tfStartTag | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == "Parameters" ) {
				if ( ( Status = BrowseParametersSpecifications_( Browser, Address, GroupRow, Implementation ) ) != fsOK )
					ERRReturn;

				if ( ( Status = BrowseParameters_( Browser, Address, Size, GroupRow, Implementation ) ) != fsOK )
					ERRReturn;

				if ( ( GroupRow = Implementation.Definitions( GroupRow ).GroupRow() ) != NONE )
					ERRc();

				Implementation.Definitions.Flush();
			} else {
				Status = fsUnexpectedTag;
				ERRReturn;
			}

			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tError:
			return fsBrowserError;
			break;
		default:
			ERRc();
			break;
		}
	}

	Status = fsOK;

ERRErr
ERREnd
ERREpilog
	return Status;
}

fill_status__ mscrmi::Fill(
	browser___ &Browser,
	midi_implementation_ &Implementation )
{
	fill_status__ Status = fs_Undefined;
ERRProlog
ERRBegin
	switch ( Browser.Browse( xml::tfStartTag ) ) {
	case xml::tStartTag:
		if ( Browser.TagName() != "MIDIImplementation" ) {
			Status = fsUnexpectedTag;
			ERRReturn;
		}

		if ( ( Status = BrowseImplementationSpecifications_( Browser, Implementation ) ) != fsOK )
			ERRReturn;

		if ( ( Status = BrowseImplementation_( Browser, Implementation ) ) != fsOK )
			ERRReturn;

	case xml::tProcessed:
		break;
	case xml::tError:
		Status = fsBrowserError;
		ERRReturn;
	default:
		ERRc();
		break;
	}

ERRErr
ERREnd
ERREpilog
	return Status;
}

static fill_status__ BrowseImplementationsSpecifications_(
	browser___ &Browser,
	midi_implementations_ &Implementations )
{
	switch ( Browser.Browse( xml::tfStartTagClosed ) ) {
	case xml::tStartTagClosed:
		break;
	case xml::tProcessed:
		ERRc();
		break;
	case xml::tError:
		return fsBrowserError;
		ERRReturn;
	default:
		ERRc();
		break;
	}

	return fsOK;
}

static fill_status__ BrowseImplementations_(
	browser___ &Browser,
	midi_implementations_ &Implementations )
{
	fill_status__ Status = fs_Undefined;
ERRProlog
	midi_implementation Implementation;
	bso::bool__ Continue = true;
ERRBegin
	while ( Continue ) {
		switch( Browser.Browse( xml::tfStartTag |xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Browser.TagName() == "MIDIImplementation" ) {
				Implementation.Init();

				if ( ( Status = BrowseImplementationSpecifications_( Browser, Implementation ) ) != fsOK )
					ERRReturn;

				if ( ( Status = BrowseImplementation_( Browser, Implementation ) ) != fsOK )
					ERRReturn;

				Implementations.Append( Implementation );
			} else {
				Status = fsUnexpectedTag;
				ERRReturn;
			}
			break;
		case xml::tEndTag:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			return fsBrowserError;
			ERRReturn;
		default:
			ERRc();
			break;
		}
	}

	Status = fsOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

fill_status__ mscrmi::Fill(
	browser___ &Browser,
	midi_implementations_ &Implementations )
{
	fill_status__ Status = fs_Undefined;
ERRProlog
ERRBegin
	switch ( Browser.Browse( xml::tfStartTag ) ) {
	case xml::tStartTag:
		if ( Browser.TagName() != "MIDIImplementations" ) {
			Status = fsUnexpectedTag;
			ERRReturn;
		}

		if ( ( Status = BrowseImplementationsSpecifications_( Browser, Implementations ) ) != fsOK )
			ERRReturn;

		if ( ( Status = BrowseImplementations_( Browser, Implementations ) ) != fsOK )
			ERRReturn;

	case xml::tProcessed:
		break;
	case xml::tError:
		Status = fsBrowserError;
		ERRReturn;
	default:
		ERRc();
		break;
	}

ERRErr
ERREnd
ERREpilog
	return Status;
}

static void ConvertModelID_(
	const str::string_ &RawModelID,
	char *Buffer )
{
	epeios::row__ Row = RawModelID.First();

	if ( RawModelID.Amount() > 4 )
		ERRc();

	if ( RawModelID.Amount() == 0 )
		ERRc();

	while ( Row != NONE ) {
		sprintf( Buffer + ( *Row << 1 ), "%02X", RawModelID( Row ) );

		Row = RawModelID.Next( Row );
	}
}

void mscrmi::Print(
	const midi_implementation_ &Implementation,
	xml::writer_ &Writer )
{
	char ModelID[] ="12345678";

	Writer.PushTag( "MIDIImplementation" );
	Writer.PutAttribute( "Model" , Implementation.ModelLabel );

	ConvertModelID_( Implementation.ModelID, ModelID );
	Writer.PutAttribute( "ModelID", ModelID );

	Print( Implementation.Definitions, Writer );

	Writer.PopTag();
}

void mscrmi::Print(
	const midi_implementations_ &Implementations,
	xml::writer_ &Writer )
{
	ctn::E_CITEM( midi_implementation_ ) Implementation;
	epeios::row__ Row = Implementations.First();

	Implementation.Init( Implementations );

	while ( Row != NONE ) {
		Print( Implementation( Row ), Writer );

		Row = Implementations.Next( Row );
	}
}

static datum__ Checksum_(
	const data_ &Data,
	epeios::row__ First = 0,
	epeios::row__ Last = NONE )
{
	bso::ubyte__ Checksum = 0;
	epeios::row__ Row = First;

	while( Row != Last ) {
		Checksum += Data( Row );
		Row = Data.Next( Row );
	}

	if ( Last != NONE ) 
		Checksum += Data( Row );

	return ( 128 - ( Checksum & 0x7f ) ) & 0x7f;
}

static void Put_(
	bso::ulong__ Value,
	data_ &Data )
{
	Data.Append(  ( Value >> 24 ) & 0xff );
	Data.Append(  ( Value >> 16 ) & 0xff );
	Data.Append(  ( Value >> 8 ) & 0xff );
	Data.Append(  Value  & 0xff );
}

void mscrmi::RequestData(
	const str::string_ &ModelID,
	address__ Address,
	size__ Size,
	flw::oflow__ &Flow )
{
ERRProlog
	data Data;
	STR_BUFFER___ Buffer;
ERRBegin
	Data.Init();

	Put_( Address, Data );
	Put_( Encode_( Size ), Data );


	Flow.Write( "\xf0\x41\x10", 3 );
	Flow.Write( ModelID.Convert( Buffer ), ModelID.Amount() );
	Flow.Put( 0x11 );

	Flow.Write( Data.Convert( Buffer ), Data.Amount() );
	Flow.Put( Checksum_( Data ) );

	Flow.Put( 0xF7 );

	Flow.Commit();
ERRErr

ERREnd
ERREpilog
}

mscmdm::size__ ExtractSysEx_(
	flw::iflow__ &Flow,
	mscmdm::origin__ Origin,
	mscmdm::data_ &Data )
{
	mscmdm::size__ Size = 0;
ERRProlog
	mscmdm::event_header__ EventHeader;
	mscmdm::data Dummy;
	bso::bool__ Continue = true;
ERRBegin
	while ( Continue ) {
		mscmdm::GetEventHeader( Flow, Origin, EventHeader );

		if ( ( EventHeader.EventType == mscmdm::etSystem ) && ( EventHeader.SystemEvent.Event == mscmdm::sysExclusive ) ) {
			Size = mscmdm::GetEventData( EventHeader, Flow, mscmdm::oDevice, Data );
			Continue = false;
		} else {
			Dummy.Init();
			mscmdm::GetEventData( EventHeader, Flow, mscmdm::oDevice, Dummy );
		}
	}
ERRErr
ERREnd
ERREpilog
	return Size;
}

static extraction_status__  Extract_(
	const mscmdm::data_ &RawData,
	const str::string_ &ModelID,
	address__ &Address,
	data_ &Data )
{
	extraction_status__ Status = xs_Undefined;
ERRProlog
	epeios::row__ Row = RawData.First();
	bso::size__ Counter = 0;
	str::string Header, Buffer;
ERRBegin
	Buffer.Init( "\x41\x10" );
	Buffer.Append( ModelID );
	Buffer.Append( '\x12' );

	Header.Init();

	Counter = Buffer.Amount();

	while ( ( Row != NONE ) && ( Counter-- ) ) {
		Header.Append( RawData( Row ) );
		Row = RawData.Next( Row );
	}

	if ( Row == NONE ) {
		Status = xsBadData;
		ERRReturn;
	}

	if ( Buffer != Header ) {
		Status = xsIncorrectData;
		ERRReturn;
	}

	if ( Checksum_( RawData, Row, RawData.Previous( RawData.Last() ) ) != RawData( RawData.Last() ) ) {
		Status =  xsChecksumError;
		ERRReturn;
	}

	Counter = 4;	// Taille d'une addresse.

	while ( Counter-- ) {

		if ( Row == NONE ) {
			Status = xsBadData;
			ERRReturn;
		}

		Address = Address << 8 | RawData( Row );

		Row = RawData.Next( Row );
	}

	if ( Row == NONE ) {
		Status = xsBadData;
		ERRReturn;
	}
	
	Data.Append( RawData, Row, RawData.Previous( RawData.Last() ) );

	Status = xsOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

static void Fill_(
	const adata_ &Data,
	const parameter_definitions_ &Definitions,
	row__ &DefinitionRow,
	parameters_ &Parameters )
{
ERRProlog
	parameter Parameter;
	epeios::row__ DataRow = NONE;
	address__ Address = NONE;
	ctn::E_CMITEMt( parameter_definition_, row__ ) Definition;
	size__ Size;
ERRBegin
	Address = Data.Address();
	DataRow = Data.First();
	Definition.Init( Definitions );

	while ( DataRow != NONE ) {
		while ( ( DefinitionRow != NONE ) &&
			    ( ( Definition( DefinitionRow ).Address() < Address )
				   || ( ( Size = Definition( DefinitionRow ).Size() ) == 0 ) ) )
			DefinitionRow = Definitions.Next( DefinitionRow );

		if ( ( DefinitionRow == NONE ) || ( Definition( DefinitionRow ).Address() > Address ) )
			ERRu();

		if ( !Data.Exists( Data.Next( DataRow, Size - 1 ) ) )
			ERRu();

		Parameter.Init( DefinitionRow );

		Parameter.Data.Allocate( Size );
		Parameter.Data.Store( Data, Size, 0, DataRow );

		Parameters.Append( Parameter );

		Address = Sum_( Address, Size );

		DataRow = Data.Next( DataRow, Size );
	}
ERRErr
ERREnd
ERREpilog
}

static bso::ulong__ Convert_( const str::string_ &Value )
{
	bso::ulong__ V = 0;
	epeios::row__ Row = Value.First();

	if ( Value.Amount() > 4 )
		ERRl();

	while ( Row != NONE ) {
		V = ( V << 8 ) + Value( Row );

		Row = Value.Next( Row );
	}

	return V;
}

static inline void Print_(
	const parameter_ &Parameter,
	const parameter_definitions_ &Definitions,
	xml::writer_ &Writer )
{
	char Address[] = "12345678";
	ctn::E_CMITEMt( parameter_definition_, row__ ) Definition;
	bso::integer_buffer__ Buffer;

	Definition.Init( Definitions );

	Writer.PutAttribute( "Label", Definition( Parameter.Row() ).Label );

	sprintf( Address, "%0*X", 2 * ( 4 - _StencilSize( Definition( Parameter.Row() ).S_.Address ) ), Definition( Parameter.Row() ).Offset() );
	Writer.PutAttribute( "Address", Address );

	Writer.PutAttribute( "Value", bso::Convert( Decode_( Convert_( Parameter.Data ) ), Buffer ) );
}

typedef bch::E_BUNCH_( row__ ) rows_;
E_AUTO( rows );

static void BuildPath_(
	row__ Row,
	const parameter_definitions_ &Definitions,
	rows_ &Rows )
{
	ctn::E_CMITEMt( parameter_definition_, row__ ) Definition;

	Definition.Init( Definitions );

	if ( Row != NONE )
		Row = Definition( Row ).GroupRow();

	while ( Row != NONE ) {
		Rows.Insert( Row, 0 );
		Row = Definition( Row ).GroupRow();
	}
}

static epeios::row__ SearchLastCommon_(
	const rows_ &Rows1,
	const rows_ &Rows2 )
{
	epeios::row__ Row1 = Rows1.First(), Row2 = Rows2.First();

	while ( ( Row1 != NONE ) && ( Row2 != NONE ) && ( Rows1( Row1 ) == Rows2( Row2 ) ) ) {
		Row1 = Rows1.Next( Row1 );
		Row2 = Rows2.Next( Row2 );
	}

	if ( Row1 == NONE )
		return Rows1.Last();
	else if ( Row2 == NONE )
		return Rows2.Last();
	else if ( Row1 != Row2 )
		ERRc();

	return Rows1.Previous( Row1 );
}

static void Pop_(
	const rows_ &Rows,
	epeios::row__ TargetRow,
	xml::writer_ &Writer )
{
	epeios::row__ Row = Rows.Last();

	while ( Row != TargetRow ) {
		Writer.PopTag();

		Row = Rows.Previous( Row );
	}
}

static void Push_(
	const rows_ &Rows,
	epeios::row__ Row,
	const parameter_definitions_ &Definitions,
	xml::writer_ &Writer )
{
	ctn::E_CMITEMt( parameter_definition_, row__ ) Definition;
	Definition.Init( Definitions );

	Row = ( Row == NONE ? Rows.First() : Rows.Next( Row ) );

	while ( Row != NONE ) {
		Writer.PushTag( "Parameters" );
		Print_( Definition( Rows( Row ) ), Writer );

		Row = Rows.Next( Row );
	}
}

static void PrintTags_(
	row__ Current,
	row__ Previous,
	const parameter_definitions_ &Definitions,
	xml::writer_ &Writer )
{
ERRProlog
	rows CurrentRows, PreviousRows;
	epeios::row__ Row = NONE;
ERRBegin
	CurrentRows.Init();
	BuildPath_( Current, Definitions, CurrentRows );

	PreviousRows.Init();
	BuildPath_( Previous, Definitions, PreviousRows );

	Row = SearchLastCommon_( CurrentRows, PreviousRows );

	Pop_( PreviousRows, Row, Writer );

	Push_( CurrentRows, Row, Definitions, Writer );
ERRErr
ERREnd
ERREpilog
}

void mscrmi::Print(
	const parameters_ &Parameters,
	const parameter_definitions_ &Definitions,
	xml::writer_ &Writer )
{
ERRProlog
	rows Rows;
	row__ DefinitionRow = NONE;
	epeios::row__ Row = NONE;
	row__ PreviousRow = NONE;
	ctn::E_CMITEM( parameter_ ) Parameter;
	ctn::E_CMITEMt( parameter_definition_, row__ ) Definition;
	Definition.Init( Definitions );
ERRBegin
	Parameter.Init( Parameters );
	Definition.Init( Definitions );
	Row = Parameters.First();

	Rows.Init();

	while ( Row != NONE ) {

		PrintTags_( Parameter( Row ).Row(), PreviousRow, Definitions, Writer );

		Writer.PushTag( "Parameter" );
		Print_( Parameter( Row ), Definitions, Writer );
		Writer.PopTag();

		PreviousRow = Parameter( Row ).Row();

		Row = Parameters.Next( Row );
	}

	PrintTags_( NONE, PreviousRow, Definitions, Writer );
ERRErr
ERREnd
ERREpilog
}


static void Fill_(
	const adata_set_ &DataSet,
	const parameter_definitions_ &Definitions,
	parameters_ &Parameters )
{
	epeios::row__ DataSetRow = DataSet.First();
	row__ DefinitionRow = Definitions.First();
	ctn::E_CMITEM( adata_ ) Data;

	Data.Init( DataSet );

	while ( DataSetRow != NONE ) {
		Fill_( Data( DataSetRow ), Definitions, DefinitionRow, Parameters );

		DataSetRow = DataSet.Next( DataSetRow );
	}
}


void mscrmi::Fill(
	const adata_set_ &DataSet,
	const midi_implementation_ &Implementation,
	parameters_ &Parameters )
{
	Fill_( DataSet, Implementation.Definitions, Parameters );
}


extraction_status__ mscrmi::ExtractData(
	flw::iflow__ &Flow,
	const str::string_ &ModelID,
	mscmdm::origin__ Origin,
	address__ &Address,
	data_ &Data )
{
	extraction_status__ Status = xs_Undefined;
ERRProlog
	mscmdm::data RawData;
	epeios::row__ Row = NONE;
ERRBegin
	RawData.Init();
	ExtractSysEx_( Flow, Origin, RawData );

	if ( ( Status = Extract_( RawData, ModelID, Address, Data ) ) != xsOK )
		ERRReturn;

	Status = xsOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

extraction_status__ mscrmi::RetrieveData(
	flw::oflow__ &OFlow,
	flw::iflow__ &IFlow,
	address__ Address,
	size__ Size,
	const str::string_ &ModelID,
	adata_ &Data )
{
	extraction_status__ Status = xs_Undefined;
	mscmdm::size__ HandledSize = Size + Data.Amount();

	RequestData( ModelID, Address, Size, OFlow );

	Data.Address() = MSCRMI_UNDEFINED_ADDRESS;

	if ( Data.Amount() != HandledSize )
		if ( ( Status = ExtractData( IFlow, ModelID, mscmdm::oDevice, Data.Address(), Data ) ) != xsOK )
			return Status;

	if ( Data.Address() != Address )
		ERRf();

	while ( Data.Amount() != HandledSize )
		if ( ( Status = ExtractData( IFlow, ModelID, mscmdm::oDevice, Address, Data ) ) != xsOK )
			return Status;

	Status = xsOK;

	return Status;
}

extraction_status__ mscrmi::RetrieveDataSet(
	flw::oflow__ &OFlow,
	flw::iflow__ &IFlow,
	const blocs_ &Blocs,
	const str::string_ &ModelID,
	adata_set_ &DataSet )
{
ERRProlog
	epeios::row__ Row = NONE;
	adata Data;
ERRBegin
	Row = Blocs.First();

	while ( Row != NONE ) {
		Data.Init();
		RetrieveData( OFlow, IFlow, Blocs( Row ).Address, Blocs( Row ).Size, ModelID, Data );

		DataSet.Append( Data );

		Row = Blocs.Next( Row );

		Sleep( 40 );
	}
ERRErr
ERREnd
ERREpilog
}






/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mscrmipersonnalization
: public mscrmitutor
{
public:
	mscrmipersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mscrmipersonnalization( void )
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

static mscrmipersonnalization Tutor;

ttr_tutor &MSCRMITutor = Tutor;
