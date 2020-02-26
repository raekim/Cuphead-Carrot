#include "stdafx.h"
#include "MainGame.h"
#include "CarrotBullet.h"

MainGame::MainGame()
{
	srand(time(NULL));

	SetViewProjectionMatrixes();
	SetBlendStates();

	CreateGameClasses();

	this->Init();

	started = false;
	g_pTextureManager->AddTexture(L"title-screen", L"title-screen.png");
	titleScreen = new Sprite(L"title-screen", 1, 1, 0);
	titleScreen->Init();
}

void MainGame::SetViewProjectionMatrixes()
{
	ViewProjectionDesc VPDesc;

	// 뷰 매트릭스 셋팅
	D3DXVECTOR3 eye(0, 0, -1);
	D3DXVECTOR3 lookAt(0, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&VPDesc.View, &eye, &lookAt, &up);
	D3DXMatrixTranspose(&VPDesc.View, &VPDesc.View);
	// 셰이더에서 매트릭스의 행과 열의 방향이 바껴있기 때문에 트랜스포즈를 해주어야 한다.

	// 프로젝션 매트릭스 셋팅
	D3DXMatrixOrthoOffCenterLH(&VPDesc.Projection, 0, (float)WINSIZEX, 0, (float)WINSIZEY, -1, 1);
	D3DXMatrixTranspose(&VPDesc.Projection, &VPDesc.Projection);

	// 버퍼 생성
	D3D11_BUFFER_DESC desc = { 0, };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(ViewProjectionDesc);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// 버퍼에 담을 데이터
	D3D11_SUBRESOURCE_DATA data = { 0, };
	data.pSysMem = &VPDesc;

	HRESULT hr = Device->CreateBuffer(&desc, &data, &m_pViewProjBuffer);
}

void MainGame::SetBlendStates()
{
	D3D11_BLEND_DESC desc = { 0, };

	desc.RenderTarget[0].BlendEnable = false;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Device->CreateBlendState(&desc, &m_pNormalBlendState);

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Device->CreateBlendState(&desc, &m_pAlphaBlendState);
}

MainGame::~MainGame()
{
	DeleteGameClasses();

	SAFE_RELEASE(m_pNormalBlendState);
	SAFE_RELEASE(m_pAlphaBlendState);

	SAFE_RELEASE(m_pViewProjBuffer);
}

void MainGame::CreateGameClasses()
{
	// 클래스 생성
	m_pPlayer = new Player;
	m_pMap = new Map;

	// 클래스 참조
	m_pPlayer->SetMap(m_pMap);
	m_pMap->SetPlayer(m_pPlayer);

	m_pBoss = new Boss;

	m_pRect = new Rect;
	m_pProgressBar = new ProgressBar(L"ProgressBack", L"ProgressFront");
}

void MainGame::DeleteGameClasses()
{
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pPlayer);
}

void MainGame::Init()
{
	SAFE_INIT(m_pMap);
	SAFE_INIT(m_pPlayer);
	m_pBoss->Init();
	SAFE_INIT(m_pRect);

	if (m_pProgressBar)
	{
		m_pProgressBar->SetSize(WINSIZEX - 200, 50);
		m_pProgressBar->Init();
		m_pProgressBar->SetPos(100, WINSIZEY - 60);
	}

	g_camX = 0;
}

void MainGame::Update()
{
	if (g_pKeyManager->isOnceKeyDown(VK_F9))
	{ // 게임 초기화
		started = true;
		this->Init();
	}

	if (!started)
	{
		titleScreen->Update();
	}
	
	m_pPlayer->Update(m_pBoss->GetCarrotBulletes());
	m_pBoss->Update(m_pPlayer->GetBullets(), m_pPlayer->GetPosition());
	SAFE_UPDATE(m_pMap);

	float mouse[2] = { g_ptMouse.x, g_ptMouse.y };

	ImGui::DragFloat2("Mouse", mouse);
}

void MainGame::Render()
{
	D3DXCOLOR Map = D3DXCOLOR(0, 0, 0, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)Map);
	DeviceContext->VSSetConstantBuffers(0, 1, &m_pViewProjBuffer);

	DeviceContext->OMSetBlendState(m_pAlphaBlendState, NULL, 0xFF);

	//SAFE_RENDER(m_pMap);
	m_pMap->Render(4);
	m_pMap->Render(3);

	// render monster here
	SAFE_RENDER(m_pBoss);

	m_pMap->Render(2);
	m_pMap->Render(1);
	//SAFE_RENDER(m_pRect);
	//SAFE_RENDER(m_pProgressBar);

	SAFE_RENDER(m_pPlayer);
	m_pBoss->RenderBullets();
	m_pMap->Render(0);


	// 타이틀 스크린
	if (!started)
	{
		titleScreen->Render();
	}

	//g_pTextManager->AlphabetRender("ABCDEFG", 10, WINSIZEY - 60, 50);

	DeviceContext->OMSetBlendState(m_pNormalBlendState, NULL, 0xFF);

	ImGui::Render();

	SwapChain->Present(0, 0);
}