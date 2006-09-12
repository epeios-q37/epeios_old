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

#define NODES_FILE_NAME_EXTENSION	".edn"
#define COLORS_FILE_NAME_EXTENSION	".edc"
#define QUEUE_FILE_NAME_EXTENSION	".edq"
#define ROOT_FILE_NAME_EXTENSION	".edr"

bso::sign__ dbsidx::index_::_Seek(
	const datum_ &Datum,
	rrow__ &Row ) const
{
	bso::sign__ Result = 0;
ERRProlog
	datum DatumToCompare;
	idxbtq::E_ISEEKERt__( rrow__ ) Seeker;
ERRBegin
	if ( S_.Root == NONE ) {
		Row = NONE;
		ERRReturn;
	}

	Seeker.Init( BaseIndex, S_.Root );

	Row = Seeker.GetCurrent();

	while ( Row != NONE ) {
		DatumToCompare.Init();

		_Retrieve( Row, DatumToCompare );

		switch ( Result = S_.Sort->Compare( Datum, DatumToCompare ) ) {
		case 0:
			Row = NONE;	// Pour sortir de la boucle.
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
	}

	Row = Seeker.GetCurrent();
ERRErr
ERREnd
ERREpilog
	return Result;
}

void dbsidx::index_::Index( rrow__ Row )
{
ERRProlog
	datum Datum;
	rrow__ TargetRow = NONE;
ERRBegin

	if ( _Content().Extent() > BaseIndex.Extent() )
		BaseIndex.Allocate( _Content().Extent() );

	if ( S_.Root == NONE ) {
		S_.Root = Row;
		ERRReturn;
	}

	Datum.Init();

	_Retrieve( Row, Datum );

	switch ( _Seek( Datum, TargetRow ) ) {
	case 1:
	case 0:
		S_.Root = BaseIndex.BecomeGreater( Row, TargetRow, S_.Root );
		break;
	case -1:
		S_.Root = BaseIndex.BecomeLesser( Row, TargetRow, S_.Root );
		break;
	default:
		ERRc();
		break;
	}

#ifdef DBSIDX_DBG
	if ( TargetRow == NONE )
		ERRc();
#endif

	S_.ModificationTimeStamp = tol::Clock();
ERRErr
ERREnd
ERREpilog
}

rrow__ dbsidx::index_::Seek( 
	const datum_ &Datum,
	bso::sign__ &Sign ) const
{
	rrow__ Row = NONE;

	if ( S_.Root == NONE )
		return NONE;

	Sign = _Seek( Datum, Row );

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

	_Content().Retrieve( RecordRow, Datum );

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

	_Content().Retrieve( RecordRow, Datum );

	Result = S_.Sort->Compare( Datum, Pattern );
ERRErr
ERREnd
ERREpilog
	return Result;
}

template <typename container> static bso::bool__ CoreSet_(
	flm::E_FILE_MEMORY_DRIVER___ &MemoryDriver,
	const str::string_ &FileName,
	container &C )
{
	bso::bool__ Exists = false;
ERRProlog
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileNameBuffer = FileName.Convert();

	Exists = tol::FileExists( FileNameBuffer );

	MemoryDriver.Init( FileNameBuffer );
	MemoryDriver.Persistant();
	C.plug( MemoryDriver );
	C.SetStepValue( 0 );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

template <typename container> static bso::bool__ Set_(
	flm::E_FILE_MEMORY_DRIVER___ &MemoryDriver,
	const str::string_ &FileName,
	container &C )
{
	bso::bool__ Exists = CoreSet_( MemoryDriver, FileName, C );

	C.Allocate( MemoryDriver.Size() / C.GetItemSize() );

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
	sort_function__ &Sort )
{
	bso::bool__ Exists = false;
ERRProlog
	str::string NodesFileName;
	str::string ColorsFileName;
	str::string QueueFileName;
ERRBegin
	index_::Init( Content, Sort );

	NodesFileName.Init( RootFileName );
	NodesFileName.Append( NODES_FILE_NAME_EXTENSION );
	Exists = Set_( S_.MemoryDriver.Tree.Nodes, NodesFileName, index_::BaseIndex.Tree().BaseTree.Nodes );

	ColorsFileName.Init( RootFileName );
	ColorsFileName.Append( COLORS_FILE_NAME_EXTENSION );
	if ( CoreSet_( S_.MemoryDriver.Tree.Colors, ColorsFileName, index_::BaseIndex.Tree().Colors ) != Exists )
		ERRu();
	S_.MemoryDriver.Tree.Colors.Size();	// Pour forcer la création du fichier.
	index_::BaseIndex.Tree().Colors.Allocate( index_::BaseIndex.Tree().BaseTree.Amount() );


	QueueFileName.Init( RootFileName );
	QueueFileName.Append( QUEUE_FILE_NAME_EXTENSION );
	if ( Set_( S_.MemoryDriver.Queue, QueueFileName, index_::BaseIndex.Queue().Links ) != Exists )
		ERRu();

	this->RootFileName.Init( RootFileName );

	if ( Exists ) {
		time_t NodesTimeStamp, ColorsTimeStamp, QueueTimeStamp, LastTimeStamp;

		NodesTimeStamp = GetModificationTimeStamp_( NodesFileName );
		ColorsTimeStamp = GetModificationTimeStamp_( ColorsFileName );
		QueueTimeStamp = GetModificationTimeStamp_( QueueFileName );

		if ( NodesTimeStamp > NodesTimeStamp )
			LastTimeStamp = NodesTimeStamp;
		else
			LastTimeStamp = QueueTimeStamp;

		if ( ColorsTimeStamp > LastTimeStamp )
			LastTimeStamp = ColorsTimeStamp;

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
