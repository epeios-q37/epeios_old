// $Id$

#include "stf.h"
#include "epsmsc.h"
#include "clnarg.h"
#include "fil.h"
#include "brkanl.h"
#include "sck.h"
#include "clt.h"

#define NAME			"ebrk2py"
#define VERSION			"0.0.1"
#define AUTHOR_LINK		EPSMSC_AUTHOR_LINK
#define AUTHOR			EPSMSC_AUTHOR
#define AUTHOR_EMAIL	EPSMSC_AUTHOR_EMAIL

enum type {
	tIn,
	tOut
};

enum command {
        cHelp,
        cVersion,
        cGenerate
};

static const char *Filter( const str_string_ &String )
{
	static char Buffer[50];
	char *P;
	
	if ( String.Amount() >= sizeof( Buffer ) )
		ERRl();
		
	String.Read( 0, String.Amount(), Buffer );
	
	P = strchr( Buffer, '$' );
	
	if ( !P )
		ERRu();
		
	*P = 0;

	return Buffer;
}


void PrintVariableType(
	broker8__cast Parameter,
	txf__text_oflow_ &Flow )
{
	switch ( Parameter ) {
	case broker0::cObject:
		Flow << "Object";
		break;
	case broker0::cType:
		Flow << "Type";
		break;
	case broker0::cCasts:
		Flow << "Casts";
		break;
	case broker0::cCommand:
		Flow << "Command";
		break;
	case broker0::cBoolean:
		Flow << "Boolean";
		break;
	case broker0::cId8:
		Flow << "Id8";
		break;
	case broker0::cId16:
		Flow << "Id16";
		break;
	case broker0::cId32:
		Flow << "Id32";
		break;
	case broker0::cChar:
		Flow << "Char";
		break;
	case broker0::cString:
		Flow << "String";
		break;
	case broker0::cByte:
		Flow << "Byte";
		break;
	case broker0::cBinary:
		Flow << "Binay";
		break;
	case broker0::cPointer:
		Flow << "Pointer";
		break;
	case broker0::cMulti:
		Flow << "Multi";
		break;
	default:
		ERRu();
		break;
	}
}


void PrintVariableName(
	broker8__cast Parameter,
	type Type,
	int Number,
	txf__text_oflow_ &Flow )
{
	
	switch( Type ) {
	case tIn:
		Flow << 'I';
		break;
	case tOut:
		Flow << 'O';
		break;
	default:
		ERRc();
		break;
	}
		
	Flow << (unsigned long)Number;
	
	PrintVariableType( Parameter, Flow );

}

void PrintParameters(
	const brkanl::broker_parameters_ &Parameters,
	txf__text_oflow_ &Flow )
{
	POSITION__ P = Parameters.First();
	int Number = 1;
	type Type = tIn;
	
	while( ( P != NONE ) && ( Parameters( P ) != broker0::cEnd ) ) {
		Flow << ", ";
		PrintVariableName( Parameters( P ), Type, Number++, Flow );
		P = Parameters.Next( P );
	}
}

void PrintBody( 
	broker8__cast Parameter,
	type Type,
	int Number,
	txf__text_oflow_ &Flow )
{	

	Flow << tab << tab;

	switch( Type ) {
	case tIn:
		Flow << "self.Broker_.Add";
		PrintVariableType( Parameter, Flow );		
		Flow << "( ";
		PrintVariableName( Parameter, Type, Number, Flow );
		Flow << " )" << nl;
		break;
	case tOut:
		PrintVariableName( Parameter, Type, Number, Flow );
		Flow << " = self.Broker_.Get";
		PrintVariableType( Parameter, Flow );		
		Flow << "()" << nl;
		break;
	default:
		ERRc();
		break;
	}


}

void PrintInstructions(
	const brkanl::broker_parameters_ &Parameters,
	txf__text_oflow_ &Flow )
{
	POSITION__ P = Parameters.First();
	int Number = 1;
	type Type = tIn;
	
	while( P != NONE ) {
		if ( Parameters( P ) == broker0::cEnd ) {
			Number = 1;
			Type = tOut;
			Flow << tab << tab << "self.Broker_.Send()" << nl;
		} else
			PrintBody( Parameters( P ), Type, Number++, Flow );
			
		P = Parameters.Next( P );
	}
	
	Flow << tab << tab << "self.Broker_.TestCompletion()" << nl;

}


