/*
	'rgstry' library by Claude SIMON (csimon@epeios.org)
	Requires the 'rgstry' header file ('rgstry.h').
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

#define RGSTRY__COMPILATION

#include "rgstry.h"

class rgstrytutor
: public ttr_tutor
{
public:
	rgstrytutor( void )
	: ttr_tutor( RGSTRY_NAME )
	{
#ifdef RGSTRY_DBG
		Version = RGSTRY_VERSION "\b\bD $";
#else
		Version = RGSTRY_VERSION;
#endif
		Owner = RGSTRY_OWNER;
		Date = "$Date$";
	}
	virtual ~rgstrytutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flx.h"
#include "flf.h"

using namespace rgstry;

erow__ rgstry::registry_::_SearchEntry(
	const term_ &Name,
	const erows_ &EntryRows ) const
{
	term_buffer Buffer;
	epeios::row__ Row = EntryRows.First();

	Buffer.Init( Terms );

	while ( ( Row != NONE ) && ( _GetName( EntryRows( Row ), Buffer ) != Name ) )
		Row = EntryRows.Next( Row );

	if ( Row != NONE )
		return EntryRows( Row );
	else
		return NONE;
}

nrow__ rgstry::registry_::_SearchNode(
	const term_ &Name,
	const nrows_ &NodeRows,
	epeios::row__ &Cursor ) const
{
	node_buffer NodeBuffer;
	term_buffer NameBuffer;

	epeios::row__ &Row = Cursor;
	
	if ( Row == NONE )
		Row = NodeRows.First();
	else
		Row = NodeRows.Next( Row );

	NodeBuffer.Init( Nodes );
	NameBuffer.Init( Terms );

	while ( ( Row != NONE ) && ( _GetName( NodeRows( Row ), NameBuffer, NodeBuffer ) != Name ) )
		Row = NodeRows.Next( Row );

	if ( Row != NONE )
		return NodeRows( Row );
	else
		return NONE;

}

nrow__ rgstry::registry_::_SearchNode(
	const term_ &NodeName,
	const term_ &AttributeName,
	const value_ &AttributeValue,
	nrow__ ParentNodeRow,
	epeios::row__ &Cursor ) const
{
	nrow__ NodeRow = _SearchNode( NodeName, ParentNodeRow, Cursor );

	while ( ( NodeRow != NONE ) && ( !_AttributeExists( AttributeName, AttributeValue, NodeRow ) ) )
		NodeRow = _SearchNode( NodeName, ParentNodeRow, Cursor );

	return NodeRow;
}

const term_ &rgstry::registry_::GetCompleteName(
	nrow__ NodeRow,
	term_ &Term,
	const char *Separator ) const
{
	term_buffer TermBuffer;
	node_buffer NodeBuffer;

	TermBuffer.Init( Terms );
	NodeBuffer.Init( Nodes );

	if ( NodeRow != NONE ) {
		Term.Append( _GetName( NodeRow, TermBuffer, NodeBuffer ) );
		NodeRow = GetParent( NodeRow );
	}

	while ( NodeRow != NONE ) {
		Term.Insert( Separator, 0 );
		Term.Insert( _GetName( NodeRow, TermBuffer, NodeBuffer ), 0 );

		NodeRow = GetParent( NodeRow );
	}

	return Term;
}

enum state__ {
	sTagName,
	sAttributeName,
	sAttributeValue,
	s_amount,
	s_Undefined
};

epeios::row__ rgstry::BuildPath(
	const term_ &Term,
	path_ &Path )
{
	epeios::row__ TermRow = NONE;
ERRProlog
	bso::bool__ Continue = true;
	state__ State = s_Undefined;
	bso::char__ C;
	path_item Item;
	bso::bool__ TagNameAsAttribute = false;
ERRBegin
	TermRow = Term.First();

	Item.Init();

	Continue = TermRow != NONE;

	State = sTagName;

	while ( Continue ) {
		C = Term( TermRow );
		switch ( State ) {
		case sTagName:
			switch ( C ) {
			case '=':
			case '"':
				Continue = false;
				break;
			case '[':
				if ( TagNameAsAttribute )
					Continue = false;
				else if ( Item.TagName.Amount() == 0 )
					Continue = false;
				State = sAttributeName;
				Item.AttributeName.Init();
				break;
			case '/':
				if ( TagNameAsAttribute )
					Continue = false;
				else if ( Item.TagName.Amount() == 0 )
					Continue = false;
				else
					Path.Append( Item );
				Item.Init();
				break;
			case '@':
				if ( Item.TagName.Amount() != 0 )
					Continue = false;
				else
					TagNameAsAttribute = true;
				break;
			default:
				Item.TagName.Append( C );
				break;
			}
			break;
			case sAttributeName:
				switch ( C ) {
				case ']':
					Continue = false;
					break;
				case '"':
					Continue = false;
					break;
				case '=':
					if ( Item.AttributeName.Amount() == 0 )
						Continue = false;
					State = sAttributeValue;
					Item.AttributeValue.Init();
					if ( ( TermRow = Term.Next( TermRow ) ) == NONE )
						Continue = false;
					else if ( Term( TermRow ) != '"' )
						Continue = false;
					break;
				default:
					Item.AttributeName.Append( C );
					break;
				}
				break;
			case sAttributeValue:
				switch ( C ) {
				case '[':
				case ']':
				case '/':
					Continue = false;
					break;
				case '"':
					if ( ( TermRow = Term.Next( TermRow ) ) == NONE )
						Continue = false;
					else if ( Term( TermRow ) != ']' )
						Continue = false;
					else if ( ( TermRow = Term.Next( TermRow ) ) == NONE )
						Continue = false;
					else if ( Term( TermRow ) != '/' )
						Continue = false;
					Path.Append( Item );
					Item.Init();
					State = sTagName;
					break;
				default:
					Item.AttributeValue.Append( C );
					break;
				}
				break;
			default:
				ERRc();
				break;
		}

		if ( Continue )
			TermRow = Term.Next( TermRow );

		if ( TermRow == NONE )
			Continue = false;
	}

	if ( Item.TagName.Amount() != 0 ) {
		if ( TagNameAsAttribute ) {
			Item.AttributeName = Item.TagName;
			Item.TagName.Init();
		}

		Path.Append( Item );
	}
ERRErr
ERREnd
ERREpilog
	return TermRow;
}

/*
nrow__ rgstry::registry_::_SearchPath(
	const path_ &Path,
	nrow__ &ForkRow,
	erow__ &AttributeEntryRow,
	epeios::row__ &PathRow,
	epeios::row__ &Cursor ) const
{
	ctn::E_CITEM( path_item_ ) Item;
	nrow__ Row = ForkRow;
	bso::bool__ ResetCursor = Cursor == NONE;
	epeios::row__ LocalPathRow = ( PathRow == NONE ? Path.First() : PathRow );

	AttributeEntryRow = NONE;

	Item.Init( Path );

	while ( ( LocalPathRow != NONE ) && ( Row != NONE ) ) {
		if ( Item( LocalPathRow ).TagName.Amount() != 0 ) {// '.../Name[@AttributeName='AttributeValue']/...'
			ForkRow = Row;
			PathRow = LocalPathRow;
			if ( ResetCursor )
				Cursor = NONE;
			Row = _SearchChild( Item( LocalPathRow ), Row, Cursor );	
		} else if ( ( AttributeEntryRow = _GetAttribute( Item( LocalPathRow ).AttributeName, Row ) ) == NONE ) // '.../@Name'
			Row = NONE;

		LocalPathRow = Path.Next( LocalPathRow );

#ifdef RGSTRY_DBG
			if ( ( AttributeEntryRow != NONE ) && ( LocalPathRow != NONE ) )
				ERRu();
#endif
	}

	return Row;
}
*/

