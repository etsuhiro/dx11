// ex4_texture.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
	TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
	TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
	HACCEL hAccelTable;

	static const int TYOUTEN = 4;	//�|���S���̒��_��

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
		//�|���S�����_�\����
		struct Vertex3D {
			float pos[3];	//x-y-z
			float col[4];	//r-g-b-a
			float tex[2];	//x-y
		};

		//���_�f�[�^(�O�p�|���S��1��)
		Vertex3D hVectorData[TYOUTEN] = {
			{ { -1.0f, +1.0f, +0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },	// ����
			{ { +1.0f, +1.0f, +0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },	// �E��
			{ { -1.0f, -1.0f, +0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },	// ����
			{ { +1.0f, -1.0f, +0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }		// �E��
		};

		//���_���C�A�E�g
		//�T�Ԗڂ̃p�����[�^�͐擪����̃o�C�g���Ȃ̂ŁCCOLOR�ɂ�POSITION��float�^4�o�C�g�~3���L�q
		D3D11_INPUT_ELEMENT_DESC hInElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ID3D11Device* hpDevice = dx11.Device();
		ID3D11DeviceContext* hpDeviceContext = dx11.DeviceContext();

		//���_�o�b�t�@�쐬
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

		//���̒��_�o�b�t�@���R���e�L�X�g�ɐݒ�
		UINT hStrides = sizeof(Vertex3D);
		UINT hOffsets = 0;
		hpDeviceContext->IASetVertexBuffers(0, 1, &hpBuffer, &hStrides, &hOffsets);

		//�v���~�e�B�u(�|���S���̌`��)���R���e�L�X�g�ɐݒ�
		hpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// ���_�V�F�[�_�[�ǂݍ���
		FileRead v_shader("vs_texture.cso");

		//���_���C�A�E�g�쐬
		ID3D11InputLayout* hpInputLayout = NULL;
		if (FAILED(hpDevice->CreateInputLayout(hInElementDesc, ARRAYSIZE(hInElementDesc), v_shader.Buf(), v_shader.Size(), &hpInputLayout))){
			MessageBox(hWnd, _T("CreateInputLayout"), _T("Err"), MB_ICONSTOP);
			return FALSE;
		}

		//���_���C�A�E�g���R���e�L�X�g�ɐݒ�
		hpDeviceContext->IASetInputLayout(hpInputLayout);

		//���_�V�F�[�_�[����
		ID3D11VertexShader* hpVertexShader;
		if (FAILED(hpDevice->CreateVertexShader(v_shader.Buf(), v_shader.Size(), NULL, &hpVertexShader))){
			MessageBox(hWnd, _T("CreateVertexShader"), _T("Err"), MB_ICONSTOP);
			return FALSE;
		}

		//���_�V�F�[�_�[���R���e�L�X�g�ɐݒ�
		hpDeviceContext->VSSetShader(hpVertexShader, NULL, 0);

		// �s�N�Z���V�F�[�_�[�ǂݍ���
		FileRead p_shader("ps_texture.cso");

		//�s�N�Z���V�F�[�_�[����
		ID3D11PixelShader* hpPixelShader;
		if (FAILED(hpDevice->CreatePixelShader(p_shader.Buf(), p_shader.Size(), NULL, &hpPixelShader))){
			MessageBox(hWnd, _T("CreatePixelShader"), _T("Err"), MB_ICONSTOP);
			return FALSE;
		}

		//�s�N�Z���V�F�[�_�[���R���e�L�X�g�ɐݒ�
		hpDeviceContext->PSSetShader(hpPixelShader, NULL, 0);

		//�e�N�X�`���[�ǂݍ���
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

		//���̃e�N�X�`���[���R���e�L�X�g�ɐݒ�
		ID3D11ShaderResourceView* hpShaderResourceViews[] = { hpShaderResourceView };
		hpDeviceContext->PSSetShaderResources(0, 1, hpShaderResourceViews);

		return TRUE;
	}

	virtual void Render(ID3D11DeviceContext* pDeviceContext) override
	{
		pDeviceContext->Draw(TYOUTEN, 0);
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
		case IDM_OPEN:
		{
			if (pao::OpenFileName(hWnd, L"png(*.png)\0*.png\0\0", L"�t�@�C�����J��") == TRUE){
			}

#if 0
			OPENFILENAME ofn;
			TCHAR fname_full[MAX_PATH] = L"";   // �t�@�C����(�t���p�X)���󂯎��̈�
			// �\���̂ɏ����Z�b�g
			ZeroMemory(&ofn, sizeof(ofn));				// �ŏ��Ƀ[���N���A���Ă���
			ofn.lStructSize = sizeof(ofn);				// �\���̂̃T�C�Y
			ofn.hwndOwner = hWnd;						// �R�����_�C�A���O�̐e�E�B���h�E�n���h��
			ofn.lpstrFilter = L"png(*.png)\0*.png\0\0";	// �t�@�C���̎��
			ofn.lpstrFile = fname_full;				// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
			ofn.nMaxFile = MAX_PATH;		// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
			ofn.Flags = OFN_FILEMUSTEXIST;		// �t���O�w��
			ofn.lpstrTitle = L"�t�@�C�����J��";		// �R�����_�C�A���O�̃L���v�V����
			ofn.lpstrDefExt = L"png";					// �f�t�H���g�̃t�@�C���̎��
			// �����t�H���_�̎w��
//			ofn.lpstrInitialDir = g_SetupData.dataPath;
			// �t�@�C�����J���R�����_�C�A���O���쐬
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

