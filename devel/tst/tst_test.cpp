/*
  Test source for the 'tst' library by Claude L. Simon (simon@epeios.org).
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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

#if 0
/*
 * hworld.cpp
 * Hello world sample by Robert Roebling
 */

#include "wx/wx.h" 
#include "flx.h"
#include "str.h"

class MyApp: public wxApp
{
    virtual bool OnInit();
};

class MyFrame: public wxFrame
{
public:

    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};

enum
{
    ID_Quit = 1,
    ID_About,
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame( "Hello World", wxPoint(50,50), wxSize(450,340) );
    frame->Show(TRUE);
    SetTopWindow(frame);
    return TRUE;
} 

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;

    menuFile->Append( ID_About, "&About..." );
    menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, "E&xit" );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText( "Welcome to wxWindows!" );
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
ERRFProlog
	str::string String;
	flx::bunch_oflow__<str::string_, bso::char__> Flow;
	txf::text_oflow__ TFlow;
	char *Buffer = NULL;
ERRFBegin
	String.Init();
	Flow.Init( String );
	TFlow.Init( Flow );

	TTR.Advertise( TFlow );
	TFlow.reset();
	Flow.reset();

//	ERRi();

	Buffer = String.Convert();

    wxMessageBox(Buffer,
        "About Hello World", wxOK | wxICON_INFORMATION, this);
ERRFErr
ERRFEnd
	tol::Free( Buffer );
ERRFEpilog
}
#endif

#if 0

namespace dbibsc {
	//e Field types.
	enum field_type__ {
		//i Unknow type.
		ftUnknow,
		//i Record id (used by slave tables).
		ftRecordId,
		//i Boolean,
		ftLogical,
		//i Signed integer type (32bits)
		ftInteger,
		//i Float type.
		ftFloat,
		//i Alphanumeric, indexed but not dynamic.
		ftIndexedText,
		//i Alphanumeric, dynamic but not indexed.
		ftDynamicText,
		//i Date.
		ftDate,
		//i Composite field, for indexation.
		ftComposite,
		// amount of field type.
		ft_amount
	};
}

namespace dbimng {
	using dbibsc::field_type__;
}

namespace gsbbsc {	
	//e Field types.
	enum field_type__ {
		ft_first,
		//i Numeric.
		ftNumeric = ft_first,
		//i Date.
		ftDate,
		//i Text.
		ftText,
		//i Relational,
		ftRelational,
		ft_last = ftRelational,
		//i Relational to itself.
		ft_amount,
		//i Unknown field type.
		ft_Undefined,
	};

//	dbibsc::field_type__ Convert( field_type__ Type );
}

using namespace gsbbsc;

void Test( void )
{
	field_type__ FieldType = ft_amount;
}

#endif

#if 0

#include "tht.h"

#include "signal.h"
#include "cio.h"
#include "tol.h"

/*
void f_signal(int s)
{
	cio::cout << "Fonction associée à 'signal()' : " << (bso::ulong__)s << txf::nl << txf::sync;
	exit( EXIT_SUCCESS );
}
*/

void f_atexit1( void )
{
	cio::cout << "Fonction associée à 'atexit()' 1" << txf::nl << txf::sync;
	tht::Wait( 2 );
}

void f_atexit2( void )
{
	cio::cout << "Fonction associée à 'atexit()' 2" << txf::nl << txf::sync;
	tht::Wait( 2 );
}


int main( int argc, const char **argv )
{
ERRFProlog
ERRFBegin
/*
	signal( SIGBREAK, f_signal );
	signal( SIGTERM, f_signal );
	signal( SIGABRT, f_signal );
	signal( SIGINT, f_signal );
*/
	tol::SignalExits();

	atexit( f_atexit1 );
	atexit( f_atexit2 );

	for(;;)
		tht::Defer( 10 );
ERRFErr
ERRFEnd
	cio::cout << "Passe par le destructeur. " << txf::nl << txf::sync;
ERRFEpilog
	return EXIT_SUCCESS;
}

#endif

#if 0
class a
{
protected:
	virtual void f( void ) = 0;
};

class b
: public a
{
	void f( void ) {}
};

template <typename t> class tt
{
public:
	t TT;
};

template <typename t> class at
{
protected:
	virtual tt<t> &f( void ) = 0;
};

template <typename t, typename v> class bt
: public t
{
	tt<v> T;
	tt<v> &f( void ) {return T;}
};