nrow__ rgstry::registry_::_SearchChild(
	const path_item_ &Item,
	nrow__ Row,
	erow__ &AttributeEntryRow,
	epeios::row__ &Cursor ) const
{
	if ( Item.TagName.Amount() != 0 )
		Row = _SearchChild( Item, Row, Cursor );	// '.../TagName[@AttributeName='AttributeValue']/...'
	else if ( Cursor == NONE ) {
		AttributeEntryRow = _GetAttribute( Item.AttributeName, Row );	// '.../@AttributeName'
		if ( AttributeEntryRow == NONE ) {
			AttributeEntryRow = *Row;	// Peu importe la valeur, pourvu qu'elle ce soit différente de 'NONE'.
			Row = NONE;	// Pour signaler que l'on n'a pas trouvé.
			// 'Row' == 'NONE 'et 'AttributeEntryRow' != 'NONE' signale que l'on a trouvé le tag, mais pas l'attribut.
		}
		Cursor = *Row;	// Peu importe la valeur, pourvu qu'elle ce soit différente de 'NONE'.
	} else
		Row = NONE;

	return Row;
}


nrow__ rgstry::registry_::_SearchPath(
	const path_ &Path,
	epeios::row__ PathRow,
	nrow__ Row,
	erow__ &AttributeEntryRow,
	nrows_ &ResultTags,
	erows_ &ResultAttributes ) const
{
	epeios::row__ CandidateRow = NONE;
	epeios::row__ Cursor = NONE;
	ctn::E_CITEM( path_item_ ) Item;
	nrow__ Result, ChildRow = NONE;
	bso::bool__ All = &ResultTags != NULL;

	Item.Init( Path );

	Result = ChildRow = _SearchChild( Item( PathRow ), Row, AttributeEntryRow, Cursor );

	if ( PathRow != Path.Last() ) {
#ifdef RGSTRY_DBG
		if ( AttributeEntryRow != NONE )
			ERRc();
#endif
		while ( ChildRow != NONE )  {
			AttributeEntryRow = NONE;
			Result = _SearchPath( Path, Path.Next( PathRow ), ChildRow, AttributeEntryRow, ResultTags, ResultAttributes );

			if ( Result != NONE ) {
				if ( All ) {
					ChildRow = _SearchChild( Item( PathRow ), Row, AttributeEntryRow, Cursor );
				} else
					ChildRow = NONE;
			} else if ( AttributeEntryRow == NONE )	// Voir '_SearchChild()'.
				ChildRow = _SearchChild( Item( PathRow ), Row, AttributeEntryRow, Cursor );
			else
				ChildRow = NONE;	// pour sortir de la boucle.
		}

	} else if ( All ) {

		while ( ChildRow != NONE ) {
			ResultTags.Append( ChildRow );
			ResultAttributes.Append( AttributeEntryRow );

			ChildRow = _SearchChild( Item( PathRow ), Row, AttributeEntryRow, Cursor );
		}

	}

	return Result;
}

