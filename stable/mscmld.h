/*
	Header for the 'mscmld' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#ifndef MSCMLD__INC
#define MSCMLD__INC

#define MSCMLD_NAME		"MSCMLD"

#define	MSCMLD_VERSION	"$Revision$"

#define MSCMLD_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &MSCMLDTutor;

#if defined( XXX_DBG ) && !defined( MSCMLD_NODBG )
#define MSCMLD_DBG
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
//D MuSiC MeLody Description 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bch.h"
#include "xml.h"

namespace mscmld {

	inline bso::ubyte__ Log2( bso::ubyte__ Value )
	{
		switch ( Value ) {
		case 0:
			ERRu();
			break;
		case 1:
			return 0;
			break;
		case 2:
			return 1;
			break;
		case 4:
			return 2;
			break;
		case 8:
			return 3;
			break;
		case 16:
			return 4;
			break;
		case 32:
			return 5;
			break;
		case 64:
			return 6;
			break;
		case 128:
			return 7;
			break;
		default:
			ERRc();
			break;
		}

		return 0;	// To avoid a warning;
	}

	enum pitch_name__ {
		pnA,
		pnB,
		pnC,
		pnD,
		pnE,
		pnF,
		pnG,
		pnRest,
		pn_amount,
		pn_Undefined
	};

	const char *GetPitchNameLabel( pitch_name__ Name );

	enum pitch_accidental__ {
		paFlat,
		paNatural,
		paSharp,
		pa_amount,
		pa_Undefined
	};

	const char *GetPitchAccidentalLabel( pitch_accidental__ Accidental );

	typedef bso::ubyte__ pitch_octave__;
#define MSCMLD_UNDEFINED_PITCH_OCTAVE BSO_UBYTE_MAX

	struct pitch__ {
		pitch_name__ Name;
		pitch_accidental__ Accidental;
		pitch_octave__ Octave;
		void reset( bso::bool__ P = true )
		{
			Name = pn_Undefined;
			Accidental = pa_Undefined;
			Octave = MSCMLD_UNDEFINED_PITCH_OCTAVE;
		}
		pitch__( void )
		{
			reset( false );
		}
		pitch__(
			pitch_octave__ Octave,
			pitch_name__ Name,
			pitch_accidental__ Accidental = paNatural )
		{
			reset( false );

			this->Name = Name;
			this->Accidental = Accidental;
			this->Octave = Octave;
		}
		pitch__(
			pitch_name__ Name,
			pitch_octave__ Octave,
			pitch_accidental__ Accidental = paNatural )
		{
			reset( false );

			this->Name = Name;
			this->Accidental = Accidental;
			this->Octave = Octave;
		}
		pitch__(
			pitch_name__ Name,
			pitch_accidental__ Accidental,
			pitch_octave__ Octave )
		{
			this->Name = Name;
			this->Accidental = Accidental;
			this->Octave = Octave;
		}
		bso::bool__ IsValid( void ) const
		{
			return ( Name != pn_Undefined ) && ( Accidental != pa_Undefined ) && ( Octave != MSCMLD_UNDEFINED_PITCH_OCTAVE );
		}
	};

	inline int operator ==(
		const pitch__ &Op1,
		const pitch__ &Op2 )
	{
		return ( Op1.Name == Op2.Name ) && ( Op1.Accidental == Op2.Accidental ) && ( Op1.Octave == Op2.Octave );
	}

	inline int operator !=(
		const pitch__ &Op1,
		const pitch__ &Op2 )
	{
		return ( Op1.Name != Op2.Name ) || ( Op1.Accidental != Op2.Accidental ) || ( Op1.Octave != Op2.Octave );
	}

	E_ROW( prow__ );

	typedef bch::E_BUNCHt_( pitch__, prow__ ) pitches_;
	E_AUTO( pitches )

	enum duration_tuplet__ {
		// Not a member of a tuplet.
		dtNo,
		// First member of a tuplet.
		dtFirst,
		// Member (not the first one) or a tuplet.
		dtYes,
		dt_amount,
		dt_Undefined
	};

	struct duration__
	{
		bso::ubyte__ Base;	// 1 : whole, 2 : half, 3 : quarter, 4 : eight, ...
		bso::ubyte__ Modifier;	// Amount of dot.
		duration_tuplet__ Tuplet;
		bso::bool__ TiedToNext;
		void reset( bso::bool__ P = true )
		{
			Base = 0;
			Modifier = 0;
			Tuplet = dt_Undefined;
			TiedToNext = false;
		}
		duration__( void )
		{
			reset( false );
		}
		duration__(
			bso::ubyte__ Base,
			bso::ubyte__ Modifier = 0,
			duration_tuplet__ Tuplet = dtNo,
			bso::bool__ TiedToNext = false )
		{
			this->Base = Base;
			this->Modifier = Modifier;
			this->Tuplet = Tuplet;
			this->TiedToNext = TiedToNext;
		}
		duration__(
			bso::ubyte__ Base,
			bso::ubyte__ Modifier,
			bso::bool__ TiedToNext,
			duration_tuplet__ Tuplet = dtNo )
		{
			this->Base = Base;
			this->Modifier = Modifier;
			this->Tuplet = Tuplet;
			this->TiedToNext = TiedToNext;
		}
		bso::bool__ IsValid( void ) const
		{
			return Base != 0;
		}
	};

	inline int operator ==(
		duration__ Op1,
		duration__ Op2 )
	{
		return ( ( Op1.Base == Op2.Base ) && ( Op1.Modifier == Op2.Modifier ) );
	}

	inline int operator !=(
		duration__ Op1,
		duration__ Op2 )
	{
		return ( ( Op1.Base != Op2.Base ) || ( Op1.Modifier != Op2.Modifier ) );
	}

	E_ROW( drow__ );

	typedef bch::E_BUNCHt_( duration__, drow__ ) durations_;
	E_AUTO( durations )

	typedef bso::sbyte__ signature_key__;	// -1: 1 flat, -2, 2 flats, ..., 0 : C key, 1 : 1 sharp, 2 : 2 sharps, ...
#define MSCMLD_UNDEFINED_KEY_SIGNATURE	BSO_SBYTE_MAX

	struct signature_time__ {
		bso::ubyte__ Numerator;
		bso::ubyte__ DenominatorPower;	// Real denominatro = 2 ^ 'DenominatorPower'.
		void reset( bso::bool__ P = true )
		{
			Numerator = 0;
			DenominatorPower = 0;
		}
		signature_time__( void )
		{
			reset( false );
		}
		signature_time__ (
			bso::ubyte__ Numerator,
			bso::ubyte__ Denominator )
		{
			reset( false );

			if ( Numerator == 0 )
				ERRu();

			this->Numerator = Numerator;

			this->DenominatorPower = Log2( Denominator );
		}
		bso::bool__ IsValid( void ) const
		{
			return ( ( Numerator != 0 ) && ( DenominatorPower != 0 ) );
		}
		bso::ubyte__ ComputeDenominator ( void ) const
		{
#ifdef MSCMLD_DBG
			if ( !IsValid() )
				ERRu();
#endif
			return 1 << DenominatorPower;
		}
	};

	inline int operator ==(
		const signature_time__ &Op1,
		const signature_time__ &Op2 )
	{
		return ( ( Op1.Numerator == Op2.Numerator ) && ( Op1.DenominatorPower == Op2.DenominatorPower ) );
	}

	inline int operator !=(
		const signature_time__ &Op1,
		const signature_time__ &Op2 )
	{
		return ( ( Op1.Numerator != Op2.Numerator ) || ( Op1.DenominatorPower != Op2.DenominatorPower ) );
	}

	struct signature__ {
		signature_key__ Key;
		signature_time__ Time;
		void reset( bso::bool__ P = true )
		{
			Key = MSCMLD_UNDEFINED_KEY_SIGNATURE;
			Time.reset( P );
		}
		signature__( void )
		{
			reset( false );
		}
		signature__(
			const signature_key__ &Key,
			const signature_time__ &Time )
		{
			reset( false );

			this->Key = Key;
			this->Time = Time;
		}
		signature__(
			const signature_time__ &Time,
			const signature_key__ &Key )
		{
			reset( false );

			this->Key = Key;
			this->Time = Time;
		}
		bso::bool__ IsValid( void ) const
		{
			return Time.IsValid() && ( Key != MSCMLD_UNDEFINED_KEY_SIGNATURE );
		}
	};

	inline int operator ==(
		const signature__ &Op1,
		const signature__ &Op2 )
	{
		return ( ( Op1.Key == Op2.Key ) && ( Op1.Time == Op2.Time ) );
	}

	inline int operator !=(
		const signature__ &Op1,
		const signature__ &Op2 )
	{
		return ( ( Op1.Key != Op2.Key ) || ( Op1.Time != Op2.Time ) );
	}

	E_ROW( srow__ );

	typedef bch::E_BUNCHt_( signature__, srow__ ) signatures_;
	E_AUTO( signatures )


	struct note__ {
		pitch__ Pitch;
		duration__ Duration;
		signature__ Signature;
		void reset( bso::bool__ P = true )
		{
			Pitch.reset( P );
			Duration.reset( P );
			Signature.reset( P );
		}
		note__( void )
		{
			reset( false );
		}
		note__(
			const pitch__ &Pitch,
			const duration__ &Duration,
			const signature__ &Signature )
		{
			reset( false );

			this->Pitch = Pitch;
			this->Duration = Duration;
			this->Signature = Signature;
		}
		bso::bool__ IsValid( void ) const
		{
			return ( Pitch.IsValid() && Duration.IsValid() && Signature.IsValid() );
		}
	};

	E_ROW( row__ );

	typedef bch::E_BUNCHt_( note__, row__ ) notes_;
	E_AUTO( notes );

	void Merge(
		const pitches_ &Pitches,
		const durations_ &Durations,
		const signatures_ &Signatures,
		notes_ &Notes );

	typedef notes_	melody_;
	E_AUTO( melody );


	void SplitToMatchBars(
		const melody_ &Source,
		melody_ &Target );

	void WriteXML(
		const melody_ &Melody,
		xml::writer_ &Writer );
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
