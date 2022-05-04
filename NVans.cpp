// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
// ---------------------------------------------------------------------------
USEFORM("NVansServerList.cpp", frmServerList);
USEFORM("NVansLogin.cpp", frmLogin);
USEFORM("NVansMain.cpp", Main);
USEFORM("NVansOptions.cpp", frmOptions);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
	try {
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "�������� ����";
		Application->CreateForm(__classid(TMain), &Main);
		Application->CreateForm(__classid(TfrmServerList), &frmServerList);
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