nrow__ rgstry::registry_::_SearchPath(
	const path_ &Path,
	nrow__ Row,
	erow__ &AttributeEntryRow ) const
{
	epeios::row__ PathRow = NONE, Cursor = NONE;

	if ( Path.Amount() != 0 )
		return _SearchPath( Path, Path.First(), Row, AttributeEntryRow, *(nrows_ *)NULL, *(erows_ *)NULL );
	else
		return NONE;
}

nrow__ rgstry::registry_::_SearchPath(
	const term_ &Term,
	nrow__ Row,
	erow__ &AttributeEntryRow,
	epeios::row__ &PathErrorRow ) const
{
ERRProlog
	epeios::row__ PathRow = NONE;
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( Term, Path );

	if ( PathRow != NONE ) {
		Row = NONE;
		PathErrorRow = PathRow;
	} else
		Row = _SearchPath( Path, Row, AttributeEntryRow );

ERRErr
ERREnd
ERREpilog
	return Row;
}

nrow__ rgstry::registry_::CreatePath(
	const path_ &Path,
	nrow__ Row )
{
	bso::bool__ Exists = true;
	ctn::E_CITEM( path_item_ ) Item;
	epeios::row__ PathRow = Path.First();
	nrow__ Candidate = NONE;

	Item.Init( Path );

	while ( ( PathRow != NONE ) && Exists ) {
		Candidate = _SearchChild( Item( PathRow ), Row );

		if ( Candidate == NONE ) {
			Exists = false;

			Row = _CreateChild( Item( PathRow ), Row );
		} else
			Row = Candidate;

		PathRow = Path.Next( PathRow );
	}

	while ( PathRow != NONE ) {
		Row = _CreateChild( Item( PathRow ), Row );

		PathRow = Path.Next( PathRow );
	}

	return Row;
}

nrow__ rgstry::registry_::CreatePath(
	const term_ &Term,
	nrow__ Row,
	epeios::row__ &PathErrorRow )
{
ERRProlog
	epeios::row__ PathRow = NONE;
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( Term, Path );

	if ( PathRow != NONE ) {
		Row = NONE;
		PathErrorRow = PathRow;
	} else
		Row = CreatePath( Path, Row );

ERRErr
ERREnd
ERREpilog
	return Row;
}


nrow__ rgstry::registry_::SetPathValue(
	const term_ &Term,
	const value_ &Value,
	nrow__ Row,
	epeios::row__ &PathErrorRow )
{
ERRProlog
	epeios::row__ PathRow = NONE;
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( Term, Path );

	if ( PathRow != NONE ) {
		Row = NONE;
		PathErrorRow = PathRow;
	} else
		Row = SetPathValue( Path, Value, Row );

ERRErr
ERREnd
ERREpilog
	return Row;
}

struct kernel__ {
	registry_ &Registry;
	nrow__ Root;
	nrow__ Current;
	kernel__(
		registry_ &Registry,
		nrow__ Root )
	: Registry( Registry )
	{
		this->Root = Current = Root;
	}
};

enum mode__ {
	mRegular,	// mode normale.
	mDefineDeclaration,	// mode définition d'un  bloc, partie déclration ('<xcf:define ...>')
	mDefineDefinition,	// mode définition d'un  bloc, partie définition ('<xcf:define name="test">...')
	mExpandInRegular,	// mode déploiement d'un bloc ou inclusion d'un fichier en mode 'Regular'.
	mExpandInDefine,	// mode déploiement d'un bloc ou inclusion d'un fichier en mode 'Define'.
	m_amount,
	m_Undefined,
};