void Bug( void )
{
	b B;
	bt<at<int>,int> BT;
}
#endif

#if 0

	template <typename t> class bunch_
	{};

	template <typename type__> class bunch_caller__
	{
	protected:
		virtual const bunch_<type__> &VF( void ) = 0;
	};

	// The core of a cache of static objects of type 'type'.
	template <typename type__, typename bc> class bunch_cache___
	: public bc
	{
	};

	//c A read-only cache of static object of type 't'.
	template <typename type__> class core_cache___
	: public bunch_cache___<type__, bunch_caller__<type__> >
	{
	private:
	public:
		const bunch_<type__> &F( void )
		{
			return this->VF();
		}
	};



	//c A read-only cache of static object of type 't'.
	template <typename type__> class cache___
	: public core_cache___<type__>
	{
	private:
		const bunch_<type__> *Bunch_;
	protected:
		virtual const bunch_<type__> &VF( void )
		{
			return *Bunch_;
		}
	};




int main( int argc, const char **argv )
{
	cache___<int> Cache;

	Cache.F();

	return EXIT_SUCCESS;
}

#endif

#if 0

#include "bso.h"
#include "err.h"
#include "cio.h"
#include "ltf.h"
#include "mmi.h"
#include "xtf.h"

int main( int argc, const char **argv )
{
ERRFProlog
	xtf::extended_text_iflow__ XFlow;
	str::string Line;
ERRFBegin
	XFlow.Init( cio::cinf );

#if 0
/*	while ( 1 )
		cio::cout << (char)cio::cin.Get() << txf::sync;
*/
	while ( !XFlow.EOX() ) {
		Line.Init();
		XFlow.GetLine( Line );
		cio::cout << Line << txf::nl;
	}

#else
	while ( 1 )
		cio::cout << (char)getchar() << txf::sync;
#endif

ERRFErr
ERRFEpilog
ERRFEnd
}
#endif

#if 1
// TestService.cpp : Defines the entry point for the console application.
//
// Exemple de service windows comprenat l'installation l'execution 
// et la desinstallation du service.
//  
// Farscape le 16/09/2004 : farscape-dev@tiscali.fr
//
//#include "stdafx.h"
#include <windows.h>
//#include <Winsvc.h>

#include <iostream>


#include "tol.h"
#include "flf.h"
#include "fnm.h"
#include <stdlib.h>

//#include "TestService.h"

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

//CWinApp theApp;

#define LOCK_FILE_NAME	"LOCK"

static void CreateLockFile_( void )
{
ERRProlog
	tol::E_FPOINTER___( char ) Buffer;
	flf::file_oflow___ OFlow;
	txf::text_oflow__ TOFlow( OFlow );
ERRBegin
Buffer = fnm::BuildFileName( "c:\\temp\\", LOCK_FILE_NAME, NULL );

/*
	if ( tol::FileExists( Buffer ) ) {
		Status = sRepositoryLocked;
		ERRReturn;
	}
*/
	OFlow.Init( Buffer );
	TOFlow << tol::DateAndTime();
ERRErr
ERREnd
ERREpilog
}

static void RemoveLockFile_( void )
{
ERRProlog
	tol::E_FPOINTER___( char ) Buffer;
ERRBegin
	Buffer = fnm::BuildFileName( "c:\\temp\\", LOCK_FILE_NAME, NULL );

	if ( tol::FileExists( Buffer ) )
		tol::RemoveFile( Buffer );
ERRErr
ERREnd
ERREpilog
}



using namespace std;

BOOL SendStatusToSCM(DWORD dwCurrentState,
					 DWORD dwWin32ExitCode,
					 DWORD dwServiceSpecificExitCode,
					 DWORD dwCheckPoint,
					 DWORD dwWaitHint);

char *SERVICE_NAME= "TestService";

HANDLE hTerminateEvent=NULL;
HANDLE hTerminateThread=NULL;
HANDLE ThreadHandle = NULL;

SERVICE_STATUS_HANDLE ServiceStatusHandle;

BOOL bPauseService=FALSE;
BOOL bRunningService=FALSE;

