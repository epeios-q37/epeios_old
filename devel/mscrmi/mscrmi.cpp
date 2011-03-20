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

#include "tht.h"
#include "cio.h"

using namespace mscrmi;

using xml::parser___;

void mscrmi::Print(
	const adata_ &Data,
	txf::text_oflow__ &Flow )
{
	char Buffer[]= "12345678";
	epeios::row__ Row = Data.First();

	sprintf( Buffer, "%08lX", Data.Address() );
	Flow << Buffer << " : ";

	while ( Row != NONE ) {
		sprintf( Buffer, "%02lX", (bso::ulong__)Data( Row ) );

		Flow << Buffer << txf::pad;

		Row = Data.Next( Row );
	}
}

void mscrmi::Print(
	const adata_set_ &DataSet,
	txf::text_oflow__ &Flow )
{
	ctn::E_CMITEM( adata_ ) Data;
	epeios::row__ Row = DataSet.First();

	Data.Init( DataSet );

	while ( Row != NONE ) {
		Print( Data( Row ), Flow );

		Flow << txf::nl;

		Row = DataSet.Next( Row );
	}

}

static inline bso::ulong__ Encode_( bso::ulong__ Value )
// Encodage pour flux MIDI.
{
	return ( Value & 0x7f ) | ( ( Value & 0x3f80) << 1 ) | ( ( Value & 0x1fc000 ) << 2 ) | ( ( Value & 0xfe00000 ) << 3 );
}


static inline bso::ulong__ Decode_( bso::ulong__ Value )
// Decodage d'un flux MIDI.
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
	const parameters_ &Parameters,
	blocs_ &Blocs )
{
	bloc__ Bloc;
	row__ Row = Parameters.First();
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;

	Parameter.Init( Parameters );

	if ( Row != NONE ) {
		while ( ( Row != NONE ) && ( Parameter( Row ).Size() == 0 ) )
			Row = Parameters.Next( Row );

		if ( Row != NONE ) {
			Bloc.Address = Parameter( Row ).Address();
			Bloc.Size = Parameter( Row ).Size();

			Row = Parameters.Next( Row );
		}
	}


	while ( Row != NONE ) {
		while ( ( Row != NONE ) && ( Parameter( Row ).Size() == 0 ) )
			Row = Parameters.Next( Row );

		if ( Row != NONE ) {
			if ( Sum_( Bloc.Address, Bloc.Size ) == Parameter( Row ).Address() )
				Bloc.Size += Parameter( Row ).Size();
			else {
				Blocs.Append( Bloc );
				Bloc.Address = Parameter( Row ).Address();
				Bloc.Size = Parameter( Row ).Size();
			}
		}

		Row = Parameters.Next( Row );
	}

	Blocs.Append( Bloc );
}


static void PrintPath_(
	row__ Row,
	const parameters_ &Parameters,
	txf::text_oflow__ &Flow )
{
ERRProlog
	str::string Path;
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
ERRBegin
	Parameter.Init( Parameters );
	Path.Init();

	Row = Parameter( Row ).GroupRow();

	while ( Row != NONE ) {
		Path.Insert( '/', 0 );
		Path.Insert( Parameter( Row ).Label, 0 );

		Row = Parameter( Row ).GroupRow();
	}

	Flow << Path;

ERRErr
ERREnd
ERREpilog
}

static inline void Print_(
	const parameter_ &Parameter,
	xml::writer_ &Writer )
{
	char Address[] = "12345678";

	Writer.PutAttribute( "Label", Parameter.Label );

	sprintf( Address, "%08lX", Parameter.Address() );
	Writer.PutAttribute( "Address", Address );
/*
	sprintf( Address, "%0*lX", 2 * ( 4 - _StencilSize( Parameter.S_.Address ) ), Parameter.Offset() );
	Writer.PutAttribute( "Offset", Address );

	if ( Parameter.Size() != 0 )
		Writer.PutAttribute( "Size", bso::Convert( Parameter.Size(), Buffer ) );
*/
}

typedef stk::E_BSTACK_( row__ ) srows_;
E_AUTO( srows );

void mscrmi::Print(
	const parameters_ &Parameters,
	xml::writer_ &Writer )
{
ERRProlog
	srows Rows;
	row__ Row = NONE;
	row__ ParentRow = NONE;
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
ERRBegin
	Parameter.Init( Parameters );
	Row = Parameters.First();

	Rows.Init();

	while ( Row != NONE ) {

		while ( Parameter( Row ).GroupRow() != ParentRow ) {
			Writer.PopTag();
			ParentRow = Rows.Pop();
		}

		if ( Parameter( Row ).Size() == 0 ) {
			Rows.Push( ParentRow );
			ParentRow = Row;
			Writer.PushTag( "Parameters" );
		} else
			Writer.PushTag( "Parameter" );

		Print_( Parameter( Row ), Writer );

		if ( Parameter( Row ).Size() != 0 )
			Writer.PopTag();

		Row = Parameters.Next( Row );
	}

	while ( Rows.Amount() != 0 ) {
		Rows.Pop();
		Writer.PopTag();
	}
ERRErr
ERREnd
ERREpilog
}

