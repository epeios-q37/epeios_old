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

static BOOL SendStatusToSCM_(DWORD dwCurrentState,
					 DWORD dwWin32ExitCode,
					 DWORD dwServiceSpecificExitCode,
					 DWORD dwCheckPoint,
					 DWORD dwWaitHint);

static HANDLE hTerminateEvent_ = NULL;
static HANDLE hTerminateThread_ = NULL;
static HANDLE ThreadHandle_ = NULL;

static SERVICE_STATUS_HANDLE ServiceStatusHandle_;

static BOOL bPauseService_ = FALSE;
static BOOL bRunningService_ = FALSE;



static DWORD ServiceThread_(LPDWORD param)
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
static BOOL InitService_( void )
{
	DWORD id=0;	
	ThreadHandle_ = CreateThread(0,0,(LPTHREAD_START_ROUTINE)ServiceThread_,NULL,0,&id);
	if(ThreadHandle_==0)
	{
		if(ServiceStatusHandle_)  SendStatusToSCM_(SERVICE_STOPPED,0,0,0,0);
		if(hTerminateEvent_)		 CloseHandle(hTerminateEvent_);
		if(hTerminateThread_)	 CloseHandle(hTerminateThread_);

		return FALSE;
	}	
	bRunningService_=TRUE;
	return TRUE;	
}
// ------------------------------------------------------
static void ResumeService_()
{
	bPauseService_ = FALSE;
	ResumeThread(ThreadHandle_);
}
// ------------------------------------------------------
static void PauseService_()
{
	bPauseService_ = TRUE;
	SuspendThread(ThreadHandle_);
}
// ------------------------------------------------------
static void StopService_()
{
	bRunningService_=FALSE;
	SetEvent(hTerminateThread_);
}
// ------------------------------------------------------
static BOOL SendStatusToSCM_(DWORD dwCurrentState,
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
    
    bSuccess = SetServiceStatus(ServiceStatusHandle_,&ServiceStatus);
    if(!bSuccess) StopService_();
    return bSuccess;
}
// ------------------------------------------------------
static void ServiceCtrlHandler_(DWORD controlCode)
{
	DWORD currentState= 0;
	BOOL  bSuccess;
	switch(controlCode)
	{
		case SERVICE_CONTROL_STOP:
				currentState= SERVICE_STOP_PENDING;
				bSuccess= SendStatusToSCM_(SERVICE_STOP_PENDING,NO_ERROR,0,1,5000);
				StopService_();
				return;

		case SERVICE_CONTROL_PAUSE:
				if(bRunningService_ && !bPauseService_)
				{
					bSuccess= SendStatusToSCM_(SERVICE_PAUSE_PENDING,NO_ERROR,0,1,1000);
					PauseService_();
					currentState= SERVICE_PAUSED;
				}
				break;
		case SERVICE_CONTROL_CONTINUE:
				if(bRunningService_ && bPauseService_)
				{
					bSuccess= SendStatusToSCM_(SERVICE_CONTINUE_PENDING,NO_ERROR,0,1,1000);
					ResumeService_();
					currentState= SERVICE_RUNNING;
				}
				break;
		case SERVICE_CONTROL_INTERROGATE:break;

		case SERVICE_CONTROL_SHUTDOWN:
			ERRExit( EXIT_SUCCESS );
//			return;

		default:break;						
	}
	SendStatusToSCM_(currentState,NO_ERROR,0,0,0);
}
// ------------------------------------------------------
static void Terminate_(DWORD error)
{		
	if(ServiceStatusHandle_) SendStatusToSCM_(SERVICE_STOPPED,error,0,0,0);
	if(ThreadHandle_)		 CloseHandle(ThreadHandle_);	
	if(hTerminateEvent_)		 CloseHandle(hTerminateEvent_);
	if(hTerminateThread_)	 CloseHandle(hTerminateThread_);

	if ( error != 0 )
		ERRs();
}
// ------------------------------------------------------
static bso::bool__ AddService_(
	const char *ServiceName,
	const char *DisplayName,
	const char *Description )
{
	SC_HANDLE newService,scm;
//	CString strPath;
	char szFilename[255];

	GetModuleFileName(NULL,szFilename,sizeof(szFilename));
//	strPath=szFilename;

	scm= OpenSCManager(0,0,SC_MANAGER_CREATE_SERVICE);

//	if(!scm) ErrorHandler("Dans OpenScManager",GetLastError());

	if ( !scm )
		return false;

	newService= CreateService(scm,ServiceName,
							  ( DisplayName == NULL ? ServiceName : DisplayName ),
							  SERVICE_ALL_ACCESS,
							  SERVICE_WIN32_OWN_PROCESS,
							  SERVICE_AUTO_START,
							  SERVICE_ERROR_NORMAL,
							  szFilename,
							  0,0,0,0,0);
	if (!newService )
		return false;

	if ( Description != NULL ) {
		SERVICE_DESCRIPTION ServiceDescriptionStruct = { (char *)Description };

		ChangeServiceConfig2( newService, SERVICE_CONFIG_DESCRIPTION, &ServiceDescriptionStruct );
	}

	return true;
}
// ------------------------------------------------------
static bso::bool__ RemoveService_( const char *Name )
{
	SC_HANDLE newService,scm;
//	CString strPath;
	char szFilename[255];

	GetModuleFileName(NULL,szFilename,sizeof(szFilename));
//	strPath=szFilename;

	scm = OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);

