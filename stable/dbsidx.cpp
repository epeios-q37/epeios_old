/*
	'dbsidx' library by Claude SIMON (csimon@epeios.org)
	Requires the 'dbsidx' header file ('dbsidx.h').
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

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

#define DBSIDX__COMPILATION

#include "dbsidx.h"

class dbsidxtutor
: public ttr_tutor
{
public:
	dbsidxtutor( void )
	: ttr_tutor( DBSIDX_NAME )
	{
#ifdef DBSIDX_DBG
		Version = DBSIDX_VERSION "\b\bD $";
#else
		Version = DBSIDX_VERSION;
#endif
		Owner = DBSIDX_OWNER;
		Date = "$Date$";
	}
	virtual ~dbsidxtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flf.h"

using namespace dbsidx;

#define TREE_FILE_NAME_EXTENSION	".edt"
#define QUEUE_FILE_NAME_EXTENSION	".edq"
// #define ROOT_FILE_NAME_EXTENSION	".edr"

/*
#include "cio.h"

void DisplayTree(
	const idxbtr::E_IBTREEt_( rrow__ ) &Index,
	rrow__ Root,
	txf::text_oflow__ &cout )
{
	rrow__ Row = Index.First( Root );

	while ( Row != NONE ) {
		cout << *Row << txf::tab << txf::sync;

		Row = Index.Next( Row );
	}

	cout << txf::nl;
}

void DisplayQueue(
	const idxque::E_IQUEUEt_( rrow__) &Index,
	rrow__ Root,
	txf::text_oflow__ &cout )
{
	rrow__ Row = Root;

	while ( Root != NONE ) {
		Row = Root;

		Root = Index.Previous( Root );
	}

	while ( Row != NONE ) {
		cout << *Row << txf::tab << txf::sync;

		Row = Index.Next( Row );
	}

	cout << txf::nl;
}

void Display(
	const idxbtq::E_INDEXt_( rrow__) &Index,
	rrow__ Root,
	txf::text_oflow__ &cout )
{
	cout <<  "Q :" << txf::tab << txf::sync;
	DisplayQueue( Index, Root, cout );
	cout << "T :" << txf::tab << txf::sync;
	DisplayTree( Index, Root, cout );
}
*/

bso::sign__ dbsidx::index_::_Seek(
	const datum_ &Datum,
	behavior__ EqualBehavior,
	rrow__ &Row,
	bso::ubyte__ &Round,
	dbsctt::_cache_ &Cache ) const
{
	bso::sign__ Result = 0;
ERRProlog
	datum DatumToCompare;
	idxbtq::E_ISEEKERt__( rrow__ ) Seeker;
ERRBegin
	Round = 0;

	if ( S_.Root == NONE ) {
		Row = NONE;

		ERRReturn;
	}

	Seeker.Init( BaseIndex, S_.Root );

	Row = Seeker.GetCurrent();

	while ( Row != NONE ) {
		DatumToCompare.Init();

		_Retrieve( Row, DatumToCompare, Cache );

		switch ( Result = S_.Sort->Compare( Datum, DatumToCompare ) ) {
		case 0:
			switch ( EqualBehavior ) {
			case bStop:
				Row = NONE;	// Pour sortir de la boucle.
				break;
			case bGreater:
				Row = Seeker.SearchGreater();
				break;
			case bLesser:
				Row = Seeker.SearchLesser();
				break;
			case bStopIfOneChildMissing:
				if ( !Seeker.HasLesser() || !Seeker.HasGreater() )
					Row = NONE;	// Popur sortir de la boucle.
				else if ( *Row & 1 )	// Petit générateur aléatoire (sans doute inutile ?).
					Row = Seeker.SearchLesser();
				else
					Row = Seeker.SearchGreater();
				break;
			default:
				ERRu();
			}
			break;
		case 1:
			Row = Seeker.SearchGreater();
			break;
		case -1:
			Row = Seeker.SearchLesser();
			break;
		default:
			ERRc();
			break;
		}

		Round++;
	}

	Row = Seeker.GetCurrent();
ERRErr
ERREnd
ERREpilog
	return Result;
}

