#include "Application.h"
#include "DxLib.h"
#include "InputManager.h"
#include "../Scenes/SceneManager.h"
#include "../Scenes/SceneBase.h"

Application::Application() :
	delta_second(0), start_time(0),
	now_time(0), refresh_rate(0), scene(nullptr), exit(true)
{

}

Application::~Application()
{
	//解放忘れ防止
	Shutdown();
}

bool Application::WakeUp()
{
	scene = SceneManager::GetInstance();

	//ウィンドウモードで起動する
	ChangeWindowMode(TRUE);

	//ウィンドウサイズの設定	
	SetGraphMode(D_WIN_MAX_X, D_WIN_MAX_Y, D_COLOR_BIT);

	//DXライブラリの初期化
	if (DxLib_Init() == D_FAILURE)
	{
		throw std::string("Dxライブラリの初期化に失敗しました\n");
	}

	//描画先を表画面に反映する
	SetDrawScreen(DX_SCREEN_BACK);

	//シーンの初期化処理
	scene->Initialize();


	return true;
}

void Application::Run()
{

	while (ProcessMessage() == D_SUCCESS)
	{
		//フレームレートの制御
		UpdateDeLtaTime();

		//sceneがtrueかfalseかチェック
		exit = scene->Update(delta_second);

		//終了フラグ
		if (exit == false)
		{
			break;
		}
	}

	//終了時処理
	Shutdown();
}

void Application::Shutdown()
{
	//シーンの終了処理
	scene->Finalize();

	//Dxライブラリの終了処理
	DxLib_End();
}

void Application::UpdateDeLtaTime()
{
	//現在の時間取得
	now_time = GetNowHiPerformanceCount();

	// 開始時間から現在時間までに経過した時間を計算する（μ秒）
	// 分解能をμ秒→秒に変換する
	delta_second = (float)(now_time - start_time) * 1.0e-6f;

	// 計測開始時間を更新する
	start_time = now_time;

	//リフレッシュレートを取得する
	refresh_rate = (float)GetRefreshRate();

	//１フレームあたり時間が1/refresh_rate秒を超えたら調整する
	if (delta_second > (1.0f / refresh_rate))
	{
		delta_second = (1.0f / refresh_rate);
	}
}

int Application::ErrorThrow(std::string error_log)
{
	ErrorLogFmtAdd(error_log.c_str());
	return D_FAILURE;
}

float Application::Get_delta_Second()
{
	return delta_second;
}
