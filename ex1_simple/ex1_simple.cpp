// ex1_simple.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "ex1_simple.h"
#include "Windows/FrameworkWindows.h"

class MyFramework : public pao::FrameworkWindows {
	static const int MAX_LOADSTRING = 100;
	TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
	TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
	HACCEL hAccelTable;
public:
	MyFramework(HINSTANCE hInstance) : pao::FrameworkWindows(hInstance)
	{
		LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadString(hInstance, IDC_EX1_SIMPLE, szWindowClass, MAX_LOADSTRING);

		SetIcon(LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EX1_SIMPLE)), LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)));
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		SetMenu(MAKEINTRESOURCE(IDC_EX1_SIMPLE));
		SetClassName(szWindowClass);

		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EX1_SIMPLE));
	}
	~MyFramework()
	{
	}

	int Execute(int nCmdShow)
	{
		return FrameworkWindows::Execute(szTitle, nCmdShow);
	}

	virtual int MainLoop() override
	{
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return (int)msg.wParam;
	}

	// �A�v���P�[�V���� ���j���[�̏���
	virtual LRESULT WmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) override
	{
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(GetAppInstanceHandle(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
		}
		return 0;
	}
};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyFramework myFrameWork(hInstance);
	return myFrameWork.Execute(nCmdShow);
}