rrow__ dbsidx::index_::_SearchStrictGreater( rrow__ Row ) const
{
	rrow__ Buffer = BaseIndex.GetTreeGreater( Row );
	rrow__ Candidate = NONE;

	while ( ( Buffer != NONE ) && ( Compare( Buffer, Row ) == 0 ) )
		Buffer = BaseIndex.GetTreeGreater( Buffer );

	if ( Buffer != NONE ) {
		Candidate = Buffer;

		Buffer = BaseIndex.GetTreeLesser( Buffer );

		while ( ( Buffer != NONE ) && ( Compare( Buffer, Row ) != 0 ) ) {
			Candidate = Buffer;

			Buffer = BaseIndex.GetTreeLesser( Buffer );
		}

		if ( Buffer != NONE ) {
			Buffer = _SearchStrictGreater( Buffer );

			if ( Buffer != NONE )
				Candidate = Buffer;
		}
	} else {
		Buffer = BaseIndex.GetTreeParent( Row );

		if ( Buffer != NONE ) {

			if ( BaseIndex.IsTreeGreater( Row ) ) {
				while ( ( Buffer != NONE ) && BaseIndex.IsTreeGreater( Buffer ) )
					Buffer = BaseIndex.GetTreeParent( Buffer );
			} else
				Buffer = Row;

			if ( Buffer != NONE ) {
				Buffer = BaseIndex.GetTreeParent( Buffer );

				if ( Buffer != NONE ) {
					if ( Compare( Row, Buffer ) != 0 )
						Candidate = Buffer;
					else
						Candidate = _SearchStrictGreater( Buffer );
				}
			}
		}
	}

	return Candidate;
}