void PrintReturn(
	const brkanl::broker_parameters_ &Parameters,
	txf__text_oflow_ &Flow )
{
	POSITION__ P = Parameters.First();
	int Number = 1;
	type Type = tOut;
	
	while( ( P != NONE ) && ( Parameters( P ) != broker0::cEnd ) )
		P = Parameters.Next( P );
	
	P = Parameters.Next( P );
	
	Flow << tab << tab << "return [ ";
	
	while(  P != NONE ) {
		PrintVariableName( Parameters( P ), Type, Number++, Flow );
		Flow << ", ";
			
		P = Parameters.Next( P );
	}
	
	Flow << "None ]" << nl;
}


void PrintBody(
	const brkanl::broker_parameters_ &Parameters,
	txf__text_oflow_ &Flow )
{
	PrintInstructions( Parameters, Flow );
	PrintReturn( Parameters, Flow );
}


void PrintBody(
	const brkanl::broker_command_ &Command,
	txf__text_oflow_ &Flow )
{
	Flow << tab << tab << "self.Broker_.PutObject( self.ID_ )" << nl;
	Flow << tab << tab << "self.Broker_.PutCommand( " << Command.Identification.ID() << " )" << nl;
	
	PrintBody( Command.Parameters, Flow );	
}
	

void Print(
	const brkanl::broker_commands_ &Commands,
	txf__text_oflow_ &Flow )
{
	CITEM( brkanl::broker_command_ ) Command;
	POSITION__ P = Commands.First();

	Command.Init( Commands );

	while( P != NONE ) {
		Flow << tab << "def " << Filter( Command( P ).Identification.Value ) << "( self" ;
		PrintParameters( Command( P ).Parameters, Flow );
		Flow << " ):" << nl;
		PrintBody( Command( P ), Flow );
		P = Commands.Next( P );
	}

	fout << nl;
}

void Print(
	const brkanl::broker_types_ &Types,
	txf__text_oflow_ &Flow )
{
	CITEM( brkanl::broker_type_ ) Type;
	POSITION__ P = Types.First();

	Type.Init( Types );

	while( P != NONE ) {
		if ( Type( P ).Identification.ID() == (bso__ushort)-1 )
			Flow << "class epeios_master :" << nl;
		else
			Flow << "class " << Filter( Type( P ).Identification.Value ) << ':' << nl;
			
		Flow << tab << "def __init__( self, Broker ):" << nl;
		Flow << tab << tab << "self.Broker_ = Broker" << nl;
		
		if ( Type( P ).Identification.ID() == (bso__ushort)-1 )
			Flow << tab << tab << "self.ID_ = -1" << nl;
		else	
			Flow << tab << tab << "self.ID_ = Broker_.GetNewObject( " << Type( P ).Identification.ID() << " )" << nl;
			
		Flow << tab << "def __del__( self ):" << nl;
		Flow << tab << tab << "Broker_.removeObject( self.ID_ )" << nl;
		
		Print( Type( P ).Commands, Flow );
		
		P = Types.Next( P );
	}
}



void GetBrokerDesc(
	const char *BrokerRef,
	brkanl::broker_types_ &Broker )
{
ERRProlog
	sck__socket_flow_ Socket;
	broker8__core_ Core;
	broker8__data Data;
	bso__bool Connected = false;
ERRBegin
	Socket.Init( CLTConnect( BrokerRef ) );
	Core.Init( Socket, Data );
	Connected = true;
	
	brkanl::Analyze( Core, Broker );
ERRErr
ERREnd
	if ( Connected )
		Core.Disconnect();
ERREpilog
}

void Go(
	const char *BrokerRef,
	txf__text_oflow_ &Flow )
{
ERRProlog
	brkanl::broker_types Broker;
ERRBegin
	Broker.Init();

	GetBrokerDesc( BrokerRef, Broker );
	
	Print( Broker, Flow );
ERRErr
ERREnd
ERREpilog
}

