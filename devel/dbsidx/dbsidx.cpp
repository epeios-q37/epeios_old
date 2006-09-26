/*
	'dbsidx' library by Claude SIMON (csimon@epeios.org)
	Requires the 'dbsidx' header file ('dbsidx.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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
#define ROOT_FILE_NAME_EXTENSION	".edr"

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
		case 1:
			Row = Seeker.SearchLesser();
			break;
		case -1:
			Row = Seeker.SearchGreater();
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
		BaseIndex.Allocate( _Content().Extent() );

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
			Result = 1;	// Pour forcer son positionnement en tant que premier.
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

	if ( ( TargetRow == NONE ) && ( Extremities != NULL ) ) {
		if ( Extremities->Greatest == NONE )
			Extremities->Greatest = BaseIndex.Last( S_.Root );

		TargetRow = Extremities->Greatest;

		DatumToCompare.Init();

		_Retrieve( TargetRow, DatumToCompare, Cache );

		switch ( Result = S_.Sort->Compare( Datum, DatumToCompare ) ) {
		case 0:
			Result = -1;	// Pour forcer son positionnement en tant que dernier.
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

	if ( TargetRow == NONE )
		Result = _Seek( Datum, bStopIfOneChildMissing, TargetRow, Round, Cache );
	else
		Extremities->Used++;

//	cout << tol::DateAndTime( Buffer ) << txf::tab << &BaseIndex << txf::nl <<  txf::sync;
//	Display( BaseIndex, S_.Root, cout );

	switch ( Result ) {
	case 1:
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
	case -1:
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

	S_.ModificationTimeStamp = tol::Clock();
ERRErr
ERREnd
ERREpilog
	return Round;
}

rrow__ dbsidx::index_::Seek( 
	const datum_ &Datum,
	bso::sign__ &Sign ) const
{
	rrow__ Row = NONE;
	bso::ubyte__ Round;

	if ( S_.Root == NONE )
		return NONE;

	Sign = _Seek( Datum, bStop, Row, Round, *(dbsctt::_cache_ *)NULL );

#ifdef DBSIDX_DBG
	if ( Row == NONE )
		ERRc();
#endif

	return Row;
}

bso::bool__ dbsidx::index_::Begins(
	rrow__ RecordRow,
	const datum_ &Pattern ) const
{
	bso::bool__ Result = false;
ERRProlog
	datum Datum;
ERRBegin
	Datum.Init();

	_Content().Retrieve( RecordRow, Datum, *(dbsctt::_cache_ *)NULL );

	Result = S_.Sort->Begins( Datum, Pattern );
ERRErr
ERREnd
ERREpilog
	return Result;
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

template <typename container> static bso::bool__ CoreSet_(
	flm::E_FILE_MEMORY_DRIVER___ &MemoryDriver,
	const char *FileName,
	container &C,
	mdr::mode Mode,
	bso::bool__ Erase )
{
	bso::bool__ Exists = false;

	Exists = tol::FileExists( FileName );

	if ( Exists && Erase ) {
		remove( FileName );
		Exists = false;
	}

	MemoryDriver.Init( FileName, Mode );
	MemoryDriver.Persistant();
	C.plug( MemoryDriver );
	C.SetStepValue( 0 );

	return Exists;
}

template <typename container> static bso::bool__ Set_(
	flm::E_FILE_MEMORY_DRIVER___ &MemoryDriver,
	const char *FileName,
	container &C,
	mdr::mode Mode,
	bso::bool__ Erase )
{
	bso::bool__ Exists = CoreSet_( MemoryDriver, FileName, C, Mode, Erase );

	if ( Exists )
		C.Allocate( tol::GetFileSize( FileName ) / C.GetItemSize() );

	return Exists;
}

static time_t GetModificationTimeStamp_( const str::string_ &FileName )
{
	time_t TimeStamp;
ERRProlog
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileNameBuffer = FileName.Convert();

	TimeStamp = tol::GetFileLastModificationTime( FileNameBuffer );
ERRErr
ERREnd
ERREpilog
	return TimeStamp;
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
	const char *Extension )
{
ERRProlog
	str::string FileName;
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	Save_( Row, FileNameBuffer = FileName.Convert() );
ERRErr
ERREnd
ERREpilog
}

void dbsidx::file_index_::_SaveRoot( void ) const
{
	Save_( index_::S_.Root, RootFileName, ROOT_FILE_NAME_EXTENSION );
}

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


bso::bool__ dbsidx::file_index_::Init(
	const str::string_ &RootFileName,
	const content_ &Content,
	sort_function__ &Sort,
	mdr::mode Mode,
	bso::bool__ Erase )
{
	bso::bool__ Exists = false;
ERRProlog
	str::string TreeFileName;
	tol::E_FPOINTER___( bso::char__ ) TreeFileNameBuffer;
	str::string QueueFileName;
	tol::E_FPOINTER___( bso::char__ ) QueueFileNameBuffer;
ERRBegin
	index_::Init( Content, Sort );

	TreeFileName.Init( RootFileName );
	TreeFileName.Append( TREE_FILE_NAME_EXTENSION );
	TreeFileNameBuffer = TreeFileName.Convert();
	Exists = Set_( S_.MemoryDriver.Tree, TreeFileNameBuffer, index_::BaseIndex.Tree().BaseTree.Nodes, Mode, Erase );

	QueueFileName.Init( RootFileName );
	QueueFileName.Append( QUEUE_FILE_NAME_EXTENSION );
	QueueFileNameBuffer = QueueFileName.Convert();
	if ( Set_( S_.MemoryDriver.Queue, QueueFileNameBuffer, index_::BaseIndex.Queue().Links, Mode, Erase ) != Exists )
		ERRu();

	this->RootFileName.Init( RootFileName );

	if ( Exists ) {
		time_t TreeTimeStamp, QueueTimeStamp, LastTimeStamp;

		TreeTimeStamp = GetModificationTimeStamp_( TreeFileName );
		QueueTimeStamp = GetModificationTimeStamp_( QueueFileName );

		if ( QueueTimeStamp > TreeTimeStamp )
			LastTimeStamp = QueueTimeStamp;
		else
			LastTimeStamp = TreeTimeStamp;

		if ( !Load_( RootFileName, index_::S_.Root, ROOT_FILE_NAME_EXTENSION, LastTimeStamp ) )
			SearchRoot();
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
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