#define NAMESPACE			"xcf"
#define PREFIX				NAMESPACE ":"

#define DEFINE_TAG			PREFIX "define"
#define EXPAND_TAG			PREFIX "expand"

#define DEFINE_ATTRIBUTE			"name"
#define EXPAND_EXPLODE_ATTRIBUTE	"select"
#define EXPAND_INCLUDE_ATTRIBUTE	"href"

#define BLOC_TAG			PREFIX "bloc"
#define BLOC_ATTRIBUTE		""	// Pas d'attribut.


class callback___
: public xml::callback__
{
private:
	registry _ManagerRegistry;
	kernel__ _User, _Manager;
	mode__ _Mode;
	term _ExpandTagAttributeValue;
	bso::bool__ _ExpandIsInclude;
	location__ _IncludeErrorLine, _IncludeErrorColumn;
	static struct define {
		term TagName;
		term AttributeName;
	} _Define;
	static struct expand {
		term TagName;
		term ExplodeAttributeName;
		term IncludeAttributeName;
	} _Expand;
	static struct bloc {
		term TagName;
	} _Bloc;
	static term _Prefix;
	kernel__ &_Kernel( void )
	{
		switch ( _Mode ) {
		case mDefineDeclaration:
		case mDefineDefinition:
		case mExpandInDefine:
			return _Manager;
			break;
		case mExpandInRegular:
		case mRegular:
			return _User;
			break;
		default:
			ERRc();
			break;
		}

		ERRc();

		return *(kernel__ *)NULL;	// Pour éviter un 'warning'.
	}
	registry_ &_Registry( void )
	{
		return _Kernel().Registry;
	}
	nrow__ &_Root( void )
	{
		return _Kernel().Root;
	}
	nrow__ &_Current( void )
	{
		return _Kernel().Current;
	}
	bso::bool__ _BelongsToNamespace( const str::string_ &TagName )
	{
		return str::Compare( TagName, _Prefix, TagName.First(), _Prefix.First(), _Prefix.Amount() ) == 0;
	}
	bso::bool__ _IsDefineTagName( const str::string_ &TagName )
	{
		return str::Compare( TagName, _Define.TagName, TagName.First(), _Define.TagName.First() ) == 0;
	}
	bso::bool__ _IsDefineTagAttributeName( const str::string_ &AttributeName )
	{
		return str::Compare( AttributeName, _Define.AttributeName, AttributeName.First(), _Define.AttributeName.First() ) == 0;
	}
	bso::bool__ _IsExpandTagName( const str::string_ &TagName )
	{
		return str::Compare( TagName, _Expand.TagName, TagName.First(), _Expand.TagName.First() ) == 0;
	}
	bso::bool__ _IsExpandTagExplodeAttributeName( const str::string_ &AttributeName )
	{
		return str::Compare( AttributeName, _Expand.ExplodeAttributeName, AttributeName.First(), _Expand.ExplodeAttributeName.First() ) == 0;
	}
	bso::bool__ _IsExpandTagIncludeAttributeName( const str::string_ &AttributeName )
	{
		return str::Compare( AttributeName, _Expand.IncludeAttributeName, AttributeName.First(), _Expand.IncludeAttributeName.First() ) == 0;
	}
	bso::bool__ _IsBlocTagName( const str::string_ &TagName )
	{
		return str::Compare( TagName, _Bloc.TagName, TagName.First(), _Bloc.TagName.First() ) == 0;
	}
	void _Dump(
		registry_ &Registry,
		nrow__ Root,
		str::string_ &Result )
	{
	ERRProlog
		flx::E_STRING_OFLOW___ Flow;
		txf::text_oflow__ TFlow( Flow );
	ERRBegin
		Flow.Init( Result );
//		TFlow.Init( Flow );

		Registry.Dump( Root, false, false, TFlow );
	ERRErr
	ERREnd
	ERREpilog
	}
	void _Import(
		const str::string_ &Buffer,
		registry_ &Registry,
		nrow__ Root )
	{
	ERRProlog
		flx::E_STRING_IFLOW__ Flow;
		xtf::extended_text_iflow__ TFlow;
		xtf::location__ Error;
	ERRBegin
		Flow.Init( Buffer );
		TFlow.Init( Flow );

		if ( Parse( TFlow, Registry, Root, *(str::string_ *)NULL, Error, Error ) == NONE )
			ERRc();	// Normalement, toutes les erreurs ont été détectées lors du 'define'.
	ERRErr
	ERREnd
	ERREpilog
	}
	bso::bool__ _ExplodeNode( void )
	{
		bso::bool__ Success = false;
	ERRProlog
		str::string Buffer;
		nrow__ Root = NONE;
		term Path;
		erow__ AttributeEntryRow = NONE;
		epeios::row__ PathErrorRow = NONE;
	ERRBegin
		Path.Init();

		Path.Append( DEFINE_TAG "[" DEFINE_ATTRIBUTE "=\"" );
		Path.Append( _ExpandTagAttributeValue );
		Path.Append( "\"]" );

		_ExpandTagAttributeValue.Init();

		Root = _Manager.Registry.SearchPath( Path, _Manager.Root, AttributeEntryRow, PathErrorRow );

		if ( PathErrorRow != NONE )
			ERRc();

		if ( AttributeEntryRow != NONE )
			ERRc();

		if ( Root == NONE )
			ERRReturn;

		Buffer.Init();

		_Dump( _Manager.Registry, Root, Buffer );

		Buffer.Append( XTF_EOXC );

		_Import( Buffer, _Registry(), _Current() );

		Success = true;
	ERRErr
	ERREnd
	ERREpilog
		return Success;
	}
	bso::bool__ _IncludeFile( void )
	{
		bso::bool__ Success = false;
	ERRProlog
		flf::file_iflow___ FFlow;
		xtf::extended_text_iflow__ XFlow;
		tol::E_FPOINTER___( bso::char__ ) Buffer;
		location__ ErrorLine, ErrorColumn;
	ERRBegin
		Buffer = _ExpandTagAttributeValue.Convert();

		_ExpandTagAttributeValue.Init();

		if ( !FFlow.Init( Buffer, err::hSkip ) )
			ERRReturn;

		FFlow.EOFD( XTF_EOXT );

		XFlow.Init( FFlow );

		if ( !xml::Parse( XFlow, *this, ErrorLine, ErrorColumn ) )  {
			if ( _ExpandTagAttributeValue.Amount() == 0 ) {	// Si pas == 0, alors l'erreur se trouve dans un sous-fcihier.
				_ExpandTagAttributeValue.Init( Buffer );	// Pour indiquer que l'erreur se situe dans un fichier et lequel.
				_IncludeErrorLine = ErrorLine;
				_IncludeErrorColumn = ErrorColumn;
			}
			ERRReturn;
		}

		Success = true;
	ERRErr
	ERREnd
	ERREpilog
		return Success;
	}
	bso::bool__ _ExpandNode( void )
	{
		if ( _ExpandIsInclude )
			return _IncludeFile();
		else
			return _ExplodeNode();
	}
protected:
	virtual bso::bool__ XMLTag( const str::string_ &TagName )
	{
		bso::bool__ Ignore = false;

		if ( _BelongsToNamespace( TagName ) ) {
			if ( _IsDefineTagName( TagName ) ) {
				switch ( _Mode ) {
				case mDefineDeclaration:
					return false;
				case mDefineDefinition:
				case mExpandInRegular:
				case mExpandInDefine:
					return false;
					break;
				case mRegular:
					_Mode = mDefineDeclaration;
					break;
				default:
					ERRc();
					break;
				}
			} else if ( _IsExpandTagName( TagName ) ) {
				Ignore = true;

				switch ( _Mode ) {
				case mDefineDeclaration:
					return false;
					break;
				case mDefineDefinition:
					_Mode = mExpandInDefine;
					break;
				case mRegular:
					_Mode = mExpandInRegular;
					break;
				case mExpandInRegular:
				case mExpandInDefine:
					return false;
					break;
				default:
					ERRc();
					break;
				}
			} else if ( _IsBlocTagName( TagName ) ) {
				if ( _Mode == mRegular )
					Ignore = true;
			} else 
				return false;
		}

		if ( !Ignore ) {
			if ( _Current() == NONE ) {
				if ( _Root() == NONE )
					_Root() = _Current() = _Registry().CreateNode( TagName );
				else
					return false;
			} else
				_Current() = _Registry().AddChild( TagName, _Current() );
		}

		return true;
	}
	virtual bso::bool__ XMLValue(
		const str::string_ &TagName,
		const str::string_ &Value )
	{
		switch ( _Mode ) {
		case mDefineDeclaration:
			return false;
			break;
		case mDefineDefinition:
			if ( _IsDefineTagName( TagName ) )
				return false;
		case mRegular:
			break;
		case mExpandInRegular:
		case mExpandInDefine:
			break;
		default:
			ERRc();
			break;
		}

		_Registry().SetValue( Value, _Current(), true );

		return true;
	}
	virtual bso::bool__ XMLAttribute(
		const str::string_ &TagName,
		const str::string_ &Name,
		const str::string_ &Value )
	{
		switch ( _Mode ) {
		case mDefineDeclaration:
			if ( _IsDefineTagAttributeName( Name ) ) {
				_Registry().AddAttribute( Name, Value, _Current() );
				_Mode = mDefineDefinition;
			} else
				return false;
			break;
		case mDefineDefinition:
		case mRegular:
			_Registry().AddAttribute( Name, Value, _Current() );
			break;
		case mExpandInRegular:
		case mExpandInDefine:
#ifdef RGSTRY_DBG
			if ( !_IsExpandTagName( TagName ) )
				ERRc();
#endif
			if ( _IsExpandTagExplodeAttributeName( Name ) )
				_ExpandIsInclude = false;
			else if ( _IsExpandTagIncludeAttributeName( Name ) )
				_ExpandIsInclude = true;
			else
				return false;

			_ExpandTagAttributeValue.Init( Value );
			break;
		default:
			ERRc();
			break;
		}

		return true;
	}
	virtual bso::bool__ XMLTagClosed( const str::string_ &TagName )
	{
		switch ( _Mode ) {
		case mRegular:
			if ( _IsBlocTagName( TagName ) )
				return true;
		case mDefineDeclaration:
		case mDefineDefinition:
			_Current() = _Registry().GetParent( _Current() );
			break;
		case mExpandInRegular:
#ifdef RGSTRY_DBG
			if ( !_IsExpandTagName( TagName ) )
				ERRc();
#endif
			_Mode = mRegular;

			if ( !_ExpandNode() )
				return false;

//			_Current() = _Registry().GetParent( _Current() );

			break;
		case mExpandInDefine:
#ifdef RGSTRY_DBG
			if ( !_IsExpandTagName( TagName ) )
				ERRc();
#endif
			if ( !_ExpandNode() )
				return false;

//			_Current() = _Registry().GetParent( _Current() );

			_Mode = mDefineDefinition;
			break;
		default:
			ERRc();
			break;
		}

		if ( ( _Current() == NONE ) && ( _Root() == NONE ) )
			return false;

		if ( _BelongsToNamespace( TagName ) )
			if ( _IsDefineTagName( TagName ) )
				_Mode = mRegular;
		/* Pas de test extensif, puisque que l'on est sûr que le nom de la balise est correct,
		sinon cela aurait été déteté lors de l'ouverture de cette balise. */

		return true;
	}
public:
	callback___(
		registry_ &Registry,
		nrow__ Root )
	: _User( Registry, Root ),
	  _Manager( _ManagerRegistry, NONE )
	{
		_ManagerRegistry.Init();
		_Manager.Root = _Manager.Current = _Manager.Registry.CreateNode( term( "_internal_" ) );
		_ExpandTagAttributeValue.Init();
		_Mode = mRegular;
		_IncludeErrorLine = _IncludeErrorColumn = 0;
	}
	nrow__ GetRoot( void ) const
	{
		if ( _User.Current != NONE )
			return _User.Current;
		else
			return _User.Root;
	}
	~callback___( void )
	{
/*
		_Manager.Registry.Dump( _Manager.Root, true, true, cio::cout );
		cio::cout << txf::nl;
		_User.Registry.Dump( _User.Root, true, true, cio::cout );
		cio::cout << txf::nl;
*/
	}
	const str::string_ &IncludeErrorFileName( void )
	{
		return _ExpandTagAttributeValue;
	}
	E_RODISCLOSE__( location__, IncludeErrorLine )
	E_RODISCLOSE__( location__, IncludeErrorColumn )

	friend class rgstrypersonnalization;
};