void PrintUsage( void )
{
        fout << "Usage: " << NAME << " [command] broker [dest-file]" << nl;
        fout << "broker:" << tab << tab << "broker reference (host:service)" << nl;
        fout << "dest-file:" << tab << "destination file; stdout if none." << nl;
        fout << "Command: " << nl;
        fout << tab << "<none>, -g, --generate:" << nl << tab << tab << "generate python source" << nl;
        fout << tab << "--version:" << nl << tab << tab << "print version of " NAME " components." << nl;
        fout << tab << "--help:" << tab << "print this message." << nl;
}

void PrintHeader( void )
{
        fout << NAME " V" VERSION " "__DATE__ " " __TIME__;
        fout << " by "AUTHOR " (" AUTHOR_EMAIL ")" << nl;
        fout << "Copyright the Epeios project (" EPSMSC_EPEIOS_URL "). " << nl;
//      fout << EPSMSC_GNU_TEXT << nl;
}

static void AnalyzeFreeArguments(
        clnarg__analyzer_ &Analyzer,
        char *&Broker,
        char *&Dest )
{
ERRProlog
        clnarg_arguments Free;
        POSITION__ P;
ERRBegin
        Free.Init();

        Analyzer.GetArguments( Free );

        P = Free.Last();

        switch( Free.Amount() ) {
        case 2:
                Dest = Free( P ).Convert();
                P = Free.Previous( P );
        case 1:
                Broker = Free( P ).Convert();
                P = Free.Previous( P );
                break;
        case 0:
                ferr << "Too few arguments." << nl;
                fout << "Try '" NAME " --help' to get more informations." << nl;
                ERRt();
                break;
        default:
                ferr << "Too many arguments." << nl;
                fout << "Try '" NAME " --help' to get more informations." << nl;
                ERRt();
                break;
        }

ERRErr
ERREnd
ERREpilog
}

static void AnalyzeArgs(
        int argc,
        const char *argv[],
        char *&Broker,
        char *&Dest )
{
ERRProlog
        clnarg_description Description;
        clnarg__analyzer_ Analyzer;
ERRBegin
        Description.Init();

        Description.AddCommand( 'g', "generate", cGenerate );
        Description.AddCommand( CLNARG_NO_SHORT, "version", cVersion );
        Description.AddCommand( CLNARG_NO_SHORT, "help", cHelp );

        Analyzer.Init( argc, argv, Description );

        switch ( Analyzer.GetCommand() ) {
        case cVersion:
                PrintHeader();
                TTR.Advertise();
                ERRt();
                break;
        case cHelp:
                PrintUsage();
                ERRt();
                break;
        case cGenerate:
        case CLNARG_NONE:
                break;
        default:
                ERRc();
        }

        AnalyzeFreeArguments( Analyzer, Broker, Dest );

ERRErr
ERREnd
ERREpilog
}

static inline void Main(
        int argc,
        const char *argv[] )
{
ERRProlog
        fil::file_oflow___ OFile;
		txf__text_oflow_ OText;
        txf__text_oflow_ *OFlow = NULL;
        bso__bool Backup = false;
        char *Broker = NULL;
        char *Dest = NULL;
ERRBegin
        AnalyzeArgs( argc, argv, Broker, Dest );

        if ( Dest ) {

                TOLCreateBackupFile( Dest, tol::hbfRename );
                Backup = true;

                if ( OFile.Init( Dest, err::hSkip ) != fil::sSuccess )
                {
                        ferr << "Error while opening '" << Dest << "' for writing." << nl;
                        ERRt();
                }

                OText.Init( OFile );

                OFlow = &OText;
        }
        else
                OFlow = &fout;

        Go( Broker, *OFlow );

ERRErr
        OFile.reset();

        if ( Backup )
                TOLRecoverBackupFile( Dest );
ERREnd
ERREpilog
}


int main(
	int argc,
	const char *argv[] )
{
	int ExitValue = EXIT_SUCCESS;
ERRFProlog
ERRFBegin
	Main( argc, argv );
ERRFErr
	if ( ERRMajor == err::thw )
		ERRRst()
	else
		ExitValue = EXIT_FAILURE;
ERRFEnd
ERRFEpilog
	return ExitValue;
}