bso::ubyte__ dbsidx::index_::Index(
	rrow__ Row,
	extremities__ *Extremities,
	dbsctt::_cache_ &Cache )
{
	bso::ubyte__ Round = 0;
ERRProlog
	datum DatumToCompare;
	datum Datum;
	rrow__ TargetRow = NONE;
	bso::sign__ Result;
//	tol::buffer__ Buffer;
//	cio::aware_cout___ cout;
ERRBegin

	if ( _Content().Extent() > BaseIndex.Extent() )
		BaseIndex.Allocate( _Content().Extent(), aem::mDefault );

	if ( S_.Root == NONE ) {
		S_.Root = Row;

		BaseIndex.BecomeRoot( Row );

		ERRReturn;
	}

	Datum.Init();

	_Retrieve( Row, Datum, *(dbsctt::_cache_ *)NULL );


	if ( Extremities != NULL ) {
		if ( Extremities->Smallest == NONE )
			Extremities->Smallest = BaseIndex.First( S_.Root );

		TargetRow = Extremities->Smallest;

		DatumToCompare.Init();

		_Retrieve( TargetRow, DatumToCompare, Cache );

		switch ( Result = S_.Sort->Compare( Datum, DatumToCompare ) ) {
		case 0:
			Result = -1;	// Pour forcer son positionnement en tant que premier.
		case -1:
			break;
		case 1:
			TargetRow = NONE;
			break;
		default:
			ERRc();
			break;
		}
	}

	if ( ( TargetRow == NONE ) && ( Extremities != NULL ) ) {
		if ( Extremities->Greatest == NONE )
			Extremities->Greatest = BaseIndex.Last( S_.Root );

		TargetRow = Extremities->Greatest;

		DatumToCompare.Init();

		_Retrieve( TargetRow, DatumToCompare, Cache );

		switch ( Result = S_.Sort->Compare( Datum, DatumToCompare ) ) {
		case 0:
			Result = 1;	// Pour forcer son positionnement en tant que dernier.
		case 1:
			break;
		case -1:
			TargetRow = NONE;
			break;
		default:
			ERRc();
			break;
		}
	} 

	if ( TargetRow == NONE )
		Result = _Seek( Datum, bStopIfOneChildMissing, TargetRow, Round, Cache );
	else
		Extremities->Used++;

//	cout << tol::DateAndTime( Buffer ) << txf::tab << &BaseIndex << txf::nl <<  txf::sync;
//	Display( BaseIndex, S_.Root, cout );

	switch ( Result ) {
	case -1:
		if ( ( Extremities != NULL ) && ( Extremities->Smallest == TargetRow ) )
			Extremities->Smallest = Row;
		S_.Root = BaseIndex.BecomeLesser( Row, TargetRow, S_.Root );
		break;
	case 0:	// Pas de problème avec la gestion des 'extremities', 
		if ( !BaseIndex.TreeHasLesser( TargetRow ) )
			S_.Root = BaseIndex.BecomeLesser( Row, TargetRow, S_.Root );
		else if ( !BaseIndex.TreeHasGreater( TargetRow ) )
			S_.Root = BaseIndex.BecomeGreater( Row, TargetRow, S_.Root );
		else
			ERRc();
		break;
	case 1:
		if ( ( Extremities != NULL ) && ( Extremities->Greatest == TargetRow ) )
			Extremities->Greatest = Row;
		S_.Root = BaseIndex.BecomeGreater( Row, TargetRow, S_.Root );
		break;
	default:
		ERRc();
		break;
	}

//	Display( BaseIndex, S_.Root, cout );

#ifdef DBSIDX_DBG
	if ( ( Round > 32 ) || ( ( 2UL << ( Round >> 1 ) ) > _Content().Amount() ) )
		S_.Root = S_.Root;	// Juste pour pouvoir placer un point d'arrêt.
#endif

#ifdef DBSIDX_DBG
	if ( TargetRow == NONE )
		ERRc();
#endif

ERRErr
ERREnd
	_Touch( false );
ERREpilog
	return Round;
}

rrow__ dbsidx::index_::Seek( 
	const datum_ &Datum,
	behavior__ EqualBehavior,
	bso::sign__ &Sign ) const
{
	rrow__ Row = NONE;
	bso::ubyte__ Round;

	if ( S_.Root == NONE )
		return NONE;

	Sign = _Seek( Datum, EqualBehavior, Row, Round, *(dbsctt::_cache_ *)NULL );

#ifdef DBSIDX_DBG
	if ( Row == NONE )
		ERRc();
#endif

	return Row;
}

rrow__ dbsidx::index_::Test( void ) const
{
	rrow__ Row = NONE;
ERRProlog
	datum Datum;
ERRBegin
	if ( S_.Root == NONE )
		ERRReturn;

	Row = First();

	Datum.Init();

	_Content().Retrieve( Row, Datum );

	Row = Next( Row );

	while ( Row != NONE ) {
		if ( Compare( Row, Datum ) == 1 )
			ERRReturn;

		Datum.Init();

		_Content().Retrieve( Row, Datum );

		Row = Next( Row );
	}
ERRErr
ERREnd
ERREpilog
	return Row;
}


bso::sign__ dbsidx::index_::Compare(
	rrow__ RecordRow,
	const datum_ &Pattern ) const
{
	bso::sign__ Result = 0;
ERRProlog
	datum Datum;
ERRBegin
	Datum.Init();

	_Content().Retrieve( RecordRow, Datum, *(dbsctt::_cache_ *)NULL );

	Result = S_.Sort->Compare( Datum, Pattern );
ERRErr
ERREnd
ERREpilog
	return Result;
}

