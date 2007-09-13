/*
	'wintol' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'wintol' header file ('wintol.h').
	Copyright (C) 2004 Claude SIMON.

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

#define WINTOL__COMPILATION

#include "wintol.h"

class wintoltutor
: public ttr_tutor
{
public:
	wintoltutor( void )
	: ttr_tutor( WINTOL_NAME )
	{
#ifdef WINTOL_DBG
		Version = WINTOL_VERSION "\b\bD $";
#else
		Version = WINTOL_VERSION;
#endif
		Owner = WINTOL_OWNER;
		Date = "$Date$";
	}
	virtual ~wintoltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace wintol;
#include "tol.h"

wintol::service__ *Service = NULL;

// Inspired from http://cpp.developpez.com/faq/vc/?page=ProcessThread#HowToMakeService 

#include <windows.h>

BOOL SendStatusToSCM(DWORD dwCurrentState,
					 DWORD dwWin32ExitCode,
					 DWORD dwServiceSpecificExitCode,
					 DWORD dwCheckPoint,
					 DWORD dwWaitHint);

HANDLE hTerminateEvent=NULL;
HANDLE hTerminateThread=NULL;
HANDLE ThreadHandle = NULL;

SERVICE_STATUS_HANDLE ServiceStatusHandle;

BOOL bPauseService=FALSE;
BOOL bRunningService=FALSE;



DWORD ServiceThread(LPDWORD param)
{		
/*
	if(::WaitForSingleObject(hTerminateThread, 0) == WAIT_OBJECT_0)
	{
		// signale l'objet event d'attente et sort.
		::SetEvent(hTerminateEvent);
		return 0;
	}
*/
	service__ *Service = ::Service;

	::Service = NULL;

	Service->Callback();

	return 0;
}
// ------------------------------------------------------
BOOL InitService( void )
{
	DWORD id=0;	
	ThreadHandle = CreateThread(0,0,(LPTHREAD_START_ROUTINE)ServiceThread,NULL,0,&id);
	if(ThreadHandle==0)
	{
		if(ServiceStatusHandle)  SendStatusToSCM(SERVICE_STOPPED,0,0,0,0);
		if(hTerminateEvent)		 CloseHandle(hTerminateEvent);
		if(hTerminateThread)	 CloseHandle(hTerminateThread);

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
	::SetEvent(hTerminateEvent);
	if(ThreadHandle)
		CloseHandle(ThreadHandle);	
	exit( EXIT_SUCCESS );
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

		case SERVICE_CONTROL_SHUTDOWN:
			ERRExit( EXIT_SUCCESS );
//			return;

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

	if ( error != 0 )
		ERRs();
}
// ------------------------------------------------------
void AddService( const char *Name )
{
	SC_HANDLE newService,scm;
//	CString strPath;
	char szFilename[255];

	GetModuleFileName(NULL,szFilename,sizeof(szFilename));
//	strPath=szFilename;

	scm= OpenSCManager(0,0,SC_MANAGER_CREATE_SERVICE);

//	if(!scm) ErrorHandler("Dans OpenScManager",GetLastError());

	if ( !scm )
		ERRs();

	newService= CreateService(scm,Name,
							  Name,
							  SERVICE_ALL_ACCESS,
							  SERVICE_WIN32_OWN_PROCESS,
							  SERVICE_DEMAND_START,
							  SERVICE_ERROR_NORMAL,
							  szFilename,
							  0,0,0,0,0);
	if (!newService )
		ERRs();
}
// ------------------------------------------------------
void RemoveService( const char *Name )
{
	SC_HANDLE newService,scm;
//	CString strPath;
	char szFilename[255];

	GetModuleFileName(NULL,szFilename,sizeof(szFilename));
//	strPath=szFilename;

	scm = OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);

//	if (!scm) ErrorHandler("Dans OpenScManager",GetLastError());

	if ( !scm )
		ERRs();

	newService=OpenService(scm,Name,SERVICE_ALL_ACCESS| DELETE);

	if(newService)	DeleteService(newService);	

//	if(!newService)	ErrorHandler("Dans DeleteService",GetLastError());

	if ( !newService )
		ERRs();
}
// ------------------------------------------------------
void ServiceMain(DWORD argc,LPTSTR *argv)
{
	BOOL bSuccess;

	if ( argc < 1 )
		ERRs();
    
	ServiceStatusHandle = RegisterServiceCtrlHandler(argv[0],(LPHANDLER_FUNCTION)ServiceCtrlHandler);
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

void wintol::service__::Install( void )
{
	AddService( _Name );
}

void wintol::service__::Remove( void )
{
	RemoveService( _Name );
}

void wintol::service__::Launch( void )
{
ERRProlog
	tol::E_FPOINTER___( char ) Buffer;
ERRBegin
	Buffer = malloc( strlen( _Name ) + 1 );

	strcpy( Buffer, _Name );

	SERVICE_TABLE_ENTRY serviceTable[]=
	{
		{ Buffer,(LPSERVICE_MAIN_FUNCTION)ServiceMain},
		{NULL , NULL}
	};

	while ( Service != NULL );	// Light mutex simulation.

	Service = this;
 
	if ( !StartServiceCtrlDispatcher(serviceTable) )
		ERRs();

ERRErr
ERREnd
ERREpilog
}

bso::bool__ wintol::service__::TestTermination( void )
{
	if(::WaitForSingleObject(hTerminateThread, 0) == WAIT_OBJECT_0)
	{
		// signale l'objet event d'attente et sort.
		::SetEvent(hTerminateEvent);
		return true;
	} else
		return false;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class wintolpersonnalization
: public wintoltutor
{
public:
	wintolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~wintolpersonnalization( void )
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

static wintolpersonnalization Tutor;

ttr_tutor &WINTOLTutor = Tutor;
