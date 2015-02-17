// ex4_texture.cpp : アプリケーションのエントリ ポイントを定義します。
//

#define D3DX

#include "stdafx.h"
#include "ex4_texture.h"
#include "DX11/FrameworkDX11.h"
#include "cFileWrite.h"

#if defined(D3DX)
	#include <d3dx11.h>
	#pragma comment(lib, "d3dx11.lib")
#else
#include "DX11/DirectXTex/WICTextureLoader.h"
#endif

class MyFramework : public pao::FrameworkDX11 {
	static const int MAX_LOADSTRING = 100;
	TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
	TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名
	HACCEL hAccelTable;

	static const int TYOUTEN = 4;	//ポリゴンの頂点数

public:
	MyFramework(HINSTANCE hInstance) : pao::FrameworkDX11(hInstance)
	{
		LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadString(hInstance, IDC_EX4_TEXTURE, szWindowClass, MAX_LOADSTRING);

		SetIcon(LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EX4_TEXTURE)), LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)));
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		SetMenu(MAKEINTRESOURCE(IDC_EX4_TEXTURE));
		SetClassName(szWindowClass);

		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EX4_TEXTURE));
	}
	~MyFramework()
	{
	}

	int Execute(int nCmdShow)
	{
		return FrameworkWindows::Execute(szTitle, nCmdShow);
	}

	virtual BOOL Setup(HWND hWnd) override
	{
		//ポリゴン頂点構造体
		struct Vertex3D {
			float pos[3];	//x-y-z
			float col[4];	//r-g-b-a
			float tex[2];	//x-y
		};

		//頂点データ(三角ポリゴン1枚)
		Vertex3D hVectorData[TYOUTEN] = {
			{ { -1.0f, +1.0f, +0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },	// 左上
			{ { +1.0f, +1.0f, +0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },	// 右上
			{ { -1.0f, -1.0f, +0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },	// 左下
			{ { +1.0f, -1.0f, +0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }		// 右下
		};

		//頂点レイアウト
		//５番目のパラメータは先頭からのバイト数なので，COLORにはPOSITIONのfloat型4バイト×3を記述
		D3D11_INPUT_ELEMENT_DESC hInElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ID3D11Device* hpDevice = dx11.Device();
		ID3D11DeviceContext* hpDeviceContext = dx11.DeviceContext();

		//頂点バッファ作成
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = sizeof(Vertex3D) * TYOUTEN;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		D3D11_SUBRESOURCE_DATA hSubResourceData;
		hSubResourceData.pSysMem = hVectorData;
		hSubResourceData.SysMemPitch = 0;
		hSubResourceData.SysMemSlicePitch = 0;

		ID3D11Buffer* hpBuffer;
		if (FAILED(hpDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &hpBuffer))){
			MessageBox(hWnd, _T("CreateBuffer"), _T("Err"), MB_ICONSTOP);
			return FALSE;
		}

		//その頂点バッファをコンテキストに設定
		UINT hStrides = sizeof(Vertex3D);
		UINT hOffsets = 0;
		hpDeviceContext->IASetVertexBuffers(0, 1, &hpBuffer, &hStrides, &hOffsets);

		//プリミティブ(ポリゴンの形状)をコンテキストに設定
		hpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// 頂点シェーダー読み込み
		FileRead v_shader("vs_texture.cso");

		//頂点レイアウト作成
		ID3D11InputLayout* hpInputLayout = NULL;
		if (FAILED(hpDevice->CreateInputLayout(hInElementDesc, ARRAYSIZE(hInElementDesc), v_shader.Buf(), v_shader.Size(), &hpInputLayout))){
			MessageBox(hWnd, _T("CreateInputLayout"), _T("Err"), MB_ICONSTOP);
			return FALSE;
		}

		//頂点レイアウトをコンテキストに設定
		hpDeviceContext->IASetInputLayout(hpInputLayout);

		//頂点シェーダー生成
		ID3D11VertexShader* hpVertexShader;
		if (FAILED(hpDevice->CreateVertexShader(v_shader.Buf(), v_shader.Size(), NULL, &hpVertexShader))){
			MessageBox(hWnd, _T("CreateVertexShader"), _T("Err"), MB_ICONSTOP);
			return FALSE;
		}

		//頂点シェーダーをコンテキストに設定
		hpDeviceContext->VSSetShader(hpVertexShader, NULL, 0);

		// ピクセルシェーダー読み込み
		FileRead p_shader("ps_texture.cso");

		//ピクセルシェーダー生成
		ID3D11PixelShader* hpPixelShader;
		if (FAILED(hpDevice->CreatePixelShader(p_shader.Buf(), p_shader.Size(), NULL, &hpPixelShader))){
			MessageBox(hWnd, _T("CreatePixelShader"), _T("Err"), MB_ICONSTOP);
			return FALSE;
		}

		//ピクセルシェーダーをコンテキストに設定
		hpDeviceContext->PSSetShader(hpPixelShader, NULL, 0);

		//テクスチャー読み込み
		ID3D11ShaderResourceView* hpShaderResourceView = NULL;
#if defined(D3DX)
		if (FAILED(D3DX11CreateShaderResourceViewFromFile(hpDevice, _T("bg_04_1.png"), NULL, NULL, &hpShaderResourceView, NULL))){
			MessageBox(hWnd, _T("D3DX11CreateShaderResourceViewFromFile"), _T("Err"), MB_ICONSTOP);
			return FALSE;
		}
#else
		if (FAILED(DirectX::CreateWICTextureFromFile(hpDevice, _T("bg_04_1.png"), NULL, &hpShaderResourceView))){
			MessageBox(hWnd, _T("CreateWICTextureFromFile"), _T("Err"), MB_ICONSTOP);
			return FALSE;
		}
#endif

		//そのテクスチャーをコンテキストに設定
		ID3D11ShaderResourceView* hpShaderResourceViews[] = { hpShaderResourceView };
		hpDeviceContext->PSSetShaderResources(0, 1, hpShaderResourceViews);

		return TRUE;
	}

	virtual void Render(ID3D11DeviceContext* pDeviceContext) override
	{
		pDeviceContext->Draw(TYOUTEN, 0);
	}

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
		case IDM_OPEN:
		{
			if (pao::OpenFileName(hWnd, L"png(*.png)\0*.png\0\0", L"ファイルを開く") == TRUE){
			}

#if 0
			OPENFILENAME ofn;
			TCHAR fname_full[MAX_PATH] = L"";   // ファイル名(フルパス)を受け取る領域
			// 構造体に情報をセット
			ZeroMemory(&ofn, sizeof(ofn));				// 最初にゼロクリアしておく
			ofn.lStructSize = sizeof(ofn);				// 構造体のサイズ
			ofn.hwndOwner = hWnd;						// コモンダイアログの親ウィンドウハンドル
			ofn.lpstrFilter = L"png(*.png)\0*.png\0\0";	// ファイルの種類
			ofn.lpstrFile = fname_full;				// 選択されたファイル名(フルパス)を受け取る変数のアドレス
			ofn.nMaxFile = MAX_PATH;		// lpstrFileに指定した変数のサイズ
			ofn.Flags = OFN_FILEMUSTEXIST;		// フラグ指定
			ofn.lpstrTitle = L"ファイルを開く";		// コモンダイアログのキャプション
			ofn.lpstrDefExt = L"png";					// デフォルトのファイルの種類
			// 初期フォルダの指定
//			ofn.lpstrInitialDir = g_SetupData.dataPath;
			// ファイルを開くコモンダイアログを作成
			if (!GetOpenFileName(&ofn)) return false;
#endif
		}
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

