// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

// ---------------------------------------------------------------------------
USEFORM("NVansMain.cpp", Main);
USEFORM("NVansLogin.cpp", frmLogin);
USEFORM("NVansOptions.cpp", frmOptions);
USEFORM("NVansLocalTrains.cpp", frmLocalTrains);
USEFORM("NVansServerTrains.cpp", frmServerTrains);

// ---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
	HANDLE hMutex;

	try {
		hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, L"ru.p3tr0vich.NVans");

		if (hMutex) {
			ReleaseMutex(hMutex);

			CloseHandle(hMutex);

			HWND hWnd = FindWindow(TEXT("TApplication"), TEXT("Натурный лист"));

			SwitchToThisWindow(hWnd, true);

			return 0;
		}

		hMutex = CreateMutex(NULL, false, L"ru.p3tr0vich.NVans");

		Application->Initialize();

		Application->MainFormOnTaskBar = false;

		Application->Title = "Натурный лист";

		Application->CreateForm(__classid(TMain), &Main);
		Application->CreateForm(__classid(TfrmServerTrains), &frmServerTrains);
		Application->CreateForm(__classid(TfrmLocalTrains), &frmLocalTrains);

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
