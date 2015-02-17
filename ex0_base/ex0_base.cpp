#include "stdafx.h"
#include "Windows/FrameworkWindows.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	pao::FrameworkWindows framework(hInstance);
	return framework.Execute(L"ex0_base", nCmdShow);
}