callback___::define callback___::_Define;
callback___::expand callback___::_Expand;
callback___::bloc callback___::_Bloc;
term callback___::_Prefix;

void rgstry::registry_::_Delete( const erows_ &Rows )
{
	epeios::row__ Row = Rows.First();

	while ( Row != NONE ) {
		_Delete( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

void rgstry::registry_::_Delete( nrow__ Row )
{
	const node_ &Node = Nodes( Row );

	_Delete( Node.Attributes );

	if ( Node.NameRow() != NONE )
		Terms.Delete( Node.NameRow() );

	if ( Node.ValueRow() != NONE )
		Terms.Delete( Node.ValueRow() );

	_Delete( Node.Children );

	Nodes.Delete( Row );
}

bso::bool__ rgstry::registry_::PathExists(
	const term_ &PathString,
	nrow__ ParentRow,
	epeios::row__ &PathRow ) const
{
	bso::bool__ Result = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( PathString, Path );

	if ( PathRow != NONE )
		Result = PathExists( Path, ParentRow );
ERRErr
ERREnd
ERREpilog
	return Result;
}

void rgstry::registry_::_DumpAttributes(
	const erows_ &Rows,
	xml::writer_ &Writer ) const
{
	epeios::row__ Row = Rows.First();
	term_buffer Name, Value;

	Name.Init( Terms );
	Value.Init( Terms );

	while ( Row != NONE ){
		Writer.PutAttribute( _GetName( Rows( Row ), Name ), _GetValue( Rows( Row ), Value ) );

		Row = Rows.Next( Row );
	}

}

void rgstry::registry_::_Dump(
	nrow__ Root,
	bso::bool__ RootToo,
	xml::writer_ &Writer,
	term_buffer &TermBuffer,
	node_buffer &NodeBuffer ) const
{
ERRProlog
	nrows Children;
epeios::row__ Row = NONE;
ERRBegin
	Children.Init();
	Children = _GetNode( Root, NodeBuffer ).Children;

	if ( RootToo )
		_DumpNode( Root, Writer, TermBuffer, NodeBuffer );
	
	Row = Children.First();

	while ( Row != NONE ) {
		_Dump( Children( Row ), true, Writer, TermBuffer, NodeBuffer );

		Row = Children.Next( Row );
	}

	if ( RootToo )
		Writer.PopTag();
ERRErr
ERREnd
ERREpilog
}


void rgstry::registry_::Dump(
	nrow__ Root,
	bso::bool__ RootToo,
	bso::bool__ Indent,
	txf::text_oflow__ &Flow ) const
{
ERRProlog
	xml::writer Writer;
	term_buffer TermBuffer;
	node_buffer NodeBuffer;
ERRBegin
	TermBuffer.Init( Terms );
	NodeBuffer.Init( Nodes );

	Writer.Init( Flow, Indent );

	_Dump( Root, RootToo, Writer, TermBuffer, NodeBuffer );
ERRErr
ERREnd
ERREpilog
}


void rgstry::registry_::_Delete( const nrows_ &Rows )
{
	epeios::row__ Row = Rows.First();

	while ( Row != NONE ) {
		_Delete( Rows( Row ) );

		Row = Rows.Next( Row );
	}
}

const value_ &rgstry::registry_::GetPathValue(
	const path_ &Path,
	nrow__ ParentRow,
	bso::bool__ *Exists,
	term_buffer &Buffer ) const	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas. Le laisse inchangé sinon.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	nrow__ Row = NONE;
	erow__ AttributeEntryRow = NONE;
ERRBegin
	Empty.Init();

	if ( ( Row = _SearchPath( Path, ParentRow, AttributeEntryRow ) ) != NONE )
		if ( AttributeEntryRow == NONE )
			Result = &GetValue( Row, Buffer );
		else
			Result = &GetValue( AttributeEntryRow, Buffer );
	else {
		*Exists = false;
	}
ERRErr
ERREnd
ERREpilog
	return *Result;
}

const value_ &rgstry::registry_::GetPathValue(
	const term_ &PathString,
	nrow__ ParentRow,
	bso::bool__ *Exists,
	epeios::row__ &PathErrorRow,
	term_buffer &Buffer ) const	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas. Le laisse inchangé sinon.
{
	static value Empty;
	const value_ *Result = &Empty;
ERRProlog
	nrow__ Row = NONE;
	path Path;
	erow__ AttributeEntryRow = NONE;
ERRBegin
	Path.Init();
	Empty.Init();

	if ( ( PathErrorRow = BuildPath( PathString, Path ) ) != NONE ) {
		*Exists = false;
		ERRReturn;
	}

	Result = &GetPathValue( Path, ParentRow, Exists, Buffer );
ERRErr
ERREnd
ERREpilog
	return *Result;
}

bso::bool__ rgstry::registry_::GetPathValues(
	const path_ &Path,
	nrow__ ParentRow,
	values_ &Values ) const
{
	bso::bool__ Exists = false;
ERRProlog
	epeios::row__ Row = NONE;
	erow__ AttributeEntryRow = NONE;
	term_buffer Buffer;
	nrows TagRows;
	erows AttributeRows;
ERRBegin
	TagRows.Init();
	AttributeRows.Init();

	_SearchPath( Path, Path.First(), ParentRow, AttributeEntryRow, TagRows, AttributeRows );

#ifdef RGSTRY_DBG
	if ( TagRows.Amount() != AttributeRows.Amount() )
		ERRc();
#endif

	if ( TagRows.Amount() != 0 ) {
		Exists = true;
		Row = TagRows.First();

		while ( Row != NONE ) {
			if ( AttributeRows( Row ) == NONE )
				Values.Append( GetValue( TagRows( Row ), Buffer ) );
			else
				Values.Append( GetValue( AttributeRows( Row ), Buffer ) );

			Row = AttributeRows.Next( Row );
		}
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::registry_::GetPathValues(
	const term_ &PathString,
	nrow__ ParentRow,
	epeios::row__ &PathErrorRow,
	values_ &Values ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( ( PathErrorRow = BuildPath( PathString, Path ) ) != NONE )
		ERRReturn;

	Exists = GetPathValues( Path, ParentRow, Values );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::registry_::DeletePath(
	const term_ &PathString,
	nrow__ ParentRow,
	epeios::row__ &PathErrorRow )
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	if ( ( PathErrorRow = BuildPath( PathString, Path ) ) != NONE )
		ERRReturn;

	Exists = DeletePath( Path, ParentRow );
ERRErr
ERREnd
ERREpilog
	return Exists;
}




nrow__ rgstry::Parse(
	xtf::extended_text_iflow__ &Flow,
	registry_ &Registry,
	nrow__ Root,
	str::string_ &ErrorFileName,
	xtf::location__ &ErrorLine,
	xtf::location__ &ErrorColumn )
{
ERRProlog
	callback___ Callback( Registry, Root );
ERRBegin
	if ( xml::Parse( Flow, Callback, ErrorLine, ErrorColumn ) )
		Root = Callback.GetRoot();
	else
		Root = NONE;

	if ( ( &ErrorFileName != NULL ) && ( Callback.IncludeErrorFileName().Amount() != 0 ) ) {
		ErrorFileName = Callback.IncludeErrorFileName();

		ErrorLine = Callback.IncludeErrorLine();
		ErrorColumn = Callback.IncludeErrorColumn();
	}

ERRErr
ERREnd
ERREpilog
	return Root;
}

const value_ &rgstry::overloaded_registry___::GetPathValue(
	const term_ &PathString,
	bso::bool__ *Exists,
	epeios::row__ &PathErrorRow,
	term_buffer &Buffer ) const	// Nota : ne met 'Exists' à 'false' que lorque 'Path' n'existe pas. Le laisse inchangé sinon.
{
	static value Empty;
	const value_ *Result = 0;
ERRProlog
	nrow__ Row = NONE;
	path Path;
	erow__ AttributeEntryRow = NONE;
	bso::bool__ LocalExists = true;
ERRBegin
	Path.Init();
	Empty.Init();

	if ( ( PathErrorRow = BuildPath( PathString, Path ) ) != NONE ) {
		Exists = false;
		ERRReturn;
	}

	Result = &Local.Registry->GetPathValue( Path, Local.Root, &LocalExists, Buffer );

	if ( !LocalExists )
		Result = &Global.Registry->GetPathValue( Path, Global.Root, Exists, Buffer );
ERRErr
ERREnd
ERREpilog
	return *Result;
}

bso::bool__ rgstry::overloaded_registry___::GetPathValues(
	const term_ &PathString,
	epeios::row__ &PathErrorRow,
	values_ &Values ) const
{
	bso::bool__ Exists = false;
ERRProlog
	path Path;
	erow__ AttributeEntryRow = NONE;
ERRBegin
	Path.Init();

	if ( ( PathErrorRow = BuildPath( PathString, Path ) ) != NONE )
		ERRReturn;

	Exists = Local.Registry->GetPathValues( Path, Local.Root, Values ) || Global.Registry->GetPathValues( Path, Global.Root, Values );
ERRErr
ERREnd
ERREpilog
	return Exists;
}

bso::bool__ rgstry::overloaded_registry___::PathExists(
	const rgstry::term_ &PathString,
	epeios::row__ &PathRow ) const
{
	bso::bool__ Result = false;
ERRProlog
	path Path;
ERRBegin
	Path.Init();

	PathRow = BuildPath( PathString, Path );

	if ( PathRow != NONE )
		Result = Local.Registry->PathExists( Path, Local.Root ) || Global.Registry->PathExists( Path, Global.Root );
ERRErr
ERREnd
ERREpilog
	return Result;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class rgstrypersonnalization
: public rgstrytutor
{
public:
	rgstrypersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
		callback___::_Define.TagName.Init( DEFINE_TAG );
		callback___::_Define.AttributeName.Init( DEFINE_ATTRIBUTE );

		callback___::_Expand.TagName.Init( EXPAND_TAG );
		callback___::_Expand.ExplodeAttributeName.Init( EXPAND_EXPLODE_ATTRIBUTE );
		callback___::_Expand.IncludeAttributeName.Init( EXPAND_INCLUDE_ATTRIBUTE );

		callback___::_Bloc.TagName.Init( BLOC_TAG );

		callback___::_Prefix.Init( PREFIX );

	}
	~rgstrypersonnalization( void )
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

static rgstrypersonnalization Tutor;

ttr_tutor &RGSTRYTutor = Tutor;