// ------------------------------------------------------
void ErrorHandler(const char *s,DWORD err)
{
	std::cout << s << endl;
	std::cout << "Erreur Numéro: " << err << endl;
	ExitProcess(err);
}
// ------------------------------------------------------
DWORD ServiceThread(LPDWORD param)
{		
	CreateLockFile_();

//	atexit( RemoveLockFile_ );

	while(1)
	{
		if(::WaitForSingleObject(hTerminateThread, 0) == WAIT_OBJECT_0)
		{
			// signale l'objet event d'attente et sort.
			::SetEvent(hTerminateEvent);
			return 0;
		}
		// placer le code de votre service ici
		Sleep(5000); // juste pour l'exemple.
    }			
	return 0;
}
// ------------------------------------------------------
BOOL InitService()
{
	DWORD id=0;	
	ThreadHandle = CreateThread(0,0,(LPTHREAD_START_ROUTINE)ServiceThread,NULL,0,&id);
	if(ThreadHandle==0)
	{
		if(ServiceStatusHandle)  SendStatusToSCM(SERVICE_STOPPED,0,0,0,0);
		if(hTerminateEvent)		 CloseHandle(hTerminateEvent);
		if(hTerminateThread)	 CloseHandle(hTerminateThread);

		ErrorHandler("Erreur sur la mise en place du service",0);
		return FALSE;
	}	
	bRunningService=TRUE;
	return TRUE;	
}
// ------------------------------------------------------
void ResumeService()
{
	bPauseService = FALSE;
	ResumeThread(ThreadHandle);
}
// ------------------------------------------------------
void PauseService()
{
	bPauseService = TRUE;
	SuspendThread(ThreadHandle);
}
// ------------------------------------------------------
void StopService()
{
	bRunningService=FALSE;
	SetEvent(hTerminateThread);
}
// ------------------------------------------------------
BOOL SendStatusToSCM(DWORD dwCurrentState,
                     DWORD dwWin32ExitCode,
                     DWORD dwServiceSpecificExitCode,
                     DWORD dwCheckPoint,
                     DWORD dwWaitHint)
{
    BOOL bSuccess;
    SERVICE_STATUS ServiceStatus;
    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwCurrentState= dwCurrentState;
    
    ServiceStatus.dwControlsAccepted=0;
    if(dwCurrentState != SERVICE_START_PENDING)        
    ServiceStatus.dwControlsAccepted= SERVICE_ACCEPT_STOP |
                                      SERVICE_ACCEPT_SHUTDOWN|         
                                      SERVICE_ACCEPT_PAUSE_CONTINUE;

    ServiceStatus.dwWin32ExitCode=ERROR_SERVICE_SPECIFIC_ERROR;
    if(!dwServiceSpecificExitCode) ServiceStatus.dwWin32ExitCode=dwWin32ExitCode;
    
    ServiceStatus.dwServiceSpecificExitCode=dwServiceSpecificExitCode;
    
    ServiceStatus.dwCheckPoint = dwCheckPoint;
    ServiceStatus.dwWaitHint = dwWaitHint;
    
    bSuccess = SetServiceStatus(ServiceStatusHandle,&ServiceStatus);
    if(!bSuccess) StopService();
    return bSuccess;
}
// ------------------------------------------------------
void ServiceCtrlHandler(DWORD controlCode)
{
	DWORD currentState= 0;
	BOOL  bSuccess;
	switch(controlCode)
	{
		case SERVICE_CONTROL_STOP:
				currentState= SERVICE_STOP_PENDING;
				bSuccess= SendStatusToSCM(SERVICE_STOP_PENDING,NO_ERROR,0,1,5000);
				StopService();
				return;

		case SERVICE_CONTROL_PAUSE:
				if(bRunningService && !bPauseService)
				{
					bSuccess= SendStatusToSCM(SERVICE_PAUSE_PENDING,NO_ERROR,0,1,1000);
					PauseService();
					currentState= SERVICE_PAUSED;
				}
				break;
		case SERVICE_CONTROL_CONTINUE:
				if(bRunningService && bPauseService)
				{
					bSuccess= SendStatusToSCM(SERVICE_CONTINUE_PENDING,NO_ERROR,0,1,1000);
					ResumeService();
					currentState= SERVICE_RUNNING;
				}
				break;
		case SERVICE_CONTROL_INTERROGATE:break;

		case SERVICE_CONTROL_SHUTDOWN:return;

		default:break;						
	}
	SendStatusToSCM(currentState,NO_ERROR,0,0,0);
}
// ------------------------------------------------------
void Terminate(DWORD error)
{		
	if(ServiceStatusHandle) SendStatusToSCM(SERVICE_STOPPED,error,0,0,0);
	if(ThreadHandle)		 CloseHandle(ThreadHandle);	
	if(hTerminateEvent)		 CloseHandle(hTerminateEvent);
	if(hTerminateThread)	 CloseHandle(hTerminateThread);
}
// ------------------------------------------------------
void AddService()
{
	SC_HANDLE newService,scm;
//	CString strPath;
	char szFilename[255];

	GetModuleFileNameA(NULL,szFilename,sizeof(szFilename));
//	strPath=szFilename;

	scm= OpenSCManager(0,0,SC_MANAGER_CREATE_SERVICE);
	if(!scm) ErrorHandler("Dans OpenScManager",GetLastError());
	newService= CreateServiceA(scm,SERVICE_NAME,
							  SERVICE_NAME,
							  SERVICE_ALL_ACCESS,
							  SERVICE_WIN32_OWN_PROCESS,
							  SERVICE_DEMAND_START,
							  SERVICE_ERROR_NORMAL,
							  szFilename,
							  0,0,0,0,0);
	if(!newService)	ErrorHandler("Dans CreateService",GetLastError());
	else std::cout << "Service " << SERVICE_NAME << " Installé\n";
}
// ------------------------------------------------------
void DeleteService()
{
	SC_HANDLE newService,scm;
//	CString strPath;
	char szFilename[255];

	GetModuleFileNameA(NULL,szFilename,sizeof(szFilename));
//	strPath=szFilename;

	scm= OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);
	if(!scm) ErrorHandler("Dans OpenScManager",GetLastError());

	newService=OpenServiceA(scm,SERVICE_NAME,SERVICE_ALL_ACCESS| DELETE);
	if(newService)	DeleteService(newService);	
	if(!newService)	ErrorHandler("Dans DeleteService",GetLastError());
	else std::cout << "Service "<< SERVICE_NAME << " Supprimé\n";
}
// ------------------------------------------------------
void ServiceMain(DWORD argc,LPTSTR *argv)
{
	BOOL bSuccess;
    
	ServiceStatusHandle = RegisterServiceCtrlHandlerA(SERVICE_NAME,(LPHANDLER_FUNCTION)ServiceCtrlHandler);
	if(!ServiceStatusHandle)
	{
		Terminate(GetLastError());
		return;
	}
	bSuccess= SendStatusToSCM(SERVICE_START_PENDING,NO_ERROR,0,1,5000);
	if(!bSuccess)
	{
		Terminate(GetLastError());
		return;
	}
	hTerminateEvent = CreateEvent(0,TRUE,FALSE,0);
	if(!hTerminateEvent)
	{
		Terminate(GetLastError());
		return;
	}
	hTerminateThread = CreateEvent(0,TRUE,FALSE,0);
	if(!hTerminateThread)
	{
		Terminate(GetLastError());
		return;
	}
	bSuccess= SendStatusToSCM(SERVICE_START_PENDING,NO_ERROR,0,2,1000);
	if(!bSuccess)
	{
		Terminate(GetLastError());
		return;
	}
	bSuccess = InitService();
	if(!bSuccess)
	{
		Terminate(GetLastError());
		return;
	}
	bSuccess= SendStatusToSCM(SERVICE_RUNNING,NO_ERROR,0,0,0);
	if(!bSuccess)
	{
		Terminate(GetLastError());
		return;
	}		
	WaitForSingleObject(hTerminateEvent,INFINITE);
	Terminate(0);
}
//------------------------------------------------------------------------------------------
int main(int argc, const char** argv )
{
	SERVICE_TABLE_ENTRYA serviceTable[]=
	{
		{SERVICE_NAME,(LPSERVICE_MAIN_FUNCTIONA)ServiceMain},
		{NULL		 , NULL}
	};
		
	// initialize MFC and print and error on failure
/*
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		ErrorHandler("Fatal Error: MFC initialization failed",0);
		return 1;
	}
*/
/*
	CString strCmdLine;
	if(argc>1) strCmdLine=argv[1];	
	strCmdLine.MakeUpper();
*/

	if ( argc > 1 ) {
		if(!strcmp( argv[1], "INSTALL"))
		{
			AddService();
			return 0;
		}
		if(!strcmp( argv[1], "DELETE"))
		{
			DeleteService();
			return 0;
		}
	}
	
	BOOL bSuccess;	
	bSuccess= StartServiceCtrlDispatcherA(serviceTable);
	if(!bSuccess) ErrorHandler("Dans StartServiceCtrlDispatcher",GetLastError());
	return bSuccess;	
}
#endif