#define CASE( label )	LCL_CASE( label, ps )

const char *mscrmi::Label( parse_status__ Status )
{
	switch ( Status ) {
		CASE( ParserError )
		CASE( UnexpectedTag )
		CASE( AttributeAlreadyDefined )
		CASE( UnexpectedAttribute )
		CASE( MissingAttribute )
		CASE( BadValue )
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.

}

#undef CASE


static bso::ubyte__ Convert_(
	const str::string_ &Source,
	bso::char__ *Target )
{
	bso::size__ Amount = Source.Amount();
	bso::ulong__ Value;
	epeios::row__ Error = NONE;

	if ( Amount == 0 )
		return 0;

	if ( Amount & 1 )
		return 0;

	if ( Amount > 8 )
		return 0;

	Value = Source.ToUL( &Error, str::b16 );

	if ( Error != NONE )
		return 0;
#if 0
	switch ( Amount >> 1 ) {
	case 4:
		Target[3] = ( Value >> 24 ) & 0xff;
	case 3:
		Target[2] = ( Value >> 16 ) & 0xff;;
	case 2:
		Target[1] = ( Value >> 8 ) & 0xff;
	case 1:
		Target[0] = Value & 0xff;
		break;
	default:
		ERRc();
		break;
	}
#else
	switch ( Amount >> 1 ) {
	case 4:
		*Target = ( Value >> 24 ) & 0xff;
		Target++;
	case 3:
		*Target = ( Value >> 16 ) & 0xff;;
		Target++;
	case 2:
		*Target = ( Value >> 8 ) & 0xff;
		Target++;
	case 1:
		*Target = Value & 0xff;
		Target++;
		break;
	default:
		ERRc();
		break;
	}
#endif

	return (bso::ubyte__)( Amount >> 1);
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

class callback__
{
public:
	virtual parse_status__ Handle(
		const _parameter_core_ &Parameter,
		const str::string_ &Value ) = 0;
};

static parse_status__ ParseParametersSpecifications_(
	parser___ &Parser,
	xaddress__ &Address,
	callback__ &Callback )
{
	parse_status__ Status = ps_Undefined;
ERRProlog
	bso::bool__ NoLabel = true;
	bso::bool__ Continue = true;
	_parameter_core Core;
	label Label;
	str::string Value;
ERRBegin
	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAttribute | xml::tfStartTagClosed ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() == "Address" ) {
				if ( ( Address = HandleAddress_( Address, Parser.Value() ) ) == MSCRMI_UNDEFINED_ADDRESS ) {
					Status = psBadValue;
					ERRReturn;
				}
			} else if ( Parser.AttributeName() == "Label" ) {
				if ( Parser.Value().Amount() == 0 ) {
					Status = psBadValue;
					ERRReturn;
				}

				NoLabel = false;

				Label.Init( Parser.Value() );
			} else {
				Status = psUnexpectedAttribute;
				ERRReturn;
			}
			break;
		case xml::tStartTagClosed:
			Continue = false;
			break;
		case xml::tProcessed:
			break;
		case xml::tError:
			Status = psParserError;
			ERRReturn;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( NoLabel ) {
		Status = psMissingAttribute;
		ERRReturn;
	}

	Status = psOK;

	Core.Init( Label, Address, 0 );

	Value.Init();

	Callback.Handle( Core, Value );
ERRErr
ERREnd
ERREpilog
	return Status;
}

static parse_status__ ParseParameter_(
	parser___ &Parser,
	xaddress__ &Address,
	size__ &Size,
	callback__ &Callback )
{
	parse_status__ Status = ps_Undefined;
ERRProlog
	bso::bool__ Continue = true;
	label Label;
	_parameter_core Core;
	str::string Value;
ERRBegin
	Label.Init();
	Value.Init();

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAttribute | xml::tfEndTag ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() == "Label" ) {
				if ( Label.Amount() != 0 ) {
					Status = psAttributeAlreadyDefined;
					ERRReturn;
				}

				Label = Parser.Value();

				if ( Label.Amount() == 0 ) {
					Status = psBadValue;
					ERRReturn;
				}
			} else if ( Parser.AttributeName() == "Address" ) {
				if ( ( Address = HandleAddress_( Address, Parser.Value() ) ) == MSCRMI_UNDEFINED_ADDRESS )
					return psBadValue;
			} else if ( Parser.AttributeName() == "Size" ) {
				  epeios::row__ Error = NONE;

				  Size = Parser.Value().ToUB( &Error );

				  if ( Error != NONE ) {
					  Status = psBadValue;
					  ERRReturn;
				  }

				  if ( Size == 0 ) {
					  Status = psBadValue;
					  ERRReturn;
				  }
			} else if ( Parser.AttributeName() == "Value" ) {
				if ( Value.Amount() != 0 ) {
					Status = psAttributeAlreadyDefined;
					ERRReturn;
				}

				Value = Parser.Value();

				if ( Value.Amount() == 0 ) {
					Status = psBadValue;
					ERRReturn;
				}
			} else {
				Status = psUnexpectedAttribute;
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
			return psParserError;
			break;
		default:
			ERRc();
			break;
		}
	}

	if( Label.Amount() == 0 ) {
		Status = psMissingAttribute;
		ERRReturn;
	}

	Status = psOK;

	Core.Init( Label, Address, Size );

	Callback.Handle( Core, Value );

	Address = Sum_( Address, Size );

