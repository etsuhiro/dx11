// ex5_direct2d.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "ex5_direct2d.h"
#include <d2d1.h>"
#include "Windows/FrameworkWindows.h"

#pragma comment( lib, "d2d1.lib" )

class MyFramework : public pao::FrameworkWindows {
	static const int MAX_LOADSTRING = 100;
	TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
	TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
	HACCEL hAccelTable;
	ID2D1Factory* pD2DFactory = NULL;
	ID2D1HwndRenderTarget* pRT = NULL;
	ID2D1SolidColorBrush* pBlackBrush = NULL;
	RECT rc;
public:
	MyFramework(HINSTANCE hInstance) : pao::FrameworkWindows(hInstance)
	{
		LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadString(hInstance, IDC_EX5_DIRECT2D, szWindowClass, MAX_LOADSTRING);

		SetIcon(LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EX5_DIRECT2D)), LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)));
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		SetMenu(MAKEINTRESOURCE(IDC_EX5_DIRECT2D));
		SetClassName(szWindowClass);

		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EX5_DIRECT2D));

		// ID2D1Factory ���쐬����
		// ID2D1Factory �C���^�[�t�F�C�X�́ADirect2D ���g�p���邽�߂̊J�n�_�ł��B�܂�AID2D1Factory ���g�p���� Direct2D ���\�[�X���쐬���܂��B
		// �ʏ�A�A�v���P�[�V�����ł́A��x�t�@�N�g�����쐬������A�A�v���P�[�V�������L���ȊԂ͍쐬�����t�@�N�g����ێ����Ă����K�v������܂��B
		HRESULT hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&pD2DFactory
			);

	}

	// ID2D1HwndRenderTarget ���쐬����
	BOOL Init(HWND hWnd) override
	{
		// �t�@�N�g�����쐬������A�쐬�����t�@�N�g�����g�p���ă����_�[ �^�[�Q�b�g���쐬���܂��B
		// Obtain the size of the drawing area.
//		RECT rc;
		GetClientRect(hWnd, &rc);

		// Create a Direct2D render target			
		HRESULT hr = pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
			hWnd,
			D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top)
			),
			&pRT
			);

		// �u���V���쐬����
		if (SUCCEEDED(hr))
		{

			pRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Red),
				&pBlackBrush
				);
		}
#if 0
		// �l�p�`��`�悷��
		{
			pRT->BeginDraw();

			pRT->DrawRectangle(
				D2D1::RectF(
				rc.left + 100.0f,
				rc.top + 100.0f,
				rc.right - 100.0f,
				rc.bottom - 100.0f),
				pBlackBrush);

			HRESULT hr = pRT->EndDraw();
		}
#endif
		return (SUCCEEDED(hr)) ? TRUE : FALSE;
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
#if 0
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
#else
		MSG msg = { 0 };
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)){
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else {
				// �l�p�`��`�悷��
				pRT->BeginDraw();

				pRT->DrawRectangle(
					D2D1::RectF(
					rc.left + 200.0f,
					rc.top + 200.0f,
					rc.right - 200.0f,
					rc.bottom - 200.0f),
					pBlackBrush);

				HRESULT hr = pRT->EndDraw();
			}
		}
#endif
		return (int)msg.wParam;
	}

#if 0
	LRESULT WmPaint(HWND hWnd, WPARAM wParam, LPARAM lParam) override
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		// �l�p�`��`�悷��
		pRT->BeginDraw();

		pRT->DrawRectangle(
			D2D1::RectF(
			rc.left + 200.0f,
			rc.top + 200.0f,
			rc.right - 200.0f,
			rc.bottom - 200.0f),
			pBlackBrush);

		HRESULT hr = pRT->EndDraw();
		return 0;
	}
#endif

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