bso::sign__ dbsidx::index_::Compare(
	rrow__ RecordRow1,
	rrow__ RecordRow2 ) const
{
	bso::sign__ Result = 0;
ERRProlog
	datum Pattern;
ERRBegin
	Pattern.Init();

	_Content().Retrieve( RecordRow2, Pattern, *(dbsctt::_cache_ *)NULL );

	Result = Compare( RecordRow1, Pattern );
ERRErr
ERREnd
ERREpilog
	return Result;
}

static inline void Save_(
	rrow__ Row,
	flw::oflow__ &Flow )
{
	dtfptb::PutULong( *Row, Flow );
}

static void Save_(
	rrow__ Row,
	const char *RootFileName )
{
ERRProlog
	flf::file_oflow___ Flow;
ERRBegin
	Flow.Init( RootFileName );

	Save_( Row, Flow );
ERRErr
ERREnd
ERREpilog
}

static void Save_(
	rrow__ Row,
	const str::string_ &RootFileName,
	const char *Extension,
	time_t UnderlyingFilesLastModificationTime )
{
ERRProlog
	str::string FileName;
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );

	Save_( Row, FileNameBuffer = FileName.Convert() );

	while ( UnderlyingFilesLastModificationTime >= tol::GetFileLastModificationTime( FileNameBuffer ) ) {
		tol::Clock( true );
		tol::Touch( FileNameBuffer );
	}

ERRErr
ERREnd
ERREpilog
}

/*
void dbsidx::file_index_::_SaveRoot( void ) const
{
	Save_( index_::S_.Root, RootFileName, ROOT_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() );
}
*/

static inline void Load_(
	flw::iflow__ &Flow,
	rrow__ &Row )
{
	Row = dtfptb::GetULong( Flow );
}

static bso::bool__ Load_(
	const char *RootFileName,
	rrow__ &Row,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
ERRProlog
	flf::file_iflow___ Flow;
ERRBegin
	if ( Flow.Init( RootFileName, err::hSkip ) == fil::sSuccess ) {
		if ( tol::GetFileLastModificationTime( RootFileName ) < TimeStamp )
			ERRReturn;

		Load_( Flow, Row );

		Success = true;
	}
ERRErr
ERREnd
ERREpilog
	return Success;
}

static bso::bool__ Load_(
	const str::string_ &RootFileName,
	rrow__ &Row,
	const char *Extension,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
ERRProlog
	str::string FileName;
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	Success = Load_( FileNameBuffer = FileName.Convert(), Row, TimeStamp );
ERRErr
ERREnd
ERREpilog
	return Success;
}


void dbsidx::file_index_::Init(
	const str::string_ &RootFileName,
	const dbsctt::content__ &Content,
	rrow__ Member,
	sort_function__ &Sort,
	mdr::mode__ Mode,
	bso::bool__ Erase,
	bso::bool__ Partial,
	flm::files_group_ &FilesGroup )
{
ERRProlog
	str::string TreeFileName;
	tol::E_FPOINTER___( bso::char__ ) TreeFileNameBuffer;
	str::string QueueFileName;
	tol::E_FPOINTER___( bso::char__ ) QueueFileNameBuffer;
ERRBegin
	reset();

	TreeFileName.Init( RootFileName );
	TreeFileName.Append( TREE_FILE_NAME_EXTENSION );
	TreeFileNameBuffer = TreeFileName.Convert();

	QueueFileName.Init( RootFileName );
	QueueFileName.Append( QUEUE_FILE_NAME_EXTENSION );
	QueueFileNameBuffer = QueueFileName.Convert();

	S_.FileManager.Init( TreeFileNameBuffer, QueueFileNameBuffer, Mode, true, FilesGroup );

	this->RootFileName.Init( RootFileName );

	index_::Init( Content, Sort, Partial );

	S_.Mode = Mode;
	S_.Erase = Erase;
	S_.Member = Member;
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dbsidxpersonnalization
: public dbsidxtutor
{
public:
	dbsidxpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dbsidxpersonnalization( void )
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

static dbsidxpersonnalization Tutor;

ttr_tutor &DBSIDXTutor = Tutor;
