/*
	Header for the 'mscrmi' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef MSCRMI__INC
#define MSCRMI__INC

#define MSCRMI_NAME		"MSCRMI"

#define	MSCRMI_VERSION	"$Revision$"

#define MSCRMI_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &MSCRMITutor;

#if defined( XXX_DBG ) && !defined( MSCRMI_NODBG )
#define MSCRMI_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MuSiC Roland MIDI Implementation 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "str.h"
#include "ctn.h"
#include "lstctn.h"
#include "xml.h"
#include "mscmdm.h"

namespace mscrmi {
	typedef str::string_	label_;
	typedef str::string		label;

	typedef bso::char__ datum__;

	typedef str::string_ data_;
	typedef str::string	data;

	E_ROW( row__ );

	typedef bso::ulong__ xaddress__;	// 'eXtended address'.
	// 0 - 29 : adresse du paramètre.
	// 30 - 31 : taille du 'stencil', en octets.
	typedef bso::ulong__ address__;
	typedef bso::ulong__ segment__;
	typedef bso::ulong__ offset__;	
	typedef bso::ushort__ size__;
	typedef bso::ulong__ mask__;
	typedef bso::ulong__ stencil__;

	struct bloc__ {
		address__ Address;
		size__ Size;
	};

	typedef bch::E_BUNCH_( bloc__ ) blocs_;
	E_AUTO( blocs );

#define MSCRMI_UNDEFINED_ADDRESS	BSO_ULONG_MAX

	// 'Assigned data'
	class adata_
	: public data_
	{
	public:
		struct s
		: public data_::s
		{
			//o The base address.
			address__ Address;
		} &S_;
		adata_( s &S )
		: S_( S ),
		  data_( S )
		{
		}
		void reset( bso::bool__ P = true )
		{
			if ( P )
				S_.Address = MSCRMI_UNDEFINED_ADDRESS;

			data_::reset( P );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			data_::plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY &MM )
		{
			data_::plug( MM );
		}
		adata_ &operator =( const adata_ &AD )
		{
			S_.Address = AD.S_.Address;

			data_::operator =( AD );

			return *this;
		}
		//f Initialisation.
		void Init( void )
		{
			S_.Address = MSCRMI_UNDEFINED_ADDRESS;

			data_::Init();
		}
		E_RWDISCLOSE_( address__, Address );
	};

	E_AUTO( adata )

	typedef ctn::E_XMCONTAINER_( adata_ ) adata_set_;
	E_AUTO( adata_set );


#define MSCRMI_MASK( size, pos )	( ( ~0UL >> ( 32 - ( size ) ) ) << ( pos ) )


// Un 'stencil' désigne le masque à appliquer sur un 'xaddress__' pour en récupèrer le segment.
#define MSCRMI_STENCIL_MASK_LENGTH		2
#define MSCRMI_STENCIL_MASK_POSITION	( 32 - MSCRMI_STENCIL_MASK_LENGTH )
#define MSCRMI_STENCIL_MASK				MSCRMI_MASK( MSCRMI_STENCIL_MASK_LENGTH, MSCRMI_STENCIL_MASK_POSITION )

#define MSCRMI_ADDRESS_MASK			( ~MSCRMI_STENCIL_MASK )

#define MSCRMI__EMBED_VALUE( address, mask, position, value )	( ( address & ~mask ) | ( 

	inline xaddress__ _EmbedValue(
		xaddress__ Address,
		mask__ Mask,
		bso::ubyte__ Position,
		bso::ubyte__ Value )
	{
		return ( Address & ~Mask ) | ( Value << Position );
	}

	inline xaddress__ _SetStencilSize(
		xaddress__ Address,
		bso::ubyte__ Size )
	{
#ifdef MSCRMI_DBG
		if ( Size > 3 )
			ERRu();
#endif
		return _EmbedValue( Address, MSCRMI_STENCIL_MASK, MSCRMI_STENCIL_MASK_POSITION, Size );
	}

	inline bso::ubyte__ _GetEmbedValue(
		xaddress__ Address,
		mask__ Mask,
		bso::ubyte__ Position )
	{
		return (bso::ubyte__)( ( Address & Mask ) >> Position );
	}

	inline bso::ubyte__ _StencilSize( xaddress__ Address )
	{
		return _GetEmbedValue( Address, MSCRMI_STENCIL_MASK, MSCRMI_STENCIL_MASK_POSITION );
	}

	inline mask__ _Stencil( bso::ubyte__ Size )
	{
		return ~MSCRMI_MASK( 32 - ( Size << 3 ), 0 );
	}

	inline mask__ _Stencil( xaddress__ Address )
	{
		return _Stencil( _StencilSize( Address ) );
	}

	inline address__ _Address( xaddress__ Address )
	{
		mask__ Mask = MSCRMI_ADDRESS_MASK;
		return Address & MSCRMI_ADDRESS_MASK;
	}

	inline segment__ _Segment( xaddress__ Address )
	{
		return _Address( Address ) & _Stencil( Address );
	}

	inline offset__ _Offset( xaddress__ Address )
	{
		return _Address( Address ) & ~_Stencil( Address );
	}

	class parameter_definition_
	{
	public:
		struct s {
			label_::s Label;
			xaddress__ Address;
			size__ Size;
			row__ GroupRow;	// Position du groupe de paramètres (paramètre de taill nulle) auquel il appartient.
		} &S_;
		label_ Label;
		parameter_definition_( s &S )
		: S_( S ),
		  Label( S.Label )
		{}
		void reset( bso::bool__ P = true )
		{
			Label.reset( P );
			S_.Address = MSCRMI_UNDEFINED_ADDRESS;
			S_.Size = 0;
			S_.GroupRow = NONE;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Label.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Label.plug( MM );
		}
		parameter_definition_ &operator =( const parameter_definition_ &PD )
		{
			Label = PD.Label;
			S_.Address = PD.S_.Address;
			S_.Size = PD.S_.Size;
			S_.GroupRow = PD.S_.GroupRow;

			return *this;
		}
		void Init( void )
		{
			Label.Init();
			S_.Address = MSCRMI_UNDEFINED_ADDRESS;
			S_.Size = 0;
			S_.GroupRow = NONE;
		}
		void Init(
			const label_ &Label,
			xaddress__ Address,
			size__ Size,
			row__ GroupRow )
		{
			this->Label.Init( Label );
			S_.Address = Address;
			S_.Size = Size;
			S_.GroupRow = GroupRow;
		}
		address__ Address( void ) const
		{
			return _Address( S_.Address );
		}
		segment__ Segment( void ) const
		{
			return _Segment( S_.Address );
		}
		offset__ Offset( void ) const
		{
			return _Offset( S_.Address );
		}
		E_RODISCLOSE_( size__, Size )
		E_RODISCLOSE_( row__, GroupRow )
	};

	E_AUTO( parameter_definition )

	typedef lstctn::E_LXMCONTAINERt_( parameter_definition_, row__ ) parameter_definitions_;
	E_AUTO( parameter_definitions );

	void Print(
		const parameter_definitions_ &Definitions,
		xml::writer_ &Writer );

	class midi_implementation_
	{
	public:
		struct s {
			str::string_::s ModelID;
			str::string_::s ModelLabel;
			parameter_definitions_::s Definitions;
		};
		str::string_ ModelID;
		str::string_ ModelLabel;
		parameter_definitions_ Definitions;
		midi_implementation_( s &S )
		: ModelID( S.ModelID ),
			ModelLabel( S.ModelLabel ),
			Definitions( S.Definitions )
		{}
		void reset( bso::bool__ P = true )
		{
			ModelID.reset( P );
			ModelLabel.reset( P );
			Definitions.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			ModelID.plug( MM );
			ModelLabel.plug( MM );
			Definitions.plug( MM );
		}
		midi_implementation_ &operator=( const midi_implementation_ &MI )
		{
			ModelID = MI.ModelID;
			ModelLabel = MI.ModelLabel;
			Definitions = MI.Definitions;

			return *this;
		}
		void Init( void )
		{
			ModelID.Init();
			ModelLabel.Init();
			Definitions.Init();
		}
		void Init(
			const str::string_ &ModelID,
			const str::string_ &ModelLabel )
		{
			this->ModelID.Init( ModelID );
			this->ModelLabel.Init( ModelLabel );
			Definitions.Init();
		}
	};

	E_AUTO( midi_implementation );

	// Donnet les blocs d'adresses contigües d'une implémentation MIDI.
	void GetBlocs(
		const parameter_definitions_ &Definitions,
		blocs_ &Blocs );

	inline void GetBlocs(
		const midi_implementation_ &Implementation,
		blocs_ &Blocs )
	{
		GetBlocs( Implementation.Definitions, Blocs );
	}

	void Print(
		const midi_implementation_ &Implementation,
		xml::writer_ &Writer );

	enum fill_status__ {
		fsOK,
		fsBrowserError,	// Une erreur s'est prioduite durant le parsage. Consulter le 'browser' pour connaître l'erreur.
		fsUnexpectedTag,
		fsAttributeDefinedTwice,
		fsUnexpectedAttribute,
		fsMissingAttribute,
		fsBadValue,
		fs_amount,
		fs_Undefined
	};

	const char *Label( fill_status__ Status );

	fill_status__ Fill(
		xml::browser___ &Browser,
		midi_implementation_ &Implementation );

	typedef ctn::E_XCONTAINER_( midi_implementation_ ) midi_implementations_;
	E_AUTO( midi_implementations );

	void Print(
		const midi_implementations_ &Implementations,
		xml::writer_ &Writer );

	fill_status__ Fill(
		xml::browser___ &Browser,
		midi_implementations_ &Implementations );

	void RequestData(
		const str::string_ &ModelID,
		address__ Address,
		size__ Size,
		flw::oflow__ &Flow );

	class parameter_ {
	public:
		struct s {
			row__ Row;
			data_::s Data;
		} &S_;
		data_ Data;
		parameter_( s &S )
		: S_( S ),
		Data( S.Data )
		{}
		void reset( bso::bool__ P = true )
		{
			Data.reset( P );
			S_.Row = NONE;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Data.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Data.plug( MM );
		}
		parameter_ &operator =( const parameter_ &P )
		{
			S_.Row = P.S_.Row;
			Data = P.Data;

			return *this;
		}
		void Init( row__ Row = NONE )
		{
			Data.Init();
			S_.Row = Row;
		}
		void Ini(
			row__ Row,
			const data_ &Data )
		{
			S_.Row = Row;
			this->Data.Init( Data );
		}
		E_RODISCLOSE_( row__, Row )
	};

	E_AUTO( parameter );

	typedef ctn::E_XMCONTAINER_( parameter_ ) parameters_;
	E_AUTO( parameters );

	void Fill(
		const adata_set_ &DataSet,
		const midi_implementation_ &Implementation,
		parameters_ &Parameters );

	void Print(
		const parameters_ &Parameters,
		const parameter_definitions_ &Definitions,
		xml::writer_ &Writer );

	inline void Print(
		const parameters_ &Parameters,
		const midi_implementation_ &Implementation,
		xml::writer_ &Writer )
	{
		Print( Parameters, Implementation.Definitions, Writer );
	}

	enum extraction_status__ {
		xsOK,
		xsChecksumError,
		xsBadData,
		xsIncorrectData,
		sx_amount,
		xs_Undefined
	};

	extraction_status__ ExtractData(
		flw::iflow__ &Flow,
		const str::string_ &ModelID,
		mscmdm::origin__ Origin,
		address__ &Address,
		data_ &Data );


	extraction_status__ RetrieveData(
		flw::oflow__ &OFlow,
		flw::iflow__ &IFlow,
		address__ Address,
		size__ Size,
		const str::string_ &ModelID,
		adata_ &Data );

	extraction_status__ RetrieveDataSet(
		flw::oflow__ &OFlow,
		flw::iflow__ &IFlow,
		const blocs_ &Blocs,
		const str::string_ &ModelID,
		adata_set_ &Data );

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
