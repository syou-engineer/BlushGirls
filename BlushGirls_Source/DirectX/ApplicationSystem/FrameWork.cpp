#include "Framework.h"
#include "..\Sequence\Title.h"
#include "..\Sequence\SequenceChanger.h"


/**
* @brief コンストラクタ
*/
FrameWork::FrameWork()
{
	// ロケールの設定
	setlocale(LC_ALL, "Japanese_Japan.932");

	// srand関数で、乱数パターンを初期化する
	srand((unsigned)time(NULL));

	// メモリーリーク検出
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

/**
* @brief デストラクタ
*/
FrameWork::~FrameWork()
{
	m_nowScene = nullptr;
}

/**
* @brief 作成
* @param インスタンス
* @return true - 成功：false - 失敗
*/
bool FrameWork::Create(HINSTANCE hInst)
{
	// 2重起動防止
	HANDLE hMutex = CreateMutex(NULL, TRUE, APPLICATION_NAME);
	if (hMutex == NULL) {
		return false;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		MessageBox(NULL, "既に起動されています。", "error", MB_OK | MB_ICONERROR);
		return false;
	}

	// インスタンスを生成
	pSys = make_unique<WindowsApplication>();
	pD3d = make_unique<Direct3DSystem>();

	// アプリケーションを作成（失敗すると"0"が返る）
	if FAILED(pSys->Create(hInst)) return false;
	// Direct3Dを作成（失敗すると"0"が返る）
	if FAILED(pD3d->Create(ApplicationData::GetInstance()->GetWindowHandle())) return false;

	// 初期化を行う
	Initialize();

	return true;
}

/**
* @brief 初期化
*/
void FrameWork::Initialize()
{
	m_nowScene = nullptr;

	// シングルトンをまとめて初期化する
	m_singletonOwner = make_unique<SingletonOwner>();
	m_singletonOwner->Initialize();

	// FPS
	m_fpsCounter = make_unique<FPSCounter>(FPSMode::FPS_60);

	// フォント
	m_font = make_unique<Font>();

	// サウンドデータを生成
	m_soundData = make_unique<SoundData>();

	// 初期シーンの設定
	SequenceChanger::GetInstance()->ChangeScene(new Title());
}

/**
* @brief 更新
*/
void FrameWork::Update()
{
	// FPSの更新
	m_fpsCounter->Update();

	// フェードの更新
	ScreenEffect::GetInstance()->Update();

	// シーンの更新
	if (m_nowScene != nullptr) m_nowScene->Update();

	SequenceChanger* seqChanger = SequenceChanger::GetInstance();
	// シーンが切り替わったら
	if (m_nowScene != seqChanger->GetNowScene()){
		m_nowScene = seqChanger->GetNowScene();
	}
}

/**
* @brief 描画
*/
void FrameWork::Render()
{
	// 描画フラグ
	if (!m_fpsCounter->IsRender()) return;

	// シーン開始
	HCls();

	// シーン描画
	if (m_nowScene != nullptr) m_nowScene->Render();

	// フェードの描画
	ScreenEffect::GetInstance()->Draw();

	// FPSの表示
	RenderFPS();

	// シーン終了
	Flip();
}

/**
* @brief 更新と描画
*/
void FrameWork::Run()
{
	while (!pSys->ProcessEvent())
	{
		// 更新
		Update();

		// 描画
		Render();
	}
}

/**
* @brief 画面クリア
*/
BOOL FrameWork::HCls()
{
	D3DIns->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// レンダリング開始
	if (SUCCEEDED(D3DIns->GetDevice()->BeginScene()))
	{
		return TRUE;
	}
	return FALSE;
}

/**
* @brief フリッピング
*/
BOOL FrameWork::Flip()
{
	// レンダリング終了
	D3DIns->GetDevice()->EndScene();
	D3DIns->GetDevice()->Present(NULL, NULL, NULL, NULL);

	return TRUE;
}

/**
* @brief FPSの表示
*/
VOID FrameWork::RenderFPS()
{
	//	フレームレートの表示
#ifdef _DEBUG
	char	str[64];
	wsprintf(str, "FPS %03d / %03d\n", m_fpsCounter->GetFPS(), m_fpsCounter->GetRenderFPS());
	m_font->DrawFont(str, 10, 10, 200, 20, FONTCOLOR_BLACK);
#endif
}