//	if (!scm) ErrorHandler("Dans OpenScManager",GetLastError());

	if ( !scm )
		return false;

	newService=OpenService(scm,Name,SERVICE_ALL_ACCESS| DELETE);

	if(newService)	DeleteService(newService);	

//	if(!newService)	ErrorHandler("Dans DeleteService",GetLastError());

	if ( !newService )
		return false;

	return true;
}
// ------------------------------------------------------
static void ServiceMain_(DWORD argc,LPTSTR *argv)
{
	BOOL bSuccess;

	if ( argc < 1 )
		ERRs();
    
	ServiceStatusHandle_ = RegisterServiceCtrlHandler(argv[0],(LPHANDLER_FUNCTION)ServiceCtrlHandler_);
	if(!ServiceStatusHandle_)
	{
		Terminate_(GetLastError());
		return;
	}
	bSuccess= SendStatusToSCM_(SERVICE_START_PENDING,NO_ERROR,0,1,5000);
	if(!bSuccess)
	{
		Terminate_(GetLastError());
		return;
	}
	hTerminateEvent_ = CreateEvent(0,TRUE,FALSE,0);
	if(!hTerminateEvent_)
	{
		Terminate_(GetLastError());
		return;
	}
	hTerminateThread_ = CreateEvent(0,TRUE,FALSE,0);
	if(!hTerminateThread_)
	{
		Terminate_(GetLastError());
		return;
	}
	bSuccess= SendStatusToSCM_(SERVICE_START_PENDING,NO_ERROR,0,2,1000);
	if(!bSuccess)
	{
		Terminate_(GetLastError());
		return;
	}
	bSuccess = InitService_();
	if(!bSuccess)
	{
		Terminate_(GetLastError());
		return;
	}
	bSuccess= SendStatusToSCM_(SERVICE_RUNNING,NO_ERROR,0,0,0);
	if(!bSuccess)
	{
		Terminate_(GetLastError());
		return;
	}		
	WaitForSingleObject(hTerminateEvent_,INFINITE);
	Terminate_(0);
}

bso::bool__ wintol::service__::Install(
	const char *DisplayName,
	const char *Description )
{
	return AddService_( _Name, DisplayName, Description );
}

bso::bool__ wintol::service__::Remove( void )
{
	return RemoveService_( _Name );
}

bso::bool__ wintol::service__::Launch( void )
{
	tol::PatchSignalHandlingForWindowsService();

	SERVICE_TABLE_ENTRY serviceTable[]=
	{
		{ (char *)_Name,(LPSERVICE_MAIN_FUNCTION)ServiceMain_},
		{NULL , NULL}
	};

	while ( Service != NULL );	// Light lock simulation.

	Service = this;
 
	return ( StartServiceCtrlDispatcher(serviceTable) == TRUE );
}

bso::bool__ wintol::service__::TestTermination( void )
{
	if(::WaitForSingleObject(hTerminateThread_, 0) == WAIT_OBJECT_0)
	{
		// signale l'objet event d'attente et sort.
		::SetEvent(hTerminateEvent_);
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
