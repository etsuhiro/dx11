// ex5_direct2d.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ex5_direct2d.h"
#include <d2d1.h>"
#include "Windows/FrameworkWindows.h"

#pragma comment( lib, "d2d1.lib" )

class MyFramework : public pao::FrameworkWindows {
	static const int MAX_LOADSTRING = 100;
	TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
	TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名
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

		// ID2D1Factory を作成する
		// ID2D1Factory インターフェイスは、Direct2D を使用するための開始点です。つまり、ID2D1Factory を使用して Direct2D リソースを作成します。
		// 通常、アプリケーションでは、一度ファクトリを作成したら、アプリケーションが有効な間は作成したファクトリを保持しておく必要があります。
		HRESULT hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&pD2DFactory
			);

	}

	// ID2D1HwndRenderTarget を作成する
	BOOL Init(HWND hWnd) override
	{
		// ファクトリを作成したら、作成したファクトリを使用してレンダー ターゲットを作成します。
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

		// ブラシを作成する
		if (SUCCEEDED(hr))
		{

			pRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Red),
				&pBlackBrush
				);
		}
#if 0
		// 四角形を描画する
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
				// 四角形を描画する
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

		// 四角形を描画する
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

	// アプリケーション メニューの処理
	virtual LRESULT WmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) override
	{
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
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
