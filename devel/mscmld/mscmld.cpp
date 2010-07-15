/*
	'mscmld' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'mscmld' header file ('mscmld.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#define MSCMLD__COMPILATION

#include "mscmld.h"

class mscmldtutor
: public ttr_tutor
{
public:
	mscmldtutor( void )
	: ttr_tutor( MSCMLD_NAME )
	{
#ifdef MSCMLD_DBG
		Version = MSCMLD_VERSION "\b\bD $";
#else
		Version = MSCMLD_VERSION;
#endif
		Owner = MSCMLD_OWNER;
		Date = "$Date$";
	}
	virtual ~mscmldtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace mscmld;

#include "mthfrc.h"


const char *mscmld::GetPitchNameLabel( pitch_name__ Name )
{
	switch( Name ) {
	case pnA:
		return "a";
		break;
	case pnB:
		return "b";
		break;
	case pnC:
		return "c";
		break;
	case pnD:
		return "d";
		break;
	case pnE:
		return "e";
		break;
	case pnF:
		return "f";
		break;
	case pnG:
		return "g";
		break;
	case pnRest:
		return "r";
		break;
	default:
		ERRu();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

const char *mscmld::GetPitchAccidentalLabel( pitch_accidental__ Accidental )
{
	switch( Accidental ) {
	case paFlat:
		return "Flat";
		break;
	case paNatural:
		return "Natural";
		break;
	case paSharp:
		return "Sharp";
		break;
	default:
		ERRu();
		break;
	}

	return NULL;	// To avoir a 'warning'.
}

static mthfrc::fraction_ &GetFraction_(
	bso::ubyte__ Base,
	bso::ubyte__ DotAmount,
	mthfrc::fraction_ &Result )
{
ERRProlog
	mthfrc::fraction Dot, Buffer;
ERRBegin
	Result.Init( 1, 1 << ( Base - 1 ) );

	Dot.Init();
	Buffer.Init();

	Dot = Result;

	while ( DotAmount-- ) {
		Buffer = Dot;
		mthfrc::Div( Buffer, mthfrc::fraction( 2 ), Dot );

		Buffer = Result;
		mthfrc::Add( Buffer, Dot, Result );
	}
ERRErr
ERREnd
ERREpilog
	return Result;
}

static inline mthfrc::fraction_ &GetFraction_(
	const duration__ &Duration,
	mthfrc::fraction_ &Result )
{
	if ( Duration.Base == 0 )
		ERRu();

	if ( Duration.Tuplet != dtNo )
		ERRl();

	return GetFraction_( Duration.Base, Duration.Modifier, Result );
}

duration__ FindDuration_(
	mthfrc::fraction_ &Fraction,
	bso::bool__ AlwaysTied )
{
	duration__ Duration;
ERRProlog
	bso::ubyte__ Base = 0;
	bso::ubyte__ Dot = 0;
	bso::bool__ Continue = true;
	mthfrc::fraction Candidate, Buffer;
ERRBegin
	Base = 1;
	Dot = 2;

	while ( 1 ) {
		Candidate.Init();

		GetFraction_( Base, Dot, Candidate );

		switch ( ( Fraction - Candidate ).GetSign() ) {
		case -1:
			switch ( Dot ) {
			case 2:
			case 1:
				Dot--;
				break;
			case 0:
				if ( Base == 7 ) {
					Duration = duration__();
					ERRReturn;
				} else {
					Base++;
					Dot = 2;
				}
				break;
			default:
				ERRc();
				break;
			}
			break;
		case 0:
			Duration = duration__( Base, Dot, AlwaysTied );
			Buffer.Init();
			Buffer = Fraction;
			mthfrc::Sub( Buffer, Candidate, Fraction );
			ERRReturn;
			break;
		case 1:
			Duration = duration__( Base, Dot, true );
			Buffer.Init();
			Buffer = Fraction;
			mthfrc::Sub( Buffer, Candidate, Fraction );
			ERRReturn;
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
	if ( Fraction.GetSign() < 0 )
		ERRc();
ERREpilog
	return Duration;
}


static void HandleOverflow_(
	mthfrc::fraction_ &Note,
	const signature_time__ &Time,
	const pitch__ &Pitch,
	const signature__ &Signature,
	mthfrc::fraction_ &Bar,
	melody_ &Melody )
{
ERRProlog
	mthfrc::fraction Buffer;
	duration__ Duration;
ERRBegin
	Buffer.Init();

	Buffer = Note;

	mthfrc::Sub( Buffer, Bar, Note );

	if ( Note.GetSign() != 1 )
		ERRc();

	while ( Bar.GetSign() != 0 ) {
		Duration = FindDuration_( Bar, true );

		if ( !Duration.IsValid() )
			ERRl();

		Melody.Append( note__( Pitch, Duration, Signature ) );
	}

	Bar.Init( Time.Numerator, Time.ComputeDenominator() );

	Buffer.Init();

	Buffer = Bar;

	mthfrc::Sub( Buffer, Note, Bar );

	if ( Bar.GetSign() != 1 )
		ERRl();

	while ( Note.GetSign() != 0 ) {
		Duration = FindDuration_( Note, false );

		if ( !Duration.IsValid() )
			ERRl();

		Melody.Append( note__( Pitch, Duration, Signature ) );
	}

ERRErr
ERREnd
ERREpilog
}

void mscmld::SplitToMatchBars(
	const melody_ &Source,
	melody_ &Target )
{
ERRProlog
	row__ Row = NONE;
	mthfrc::fraction Bar, Note, Buffer;
	signature_time__ Time;
ERRBegin
	Row = Source.First();

	while ( Row != NONE ) {
		if ( !Source( Row ).IsValid() )
			ERRu();

		if ( Time != Source( Row ).Signature.Time ) {
			Time = Source( Row ).Signature.Time;
			Bar.Init( Time.Numerator, Time.ComputeDenominator() );
		}

		Note.Init();

		GetFraction_( Source( Row ).Duration, Note );

		switch ( ( Bar - Note ).GetSign() ) {
		case 1:
			Buffer.Init();
			Buffer = Bar;
			mthfrc::Sub( Buffer, Note, Bar );

			Target.Append( Source( Row ) );
			break;
		case 0:
			Bar.Init( Time.Numerator, Time.ComputeDenominator() );
			Target.Append( Source( Row ) );
			break;
		case -1:
			HandleOverflow_( Note, Time, Source( Row ).Pitch, Source( Row ).Signature, Bar, Target );
			break;
		}

		Row = Source.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

static bso::ubyte__ GetAbsolute_( const pitch__ &Pitch )
{
	if ( ( BSO_UBYTE_MAX / 12 ) < Pitch.Octave )
		ERRu();

	bso::ubyte__ Absolute = Pitch.Octave * 12;

	switch ( Pitch.Name ) {
	case pnA:
		Absolute += 9;
		break;
	case pnB:
		Absolute += 10;
		break;
	case pnC:
		Absolute += 0;
		break;
	case pnD:
		Absolute += 2;
		break;
	case pnE:
		Absolute += 4;
		break;
	case pnF:
		Absolute += 5;
		break;
	case pnG:
		Absolute += 7;
		break;
	case pnRest:
		break;
	default:
		ERRu();
		break;
	}

	switch ( Pitch.Accidental ) {
		case paFlat:
			if ( Absolute == 0 )
				ERRu();
			Absolute--;
			break;
		case paNatural:
			break;
		case paSharp:
			if ( Absolute == BSO_UBYTE_MAX )
				ERRu();
			Absolute++;
			break;
		default:
			ERRu();
			break;
	}

	return Absolute;
}

void mscmld::Merge(
	const pitches_ &Pitches,
	const durations_ &Durations,
	const signatures_ &Signatures,
	notes_ &Notes )
{
	prow__ PRow = Pitches.First();
	drow__ DRow = Durations.First();
	srow__ SRow = Signatures.First();

	while ( ( PRow != NONE ) && ( DRow != NONE ) && ( SRow != NONE ) ) {
		Notes.Append( note__( Pitches( PRow ), Durations( DRow ), Signatures( SRow ) ) );

		PRow = Pitches.Next( PRow );
		DRow = Durations.Next( DRow );
		SRow = Signatures.Next( SRow );
	}
}


static void WriteXML_(
	const pitch__ &Pitch,
	const pitch__ &PreviousPitch,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( "Pitch" );

	Writer.PutAttribute( "Name", GetPitchNameLabel( Pitch.Name ) );

	Writer.PutAttribute( "Accidental", GetPitchAccidentalLabel( Pitch.Accidental ) );

	Writer.PutAttribute( "Octave", bso::Convert( Pitch.Octave, Buffer ) );

	Writer.PutAttribute( "Absolute", bso::Convert( GetAbsolute_( Pitch ), Buffer ) );

	if ( PreviousPitch.Name != pn_Undefined ) {
		bso::sshort__ Delta = (bso::sshort__)GetAbsolute_( Pitch ) - GetAbsolute_( PreviousPitch );

		Writer.PushTag( "Diff" );

		Writer.PutAttribute( "Interval", bso::Convert( (bso::sshort__)( ( Delta < 0 ) ? -Delta : Delta ), Buffer ) );
		Writer.PutAttribute( "Octave", bso::Convert( (bso::sshort__)( ( ( Delta < 0 ) ? -Delta : Delta ) / 12 ), Buffer ) );
		Writer.PutAttribute( "Sign", ( ( Delta < 0 ) ? "-" : ( ( Delta == 0 ) ? "0" : "+" ) ) );

		Writer.PopTag();
	}

	Writer.PopTag();
}

static void WriteXML_(
	const duration__ &Duration,
	const duration__ &PreviousDuration,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( "Duration" );

	Writer.PutAttribute( "Base", bso::Convert( Duration.Base, Buffer ) );

	Writer.PutAttribute( "Modifier", bso::Convert( Duration.Modifier, Buffer ) );

	if ( PreviousDuration.Base != 0 )
		Writer.PutAttribute( "TiedToPrevious", ( PreviousDuration.TiedToNext ? "yes" : "no" ) );

	Writer.PutAttribute( "TiedToNext", ( Duration.TiedToNext ? "yes" : "no" ) );

	Writer.PutAttribute( "Diff", ( ( Duration != PreviousDuration ) ? "yes" : "no" ) );

	Writer.PopTag();

}

static void WriteXML_(
	const signature_key__ &Key,
	const signature_key__ &PreviousKey,
	xml::writer_ &Writer )
{
	pitch_name__ Name = pn_Undefined;
	pitch_accidental__ Accidental = pa_Undefined;

	Writer.PushTag( "Key" );

	switch ( Key ) {
	case -7:
		Name = pnC;
		Accidental = paFlat;
		break;
	case -6:
		Name = pnG;
		Accidental = paFlat;
		break;
	case -5:
		Name = pnD;
		Accidental = paFlat;
		break;
	case -4:
		Name = pnA;
		Accidental = paFlat;
		break;
	case -3:
		Name = pnE;
		Accidental = paFlat;
		break;
	case -2:
		Name = pnB;
		Accidental = paFlat;
		break;
	case -1:
		Name = pnF;
		Accidental = paNatural;
		break;
	case 0:
		Name = pnC;
		Accidental = paNatural;
		break;
	case 1:
		Name = pnG;
		Accidental = paNatural;
		break;
	case 2:
		Name = pnD;
		Accidental = paNatural;
		break;
	case 3:
		Name = pnA;
		Accidental = paNatural;
		break;
	case 4:
		Name = pnE;
		Accidental = paNatural;
		break;
	case 5:
		Name = pnB;
		Accidental = paNatural;
		break;
	case 6:
		Name = pnF;
		Accidental = paSharp;
		break;
	case 7:
		Name = pnC;
		Accidental = paSharp;
		break;
	default:
		ERRu();
		break;
	}

	Writer.PutAttribute( "Name", GetPitchNameLabel( Name ) );

	Writer.PutAttribute( "Accidental", GetPitchAccidentalLabel( Accidental ) );

	Writer.PutAttribute( "Diff", ( Key != PreviousKey ) ? "yes" : "no" );

	Writer.PopTag();
}

static void WriteXML_(
	const signature_time__ &Time,
	const signature_time__ &PreviousTime,
	xml::writer_ &Writer )
{
	bso::integer_buffer__ Buffer;

	Writer.PushTag( "Time" );

	Writer.PutAttribute( "Numerator", bso::Convert( Time.Numerator, Buffer ) );

	Writer.PutAttribute( "Denominator", bso::Convert( Time.ComputeDenominator(), Buffer ) );

	Writer.PutAttribute( "Diff", ( Time != PreviousTime ) ? "yes" : "no" );

	Writer.PopTag();
}

static void WriteXML_(
	const signature__ &Signature,
	const signature__ &PreviousSignature,
	xml::writer_ &Writer )
{
	Writer.PushTag( "Signature" );

	Writer.PutAttribute( "Diff", ( Signature != PreviousSignature ) ? "yes" : "no" );

	WriteXML_( Signature.Key, PreviousSignature.Key, Writer );

	WriteXML_( Signature.Time, PreviousSignature.Time, Writer );

	Writer.PopTag();
}



static void WriteXML_(
	const note__ &Note,
	const note__ &PreviousNote,
	xml::writer_ &Writer )
{
	if ( Note.Pitch.Name == pnRest ) {
		Writer.PushTag( "Rest" );
	} else {
		Writer.PushTag( "Note" );

		WriteXML_( Note.Pitch, PreviousNote.Pitch, Writer );
	}

	WriteXML_( Note.Duration, PreviousNote.Duration, Writer );

	Writer.PopTag();
}

bso::bool__ Adjust_(
	mthfrc::fraction_ &BarFraction,
	const mthfrc::fraction_ &NoteFraction )
{
	bso::bool__ BarComplete = false;
ERRProlog
	mthfrc::fraction Buffer;
ERRBegin
#ifdef DEBUG
	cio::cout << BarFraction << txf::tab << NoteFraction << txf::tab << txf::sync;
#endif

	switch ( ( BarFraction - NoteFraction ).GetSign() ) {
	case -1 :
		ERRu();
		break;
	case 0:
		BarComplete = true;
		break;
	case 1:
		Buffer.Init();
		Buffer = BarFraction;
		mthfrc::Sub( Buffer, NoteFraction, BarFraction );
		break;
	default:
		ERRc();
		break;
	}
#ifdef DEBUG
	cio::cout << BarFraction << txf::nl;
#endif

ERRErr
ERREnd
ERREpilog
	return BarComplete;
}

void mscmld::WriteXML(
	const melody_ &Melody,
	xml::writer_ &Writer )
{
ERRProlog
	row__ Row = NONE;
	mthfrc::fraction BarFraction, NoteFraction;
	note__ PreviousNote, Note;
	bso::bool__ BarClosed = true;
ERRBegin
	Row = Melody.First();

	Writer.PushTag( "Melody" );

	while ( Row != NONE ) {
		if ( !Melody( Row ).IsValid() )
			ERRu();

		if ( Note.IsValid() )
			if ( Note.Pitch.Name != pnRest )
				PreviousNote = Note;
			else
				PreviousNote.Duration = Note.Duration;


		Note = Melody( Row );

		if ( Note.Signature != PreviousNote.Signature ) {
			if ( !BarClosed && ( PreviousNote.IsValid() ) )
				ERRu();
		}
		
		if ( BarClosed ) {
			Writer.PushTag( "Bar" );

			WriteXML_( Note.Signature, PreviousNote.Signature, Writer );

			Writer.PushTag( "Figures" );

			BarFraction.Init( Note.Signature.Time.Numerator, Note.Signature.Time.ComputeDenominator() );
		}

		WriteXML_( Note, PreviousNote, Writer );

		NoteFraction.Init();

		GetFraction_( Note.Duration, NoteFraction );

		if ( BarClosed = Adjust_( BarFraction, NoteFraction ) ) {
			Writer.PopTag();	// For 'Notes' tag.
			Writer.PopTag();	// For 'Bar' tag.
		}

		Row = Melody.Next( Row );
	}
/*
	while ( Row != NONE ) {
		if ( !Melody( Row ).IsValid() )
			ERRu();

		if ( Signature != Melody( Row ).Signature ) {
			if ( !BarClosed )
				ERRu();

			Writer.PushTag( "Bar" );

			WriteXML_( Melody( Row ).Signature, Signature, Writer );

			Writer.PushTag( "Figures" );

			Signature = Melody( Row ).Signature;

			BarFraction.Init( Signature.Time.Numerator, Signature.Time.ComputeDenominator() );
		} else if ( BarClosed ) {
			Writer.PushTag( "Bar" );

			WriteXML_( Melody( Row ).Signature, Signature, Writer );

			Writer.PushTag( "Figures" );
		}

		WriteXML_( Melody( Row ), PreviousNote, Writer );

		NoteFraction.Init();

		GetFraction_( Melody( Row ).Duration, NoteFraction );

		if ( BarClosed = Adjust_( BarFraction, NoteFraction ) ) {
			Writer.PopTag();	// For 'Notes' tag.
			Writer.PopTag();	// For 'Bar' tag.
			BarFraction.Init( Signature.Time.Numerator, Signature.Time.ComputeDenominator() );
		}

		PreviousNote = Melody( Row );

		Row = Melody.Next( Row );
	}
*/
	if ( !BarClosed )
		Writer.PopTag();	// For 'Bar' tag.

	Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mscmldpersonnalization
: public mscmldtutor
{
public:
	mscmldpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mscmldpersonnalization( void )
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

static mscmldpersonnalization Tutor;

ttr_tutor &MSCMLDTutor = Tutor;