ERRErr
ERREnd
ERREpilog
	return Status;

}

static parse_status__ ParseParameters_(
	parser___ &Parser,
	xaddress__ &Address,
	size__ &Size,
	callback__ &Callback )
{
	parse_status__ Status = ps_Undefined;
ERRProlog
	bso::bool__ Continue = true;
	_parameter_core Core;
	str::string Value;
ERRBegin
	while ( Continue ) {
		switch ( Parser.Parse( xml::tfStartTag | xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == "Parameters" ) {
				if ( ( Status = ParseParametersSpecifications_( Parser, Address, Callback ) ) != psOK )
					ERRReturn;

				if ( ( Status = ParseParameters_( Parser, Address, Size, Callback ) ) != psOK )
					ERRReturn;

				Core.Init();
				Value.Init();

				Callback.Handle( Core, Value );
			} else if ( Parser.TagName() == "Parameter" ) {
				if ( ( Status = ParseParameter_( Parser, Address, Size, Callback ) ) != psOK )
					ERRReturn;
			} else {
				Status = psUnexpectedTag;
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
			return psParserError;
			break;
		default:
			ERRc();
			break;
		}
	}

	Status = psOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

static parse_status__ ParseParameters_(
	parser___ &Parser,
	callback__ &Callback )
{
	xaddress__ Address = 0;
	size__ Size = 1;

	return ParseParameters_( Parser, Address, Size, Callback );
}

static void RevertDeviceFamily_( device_family__ DeviceFamily )
{
	tol::Swap( DeviceFamily[0], DeviceFamily[3] );
	tol::Swap( DeviceFamily[1], DeviceFamily[2] );
}

static parse_status__ ParseImplementationSpecifications_(
	parser___ &Parser,
	midi_implementation_ &Implementation )
{
	bso::bool__ Continue = true;

	while ( Continue ) {
		switch ( Parser.Parse( xml::tfAttribute | xml::tfStartTagClosed ) ) {
		case xml::tAttribute:
			if ( Parser.AttributeName() == "ModelID" ) {
				if ( Implementation.S_.ModelIDLength != 0 )
					return psAttributeAlreadyDefined;
				if ( ( Implementation.S_.ModelIDLength = Convert_( Parser.Value(), Implementation.S_.ModelID ) ) == 0 )
					return psBadValue;
			} else if ( Parser.AttributeName() == "ModelLabel" ) {
				if ( Implementation.ModelLabel.Amount() != 0 )
					return psAttributeAlreadyDefined;
				Implementation.ModelLabel = Parser.Value();
			} else if ( Parser.AttributeName() == "DeviceFamily" ) {
				if ( memcmp( Implementation.S_.DeviceFamily, MSCRMI_UNDEFINED_DEVICE_FAMILY, sizeof( Implementation.S_.DeviceFamily ) ) != 0 )
					return psAttributeAlreadyDefined;
				if ( Convert_( Parser.Value(), Implementation.S_.DeviceFamily ) == 0 )
					return psBadValue;
				// RevertDeviceFamily_( Implementation.S_.DeviceFamily );
			} else
				return psUnexpectedAttribute;
			break;
		case xml::tStartTagClosed:
			Continue = false;
			break;
		case xml::tProcessed:
			ERRc();
			break;
		case xml::tError:
			return psParserError;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( ( Implementation.S_.ModelIDLength == 0 )
		 || ( memcmp( Implementation.S_.DeviceFamily, MSCRMI_UNDEFINED_DEVICE_FAMILY, sizeof( Implementation.S_.DeviceFamily ) ) == 0 ) )
		return psMissingAttribute;

	return psOK;
}

class implementation_callback__
: public callback__
{
private:
	parameters_ *_Parameters;
	row__ _GroupRow;
protected:
	virtual parse_status__ Handle(
		const _parameter_core_ &Core,
		const str::string_ &Value )
	{
		parse_status__ Status = ps_Undefined;
	ERRProlog
		parameter Parameter;
		row__ GroupRow = NONE;
	ERRBegin
		if ( Core.Address() == MSCRMI_UNDEFINED_ADDRESS ) {
			_GroupRow = _Parameters->Get( _GroupRow ).GroupRow();
			_Parameters->Flush();
		} else {
			Parameter.Init( Core, _GroupRow );

			GroupRow = _Parameters->Add( Parameter );

			if ( Core.Size() == 0 )
				_GroupRow = GroupRow;
		}

		Status = psOK;

		ERRErr
		ERREnd
		ERREpilog
			return Status;
	}
public:
	void reset( bso::bool__ = true )
	{
		_Parameters = NULL;
		_GroupRow = NONE;
	}
	implementation_callback__( void )
	{
		reset( false );
	}
	~implementation_callback__( void )
	{
		reset();
	}
	void Init( parameters_ &Parameters )
	{
		_Parameters = &Parameters;
		_GroupRow = NONE;
	}
};

parse_status__ mscrmi::Parse(
	parser___ &Parser,
	midi_implementation_ &Implementation )
{
	parse_status__ Status = ps_Undefined;
ERRProlog
	implementation_callback__ Callback;
ERRBegin
	Callback.Init( Implementation.Parameters );

	switch ( Parser.Parse( xml::tfStartTag ) ) {
	case xml::tStartTag:
		if ( Parser.TagName() != "MIDIImplementation" ) {
			Status = psUnexpectedTag;
			ERRReturn;
		}

		if ( ( Status = ParseImplementationSpecifications_( Parser, Implementation ) ) != psOK )
			ERRReturn;

		if ( ( Status = ParseParameters_( Parser, Callback ) ) != psOK )
			ERRReturn;

	case xml::tProcessed:
		break;
	case xml::tError:
		Status = psParserError;
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

static parse_status__ ParseImplementationsSpecifications_(
	parser___ &Parser,
	midi_implementations_ &Implementations )
{
	switch ( Parser.Parse( xml::tfStartTagClosed ) ) {
	case xml::tStartTagClosed:
		break;
	case xml::tProcessed:
		ERRc();
		break;
	case xml::tError:
		return psParserError;
		ERRReturn;
	default:
		ERRc();
		break;
	}

	return psOK;
}

static parse_status__ ParseImplementations_(
	parser___ &Parser,
	midi_implementations_ &Implementations )
{
	parse_status__ Status = ps_Undefined;
ERRProlog
	midi_implementation Implementation;
	bso::bool__ Continue = true;
	implementation_callback__ Callback;
ERRBegin
	while ( Continue ) {
		switch( Parser.Parse( xml::tfStartTag |xml::tfEndTag ) ) {
		case xml::tStartTag:
			if ( Parser.TagName() == "MIDIImplementation" ) {
				Implementation.Init();
				Callback.Init( Implementation.Parameters );

				if ( ( Status = ParseImplementationSpecifications_( Parser, Implementation ) ) != psOK )
					ERRReturn;

				if ( ( Status = ParseParameters_( Parser, Callback ) ) != psOK )
					ERRReturn;

				Implementations.Append( Implementation );
			} else {
				Status = psUnexpectedTag;
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
			return psParserError;
			ERRReturn;
		default:
			ERRc();
			break;
		}
	}

	Status = psOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

parse_status__ mscrmi::Parse(
	parser___ &Parser,
	midi_implementations_ &Implementations )
{
	parse_status__ Status = ps_Undefined;
ERRProlog
ERRBegin
	switch ( Parser.Parse( xml::tfStartTag ) ) {
	case xml::tStartTag:
		if ( Parser.TagName() != "MIDIImplementations" ) {
			Status = psUnexpectedTag;
			ERRReturn;
		}

		if ( ( Status = ParseImplementationsSpecifications_( Parser, Implementations ) ) != psOK )
			ERRReturn;

		if ( ( Status = ParseImplementations_( Parser, Implementations ) ) != psOK )
			ERRReturn;

	case xml::tProcessed:
		break;
	case xml::tError:
		Status = psParserError;
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

class settings_callback___
: public callback__
{
private:
	adata_set_ *_DataSet;
	adata _Data;
	const parameters_ *_Parameters;
	row__ _Row;
	row__ _Search( address__ Address ) const
	{
		ctn::E_CMITEMt( parameter_, row__ ) Parameter;
		row__ Row = ( _Row == NONE ? _Parameters->First() : _Row );

		Parameter.Init( *_Parameters );

		while ( ( Row != NONE ) &&
			    ( ( Parameter( Row ).Address() < Address )
				   || ( ( Parameter( Row ).Size() ) == 0 ) ) )
			Row = _Parameters->Next( Row );

		return Row;
	}
	parse_status__ _Convert( 
		const str::string_ &Value,
		size__ Size,
		data_ &Data )
	{
		bso::ulong__ Long;
		epeios::row__ Error = NONE;

		switch ( Size ) {
		case 0:
			ERRc();
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			Long = Encode_( Value.ToUL( &Error, str::b10, ( 0xffffffff >> ( 8 * ( 4 - Size ) ) ) ) );
			break;
		default:
			ERRc();
			break;
		}

		if ( Error != NONE )
			return psBadValue;

		switch ( Size ) {
		case 0:
			ERRc();
			break;
		case 4:
			Data.Append( Long >> 24 );
		case 3:
			Data.Append( ( Long >> 16 ) & 0xff );
		case 2:
			Data.Append( ( Long >> 8 ) & 0xff );
		case 1:
			Data.Append( Long & 0xff );
			break;
		default:
			ERRc();
			break;
		}

		return psOK;
	}
protected:
	virtual parse_status__ Handle(
		const _parameter_core_ &Core,
		const str::string_ &Value )
	{
		parse_status__ Status = ps_Undefined;
	ERRProlog
		data RawData;
		ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	ERRBegin
		Parameter.Init( *_Parameters );

		if ( Core.Size() != 0 ) {

			if ( ( _Row = _Search( Core.Address() ) ) == NONE ) {
				Status = psBadValue;
				ERRReturn;
			}

			if ( Value.Amount() == 0 ) {
				Status =psBadValue;
				ERRReturn;
			}

			RawData.Init();

			_Convert( Value, Parameter( _Row ).Size(), RawData );

			if ( _Data.Address() == MSCRMI_UNDEFINED_ADDRESS )
				_Data.Init( RawData, Core.Address() );
			else if ( ( ( RawData.Amount() + _Data.Amount() ) > 128 )
			     || ( Sum_( _Data.Address(), (size__)_Data.Amount() ) != Core.Address() ) ) 

			{
				_DataSet->Append( _Data );
				_Data.Init( RawData, Core.Address() );
			} else {
				_Data.Append( RawData );
			}
		}

		Status = psOK;

		ERRErr
		ERREnd
		ERREpilog
			return Status;
	}
public:
	void reset( bso::bool__ P = true )
	{
		if ( P  ) {
			if ( _Data.Address() != MSCRMI_UNDEFINED_ADDRESS ) {
				_DataSet->Append( _Data );
			}
		}

		_Data.reset( P );
		_DataSet = NULL;
		_Parameters = NULL;
		_Row = NONE;

	}
	settings_callback___( void )
	{
		reset( false );
	}
	~settings_callback___( void )
	{
		reset();
	}
	void Init(
		adata_set_ &DataSet,
		const parameters_ &Parameters )
	{
		reset();

		_DataSet = &DataSet;
		_Parameters = &Parameters;
	}
};


parse_status__ mscrmi::Parse(
	parser___ &Parser,
	const midi_implementation_ &Implementation,
	adata_set_ &DataSet )
{
	parse_status__ Status = ps_Undefined;
ERRProlog
	settings_callback___ Callback;
ERRBegin
	Callback.Init( DataSet, Implementation.Parameters );

	switch ( Parser.Parse( xml::tfStartTag ) ) {
	case xml::tStartTag:
		if ( Parser.TagName() != "Settings" ) {
			Status = psUnexpectedTag;
			ERRReturn;
		}

		if ( ( Status = ParseParameters_( Parser, Callback ) ) != psOK )
			ERRReturn;

	case xml::tProcessed:
		break;
	case xml::tError:
		Status = psParserError;
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


static void Convert_(
	const char *Source,
	bso::ulong__ SourceLength,
	char *Target )
{
	if ( SourceLength > 4 )
		ERRc();

	while ( SourceLength-- ) {
		sprintf( Target, "%02X", *Source );

		Target += 2;
		Source++;
	}
}

void mscrmi::ToString(
	const device_family__ &DeviceFamily,
	str::string_ &Target )
{
	char Buffer[] ="12345678";

	Convert_( DeviceFamily, sizeof( DeviceFamily ), Buffer );

	Target.Append( Buffer );

}

void mscrmi::Print(
	const midi_implementation_ &Implementation,
	xml::writer_ &Writer )
{
	char Buffer[] ="12345678";

	Writer.PushTag( "MIDIImplementation" );
	Writer.PutAttribute( "Model" , Implementation.ModelLabel );

	Convert_( Implementation.S_.ModelID, Implementation.S_.ModelIDLength, Buffer );
	Writer.PutAttribute( "ModelID", Buffer );

	Convert_( Implementation.S_.DeviceFamily, sizeof( Implementation.S_.DeviceFamily ), Buffer );
	Writer.PutAttribute( "DeviceFamily", Buffer );

	Print( Implementation.Parameters, Writer );

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
	const identity__ &Identity,
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


	Flow.Write( "\xf0\x41", 2 );
	Flow.Put( Identity.DeviceID );
	Flow.Write( Identity.ModelID, Identity.ModelIDLength );
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

#define CASE( label )	LCL_CASE( label, ts )

const char *mscrmi::Label( transmission_status__ Status )
{
	switch( Status ) {
	CASE( ChecksumError )
	CASE( IncorrectData )
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

#undef CASE

static transmission_status__  Extract_(
	const mscmdm::data_ &RawData,
	const identity__ &Identity,
	address__ &Address,
	data_ &Data )
{
	transmission_status__ Status = ts_Undefined;
ERRProlog
	epeios::row__ Row = RawData.First();
	bso::size__ Counter = 0;
	str::string Header, Buffer;
ERRBegin
	Buffer.Init( "\x41" );
	Buffer.Append( Identity.DeviceID );
	Buffer.Append( Identity.ModelID, Identity.ModelIDLength );
	Buffer.Append( '\x12' );

	Header.Init();

	Counter = Buffer.Amount();

	while ( ( Row != NONE ) && ( Counter-- ) ) {
		Header.Append( RawData( Row ) );
		Row = RawData.Next( Row );
	}

	if ( Row == NONE ) {
		Status = tsIncorrectData;
		ERRReturn;
	}

	if ( Buffer != Header ) {
		Status = tsIncorrectData;
		ERRReturn;
	}

	if ( Checksum_( RawData, Row, RawData.Previous( RawData.Last() ) ) != RawData( RawData.Last() ) ) {
		Status =  tsChecksumError;
		ERRReturn;
	}

	Counter = 4;	// Taille d'une addresse.

	while ( Counter-- ) {

		if ( Row == NONE ) {
			Status = tsIncorrectData;
			ERRReturn;
		}

		Address = Address << 8 | RawData( Row );

		Row = RawData.Next( Row );
	}

	if ( Row == NONE ) {
		Status = tsIncorrectData;
		ERRReturn;
	}
	
	Data.Append( RawData, Row, RawData.Previous( RawData.Last() ) );

	Status = tsOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

static void Fill_(
	const adata_ &Data,
	const parameters_ &Parameters,
	row__ &ParameterRow,
	settings_ &Settings )
{
ERRProlog
	setting Setting;
	epeios::row__ DataRow = NONE;
	address__ Address = NONE;
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	size__ Size;
ERRBegin
	Address = Data.Address();
	DataRow = Data.First();
	Parameter.Init( Parameters );

	while ( DataRow != NONE ) {
		while ( ( ParameterRow != NONE ) &&
			    ( ( Parameter( ParameterRow ).Address() < Address )
				   || ( ( Size = Parameter( ParameterRow ).Size() ) == 0 ) ) )
			ParameterRow = Parameters.Next( ParameterRow );

		if ( ( ParameterRow == NONE ) || ( Parameter( ParameterRow ).Address() > Address ) )
			ERRu();

		if ( !Data.Exists( Data.Next( DataRow, Size - 1 ) ) )
			ERRu();

		Setting.Init( ParameterRow );

		Setting.Data.Allocate( Size );
		Setting.Data.Store( Data, Size, 0, DataRow );

		Settings.Append( Setting );

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
	const setting_ &Setting,
	const parameters_ &Parameters,
	xml::writer_ &Writer )
{
	char Address[] = "12345678";
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	bso::integer_buffer__ Buffer;

	Parameter.Init( Parameters );

	Writer.PutAttribute( "Label", Parameter( Setting.Row() ).Label );

	sprintf( Address, "%0*lX", 2 * ( 4 - _StencilSize( Parameter( Setting.Row() ).S_.Address ) ), Parameter( Setting.Row() ).Offset() );
	Writer.PutAttribute( "Address", Address );

	Writer.PutAttribute( "Value", bso::Convert( Decode_( Convert_( Setting.Data ) ), Buffer ) );
}

typedef bch::E_BUNCH_( row__ ) rows_;
E_AUTO( rows );

static void BuildPath_(
	row__ Row,
	const parameters_ &Parameters,
	rows_ &Rows )
{
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;

	Parameter.Init( Parameters );

	if ( Row != NONE )
		Row = Parameter( Row ).GroupRow();

	while ( Row != NONE ) {
		Rows.Insert( Row, 0 );
		Row = Parameter( Row ).GroupRow();
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
	const parameters_ &Parameters,
	xml::writer_ &Writer )
{
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	Parameter.Init( Parameters );

	Row = ( Row == NONE ? Rows.First() : Rows.Next( Row ) );

	while ( Row != NONE ) {
		Writer.PushTag( "Parameters" );
		Print_( Parameter( Rows( Row ) ), Writer );

		Row = Rows.Next( Row );
	}
}

static void PrintTags_(
	row__ Current,
	row__ Previous,
	const parameters_ &Parameters,
	xml::writer_ &Writer )
{
ERRProlog
	rows CurrentRows, PreviousRows;
	epeios::row__ Row = NONE;
ERRBegin
	CurrentRows.Init();
	BuildPath_( Current, Parameters, CurrentRows );

	PreviousRows.Init();
	BuildPath_( Previous, Parameters, PreviousRows );

	Row = SearchLastCommon_( CurrentRows, PreviousRows );

	Pop_( PreviousRows, Row, Writer );

	Push_( CurrentRows, Row, Parameters, Writer );
ERRErr
ERREnd
ERREpilog
}

void mscrmi::Print(
	const settings_ &Settings,
	const parameters_ &Parameters,
	xml::writer_ &Writer )
{
ERRProlog
	rows Rows;
	row__ ParameterRow = NONE;
	epeios::row__ Row = NONE;
	row__ PreviousRow = NONE;
	ctn::E_CMITEM( setting_ ) Setting;
	ctn::E_CMITEMt( parameter_, row__ ) Parameter;
	Parameter.Init( Parameters );
ERRBegin
	Setting.Init( Settings );
	Parameter.Init( Parameters );
	Row = Settings.First();

	Rows.Init();

	while ( Row != NONE ) {

		PrintTags_( Setting( Row ).Row(), PreviousRow, Parameters, Writer );

		Writer.PushTag( "Parameter" );
		Print_( Setting( Row ), Parameters, Writer );
		Writer.PopTag();

		PreviousRow = Setting( Row ).Row();

		Row = Settings.Next( Row );
	}

	PrintTags_( NONE, PreviousRow, Parameters, Writer );
ERRErr
ERREnd
ERREpilog
}


static void Fill_(
	const adata_set_ &DataSet,
	const parameters_ &Parameters,
	settings_ &Settings )
{
	epeios::row__ DataSetRow = DataSet.First();
	row__ ParameterRow = Parameters.First();
	ctn::E_CMITEM( adata_ ) Data;

	Data.Init( DataSet );

	while ( DataSetRow != NONE ) {
		Fill_( Data( DataSetRow ), Parameters, ParameterRow, Settings );

		DataSetRow = DataSet.Next( DataSetRow );
	}
}


void mscrmi::Fill(
	const adata_set_ &DataSet,
	const midi_implementation_ &Implementation,
	settings_ &Settings )
{
	Fill_( DataSet, Implementation.Parameters, Settings );
}

inline transmission_status__ SendIdentityRequest_(
	device_id__ Id,
	flw::oflow__ &Flow )
{
	Flow.Write( "\xf0\x7e", 2 );
	Flow.Put( Id );
	Flow.Write( "\x06\x01\xF7", 3 );

	return tsOK;
}

transmission_status__ mscrmi::GetDeviceFamily(
	device_id__ Id,
	flw::ioflow__ &Flow,
	device_family__ &DeviceFamily )
{
	transmission_status__ Status = ts_Undefined;
ERRProlog
	data Data;
	epeios::row__ Row = NONE;
	data Model;
ERRBegin

	if ( ( Status = SendIdentityRequest_( Id, Flow ) ) != tsOK )
		ERRReturn;

	Flow.Commit();

	Data.Init();

	ExtractSysEx_( Flow, mscmdm::oDevice, Data );

	Flow.Dismiss();

	if ( Data.Amount() != 13 ) {
		Status =tsIncorrectData;
		ERRReturn;
	}

	Model.Init( "\x7e" );
	Model.Append( Id );
	Model.Append( "\x06\x02\x41" );


	if ( str::Compare( Data, Model, Data.First(), Model.First(), Model.Amount() ) != 0 ) {
		Status = tsIncorrectData;
		ERRReturn;
	}

	Data.Recall( Data.First( Model.Amount() ), sizeof( DeviceFamily ), DeviceFamily );

	Status = tsOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

transmission_status__ mscrmi::Extract(
	flw::iflow__ &Flow,
	const identity__ &Identity,
	mscmdm::origin__ Origin,
	address__ &Address,
	data_ &Data )
{
	transmission_status__ Status = ts_Undefined;
ERRProlog
	mscmdm::data RawData;
	epeios::row__ Row = NONE;
ERRBegin
	RawData.Init();
	ExtractSysEx_( Flow, Origin, RawData );

	if ( ( Status = Extract_( RawData, Identity, Address, Data ) ) != tsOK )
		ERRReturn;

	Status = tsOK;
ERRErr
ERREnd
ERREpilog
	return Status;
}

transmission_status__ mscrmi::Retrieve(
	flw::ioflow__ &Flow,
	address__ Address,
	size__ Size,
	const identity__ &Identity,
	adata_ &Data )
{
	transmission_status__ Status = ts_Undefined;
	mscmdm::size__ HandledSize = Size + Data.Amount();

	RequestData( Identity, Address, Size, Flow );

	Data.Address() = MSCRMI_UNDEFINED_ADDRESS;

	if ( Data.Amount() != HandledSize )
		if ( ( Status = Extract( Flow, Identity, mscmdm::oDevice, Data.Address(), Data ) ) != tsOK )
			return Status;

	if ( Data.Address() != Address )
		ERRf();

	while ( Data.Amount() != HandledSize )
		if ( ( Status = Extract( Flow, Identity, mscmdm::oDevice, Address, Data ) ) != tsOK )
			return Status;

	Status = tsOK;

	Flow.Dismiss();

	return Status;
}

transmission_status__ mscrmi::Retrieve(
	flw::ioflow__ &Flow,
	const blocs_ &Blocs,
	const identity__ &Identity,
	adata_set_ &DataSet )
{
	transmission_status__ Status = ts_Undefined;
ERRProlog
	epeios::row__ Row = NONE;
	adata Data;
ERRBegin
	Row = Blocs.First();

	while ( Row != NONE ) {
		Data.Init();

		if( ( Status = Retrieve( Flow, Blocs( Row ).Address, Blocs( Row ).Size, Identity, Data ) ) != tsOK )
			ERRReturn;

		DataSet.Append( Data );

		Row = Blocs.Next( Row );

		tht::Suspend( 40 );
	}
ERRErr
ERREnd
ERREpilog
	return Status;
}

epeios::row__ mscrmi::GetCorrespondingMIDIImplementation(
	const device_family__ &DeviceFamily,
	const midi_implementations_ &Implementations )
{
	ctn::E_CITEM( midi_implementation_ ) Implementation;
	epeios::row__ Row = Implementations.First();

	Implementation.Init( Implementations );

	while ( Row != NONE ) {
		if ( memcmp( Implementation( Row ).DeviceFamily(), DeviceFamily, sizeof( DeviceFamily ) ) == 0 )
			break;

		Row = Implementations.Next( Row );
	}

	return Row;
}

void mscrmi::Send(
	const adata_ &Data,
	const identity__ &Identity,
	flw::oflow__ &Flow )
{
ERRProlog
	data Message;
	STR_BUFFER___ Buffer;
ERRBegin
	Message.Init();

	Put_( Data.Address(), Message );
	Message.Append( Data );


	Flow.Write( "\xf0\x41", 2 );
	Flow.Put( Identity.DeviceID );
	Flow.Write( Identity.ModelID, Identity.ModelIDLength );
	Flow.Put( 0x12 );

	Flow.Write( Message.Convert( Buffer ), Message.Amount() );
	Flow.Put( Checksum_( Message ) );

	Flow.Put( 0xF7 );

	Flow.Commit();
ERRErr

ERREnd
ERREpilog
}

void mscrmi::Send(
	const adata_set_ &DataSet,
	const identity__ &Identity,
	flw::oflow__ &Flow )
{
	ctn::E_CMITEM( adata_ ) Data;
	epeios::row__ Row = DataSet.First();

	Data.Init( DataSet );

	while ( Row != NONE ) {
		Send(Data( Row ),  Identity, Flow );

		Row = DataSet.Next( Row );

		tht::Suspend( 40 );
	}
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
