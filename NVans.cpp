// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include "NVansStrings.h"

// ---------------------------------------------------------------------------
USEFORM("NVansLocalTrains.cpp", frmLocalTrains);
USEFORM("NVansLogin.cpp", frmLogin);
USEFORM("NVansSearch.cpp", frmSearch);
USEFORM("NVansOptions.cpp", frmOptions);
USEFORM("NVansMain.cpp", Main);
USEFORM("NVansServerTrains.cpp", frmServerTrains);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
	HANDLE hMutex;

	try {
		hMutex = OpenMutex(MUTEX_ALL_ACCESS, false,
		LoadStr(IDS_APP_ID).w_str());

		if (hMutex) {
			ReleaseMutex(hMutex);

			CloseHandle(hMutex);

			HWND hWnd = FindWindow(TEXT("TApplication"),
				LoadStr(IDS_APP_TITLE).w_str());

			SwitchToThisWindow(hWnd, true);

			return 0;
		}

		hMutex = CreateMutex(NULL, false, LoadStr(IDS_APP_ID).w_str());

		Application->Initialize();

		Application->MainFormOnTaskBar = false;

		Application->Title = LoadStr(IDS_APP_TITLE);

		Application->CreateForm(__classid(TMain), &Main);
		Application->CreateForm(__classid(TfrmServerTrains), &frmServerTrains);
		Application->CreateForm(__classid(TfrmLocalTrains), &frmLocalTrains);
		Application->CreateForm(__classid(TfrmSearch), &frmSearch);
		Application->Run();
	}
	catch (Exception &exception) {
		Application->ShowException(&exception);
	}
	catch (...) {
		try {
			throw Exception("");
		}
		catch (Exception &exception) {
			Application->ShowException(&exception);
		}
	}

	ReleaseMutex(hMutex);

	CloseHandle(hMutex);

	return 0;
}

// ---------------------------------------------------------------------------
