// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
// ---------------------------------------------------------------------------
USEFORM("NVansMain.cpp", Main);
USEFORM("NVansLogin.cpp", frmLogin);
USEFORM("NVansLocalTrains.cpp", frmLocalTrains);
USEFORM("NVansOptions.cpp", frmOptions);
USEFORM("NVansServerTrains.cpp", frmServerTrains);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
	try {
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
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
	return 0;
}
// ---------------------------------------------------------------------------
